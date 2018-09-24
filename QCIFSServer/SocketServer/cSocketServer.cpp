// Copyright 2018 Grass Valley, A Belden Brand

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//cSocketServer.cpp

#include "StdAfx.h"

#include ".\csocketserver.h"
#include "cListeningSocket.h"
#include "cClientSocket.h"
#include "cIOBuffer.h"
//#include "cOverlappedSocketProgress.h"
#include "cTXMsg.h"
#include "iPacketProcessor.h"
#include "cTCPStream.h"
#include "iConnectEx.h"

#include "..\common\iCommonFactory.h"
#include "..\Common\iIOCompletionPort.h"
//#include <Iphlpapi.h>


#include <exception>
using namespace std;
using namespace vfs;

const int cSocketServer::m_InitialPendingRecvs = 5;
const int cSocketServer::m_maxRetryLimit = 3;

iActivityGroup::Ptr ActivityGroup;
std::map<unsigned int, iActivity::Ptr> ActivityPerThread;
cLock ActivityPerThreadLock;

void initActivitySocketServer(iActivityGroup::Ptr activityGroup)
{
  ActivityGroup = activityGroup;
}

iActivity::Ptr getActivity()
{
  unsigned int threadId = iThreadManager::singleton().getCurrentThreadId();

  cLockGuard g(&ActivityPerThreadLock);
  if (ActivityPerThread[threadId].isNull()) 
  {
    iActivity::Ptr activitySocketServer = ActivityGroup->addSignal (L"SocketServer");
    activitySocketServer->addActivity(kRecvNotify, L"recv notify");
    activitySocketServer->addActivity(kRecvInterlockedDec, L"recv interlocked dec");
    activitySocketServer->addActivity(kRecvDo, L"recv do");
    activitySocketServer->addActivity(kRecvWSARecv, L"recv wsa recv");
    activitySocketServer->addActivity(kRecvInterlocked, L"recv interlocked");
    activitySocketServer->addActivity(kRecvBufferAssign, L"recv buffer assign");
    activitySocketServer->addActivity(kRecvIOBufferBytes, L"recv io buffer bytes");
    activitySocketServer->addActivity(kRecvIOBufferSize, L"recv io buffer size");
    activitySocketServer->addActivity(kWaitSend, L"wait-send");
    activitySocketServer->addActivity(kSend, L"send");
    activitySocketServer->addActivity(kSent, L"sent");
    ActivityPerThread[threadId] = activitySocketServer;
  }

  return ActivityPerThread[threadId];
}

static int numProcessors()
{
  SYSTEM_INFO sysInfo;
  GetSystemInfo(&sysInfo);
  return sysInfo.dwNumberOfProcessors;
}


//-- Factory ---------------------------------------------------------------------------------------------
cSocketServerFactory::cSocketServerFactory()
{
  QTRACE((L"Socket Server Factory alive"));
}


cSocketServerFactory::~cSocketServerFactory(void)
{
  QSOS((L"Socket Server Factory dead"));
}


cPtr<iSocketServer> cSocketServerFactory::createSocketServer(const cPtr<iPacketProcessor> pPacketProcessor
  , const std::vector<USHORT> &listeningPorts)
{
  return new cSocketServer(pPacketProcessor, listeningPorts);
}

std::vector<String> cSocketServerFactory::getIpAddresses()
{
  QTRACE((L"cSocketServerFactory::getIpAddresses"));
  DWORD dwNumIF;
  if (NO_ERROR != GetNumberOfInterfaces(&dwNumIF))
  {
    const DWORD dwErr(GetLastError());
    QSOS((L"cSocketServerFactory::getIpAddresses() - couldn't determine number of interfaces with error [%d] - %s"
      , dwErr
      , win32ErrorToString(dwErr).c_str()));
    return std::vector<String>();
  }

  if (dwNumIF > 1)
  {
    ULONG bufferSize(4096);
    char* adapterbuffer = new char [bufferSize];
    PIP_ADAPTER_INFO pIPAdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(adapterbuffer);
    const DWORD dwRet(GetAdaptersInfo(pIPAdapterInfo, &bufferSize));
    if (ERROR_SUCCESS != dwRet)
    {
      QSOS((L"cSocketServer::getIpAddresses() - Failed to determine adapter info with error %d - %s"
        , dwRet
        , win32ErrorToString(dwRet).c_str()));
      delete[] adapterbuffer;
      return std::vector<String>();
    }

    std::vector<String> retAddresses;
    while (NULL != pIPAdapterInfo)
    {
      PIP_ADDR_STRING pIpAddress = &(pIPAdapterInfo->IpAddressList);
      while (NULL != pIpAddress)
      {
        const std::string ipAddress(pIpAddress->IpAddress.String);
        retAddresses.push_back(widen(ipAddress));
        pIpAddress = pIpAddress->Next;
      }
      pIPAdapterInfo = pIPAdapterInfo->Next;
    }

    delete[] adapterbuffer;
    return retAddresses;
  }

  return std::vector<String>();
}
//--------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
class cCloseSocketSerialiseThread: public cThread
{
  public:
    cCloseSocketSerialiseThread()
    : cThread(L"cCloseSocketSerialiseThread", cThread::ePriority_Normal)
    , m_bHasSignalled(false)
    {
      m_hComplete = ::CreateEvent(0, TRUE, FALSE, 0);
    }

