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

//iQueue.h 


///////////////////////////////////////////////////////////////////////////////
enum eDeQType { eBlocking, eNonBlocking };
enum eEnQType { eBack, eFront };

template <typename T>
class iQueue
{
  // Thread-safe queue of elements

  // !!! Beware !!!
  // This code uses the MessageQueueSemaphore count as a record of the MessageQueue length
  // Any code added to this class must be consistent with this usage
  // !!! Beware !!!

  HANDLE              MessageQueueSemaphore;
  CRITICAL_SECTION    MessageQueueSection;

public:
  std::list<T>        MessageQueue;

public:
  iQueue() // maxCount is maximum possible size of MessageQueue
  {
    ::InitializeCriticalSection (&MessageQueueSection);
    MessageQueueSemaphore = ::CreateSemaphore (NULL, 0, _I32_MAX, NULL);
  }

  ~iQueue()
  {
    ::CloseHandle (MessageQueueSemaphore);
    ::DeleteCriticalSection (&MessageQueueSection);
  }

  void enQElem(T elem, enum eEnQType front) // add element to queue
  {
    EnterCriticalSection(&MessageQueueSection);
    {
      if(eFront == front)
        MessageQueue.push_front(elem);
      else
        MessageQueue.push_back(elem);
    }
    LeaveCriticalSection(&MessageQueueSection);

    if (!ReleaseSemaphore(MessageQueueSemaphore, 1, NULL))
    {
      DWORD err = GetLastError();
      throw err;
    }
  }

  T deQElem(enum eDeQType blocking) // remove element from queue
  {
    T elem(0);

    DWORD waitResult = WaitForSingleObject(MessageQueueSemaphore, (eBlocking == blocking) ? INFINITE : 0);
    if (WAIT_OBJECT_0 != waitResult)
    {
      return elem;
    }

    EnterCriticalSection(&MessageQueueSection);
    {
      // there will be an element in the queue because the semaphore has been signalled
      // so don't test MessageQueue length explicitly
      elem = MessageQueue.front();
      MessageQueue.pop_front();
    }
    LeaveCriticalSection(&MessageQueueSection);

    return elem;
  }

  unsigned int numElems()
  {
    unsigned int size = 0;
    EnterCriticalSection(&MessageQueueSection);
    {
      size = (unsigned int)MessageQueue.size();
    }
    LeaveCriticalSection(&MessageQueueSection);
    return size;
  }
};

