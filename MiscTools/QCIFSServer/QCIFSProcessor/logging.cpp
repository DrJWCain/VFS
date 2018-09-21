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

//logging.cpp

#include "stdafx.h"

#if CIFS 

#include "mycifs.h"

#define XLAT_STRING(code) static const char STRING_##code[] = #code
#define XLAT_CASE(code) case code: return STRING_##code
#define XLAT_STRING_DEFAULT XLAT_STRING(Unknown)
#define XLAT_CASE_DEFAULT default: return STRING_Unknown

const char*
SmbUnparseCommand(
    UCHAR command
    )
{
  XLAT_STRING_DEFAULT;
  XLAT_STRING(SMB_COM_CREATE_DIRECTORY);
  XLAT_STRING(SMB_COM_DELETE_DIRECTORY);
  XLAT_STRING(SMB_COM_OPEN);
  XLAT_STRING(SMB_COM_CREATE);
  XLAT_STRING(SMB_COM_CLOSE);
  XLAT_STRING(SMB_COM_FLUSH);
  XLAT_STRING(SMB_COM_DELETE);
  XLAT_STRING(SMB_COM_RENAME);
  XLAT_STRING(SMB_COM_QUERY_INFORMATION);
  XLAT_STRING(SMB_COM_SET_INFORMATION);
  XLAT_STRING(SMB_COM_READ);
  XLAT_STRING(SMB_COM_WRITE);
  XLAT_STRING(SMB_COM_LOCK_BYTE_RANGE);
  XLAT_STRING(SMB_COM_UNLOCK_BYTE_RANGE);
  XLAT_STRING(SMB_COM_CREATE_TEMPORARY);
  XLAT_STRING(SMB_COM_CREATE_NEW);
  XLAT_STRING(SMB_COM_CHECK_DIRECTORY);
  XLAT_STRING(SMB_COM_PROCESS_EXIT);
  XLAT_STRING(SMB_COM_SEEK);
  XLAT_STRING(SMB_COM_LOCK_AND_READ);
  XLAT_STRING(SMB_COM_WRITE_AND_UNLOCK);
  XLAT_STRING(SMB_COM_READ_RAW);
  XLAT_STRING(SMB_COM_READ_MPX);
  XLAT_STRING(SMB_COM_READ_MPX_SECONDARY);    // server to redir only
  XLAT_STRING(SMB_COM_WRITE_RAW);
  XLAT_STRING(SMB_COM_WRITE_MPX);
  XLAT_STRING(SMB_COM_WRITE_MPX_SECONDARY);
  XLAT_STRING(SMB_COM_WRITE_COMPLETE);    // server to redir only
  XLAT_STRING(SMB_COM_QUERY_INFORMATION_SRV);
  XLAT_STRING(SMB_COM_SET_INFORMATION2);
  XLAT_STRING(SMB_COM_QUERY_INFORMATION2);
  XLAT_STRING(SMB_COM_LOCKING_ANDX);
  XLAT_STRING(SMB_COM_TRANSACTION);
  XLAT_STRING(SMB_COM_TRANSACTION_SECONDARY);
  XLAT_STRING(SMB_COM_IOCTL);
  XLAT_STRING(SMB_COM_IOCTL_SECONDARY);
  XLAT_STRING(SMB_COM_COPY);
  XLAT_STRING(SMB_COM_MOVE);
  XLAT_STRING(SMB_COM_ECHO);
  XLAT_STRING(SMB_COM_WRITE_AND_CLOSE);
  XLAT_STRING(SMB_COM_OPEN_ANDX);
  XLAT_STRING(SMB_COM_READ_ANDX);
  XLAT_STRING(SMB_COM_WRITE_ANDX);
  XLAT_STRING(SMB_COM_CLOSE_AND_TREE_DISC);
  XLAT_STRING(SMB_COM_TRANSACTION2);
  XLAT_STRING(SMB_COM_TRANSACTION2_SECONDARY);
  XLAT_STRING(SMB_COM_FIND_CLOSE2);
  XLAT_STRING(SMB_COM_FIND_NOTIFY_CLOSE);
  XLAT_STRING(SMB_COM_TREE_CONNECT);
  XLAT_STRING(SMB_COM_TREE_DISCONNECT);
  XLAT_STRING(SMB_COM_NEGOTIATE);
  XLAT_STRING(SMB_COM_SESSION_SETUP_ANDX);
  XLAT_STRING(SMB_COM_LOGOFF_ANDX);
  XLAT_STRING(SMB_COM_TREE_CONNECT_ANDX);
  XLAT_STRING(SMB_COM_QUERY_INFORMATION_DISK);
  XLAT_STRING(SMB_COM_SEARCH);
  XLAT_STRING(SMB_COM_FIND);
  XLAT_STRING(SMB_COM_FIND_UNIQUE);
  XLAT_STRING(SMB_COM_FIND_CLOSE);
  XLAT_STRING(SMB_COM_NT_TRANSACT);
  XLAT_STRING(SMB_COM_NT_TRANSACT_SECONDARY);
  XLAT_STRING(SMB_COM_NT_CREATE_ANDX);
  XLAT_STRING(SMB_COM_NT_CANCEL);
  XLAT_STRING(SMB_COM_NT_RENAME);
  XLAT_STRING(SMB_COM_OPEN_PRINT_FILE);
  XLAT_STRING(SMB_COM_WRITE_PRINT_FILE);
  XLAT_STRING(SMB_COM_CLOSE_PRINT_FILE);
  XLAT_STRING(SMB_COM_GET_PRINT_QUEUE);
  XLAT_STRING(SMB_COM_SEND_MESSAGE);
  XLAT_STRING(SMB_COM_SEND_BROADCAST_MESSAGE);
  XLAT_STRING(SMB_COM_FORWARD_USER_NAME);
  XLAT_STRING(SMB_COM_CANCEL_FORWARD);
  XLAT_STRING(SMB_COM_GET_MACHINE_NAME);
  XLAT_STRING(SMB_COM_SEND_START_MB_MESSAGE);
  XLAT_STRING(SMB_COM_SEND_END_MB_MESSAGE);
  XLAT_STRING(SMB_COM_SEND_TEXT_MB_MESSAGE);
  XLAT_STRING(SMB_COM_NO_ANDX_COMMAND);

  switch (command)
  {
    XLAT_CASE(SMB_COM_CREATE_DIRECTORY);
    XLAT_CASE(SMB_COM_DELETE_DIRECTORY);
    XLAT_CASE(SMB_COM_OPEN);
    XLAT_CASE(SMB_COM_CREATE);
    XLAT_CASE(SMB_COM_CLOSE);
    XLAT_CASE(SMB_COM_FLUSH);
    XLAT_CASE(SMB_COM_DELETE);
    XLAT_CASE(SMB_COM_RENAME);
    XLAT_CASE(SMB_COM_QUERY_INFORMATION);
    XLAT_CASE(SMB_COM_SET_INFORMATION);
    XLAT_CASE(SMB_COM_READ);
    XLAT_CASE(SMB_COM_WRITE);
    XLAT_CASE(SMB_COM_LOCK_BYTE_RANGE);
    XLAT_CASE(SMB_COM_UNLOCK_BYTE_RANGE);
    XLAT_CASE(SMB_COM_CREATE_TEMPORARY);
    XLAT_CASE(SMB_COM_CREATE_NEW);
    XLAT_CASE(SMB_COM_CHECK_DIRECTORY);
    XLAT_CASE(SMB_COM_PROCESS_EXIT);
    XLAT_CASE(SMB_COM_SEEK);
    XLAT_CASE(SMB_COM_LOCK_AND_READ);
    XLAT_CASE(SMB_COM_WRITE_AND_UNLOCK);
    XLAT_CASE(SMB_COM_READ_RAW);
    XLAT_CASE(SMB_COM_READ_MPX);
    XLAT_CASE(SMB_COM_READ_MPX_SECONDARY);    // server to redir only
    XLAT_CASE(SMB_COM_WRITE_RAW);
    XLAT_CASE(SMB_COM_WRITE_MPX);
    XLAT_CASE(SMB_COM_WRITE_MPX_SECONDARY);
    XLAT_CASE(SMB_COM_WRITE_COMPLETE);    // server to redir only
    XLAT_CASE(SMB_COM_QUERY_INFORMATION_SRV);
    XLAT_CASE(SMB_COM_SET_INFORMATION2);
    XLAT_CASE(SMB_COM_QUERY_INFORMATION2);
    XLAT_CASE(SMB_COM_LOCKING_ANDX);
    XLAT_CASE(SMB_COM_TRANSACTION);
    XLAT_CASE(SMB_COM_TRANSACTION_SECONDARY);
    XLAT_CASE(SMB_COM_IOCTL);
    XLAT_CASE(SMB_COM_IOCTL_SECONDARY);
    XLAT_CASE(SMB_COM_COPY);
    XLAT_CASE(SMB_COM_MOVE);
    XLAT_CASE(SMB_COM_ECHO);
    XLAT_CASE(SMB_COM_WRITE_AND_CLOSE);
    XLAT_CASE(SMB_COM_OPEN_ANDX);
    XLAT_CASE(SMB_COM_READ_ANDX);
    XLAT_CASE(SMB_COM_WRITE_ANDX);
    XLAT_CASE(SMB_COM_CLOSE_AND_TREE_DISC);
    XLAT_CASE(SMB_COM_TRANSACTION2);
    XLAT_CASE(SMB_COM_TRANSACTION2_SECONDARY);
    XLAT_CASE(SMB_COM_FIND_CLOSE2);
    XLAT_CASE(SMB_COM_FIND_NOTIFY_CLOSE);
    XLAT_CASE(SMB_COM_TREE_CONNECT);
    XLAT_CASE(SMB_COM_TREE_DISCONNECT);
    XLAT_CASE(SMB_COM_NEGOTIATE);
    XLAT_CASE(SMB_COM_SESSION_SETUP_ANDX);
    XLAT_CASE(SMB_COM_LOGOFF_ANDX);
    XLAT_CASE(SMB_COM_TREE_CONNECT_ANDX);
    XLAT_CASE(SMB_COM_QUERY_INFORMATION_DISK);
    XLAT_CASE(SMB_COM_SEARCH);
    XLAT_CASE(SMB_COM_FIND);
    XLAT_CASE(SMB_COM_FIND_UNIQUE);
    XLAT_CASE(SMB_COM_FIND_CLOSE);
    XLAT_CASE(SMB_COM_NT_TRANSACT);
    XLAT_CASE(SMB_COM_NT_TRANSACT_SECONDARY);
    XLAT_CASE(SMB_COM_NT_CREATE_ANDX);
    XLAT_CASE(SMB_COM_NT_CANCEL);
    XLAT_CASE(SMB_COM_NT_RENAME);
    XLAT_CASE(SMB_COM_OPEN_PRINT_FILE);
    XLAT_CASE(SMB_COM_WRITE_PRINT_FILE);
    XLAT_CASE(SMB_COM_CLOSE_PRINT_FILE);
    XLAT_CASE(SMB_COM_GET_PRINT_QUEUE);
    XLAT_CASE(SMB_COM_SEND_MESSAGE);
    XLAT_CASE(SMB_COM_SEND_BROADCAST_MESSAGE);
    XLAT_CASE(SMB_COM_FORWARD_USER_NAME);
    XLAT_CASE(SMB_COM_CANCEL_FORWARD);
    XLAT_CASE(SMB_COM_GET_MACHINE_NAME);
    XLAT_CASE(SMB_COM_SEND_START_MB_MESSAGE);
    XLAT_CASE(SMB_COM_SEND_END_MB_MESSAGE);
    XLAT_CASE(SMB_COM_SEND_TEXT_MB_MESSAGE);
    XLAT_CASE(SMB_COM_NO_ANDX_COMMAND);
    XLAT_CASE_DEFAULT;
  }
}


