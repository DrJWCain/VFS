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

// qcp_header.h
// 
//

#pragma once

enum eQcpCommand {
    eQcpCommandData   = 0x1,
    eQcpCommandWindow = 0x2,
    eQcpCommandResend = 0x3,
    eQcpCommandNoData = 0x4,
};

struct tClipnetHeader {
    unsigned char command;
    unsigned char _pad;
    unsigned short stream;
    unsigned int session;

    unsigned int args_0;
    unsigned int args_1;
    unsigned int args_2;
    unsigned int args_3;
};

struct tClipnetHeaderData {
    unsigned char command;     // 1
    unsigned char _pad;
    unsigned short stream;
    unsigned int session;

    unsigned int chunk;
    unsigned int fragment_length;
    unsigned int fragment_offset;
    unsigned int _reserved;
};

struct tClipnetHeaderWindow {
    unsigned char command;     // 2
    unsigned char _pad;
    unsigned short stream;
    unsigned int session;

    unsigned int complete_chunk;
    unsigned int complete_offset;
    unsigned int window_chunk;
    unsigned int window_offset;
};

struct tClipnetHeaderResend {
    unsigned char command;     // 3
    unsigned char _pad;
    unsigned short stream;
    unsigned int session;

    unsigned int start_chunk;
    unsigned int start_offset;
    unsigned int end_chunk;
    unsigned int end_offset;
};

struct tClipnetHeaderNoData {
    unsigned char command;     // 4
    unsigned char _pad;
    unsigned short stream;
    unsigned int session;

    unsigned int start_chunk;
    unsigned int start_offset;
    unsigned int end_chunk;
    unsigned int end_offset;
};

#define QCP_HEADER_SIZE  (14+20+8+24) // Ether + IP + UDP + Clipnet
#define MAX_SEND_PACKETS 128
#define QCP_PORT_DST     2530