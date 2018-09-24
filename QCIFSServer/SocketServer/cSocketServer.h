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

//cSocketServer.h

#pragma once

#include "isocketserver.h"
#include "cTCPStream.h"

class iPacketProcessor;
class cIOBuffer;
class iCompletionMsg;
class cListeningSocket;
class iRXPacket;
class iTXPacket;
class cTXMsg;
class cTXPacket;
class iIOCompletionPort;
class cCloseSocketSerialiseThread;

struct SOCKET_DATA
{
  SOCKET sock;
	SOCKADDR_IN localAddr;
	SOCKADDR_IN addr;
  volatile LONG sequenceNumber;
  volatile LONG outstandingRecvs;
  HANDLE hInitialise;
  cTCPStream TCPStream;
};

class cSocketServer : public iSocketServer, public vfs::cRefCount
{
public:
  friend class cCloseSocket;

  cSocketServer(const vfs::cPtr<iPacketProcessor> pPacketProcessor
    , const std::vector<USHORT> &listeningPorts);
  ~cSocketServer(void);

  //iSocketServer
  void notifyExit();
  void closeSession(UINT_PTR SessionID);
  void closeClientSession(UINT_PTR SessionID);
  bool addClientConnection(const USHORT clientPort, const in_addr& remoteAddr, const USHORT remotePort, const UINT_PTR associatedSessionID);
  bool addListeningConnection(UINT_PTR& rListenSessionID, const USHORT listeningPort, sockaddr_in& rListeningSockAddr, const UINT_PTR associatedSessionID);
  vfs::cPtr<iTXPacket> getTXPacketForSession(UINT_PTR sessionID);

  int numOfClients();
  std::string clientIP(int index);


  //public
  bool AddSocket(const SOCKET sock
    , const SOCKADDR_IN &krLocalSockAddrIn
    , const SOCKADDR_IN &krRemoteSockAddrIn
    , const vfs::cPtr<vfs::cMemoryView> pRecvMem
    , const UINT_PTR associatedSessionID);
  bool AddClientSocket(SOCKET_DATA* pSocketData, const UINT_PTR associatedSessionID);
  void onRecvErr(SOCKET_DATA *pSockData);
  void CloseSocket(SOCKET_DATA *pSockData);
  bool DoRecv(SOCKET_DATA *pSocketData);

  void AddPacket(const vfs::cPtr<iRXPacket> pPacket);
  bool AddPacket(cTXPacket *pPacket);

  DWORD PacketSize(vfs::cPtr<vfs::cMemoryView> pBuffer, SOCKET_DATA* const pSockData, const DWORD dwReceivedBytes);
  vfs::cPtr<vfs::cMemoryView> GetRXBuffer(unsigned int minLength = 0);

  void NotifyRecvCompleted(vfs::cPtr<cIOBuffer> pIOBuffer, SOCKET_DATA *pSocketData);

  bool AddCompletionKey(ULONG_PTR pCompletionKey);
  SOCKET_DATA* AddClientCompletionKey(const SOCKET sock, const SOCKADDR_IN &krLocalSockAddrIn, const SOCKADDR_IN &krSockAddrIn);

  void txMsgDone(void);
  void logSockets();

private:
  void init(const vfs::cPtr<iPacketProcessor> pPacketProcessor
    , const std::vector<USHORT> &listeningPorts);
  void startListeningSocket(USHORT listeningPort);
  void createExtensionFunctions(const SOCKET kSocket);
  bool doTransmitPackets(LPOVERLAPPED pOverlapped, const vfs::cPtr<cTXMsg> pMsg) const;
  bool doSend(LPOVERLAPPED pOverlapped, const vfs::cPtr<cTXMsg> pMsg);
  bool doWSASend(LPOVERLAPPED pOverlapped, const vfs::cPtr<cTXMsg> pMsg) const;
  bool doTransmitFile(LPOVERLAPPED pOverlapped, const vfs::cPtr<cTXMsg> pMsg) const;
  void doCloseSocket(SOCKET_DATA *pSockData);

  typedef bool (cSocketServer::* tProcessFuncPtr)(LPOVERLAPPED pOverlapped
    , const vfs::cPtr<cTXMsg> pMsg) const;

  bool handleErr(DWORD err
    , LPOVERLAPPED pOverlapped
    , const vfs::cPtr<cTXMsg> pMsg
    , tProcessFuncPtr pFunc) const;

  bool saveSocketDataAndRecv(SOCKET_DATA *pSocketData);
  bool RecvNoMutex(SOCKET_DATA *pSocketData, vfs::iActivity::Ptr activity);

private:
  static const int m_InitialPendingRecvs;
  static const int m_maxRetryLimit;

  vfs::cPtr<iPacketProcessor> m_pPacketProcessor;
  const vfs::cPtr<iIOCompletionPort> m_pCompletionPort;

  LPFN_TRANSMITFILE m_pTransmitFile;
  LPFN_TRANSMITPACKETS m_pTransmitPackets;
  bool m_bRun;

  typedef std::map<int, vfs::cPtr<vfs::cMemoryView> > tPacketMap;
  tPacketMap m_packetMap;

  typedef std::set<SOCKET_DATA *> tSocketDataList;
  tSocketDataList m_SocketData;

  vfs::cLock m_RecvLock;
  vfs::cLock m_SocketDataLock;
  HANDLE m_hTxSemaphore;

  vfs::cPtr<cCloseSocketSerialiseThread> m_pCloseSocketThread;
  vfs::cGuard::ConstPtr SocketConsoleCommand;
};


class cSocketServerFactory : public iSocketServerFactory, public vfs::cRefCount
{
public:
  QDEFINE_SINGLETON_IMPL(cSocketServerFactory, L"{7ECA0CD6-CA57-4f4e-83A1-A76F2929D098} // cSocketServerFacotry", iSocketServerFactory);

  cSocketServerFactory();
  ~cSocketServerFactory(void);

  void QAPI logSummary() {}
  void QAPI logDetailedSummary() {}

  vfs::cPtr<iSocketServer> createSocketServer(const vfs::cPtr<iPacketProcessor> pPacketProcessor
    , const std::vector<USHORT> &listeningPorts);

  std::vector<vfs::String> getIpAddresses();
};