char* dumpSMB(char* buffer, PSMB_HEADER pSmb, unsigned int size)
{
  //buffer += wsprintf(buffer,
  //  "size    : 0x%08x\n"
  //  "%s : 0x%02x (%s)\n"
  //  "NtStatus: 0x%08x\n"
  //  "Flags   : 0x%02x\n"
  //  "Flags2  : 0x%04x\n"
  //  "PidHigh : 0x%04x\n"
  //  "Key     : 0x%08x\n"
  //  "Sid     : 0x%04x\n"
  //  "SeqNum  : 0x%04x\n"
  //  "Gid     : 0x%04x\n"
  //  "Tid     : 0x%04x\n"
  //  "Pid     : 0x%04x\n"
  //  "Uid     : 0x%04x\n"
  //  "Mid     : 0x%04x\n",
  //  size,
  //  pSmb->Flags&0x80?"Reply  ":"Command",
  //  pSmb->Command,
  //  SmbUnparseCommand(pSmb->Command),
  //  pSmb->Error,
  //  pSmb->Flags,
  //  pSmb->Flags2,
  //  pSmb->PidHigh,
  //  pSmb->Key,
  //  pSmb->Sid,
  //  pSmb->SequenceNumber,
  //  pSmb->Gid,
  //  pSmb->Tid,
  //  pSmb->Pid,
  //  pSmb->Uid,
  //  pSmb->Mid);
  return buffer;
}