    ~cCloseSocketSerialiseThread()
    {
      ::CloseHandle(m_hComplete);
    }

    void QAPI signalLoopStop()
    {
      m_bHasSignalled = true;
    }

    void QAPI runLoop()
    {
      while (!m_bHasSignalled)
      {
        waitMessageForever();

        while(isMessageWaiting())
        {
          iMsgQueue::iMsg::Ptr LatestMsg = getMessage();
          if(LatestMsg->isQuitMsg())
          {
            m_bHasSignalled = true;
            break;
          }

          LatestMsg->deliver2();
        }
      }

      ::SetEvent(m_hComplete);
    }

    void waitThreadComplete(void)
    {
      WaitForSingleObject(m_hComplete, INFINITE);
    }

  private:
    bool m_bHasSignalled;
    HANDLE m_hComplete;
};


//--------------------------------------------------------------------------------------
class cCloseSocket : public iMsgQueue::iMsg, public cRefCount
{
public:
  cCloseSocket(cPtr<cSocketServer> pSocketServer, SOCKET_DATA* pSockData)
  : m_pSocketServer(pSocketServer)
  , m_pSockData(pSockData)
  {
  }

  virtual void QAPI deliver2()
  {
    m_pSocketServer->doCloseSocket(m_pSockData);
  }

protected:
  cPtr<cSocketServer> m_pSocketServer;
  SOCKET_DATA* m_pSockData;
};


//--------------------------------------------------------------------------------------
class cCloseClientSocket : public cCloseSocket
{
public:
  cCloseClientSocket(cPtr<cSocketServer> pSocketServer, SOCKET_DATA* pSockData)
  : cCloseSocket(pSocketServer, pSockData)
  {
  }

  virtual void QAPI deliver2()
  {
    HANDLE hCloseEvent = ::CreateEvent(0, FALSE, FALSE, 0);
    if (WSAEventSelect(m_pSockData->sock, hCloseEvent, FD_CLOSE) != 0)
    {
      DWORD dwErr = ::WSAGetLastError();
      QSOS((L"cCloseClientSocket::deliver2 - WSAEventSelect (socket %Iu} : err %d - %s"
        , m_pSockData->sock
        , dwErr
        , win32ErrorToString(dwErr).c_str()));
    }
    else
    {
      if (!iConnectEx::singleton().DisconnectEx(m_pSockData, m_pSocketServer.getRaw()))
        QSOS((L"cCloseClientSocket::deliver2 - disconnectEx failed"));
      else
      {
        ::WaitForSingleObject(hCloseEvent, INFINITE);
        iConnectEx::singleton().clientSessionDead(m_pSockData->sock);
        
        cCloseSocket::deliver2();
      }
    }
    ::CloseHandle(hCloseEvent);
  }
};





const int kMaxTxMessages = 100;



//--- SocketServer ---------------------------------------------------------------------------------------
cSocketServer::cSocketServer(cPtr<iPacketProcessor> pPacketProcessor, const std::vector<USHORT> &listeningPorts)
//: m_pCompletionPort(iCommonFactory::singleton().createIOCP(numProcessors()*2)) // (I read in Jeffery Ritcher that twice the number of actual processors is a good number) - RH
: m_pCompletionPort(iCommonFactory::singleton().createIOCP(numProcessors())) // This seems overly generous - we only seem to use 1/2 the above - even under load - JC
, m_bRun(true)
, m_pTransmitPackets(NULL)
, m_pTransmitFile(NULL)
{
  QTRACE((L"Socket Server alive"));
  m_hTxSemaphore = ::CreateSemaphore (NULL, kMaxTxMessages, kMaxTxMessages, NULL);
  m_pCloseSocketThread = new cCloseSocketSerialiseThread();
  m_pCloseSocketThread->startLoop();
  init(pPacketProcessor, listeningPorts);
}

cSocketServer::~cSocketServer(void)
{
  {
    cLockGuard lg(&m_SocketDataLock);
    for (tSocketDataList::const_iterator it = m_SocketData.begin(); it != m_SocketData.end(); ++it)
      CloseSocket(*it);
  }

  m_pCloseSocketThread->postMessage(new iMsgQueue::cQuitMsg());
  m_pCloseSocketThread->waitThreadComplete();
  CloseHandle(m_hTxSemaphore);
  QSOS((L"Socket Server dead"));
}

void cSocketServer::init(cPtr<iPacketProcessor> pPacketProcessor, const std::vector<USHORT> &listeningPorts)
{
  QTRACE((L"cSocketServer::init - %d listening ports", listeningPorts.size()));
  m_pPacketProcessor = pPacketProcessor;
  try
  {
    if (listeningPorts.empty())
    {
      QTRACE((L"cSocketServer::cSocketServer() - no ports specified for listening"));
      throw false;
    }

    for (unsigned int i=0
      ; i<listeningPorts.size()
      ; i++)
    {
      startListeningSocket(listeningPorts[i]);
    }
  }
  catch (const bool)
  {
  }
}


