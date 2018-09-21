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

/*
**  
*/

// --- Pre-Compiled Headers --------------------------------------------------------------

#include "stdafx.h"

// --- Headers ---------------------------------------------------------------------------

#include "QKernelBase.h"

// --- Start of Namespace ----------------------------------------------------------------

using namespace vfs;




// =======================================================================================
// === Copy Constructor & Assignment =====================================================
// =======================================================================================

cRecoverable::cRecoverable(const cRecoverable & Other) :
  m_Magic(kRecoverableMagic),
  m_Msg(Other.m_Msg),
  m_SrcFilename(Other.m_SrcFilename),
  m_SrcLineNumber(Other.m_SrcLineNumber)
{
  // Do nothing.
}

// ---------------------------------------------------------------------------------------

void QAPI cRecoverable::operator=(const cRecoverable & Other)
{
  m_Msg           = Other.m_Msg;
  m_SrcFilename   = Other.m_SrcFilename;
  m_SrcLineNumber = Other.m_SrcLineNumber;
}




// =======================================================================================
// === Comparison ========================================================================
// =======================================================================================

bool QAPI cRecoverable::operator!=(const cRecoverable & Other) const
{
  return !(*this == Other);
}

// ---------------------------------------------------------------------------------------

bool QAPI cRecoverable::operator==(const cRecoverable & Other) const
{
  return  m_Msg           == Other.m_Msg &&
          m_SrcFilename   == Other.m_SrcFilename &&
          m_SrcLineNumber == Other.m_SrcLineNumber;
}




// =======================================================================================
// === Construction & Destruction ========================================================
// =======================================================================================

cRecoverable::cRecoverable(
	const char * const	SrcFilename,
	const unsigned long	SrcLineNumber,
  const Char *        MsgFormat,
  ...) :
    m_Magic(kRecoverableMagic),
    m_SrcFilename(widen(SrcFilename)),
    m_SrcLineNumber(SrcLineNumber)
{
  va_list VArgs;
  va_start(VArgs, MsgFormat);

  Char MsgBuffer[512];
  _vsnwprintf(MsgBuffer, sizeof(MsgBuffer) / sizeof(Char), MsgFormat, VArgs);
  m_Msg = MsgBuffer;

  va_end(VArgs);
}

// ---------------------------------------------------------------------------------------

cRecoverable::cRecoverable(
	const char * const	SrcFilename,
	const unsigned long	SrcLineNumber,
  const Char *        MsgFormat,
  va_list             VArgs) :
    m_Magic(kRecoverableMagic),
    m_SrcFilename(widen(SrcFilename)),
    m_SrcLineNumber(SrcLineNumber)
{
  Char MsgBuffer[512];
  _vsnwprintf(MsgBuffer, sizeof(MsgBuffer) / sizeof(Char), MsgFormat, VArgs);
  m_Msg = MsgBuffer;
}

// ---------------------------------------------------------------------------------------

cRecoverable::cRecoverable() :
  m_Magic(kRecoverableMagic),
  m_SrcLineNumber(0xffffffff)
{
  // Do nothing.
}




// =======================================================================================
// === getSrcFilename/getSrcLineNumber/getMsg ============================================
// =======================================================================================

String QAPI cRecoverable::getSrcFilename() const
{
  return widen(m_SrcFilename);
}

// ---------------------------------------------------------------------------------------

unsigned long QAPI cRecoverable::getSrcLineNumber() const
{
  return m_SrcLineNumber;
}

// ---------------------------------------------------------------------------------------

String QAPI cRecoverable::getMsg() const
{
  return m_Msg;
}

// ---------------------------------------------------------------------------------------

void QAPI cRecoverable::setMsg(const String & NewMsg)
{
  m_Msg = NewMsg;
}