char* dumpSMB2(char* buffer, PSMB2_HEADER pSmb, unsigned int size)
{
  //buffer += wsprintf(buffer,
  //  "size    : 0x%08x\n"
  //  "%s : 0x%02x (%s)\n"
  //  "NtStatus: 0x%08x\n"
  //  "Flags   : 0x%02x\n"
  //  "Flags2  : 0x%04x\n"
  //  "PidHigh : 0x%04x\n"
  //  "Key     : 0x%08x\n"
  //  "Sid     : 0x%04x\n"
  //  "SeqNum  : 0x%04x\n"
  //  "Gid     : 0x%04x\n"
  //  "Tid     : 0x%04x\n"
  //  "Pid     : 0x%04x\n"
  //  "Uid     : 0x%04x\n"
  //  "Mid     : 0x%04x\n",
  //  size,
  //  pSmb->Flags&0x80?"Reply  ":"Command",
  //  pSmb->Command,
  //  SmbUnparseCommand(pSmb->Command),
  //  pSmb->Error,
  //  pSmb->Flags,
  //  pSmb->Flags2,
  //  pSmb->PidHigh,
  //  pSmb->Key,
  //  pSmb->Sid,
  //  pSmb->SequenceNumber,
  //  pSmb->Gid,
  //  pSmb->Tid,
  //  pSmb->Pid,
  //  pSmb->Uid,
  //  pSmb->Mid);
  return buffer;
}