void cSocketServer::startListeningSocket(USHORT listeningPort)
{
  try
  {
    QTRACE((L"cSocketServer::startListeningSocket - opening listening socket on port %d", listeningPort));
    sockaddr_in sa;
    vfs::cPtr<cListeningSocket> pListeningSocket = new cListeningSocket(listeningPort, this, sa);
    for (int i=0; i<5; ++i) // post 5 accepts so that (hopefully) a backlog doesn't build up
      pListeningSocket->postAccept();
  }
  catch (const int)
  {
    QSOS((L"COULDN'T LISTEN FOR INCOMING CONNECTIONS ON PORT %d", listeningPort));
  }
}


void cSocketServer::notifyExit()
{
  delete(this);
}

void cSocketServer::closeSession(UINT_PTR SessionID)
{
  cLockGuard lg(&m_SocketDataLock);
  for (tSocketDataList::iterator it = m_SocketData.begin(); it != m_SocketData.end(); ++it)
  {
    SOCKET_DATA* pSockData = *it;
    if (pSockData->sock == SessionID)
    {
      CloseSocket(pSockData);
      break;
    }
  }
}


//-------------------------------------------------------------------------------------------
void cSocketServer::closeClientSession(UINT_PTR SessionID)
{
  cLockGuard lg(&m_SocketDataLock);
  for (tSocketDataList::iterator it = m_SocketData.begin(); it != m_SocketData.end(); ++it)
  {
    SOCKET_DATA* pSockData = *it;
    if (pSockData->sock == SessionID)
    {
      m_pCloseSocketThread->postMessage(new cCloseClientSocket(this, pSockData));
      break;
    }
  }
}


//-------------------------------------------------------------------------------------------
bool cSocketServer::addClientConnection(const USHORT clientPort, const in_addr& remoteAddr, const USHORT remotePort, const UINT_PTR associatedSessionID)
{
  bool bSuccess(true);
  try
  {
    QTRACE((L"cSocketServer::addClientConnection - opening client socket on port %d, remote port %d", clientPort, remotePort));
    cPtr<cClientSocket> pClientSocket = new cClientSocket(clientPort, this, remoteAddr, remotePort, associatedSessionID);
  }
  catch (const int)
  {
    bSuccess = false;
    QSOS((L"cSocketServer::addClientConnection - COULDN'T OPEN CLIENT CONNECTION ON PORT %d", clientPort));
  }

  if (!bSuccess)
    QSOS((L"cSocketServer::addClientConnection - failed"));

  QTRACE((L"cSocketServer::addClientConnection - done, result = %d", bSuccess));
  return bSuccess;
}


//-------------------------------------------------------------------------------------------
bool cSocketServer::addListeningConnection(UINT_PTR& rListenSessionID, const USHORT listeningPort, sockaddr_in& rListeningSockAddr, const UINT_PTR associatedSessionID)
{
  cLockGuard lg(&m_SocketDataLock);
  bool bSuccess(true);
  bool bUseGivenAddr(false);
  try
  {
    QTRACE((L"cSocketServer::addListeningConnection - opening listening socket on port %d", listeningPort));
    if (associatedSessionID != 0)
    {
      for (tSocketDataList::iterator it = m_SocketData.begin(); it != m_SocketData.end(); ++it)
      {
        SOCKET_DATA* pSockData = *it;
        if (pSockData->sock == associatedSessionID)
        {
          rListeningSockAddr = pSockData->localAddr;
          bUseGivenAddr = true;
          break;
        }
      }
    }

    vfs::cPtr<cListeningSocket> pListeningSocket = new cListeningSocket(listeningPort, this, rListeningSockAddr, bUseGivenAddr, associatedSessionID, true);
    pListeningSocket->postAccept(); //only accept a single connection

    rListenSessionID = pListeningSocket->getListeningSocket();
  }
  catch (const int)
  {
    bSuccess = false;
    QSOS((L"COULDN'T LISTEN FOR INCOMING CONNECTIONS ON PORT %d", listeningPort));
  }

  return bSuccess;
}


//-------------------------------------------------------------------------------------------
cPtr<iTXPacket> cSocketServer::getTXPacketForSession(UINT_PTR sessionID)
{
  cPtr<cTXPacket> pPacket;
  for (tSocketDataList::iterator it = m_SocketData.begin(); it != m_SocketData.end(); ++it)
  {
    SOCKET_DATA* pSockData = *it;
    if (pSockData->sock == sessionID)
    {
      pPacket = new cTXPacket(pSockData->sock, this);
      break;
    }
  }

  return pPacket;
}


