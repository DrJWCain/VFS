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

// threads.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iThreadHeader.h"
#include "cMemory.h"
#include "iFile.h"
#include "primatives.h"

class cMyThread : public iThread
{
public:
  virtual void runLoop()
  {
    printf("Hello\n");
    Sleep(1000);
    printf("There\n");
  }
};

class cMyMsg : public iMsg, public cRefCount
{
  void deliver()
  {
    printf("begin\n");
    Sleep(500);
    printf("end\n");
  }
};

class cMyMsgQueue : public iMsgQueue, public cRefCount
{
};


///////////////////////////////////////////////////////////////////////////////

class test : public cRefCount
{
  std::string Param;
public:
  test(std::string param) : Param(param)
  {
  }
  void func() 
  {
    printf("func %s\n",Param.c_str());
  }
  void hello() 
  {
    printf("hello %s\n",Param.c_str());
  }
};

///////////////////////////////////////////////////////////////////////////////

unsigned int threadID()
{
  return GetCurrentThreadId();
}

class cMyCompletionQueue : public iLimitedCompletionQueue
{
public:
  cMyCompletionQueue(cPtr<iFile> file, unsigned int maxMessages) : iLimitedCompletionQueue(file, maxMessages)
  {
  }
  void onReadBytes(std::list<cPtr<cMemory> > memory, unsigned __int64 where, unsigned int numBytes)
  {
    unsigned int sz = memory.front()->getSize();
    unsigned char* bytes = memory.front()->getBytes();
    unsigned char checkSum = 0;
    for(unsigned int i = 0; i < sz; ++i)
    {
      checkSum = checkSum ^ bytes[i];
    }
    printf("%d:%I64x:%d\n",threadID(),where,checkSum);
    //printf("%I64x:%d\n",where,checkSum);
  }
  void onWriteBytes(cPtr<cMemory> memory, unsigned __int64 where, unsigned int numBytes)
  {
  }
};

///////////////////////////////////////////////////////////////////////////////

bool openWinsock()
{	
	WSADATA wsd;
	int ret;
	if((ret = WSAStartup(MAKEWORD(2,2), &wsd)) != 0)
	{
		printf("WSAStartup failed, return code %d", ret);
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////

#include "cqp_header.h"
#include "qcp_header.h"

class cCqpClient
{
  SOCKET socket_;
  unsigned int myaddr_;
public:
  cCqpClient();
  ~cCqpClient();
  bool open();
  void close();
  bool request(int session);
};

cCqpClient::cCqpClient()
{
  socket_ = 0;
}

cCqpClient::~cCqpClient()
{
  close();
}

void cCqpClient::close()
{
	if(socket_) {
		closesocket(socket_);
		socket_ = 0;
	}
}

#define QCP_PORT_DST 2530
#define CQP_PORT_DST 2531
#define SERVER_IP 0x0aa5280b

bool cCqpClient::open()
{
  if(socket_) return true;

	socket_ = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	if(socket_ == INVALID_SOCKET) {
		printf("CqpClient: Error %d when creating socket",WSAGetLastError());
		close();
		return false;
	}

  const unsigned int addr_ = SERVER_IP;
  const unsigned short port_ = CQP_PORT_DST;

	sockaddr_in sockAddr; 
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(addr_);
	sockAddr.sin_port = htons( port_ );

	// Connect to server.
	if(WSAConnect(socket_, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr),0,0,0,0) == SOCKET_ERROR) 
  {
		printf("CqpClient: Failed to connect to %d.%d.%d.%d:%d with error %d",addr_>>24,addr_>>16&0xff,addr_>>8&0xff,addr_&0xff,port_,WSAGetLastError());
		close();
		return false;
	}

	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&bOptVal), bOptLen);
	setsockopt(socket_, SOL_SOCKET, SO_KEEPALIVE, reinterpret_cast<char*>(&bOptVal), bOptLen);
	setsockopt(socket_, SOL_SOCKET, SO_DONTLINGER, reinterpret_cast<char*>(&bOptVal), bOptLen);
	setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&bOptVal), bOptLen);

	sockaddr_in name;
	int namelen = sizeof(name);
	if(getsockname(socket_, reinterpret_cast<sockaddr*>(&name), &namelen)==SOCKET_ERROR) 
  {
		printf("CqpClient: Error %d from getsockname()",WSAGetLastError());
		close();
		return false;
	}

	myaddr_ = ntohl(name.sin_addr.S_un.S_addr);

	return true;
}