char* dumpData(char* buffer,unsigned char* data,unsigned int size)
{
  const int line_size = 16;
  if(!size || !data)
    return buffer;
  UCHAR line_buffer[line_size + 1];
  UCHAR current = line_buffer[line_size] = '\0';

  RtlZeroMemory(line_buffer, line_size);

  //if (size < 1000)
  {
    for (unsigned int i = 0; i < size; i++) 
    {
      current = data[i];
      if (!(i % line_size)) 
      {
        if (i) 
        {
          //buffer += wsprintf(buffer, "  %s\n", line_buffer);
        }
        RtlZeroMemory(line_buffer, line_size);
        //buffer += wsprintf(buffer, "0x%08x : ", i);
      }
      //buffer += wsprintf(buffer, " %02x", (int) current);
      line_buffer[i % line_size] = isprint(current)?current:'.';
    }
    i %= line_size;
    for (i = line_size - (i?i:line_size); i > 0; i--) 
    {
      //buffer += wsprintf(buffer, "   ");
    }
  }
  //else
  {
    //buffer += wsprintf(buffer, "lots and lost of data...\n...beep...beep...beep...\n", line_buffer);
  }
  //buffer += wsprintf(buffer, "  %s\n", line_buffer);
  return buffer;
}

//void __cdecl PRINT(const wchar_t *format, ...)
//{
//  if (cSMBServer::PrintFn != NULL)
//    (cSMBServer::PrintFn)(&format);
//  else
//  {
//    va_list ap;
//    va_start(ap, format);
//    vwprintf(format, ap);
//  }
//}
//
//void __cdecl SOS(const wchar_t *format, ...)
//{
//  if (cSMBServer::SOSFn != NULL)
//    (cSMBServer::SOSFn)(&format);
//  else
//  {
//    va_list ap;
//    va_start(ap, format);
//    vwprintf(format, ap);
//  }
//}

//std::wstring toStringWin32ErrCode(const long Win32ErrCode)
//{
//  wchar_t* FormatMessageBuffer = 0;
//    (void)FormatMessageW(
//        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
//        NULL,
//        Win32ErrCode,
//        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//        (LPWSTR)&FormatMessageBuffer,
//        0,
//        NULL);
//
//  const std::wstring ErrorMessage(FormatMessageBuffer);
//  LocalFree(FormatMessageBuffer);
//
//  return ErrorMessage;
//}

#endif // CIFS 