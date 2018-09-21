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

#ifndef LOCK_H
#define LOCK_H
#include <windows.h>

class CLock;
class CCriticalSectionHolder
{
	friend CLock;
	CRITICAL_SECTION m_cs;

    CCriticalSectionHolder(const &CCriticalSectionHolder);
    CCriticalSectionHolder& operator = (const CCriticalSectionHolder&);
public:
	CCriticalSectionHolder()
	{
		InitializeCriticalSection(&m_cs);
	}

	~CCriticalSectionHolder()
	{
		DeleteCriticalSection(&m_cs);
	}
};


class CLock
{
	CLock(const CLock&);
	CLock& operator = (const CLock&);

	CRITICAL_SECTION* m_cs;
public:
	CLock(CCriticalSectionHolder& cs): m_cs(&cs.m_cs) { EnterCriticalSection(m_cs);}
	~CLock() {LeaveCriticalSection(m_cs);}
};
#endif // LOCK_H