const unsigned int kNumBytesPayload = 1048576;
const unsigned int kPayloadPerPacket = 8832;
const unsigned int kLastPacketNum = kNumBytesPayload / kPayloadPerPacket;
const unsigned int kMaxWindow = 64;
//const unsigned int kMaxWindow = 16;
const unsigned int kHalfWindow = kMaxWindow / 2;

bool cCqpClient::request(int session)
{
  if(!open())
    return false;

	tQServerBodyV3 reply;

	// Form the Cqp SReceiveV3 command structure
	tQServerCmdV3 cmd;
	cmd.hdr.magic        = htonl(eQServerMagic);
	cmd.hdr.cmd          = htonl(eQServerSendV3);
	cmd.hdr.len          = htonl(sizeof(cmd));
	cmd.body.session     = htonl(session);
	cmd.body.destIp      = htonl(myaddr_);

	cmd.body.destPort    = htonl(QCP_PORT_DST);
	cmd.body.maxPacket   = htonl(kPayloadPerPacket);
	cmd.body.maxWindow   = htonl(kMaxWindow);
	cmd.body.chunkBytes  = htonl(kNumBytesPayload);
	cmd.body.chunkUnit   = htonl(128);

	cmd.body.poolID      = htonl(2);
	cmd.body.startAtomMS = htonl(0);
	cmd.body.startAtomLS = htonl(45839);
	cmd.body.startSkew   = htonl(0);
	cmd.body.numAtoms    = htonl(1);
	cmd.body.rushIDFMS   = htonl(0);
	cmd.body.rushIDFLS   = htonl(0);
	cmd.body.rushIDSMS   = htonl(0x00010000);
	cmd.body.rushIDSLS   = htonl(0x00000212);
	cmd.body.rushFrame   = htonl(180858);
	cmd.body.format      = htonl(78);
	cmd.body.ticket      = htonl(0);
	cmd.body.priority    = htonl(0);	

	// Send it
	int bytes = send(socket_,reinterpret_cast<char*>(&cmd), sizeof(cmd),0);
  if(sizeof(cmd) != bytes)
  {
		printf("send returned %d\n",bytes);
		close();
    return false;
  }

	// Wait for reply from server
	struct fd_set readFds;
	static struct timeval timeout = { 3, 0 }; // secs,usecs

	FD_ZERO(&readFds);
	FD_SET(socket_, &readFds);
	int sel;

	if((sel = select(0, &readFds, NULL, NULL, &timeout)) == SOCKET_ERROR) 
  {
		printf("CqpClient: select error\n");
		close();
    return false;
	}
  else if (sel==0) 
  {
		printf("CqpClient: socket recv timeout\n");
		close();
    return false;
	}

	tQServerHdr hdr;
  int numBytes = recv(socket_,reinterpret_cast<char*>(&hdr), sizeof(hdr), 0);
  if(!numBytes)
  {
    printf("connection closed\n");
		close();
    return false;
  }
	else if(SOCKET_ERROR == numBytes) 
  {
		printf("CqpClient: error on recv %d", WSAGetLastError());
		close();
    return false;
	} 

	hdr.magic = ntohl(hdr.magic);
	hdr.cmd   = ntohl(hdr.cmd);
	hdr.len   = ntohl(hdr.len);

	if(hdr.magic != eQServerMagic) 
  {
		printf("CqpClient: invalid magic\n");
		close();
		return false;
	}

	if(hdr.cmd == eQServerReject) 
  {
		tQServerRejectBody body;
		if(recv(socket_,reinterpret_cast<char*>(&body), hdr.len - sizeof(hdr), 0) < static_cast<int>(hdr.len - sizeof(hdr)))
    {
			printf("CqpClient: can't get reason\n");
    }
		close();
		return false;
	}

	if(hdr.cmd != eQServerAcceptV3) 
  {
		printf("CqpClient: ACCEPTV3 not received\n");
		close();
		return false;
	}

	if(recv(socket_,reinterpret_cast<char*>(&reply), hdr.len - sizeof(hdr), 0) < static_cast<int>(hdr.len - sizeof(hdr))) 
  {
		printf("CqpClient: incomplete message received\n");
		close();
		return false;
	}

	reply.session     = ntohl(reply.session);
	reply.destIp      = ntohl(reply.destIp);
	reply.destPort    = ntohl(reply.destPort);
	reply.maxPacket   = ntohl(reply.maxPacket);
	reply.maxWindow   = ntohl(reply.maxWindow);
	reply.chunkBytes  = ntohl(reply.chunkBytes);
	reply.chunkUnit   = ntohl(reply.chunkUnit);

  printf("SSend %d.%d.%d.%d %d R:%x%08x, %d, A:%08x-%08x %d\n",
    reply.destIp>>24,reply.destIp>>16&0xff,reply.destIp>>8&0xff,reply.destIp&0xff,reply.session,
		ntohl(cmd.body.rushIDSMS), ntohl(cmd.body.rushIDSLS), ntohl(cmd.body.rushFrame), ntohl(cmd.body.startAtomMS), ntohl(cmd.body.startAtomLS),
    reply.chunkBytes);

  return true;
}