//-------------------------------------------------------------------------------------------
bool cSocketServer::AddSocket(const SOCKET sock
  , const SOCKADDR_IN &krLocalSockAddrIn
  , const SOCKADDR_IN &krRemoteSockAddrIn
  , const cPtr<cMemoryView> pRecvBuf
  , const UINT_PTR associatedSessionID)
{
  createExtensionFunctions(sock);

  std::string localIP = inet_ntoa(krLocalSockAddrIn.sin_addr);
  std::string remoteIP = inet_ntoa(krRemoteSockAddrIn.sin_addr);

  QTRACE((L"cSocketServer::AddSocket - adding socket local %S:%d, remote %S:%d to completion port"
    , localIP.c_str()
    , ntohs(krLocalSockAddrIn.sin_port)
    , remoteIP.c_str()
    , ntohs(krRemoteSockAddrIn.sin_port)));
  BOOL bOptVal = TRUE;
  int bOptLen = sizeof(BOOL);
  setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&bOptVal), bOptLen);

  const int bufSizeRcv = 4*512*1024;
  int ret = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char *>(&bufSizeRcv), sizeof(int));
  if (ret == SOCKET_ERROR)
  {
    DWORD dwRet = WSAGetLastError();
    QTRACE((L"cSocketServer::AddSocket() - setsockopt(SO_RCVBUF) returned WSA error [%d]", dwRet));
  }

  int buf = 0;
  int size = sizeof(int);
  getsockopt(sock, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&buf), &size);
  QTRACE((L"cSocketServer::AddSocket() - getsockopt(SO_RCVBUF) %d", buf));

  const int bufSizeSnd = 4*512*1024;
  ret = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char *>(&bufSizeSnd), sizeof(int));
  if (ret == SOCKET_ERROR)
  {
    DWORD dwRet = WSAGetLastError();
    QTRACE((L"cSocketServer::AddSocket() - setsockopt(SO_SNDBUF) returned WSA error [%d]", dwRet));
  }

  buf = 0;
  size = sizeof(int);
  getsockopt(sock, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&buf), &size);
  QTRACE((L"cSocketServer::AddSocket() - getsockopt(SO_SNDBUF) %d", buf));

  SOCKET_DATA *pSocketData = new SOCKET_DATA;
  pSocketData->sock = sock;
  pSocketData->localAddr = krLocalSockAddrIn;
  pSocketData->addr = krRemoteSockAddrIn;
  pSocketData->sequenceNumber = 0;
  pSocketData->outstandingRecvs = 0;
  pSocketData->hInitialise = CreateMutex(NULL, FALSE, NULL);
  if (NULL == pSocketData->hInitialise)
  {
    closesocket(sock);
    QSOS((L"cSocketServer::AddSocket() - couldn't create mutex with error %d", GetLastError()));
    delete(pSocketData);
    return false;
  }

  if (pRecvBuf.isValid() && pRecvBuf->getSize() > 0)
  {
    try
    {
      const cPtr<cIOBuffer> pIOBuffer = new cIOBuffer(this
        , pRecvBuf);
      pIOBuffer->setSequenceNumber(InterlockedIncrement(&pSocketData->sequenceNumber));
      pSocketData->TCPStream.NotifyRecvCompleted(pIOBuffer, pSocketData);
    }
    catch (const exception&)
    {
      CloseSocket(pSocketData);
      return false;
    }
  }

  bool bRet = m_pCompletionPort->AddCompletionKey(reinterpret_cast<ULONG_PTR>(pSocketData));
  
  if (FALSE != bRet)
    bRet = saveSocketDataAndRecv(pSocketData);

  if (FALSE == bRet)
  {
    QSOS((L"Failed to add socket %S:%d to completion port"
      , inet_ntoa(krRemoteSockAddrIn.sin_addr)
      , ntohs(krRemoteSockAddrIn.sin_port)));

    CloseSocket(pSocketData);
    return false;
  }

  if (bRet && (!pRecvBuf.isValid() || pRecvBuf->getSize() == 0))
    m_pPacketProcessor->onConnectionEstablished(sock, krRemoteSockAddrIn, associatedSessionID);

  return true;
}


//-------------------------------------------------------------------------------------------
bool cSocketServer::AddClientSocket(SOCKET_DATA* pSocketData, const UINT_PTR associatedSessionID)
{
  createExtensionFunctions(pSocketData->sock);

  BOOL bOptVal = TRUE;
  int bOptLen = sizeof(BOOL);
  setsockopt(pSocketData->sock, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&bOptVal), bOptLen);

  bool bRet = saveSocketDataAndRecv(pSocketData);

  if (FALSE == bRet)
  {
    QSOS((L"cSocketServer::AddClientSocket - Failed to Recv on client socket"));
    CloseSocket(pSocketData);
    return false;
  }

  if (bRet)
    m_pPacketProcessor->onClientConnectionEstablished(pSocketData->sock, associatedSessionID);

  return bRet;
}

//{{{
void cSocketServer::logSockets()
{
  cLockGuard lg(&m_SocketDataLock);
  int sockets ((int) m_SocketData.size());
  QTRACE ((L"cSocketServer::saveSocketDataAndRecv Number of sockets open - %d", sockets));

    for (tSocketDataList::const_iterator it = m_SocketData.begin(); it != m_SocketData.end(); ++it)
  {
    std::string remoteIP = inet_ntoa((*it)->addr.sin_addr);
    QTRACE ((L"IP:: %S",remoteIP.c_str()));
  }

  // print out all checked out licenses
//  cLogIndentGuard Indent (kOptionNotify, L"Log all open Sockets");
  //QMSG ((0, L"%-24s %-10s %-10s %s", L"Name", L"Option", L"Version", L"Information"));
  //QMSG ((0, L""));
  //for (tLicenseMap::iterator licIt=LicenseMap.begin(); licIt!=LicenseMap.end(); ++licIt)
  //  licIt->second->logInfo();
}
//}}}


