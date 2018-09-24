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

// cqp_header.h
// 
//

#pragma once

#pragma pack(1)

#define TESTADDR "10.162.45.56"
#define CQP_PORT_DST 2531
#define Q_PORT_CBDST 2532

struct tQServerHdr {
    unsigned int magic;
    unsigned int cmd;
    unsigned int len;
};

struct tQServerBody {
    unsigned int session;
    unsigned int destIp;
    unsigned int destPort;
    unsigned int maxPacket;
    unsigned int maxWindow;
    unsigned int chunkBytes;
    unsigned int chunkUnit;
    unsigned int poolID;
    unsigned int startFrame;
    unsigned int numFrames;
    unsigned int rushIDMS;
    unsigned int rushIDLS;
    unsigned int rushFrame;
    unsigned int format;
    unsigned int ticket;
    unsigned int priority;
};

struct tQServerBodyV3 {
    unsigned int session;
    unsigned int destIp;
    unsigned int destPort;
    unsigned int maxPacket;
    unsigned int maxWindow;
    unsigned int chunkBytes;
    unsigned int chunkUnit;
    unsigned int poolID;
    unsigned int startAtomMS;
    unsigned int startAtomLS;
    unsigned int startSkew;
    unsigned int numAtoms;
    unsigned int rushIDFMS;
    unsigned int rushIDFLS;
    unsigned int rushIDSMS;
    unsigned int rushIDSLS;
    unsigned int rushFrame;
    unsigned int format;
    unsigned int ticket;
    unsigned int priority;
};

struct tQServerGetFragBody {
    unsigned int type;
    unsigned int fragNum;
    unsigned int vol;
    unsigned int file;
    unsigned int seq;
};

struct tQServerFragListBody {
    unsigned int type;
    unsigned int fragNum;
    unsigned int vol;
    unsigned int file;
    unsigned int seq;
    unsigned int numFrags;
    unsigned int chunkBytes;
    unsigned int chunkUnit;
    unsigned int poolID;
    unsigned int startFrame;
    unsigned int numFrames;
    unsigned int rushIDMS;
    unsigned int rushIDLS;
    unsigned int rushFrame;
    unsigned int format;
};

struct tQServerCmd {
    tQServerHdr hdr;
    tQServerBody body;
};

struct tQServerCmdV3 {
    tQServerHdr hdr;
    tQServerBodyV3 body;
};

struct tQServerGetFragCmd {
    tQServerHdr hdr;
    tQServerGetFragBody body;
};

struct tQServerFragReply {
    tQServerHdr hdr;
    tQServerFragListBody body;
};

enum eQRejectReason {
    eQRejectMisc=0,
    eQRejectNotReadEnabled,
    eQRejectNotWriteEnabled,
    eQRejectOutOfSessions,
};

struct tQServerRejectBody {
    unsigned int session;
    unsigned int reason;
    char text[128];
};

struct tQServerReject {
    tQServerHdr hdr;
    tQServerRejectBody body;
};

enum { eQServerMagic = 'Q' << 24 | 'N' << 16 | 'E' << 8 | 'T' };

enum { eTypeVideo=1, eTypeAudio=2 };

enum eQServerCmd {
    eQServerReceive=1,
    eQServerSend,
    eQServerAccept,
    eQServerGetFrag,
    eQServerFragList,
    eQServerReject,
    eQServerPost,
    eQServerReceiveV3,
    eQServerSendV3,
    eQServerAcceptV3,
};