///////////////////////////////////////////////////////////////////////////////

class cCQPCompletionQueue : public iCompletionQueue
{
  //cPtr<cMemory> Buffer;
  //cEvent Event;
public:
  cCQPCompletionQueue(cPtr<iHandle> handle) : iCompletionQueue(handle)
  {
    //Buffer = new cAllocatedMemory(kNumBytesPayload);
  }

  //cPtr<cMemory> waitThunk()
  //{
    //Event.wait();
    //return Buffer;
  //}

  void sendWindow(unsigned short streamNumber, unsigned int sessionNumber,
	  unsigned int chunkStart, unsigned int chunkEnd, unsigned int offsetStart, unsigned int offsetEnd)
  {
    if(offsetEnd > kNumBytesPayload)
    {
      chunkEnd = 1;
      offsetEnd = 0;
    }

    //printf("sendWindow %d-%d\n", offsetStart, offsetEnd);
    {
      cPtr<cMemory> memory = new cAllocatedMemory(sizeof(tClipnetHeaderWindow));
	    tClipnetHeaderWindow *win = reinterpret_cast<tClipnetHeaderWindow*>(memory->getBytes());
	    win->command         = eQcpCommandWindow;
	    win->_pad            = 0;
	    win->session         = htonl(sessionNumber);
	    win->stream          = htons(streamNumber);
	    win->complete_chunk  = htonl(chunkStart);
	    win->complete_offset = htonl(offsetStart);
	    win->window_chunk    = htonl(chunkEnd);
	    win->window_offset   = htonl(offsetEnd);

      writeBytes(0, memory);
    }
  }

  void onReadBytes(unsigned int count, std::list<cPtr<cMemory> > memory, unsigned __int64 where, unsigned int numBytes)
  {
    unsigned char* buffer[10];
    int i = 0;
    for(std::list<cPtr<cMemory> >::iterator it = memory.begin(); it != memory.end(); ++it)
    {
      buffer[i] = (*it)->getBytes();
      ++i;
    }
    tClipnetHeader* header = (tClipnetHeader*)buffer[0];
    if(eQcpCommandNoData == header->command)
    {
      printf("no data\n");
      return;
    }
    if(eQcpCommandData == header->command)
    {
      tClipnetHeaderData* dataHeader = (tClipnetHeaderData*)header;

      unsigned int stream = ntohl(header->stream);
      unsigned int session = ntohl(header->session);
      unsigned int chunk = ntohl(dataHeader->chunk);
      unsigned int fragment_offset = ntohl(dataHeader->fragment_offset);
      unsigned int fragment_length = ntohl(dataHeader->fragment_length);
      unsigned int request = fragment_offset / kPayloadPerPacket;

      if(request != count)
      {
        printf("%d, %d, %d, %d, %d. %d, %d-%d %d\n",
          count, header->command, numBytes, stream, session,
          chunk,fragment_offset,fragment_length,request);
      }

      //memcpy(Buffer->getBytes() + fragment_offset,buffer[1],fragment_length);

      if(kLastPacketNum == request)
      {
        //Event.set();
        sendWindow(stream, session, 1, 1, 0, 0);
      }
      else if(0 == (request % kHalfWindow))
      {
        sendWindow(stream, session, 0, 0, fragment_offset, fragment_offset+(fragment_length*kMaxWindow));
      }

      return;
    }

    printf("%d, %d\n",header->command,numBytes);
  }
  void onWriteBytes(unsigned int count, std::list<cPtr<cMemory> > memory, unsigned __int64 where, unsigned int numBytes)
  {
    //printf("sent bytes %d\n",numBytes);
  }
};

///////////////////////////////////////////////////////////////////////////////

class cTimer
{
  LARGE_INTEGER StartTime;
public:
  void start()
  {
    QueryPerformanceCounter(&StartTime);
  }
  int stop()
  {
    LARGE_INTEGER stopTime;
    QueryPerformanceCounter(&stopTime);
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    LARGE_INTEGER result;
    result.QuadPart = ((stopTime.QuadPart - StartTime.QuadPart)*1000)/freq.QuadPart;
    return result.LowPart;
  }
};