//-------------------------------------------------------------------------------------------
bool cSocketServer::saveSocketDataAndRecv(SOCKET_DATA *pSocketData)
{
  WaitForSingleObject(pSocketData->hInitialise, INFINITE);  
  {
    cLockGuard lg(&m_SocketDataLock);
    m_SocketData.insert(pSocketData);
  }
  QTRACE ((L"cSocketServer::saveSocketDataAndRecv Number of sockets open - %d", (int) m_SocketData.size()));

  iActivity::Ptr activity = getActivity();
  bool bRet(true);
  for (int i=0; i<m_InitialPendingRecvs; i++)
    RecvNoMutex(pSocketData, activity);

  bRet = (0 == pSocketData->outstandingRecvs) ? false : true;
  ReleaseMutex(pSocketData->hInitialise);

  return bRet;
}


//-------------------------------------------------------------------------------------------
void cSocketServer::onRecvErr(SOCKET_DATA *pSockData)
{
  //make sure socket still in list - recvErr can be called several times by client sockets
  //due to disconnectEx causing numerous ioCompletionPort threads to send recvErr
  //wait for mutex will hang if socket has been closed
  cLockGuard lg(&m_SocketDataLock);
  tSocketDataList::iterator it = m_SocketData.find(pSockData);
  if (it == m_SocketData.end())
    return;

  const HANDLE hMutex(pSockData->hInitialise);
  const DWORD dwRet(WaitForSingleObject(hMutex, INFINITE));

  if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED)
  {
    ReleaseMutex(hMutex);
    if (pSockData->outstandingRecvs > 0)
      QTRACE((L"cSocketServer::onRecvErr() - still waiting for %d replies", pSockData->outstandingRecvs));
    else
      CloseSocket(pSockData);
  }
}


//-------------------------------------------------------------------------------------------
void cSocketServer::CloseSocket(SOCKET_DATA *pSockData)
{
  QTRACE((L"cSocketServer::CloseSocket - handle %d", pSockData->sock));
  m_pCloseSocketThread->postMessage(new cCloseSocket(this, pSockData));
}


//-------------------------------------------------------------------------------------------
void cSocketServer::doCloseSocket(SOCKET_DATA *pSockData)
{
  //check requested socket is valid...
  QTRACE((L"cSocketServer::doCloseSocket - handle %d", pSockData->sock));
  cLockGuard lg(&m_SocketDataLock);
  tSocketDataList::iterator it = m_SocketData.find(pSockData);
  if (it == m_SocketData.end())
    return;

  const HANDLE hMutex(pSockData->hInitialise);
  const DWORD dwRet(WaitForSingleObject(hMutex, INFINITE));

  if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED)
  {
    if (0 == closesocket(pSockData->sock))
    {
      QTRACE((L"cSocketServer::doCloseSocket() - successfully closed socket {%Iu}", pSockData->sock));
      m_SocketData.erase(pSockData);
  QTRACE ((L"cSocketServer::doCloseSocket() a Number of sockets open - %d", (int) m_SocketData.size()));
    }
    else
    {
      const DWORD dwErr(WSAGetLastError());
      if (WSAENOTSOCK != dwErr)
        QTRACE((L"cSocketServer::doCloseSocket() - couldn't close socket {%Iu} with error: %d - %s"
          , pSockData->sock
          , dwErr
          , win32ErrorToString(dwErr).c_str()));
      else
        m_SocketData.erase(pSockData); //shouldn't be in our list if no longer valid
  QTRACE ((L"cSocketServer::doCloseSocket() b Number of sockets open - %d", (int) m_SocketData.size()));
    }

    if (m_pPacketProcessor.isValid())
      m_pPacketProcessor->onConnectionClosed(pSockData->sock);

    if (0 == pSockData->outstandingRecvs)
    {
      QTRACE((L"cSocketServer::doCloseSocket() - Freeing memory associated with socket %S:%d (handle was [%Iu])"
        , inet_ntoa(pSockData->addr.sin_addr)
        , ntohs(pSockData->addr.sin_port)
        , pSockData->sock));
      delete(pSockData);
      CloseHandle(hMutex);
    }
    else
    {
      QTRACE((L"cSocketServer::doCloseSocket() - still waiting for %d replies", pSockData->outstandingRecvs));
      ReleaseMutex(hMutex);
    }
  }
}


//-------------------------------------------------------------------------------------------
void cSocketServer::NotifyRecvCompleted(cPtr<cIOBuffer> pIOBuffer, SOCKET_DATA *pSocketData)
{
  DoRecv(pSocketData);

  // post a new receive first to be quick
  {
    try
    {
      pSocketData->TCPStream.NotifyRecvCompleted(pIOBuffer, pSocketData);
    }
    catch (const iPacketProcessor::unknown_protocol&)
    {
      QSOS((L"cSocketServer::NotifyRecvCompleted - caught unknown protocol exception"));
      CloseSocket(pSocketData);
      return;
    }
  }

}


//-------------------------------------------------------------------------------------------
bool cSocketServer::AddCompletionKey(ULONG_PTR pCompletionKey)
{
  if (m_pCompletionPort.isValid())
    return m_pCompletionPort->AddCompletionKey(pCompletionKey);
  else
    QSOS((L"cSocketServer::AddCompletionKey - m_pCompletionPort invalid"));

  return false;
}


//-------------------------------------------------------------------------------------------
SOCKET_DATA* cSocketServer::AddClientCompletionKey(const SOCKET sock, const SOCKADDR_IN &krLocalSockAddrIn, const SOCKADDR_IN &krSockAddrIn)
{
  SOCKET_DATA *pSocketData = new SOCKET_DATA;
  pSocketData->sock = sock;
  pSocketData->localAddr = krLocalSockAddrIn;
  pSocketData->addr = krSockAddrIn;
  pSocketData->sequenceNumber = 0;
  pSocketData->outstandingRecvs = 0;
  pSocketData->hInitialise = CreateMutex(NULL, FALSE, NULL);
  if (NULL == pSocketData->hInitialise)
  {
    QSOS((L"cSocketServer::AddClientCompletionKey() - couldn't create mutex with error %d", GetLastError()));
    delete(pSocketData);
    return 0;
  }

  bool bRet = m_pCompletionPort->AddCompletionKey(reinterpret_cast<ULONG_PTR>(pSocketData));

  if (!bRet)
  {
    QSOS((L"cSocketServer::AddClientCompletionKey() - couldn't add completion key with error %d", GetLastError()));
    delete(pSocketData);
    return 0;
  }

  return pSocketData;
}


void cSocketServer::txMsgDone(void)
{
  ReleaseSemaphore (m_hTxSemaphore, 1, NULL);
}

bool cSocketServer::DoRecv(SOCKET_DATA *pSocketData)
{
  iActivity::Ptr activity = getActivity();
  activity->activityStart(kRecvDo);
  //return RecvNoMutex(pSocketData, activity);

  DWORD dwErr = WaitForSingleObject(pSocketData->hInitialise, INFINITE);
  if (dwErr != WAIT_OBJECT_0 && dwErr != WAIT_ABANDONED)
    return false;

  const bool bRet(RecvNoMutex(pSocketData, activity));
  ReleaseMutex(pSocketData->hInitialise);

  return bRet;
}

bool cSocketServer::RecvNoMutex(SOCKET_DATA *pSocketData, iActivity::Ptr activity)
{
  IO_DATA *pIOData = new IO_DATA;
  ZeroMemory(&(pIOData->overlapped), sizeof(OVERLAPPED));

  try
  {

    cPtr<cIOBuffer> pIOBuffer = new cIOBuffer(this);
    if(!pIOBuffer->getBuffer().isValid())
    {
      QSOS((L"cSocketServer::RecvNoMutex - Didn't get a valid buffer from packet processor"));
      throw std::exception();
    }

    pIOData->pMsg = pIOBuffer;

    DWORD dwRet;
    {
      //From: http://msdn.microsoft.com/en-us/library/windows/desktop/ms741688(v=vs.85).aspx
      //If you are using I/O completion ports, be aware that the order of calls made to WSARecv 
      //is also the order in which the buffers are populated. WSARecv should not be called on the 
      //same socket simultaneously from different threads, because it can result in an unpredictable buffer order.
      cLockGuard lg(&m_RecvLock);//This ensures the order of the pSocketData->sequenceNumber and WSARecv calls ...
      activity->activityStartId(kRecvWSARecv, (int)pIOBuffer.getRaw());
      InterlockedIncrement(&(pSocketData->outstandingRecvs));
      pIOBuffer->setSequenceNumber(InterlockedIncrement(&pSocketData->sequenceNumber));
      DWORD flags = 0;
      dwRet = WSARecv(pSocketData->sock
        , pIOBuffer->getWSABuf()
        , 1
        , 0 //pIOBuffer->getRecvBytes() - JCJCJC This was WRONG - RTM! By passing this address, our data would VERY RARELY corrupt - evil!
        , &flags
        , reinterpret_cast<LPOVERLAPPED>(pIOData)
        , NULL);
    }

    if (dwRet != ERROR_SUCCESS)
    {
      dwRet = GetLastError();
      if (dwRet != ERROR_IO_PENDING)
      {
        if ((dwRet == 10053) || (dwRet == 10054))
          QTRACE((L"cSocketServer::RecvNoMutex - Failed to WSARecv on socket [%Iu] with error [%d] - {%s}"
            , pSocketData->sock
            , dwRet
            , win32ErrorToString(dwRet).c_str()));
        else
          QSOS((L"cSocketServer::RecvNoMutex - Failed to WSARecv on socket [%Iu] with error [%d] - {%s}"
            , pSocketData->sock
            , dwRet
            , win32ErrorToString(dwRet).c_str()));
        throw std::exception();
      }
    }
  }
  catch (const std::exception&)
  {
    delete(pIOData);
    InterlockedDecrement(&(pSocketData->outstandingRecvs));
    InterlockedDecrement(&pSocketData->sequenceNumber);
    return false;
  }

  return true;
}

void cSocketServer::AddPacket(const cPtr<iRXPacket> pPacket)
{
  if (m_pPacketProcessor.isValid())
    m_pPacketProcessor->AddPacket(pPacket);
}