///////////////////////////////////////////////////////////////////////////////
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//  openWinsock();
//
//  cCqpClient cqp;
//  cPtr<iHandle> handle = new iSocket(QCP_PORT_DST,SERVER_IP,QCP_PORT_DST);
//  cPtr<cCQPCompletionQueue> completion = new cCQPCompletionQueue(handle);
//
//  cTimer timer;
//  timer.start();
//  unsigned int session = 0;
//  while (session < 150) //ten seconds HD Dv100 = 150 thunks
//  {
//    cPtr<cMemory> headers = new cAllocatedMemory(sizeof(tClipnetHeaderData) * (kLastPacketNum+1));
//    cPtr<cMemory> payloads = new cAllocatedMemory(kPayloadPerPacket * (kLastPacketNum+1));
//
//    for(int i = 0; i <= kLastPacketNum; ++i)
//    {
//      std::list<cPtr<cMemory> > memories;
//      cPtr<cMemory> header = new cMemoryView(headers, i * sizeof(tClipnetHeaderData), sizeof(tClipnetHeaderData));
//      memories.push_back(header);
//      cPtr<cMemory> payload = new cMemoryView(payloads, i * kPayloadPerPacket, kPayloadPerPacket);
//      memories.push_back(payload);
//      completion->readMemories(0, memories);
//    }
//    cqp.request(session++);
//
//    //cPtr<cMemory> mem = completion->waitThunk();
//    completion->waitComplete();
//  }
//
//  printf("%dms",timer.stop());
//
///*
//  {
//    cMyMsgQueue q;
//
//    cPtr<iMsg> msg = q.peekMessage();//non blocking null return
//    if(msg.isValid())
//      msg->deliver();
//
//    q.postMessage(new cMyMsg);
//
//    msg = q.peekMessage();//non blocking valid return
//    if(msg.isValid())
//      msg->deliver();
//  }
//*/
///*
//  {
//    cPtr<iCompletionQueue> completion = new cMyCompletionQueue(
//      new iFile("c:\\backup\\kernel20051223.zip"),
//      //new iFile("c:\\videos\\paycheck.avi"),
//      10);
//
//    const int sz = 4096*128;
//    for(int i = 0; i < 1024; ++i)
//    {
//      unsigned short ran = (unsigned short)rand();
//      int where = MulDiv(sz * 1024,ran,(1 << 16));
//      printf("%x\n",where);
//      completion->readBytes(where,sz);
//    }
//
//    completion->waitComplete();
//  }
//*/
///*
//  {
//    monitor<test> t = test("some parameter");
//
//    t->func();//locked function call by default
//    //lock released here
//    t->hello();//locked function call by default
//
//    t.unsafe()->func();//obvious at call site unlocked function call
//
//    { //lock held for duration of braces
//      monitor_guard<test> safe_t = t.lock();
//      safe_t->func();
//      safe_t->hello();
//    }
//  }
//  {
//    monitor_ptr<test> t_ptr = new test("some parameter");
//    t_ptr->func();
//  }
//*/
///*
//  printf("Start\n");
//
//  //async proc call
//  new cMyThread;
//
//  //queued proc call
//  cPtr<iThreadMsgQueue> threadMsgQueue = new iThreadMsgQueue;
//  threadMsgQueue->postMessage(new cMyMsg);
//  threadMsgQueue->postMessage(new cQuitMessage);
//  threadMsgQueue->waitLoopComplete();//blocking
//
//  //async proc call with wait
//  cPtr<cMyThread> myThread = new cMyThread;
//  myThread->waitLoopComplete();
//
//  //queued proc calls with outstanding limit
//  cPtr<iThreadLimitedMsgQueue> threadLimitedMsgQueue = new iThreadLimitedMsgQueue(1);
//  threadLimitedMsgQueue->postMessage(new cMyMsg);
//  threadLimitedMsgQueue->postMessage(new cMyMsg);
//  threadLimitedMsgQueue->postMessage(new cQuitMessage);//blocks for a while because of MsgQueue limit
//  printf("james\n");
//
//  threadLimitedMsgQueue->waitLoopComplete();//blocking
//
//  cPtr<iThreadPoolMsgQueue> threadPoolMsgQueue = new iThreadPoolMsgQueue(10);
//  threadPoolMsgQueue->postMessage(new cMyMsg);
//  threadPoolMsgQueue.invalidate();//kills all the threads (blocking)
//
//  printf("Stop\n");
//*/
//
//	return 0;
//}