bool cSocketServer::AddPacket(cTXPacket *pPacket)
{
  iActivity::Ptr activity = getActivity();
  ::WaitForSingleObject (m_hTxSemaphore, INFINITE);
  cPtr<cTXMsg> pTXMsg = new cTXMsg(pPacket);

  cActivityScopeId as(activity, kSend, (int)(pTXMsg.getRaw()));
  IO_DATA *pIOData = new IO_DATA;
  ZeroMemory(pIOData, sizeof(OVERLAPPED));
  pIOData->pMsg = pTXMsg;
  LPOVERLAPPED pOverlapped = reinterpret_cast<LPOVERLAPPED>(pIOData);

  // [SQCF-1134/AJS] If we didn't manage to send the packet then the OVERLAPPED structure will not 
  //                 come back through the completion port so we must delete it here to reclaim 
  //                 resources (including releasing the semaphore through the cTXMsg destructor)
  bool hasGone = ((m_pTransmitPackets != NULL) ? doTransmitPackets(pOverlapped, pTXMsg) : doSend(pOverlapped, pTXMsg));

  if (!hasGone)
    delete pIOData;

  return hasGone;
}

DWORD cSocketServer::PacketSize(cPtr<cMemoryView> pBuffer, SOCKET_DATA* const pSockData, const DWORD dwReceivedBytes)
{
  //QTRACE((L"cSocketServer::HavePacket()"));
  if (m_pPacketProcessor.isNull())
    return 0;

  try
  {
    return m_pPacketProcessor->PacketSize(pSockData->sock, pBuffer, dwReceivedBytes);
  }
  catch (const iPacketProcessor::unknown_protocol&)
  {
    QSOS((L"cSocketServer::PacketSize - caught unknown protocol exception"));
    CloseSocket(pSockData);
    throw iPacketProcessor::unknown_protocol();
  }

  return 0;
}

cPtr<cMemoryView> cSocketServer::GetRXBuffer(unsigned int minLength)
{
  return m_pPacketProcessor.isValid() ? m_pPacketProcessor->GetRXBuffer(minLength) : cPtr<cMemoryView>();
}

void cSocketServer::createExtensionFunctions(const SOCKET kSocket)
{
  OSVERSIONINFO osvi;
  ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osvi);
  bool bIsWindowsServer2K3Later = 
      ( (osvi.dwMajorVersion > 5) ||
      ( (osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 2) ));

  if(bIsWindowsServer2K3Later)
  {
    //do not use these on Windows XP or Windows 7 - they blue screen for some reason!
    //In fact do not use these at all, they can (rarely) blue screen the Windows Server products too!

    //if (m_pTransmitPackets == NULL)
    //{
    //  DWORD bytes;
    //  GUID guidTransmitPackets = WSAID_TRANSMITPACKETS;
    //  int ret = WSAIoctl(kSocket
    //    , SIO_GET_EXTENSION_FUNCTION_POINTER
    //    , &guidTransmitPackets
    //    , sizeof(GUID)
    //    , &m_pTransmitPackets
    //    , sizeof( LPFN_TRANSMITPACKETS )
    //    , &bytes
    //    , NULL
    //    , NULL);
    //  if (ret != 0)
    //  {
    //    QTRACE((L"Can't get pointer to TransmitPackets() function for socket %d", kSocket));
    //    m_pTransmitPackets = NULL;
    //  }
    //}

    //if (m_pTransmitFile == NULL)
    //{
    //  DWORD bytes;
    //  GUID guidTransmitFile = WSAID_TRANSMITFILE;
    //  int ret = WSAIoctl(kSocket
    //    , SIO_GET_EXTENSION_FUNCTION_POINTER
    //    , &guidTransmitFile
    //    , sizeof(GUID)
    //    , &m_pTransmitFile
    //    , sizeof( LPFN_TRANSMITFILE )
    //    , &bytes
    //    , NULL
    //    , NULL);

    //  if (ret != 0)
    //  {
    //    QTRACE((L"Can't get pointer to TransmitFile() function for socket %d", kSocket));
    //    m_pTransmitFile = NULL;
    //  }
    //}
  }
}


bool cSocketServer::doTransmitPackets(LPOVERLAPPED pOverlapped, const cPtr<cTXMsg> pMsg) const
{
  bool bRet(false);
  try
  {
    TRANSMIT_PACKETS_ELEMENT *pTPE = pMsg->getTPE();
    if (pTPE == NULL)
    {
      QSOS((L"cSocketServer::doTransmitPackets() - no data to send"));
      throw -1;
    }

    if ((*m_pTransmitPackets)(pMsg->getSocket()
      , pTPE
      , pMsg->numPackets()
      , 0
      , pOverlapped
      , TF_USE_KERNEL_APC) == FALSE)
    {
      DWORD dw(GetLastError());
      if (dw != ERROR_IO_PENDING)
      {
        QSOS((L"cSocketServer::doTransmitPackets() - Failed on call to TransmitPackets on socket {%Iu} with error [%d] - {%s}"
          , pMsg->getSocket()
          , dw
          , win32ErrorToString(dw).c_str()));
        const int retries = pMsg->retries();
        throw (retries < m_maxRetryLimit) ? retries : dw;
      }
    }

    bRet = true;
  }
  catch (const DWORD err)
  {
    bRet = handleErr(err, pOverlapped, pMsg, &cSocketServer::doTransmitPackets);
  }

  return bRet;
}

bool cSocketServer::doSend(LPOVERLAPPED pOverlapped, const cPtr<cTXMsg> pMsg)
{
  // use ordinary send API since we don't have TransmitPackets

  // see if we should use TransmitFile
  int nNumHandles(0);
  const tTransmitList &transmitList(pMsg->getTransmitList());
  for (tTransmitList::const_iterator cit = transmitList.begin()
    ; cit != transmitList.end()
    ; ++cit)
  {
    const SMART_TPE &krTPE = *cit;
    if ((krTPE.tpe.dwElFlags & TP_ELEMENT_FILE) == TP_ELEMENT_FILE)
    {
      nNumHandles++;
    }
  }

  bool bRet(false);
  if (nNumHandles == 1 && m_pTransmitFile != NULL)
  {
    return doTransmitFile(pOverlapped, pMsg);
  }
  else
  {
    if (nNumHandles != 0)
    {
      QTRACE((L"Improve performance by doing multiple transmit file calls..."));
    }
    return doWSASend(pOverlapped, pMsg);
  }

  return bRet;
}

bool cSocketServer::doWSASend(LPOVERLAPPED pOverlapped, const cPtr<cTXMsg> pMsg) const
{
  bool bRet(false);
  try
  {
    WSABUF *pWSABuf = pMsg->getWSABuf();
    if (pWSABuf == NULL)
    {
      QSOS((L"cSocketServer::doWSASend() - empty send buffer"));
      throw -1;
    }
    if (WSASend(pMsg->getSocket()
      , pWSABuf
      , pMsg->numPackets()
      , 0
      , 0
      , pOverlapped
      , NULL) == SOCKET_ERROR)
    {
      DWORD dw(GetLastError());
      if (dw != ERROR_IO_PENDING)
      {
        QSOS((L"cSocketServer::doWSASend() - Failed on call to WSASend on socket {%Iu} with error [%d] - {%s}"
          , pMsg->getSocket()
          , dw
          , win32ErrorToString(dw).c_str()));
        const int retries = pMsg->retries();
        throw (retries < m_maxRetryLimit) ? retries : dw;
      }
    }

    bRet = true;
  }
  catch (const DWORD err)
  {
    bRet = handleErr(err, pOverlapped, pMsg, &cSocketServer::doWSASend);
  }

  return bRet;
}

bool cSocketServer::doTransmitFile(LPOVERLAPPED pOverlapped, const cPtr<cTXMsg> pMsg) const
{
  bool bRet(false);

  try
  {
    LPTRANSMIT_FILE_BUFFERS pTFB = pMsg->getTFB();
    if (pTFB == NULL)
    {
      QSOS((L"cSocketServer::doTransmitFile() - invalid TransmitFile buffers"));
      throw -1;
    }
    const TRANSMIT_PACKETS_ELEMENT *pTPE = pMsg->getTPE();
    if (pTPE == NULL)
    {
      QSOS((L"cSocketServer::doTransmitFile() - invalid TransmitFile file"));
      throw -1;
    }
    pOverlapped->Offset = pTPE->nFileOffset.LowPart;
    pOverlapped->OffsetHigh = pTPE->nFileOffset.HighPart;

    if ((*m_pTransmitFile)(pMsg->getSocket()
      , pTPE->hFile
      , pTPE->cLength
      , 0
      , pOverlapped
      , pTFB
      , TF_USE_KERNEL_APC ) == FALSE)
    {
      DWORD dw(GetLastError());
      if (dw != ERROR_IO_PENDING)
      {
        QSOS((L"cSocketServer::doTransmitFile() - Failed on call to TransmitFile on socket {%Iu} with error [%d] - {%s}"
          , pMsg->getSocket()
          , dw
          , win32ErrorToString(dw).c_str()));
        const int retries = pMsg->retries();
        throw (retries < m_maxRetryLimit) ? retries : dw;
      }
    }
    
    bRet = true;
  }
  catch (const DWORD err)
  {
    bRet = handleErr(err, pOverlapped, pMsg, &cSocketServer::doTransmitFile);
  }

  return bRet;
}

bool cSocketServer::handleErr(DWORD err, LPOVERLAPPED pOverlapped, const cPtr<cTXMsg> pMsg, tProcessFuncPtr pFunc) const
{
  bool bRet(false);
  if (err >= 0)
  {
    if (err < m_maxRetryLimit)
    {
      QTRACE((L"cSocketServer::handleErr() - Retry %d", err));
      bRet = (this->*pFunc)(pOverlapped, pMsg);
    }
    else
    {
      // disconnect -> too many retries
      QSOS((L"cSocketServer::handleErr() - Disconnect because of error %d", err));
    }
  }
  return bRet;
}

  int cSocketServer::numOfClients()
  {
    return (int) m_SocketData.size();
  }


  std::string cSocketServer::clientIP(int index)
  {
    tSocketDataList::const_iterator it = m_SocketData.begin();
    while (index)
    {
      ++it;
      --index;
    }
    std::string remoteIP = inet_ntoa((*it)->addr.sin_addr);
    //QTRACE ((L"IP:: %S",remoteIP.c_str()));

    return remoteIP;
  }


