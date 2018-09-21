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

//iPointer.h

#pragma once

///////////////////////////////////////////////////////////////////////////////
// Thread safe reference count

class cRefCount
{
public:
  inline void addRef() const
  {
    unsigned int * const CountAddr = &m_RefCount;
    __asm
    {
      mov eax, 1
      mov edx, CountAddr
      lock xadd dword ptr [edx], eax      // Locks bus -> SMP thread safe
    }
  }

  inline bool removeRef() const
  {
    int Result;
    unsigned * const CountAddr = &m_RefCount;
    __asm
    {
      mov eax, -1
      mov edx, CountAddr
      lock xadd dword ptr [edx], eax      // Locks bus -> SMP thread safe
      mov Result, eax                     // Return the value that WAS in m_RefCount
    }
    return Result == 1;
  }

  inline unsigned int getNumRefs() const 
  {
    return m_RefCount;
  }
protected:
  cRefCount() : m_RefCount(0) {}
  //do not copy the original count
  cRefCount(const cRefCount&) : m_RefCount(0) {}

private:
  mutable unsigned int m_RefCount;
};

///////////////////////////////////////////////////////////////////////////////
// smart pointer that counts using cRefCount

template <typename T> class cPtr
{
public:
  cPtr() : myPointer(0), myCounter(0) {}

  cPtr(int) : myPointer(0), myCounter(0) {} //useful in template ctor

  template <class B> cPtr(B * pointer)  //non const pointer
  : myPointer(pointer), myCounter(pointer)
  {
    if(myCounter) 
      myCounter->addRef();
  }
  cPtr(T * pointer, cRefCount *counter) : myPointer(pointer), myCounter(pointer ? counter : 0) // if the cast failed i don't want this smart-pointer to have the reference count
  {
    if(myCounter) 
      myCounter->addRef();
  }
  template <class B> cPtr(const cPtr<B>& Other)
  : myPointer(Other.getRaw()), myCounter(Other.getCounter())
  {
    if (myCounter) myCounter->addRef();
  }
  template<> cPtr(const cPtr<T>& Other)
  : myPointer(Other.myPointer), myCounter(Other.myCounter)
  {
    if (myCounter) myCounter->addRef();
  }
  inline ~cPtr() 
  {
    if (myCounter && myCounter->removeRef()) 
      destroyObject();
  }
  cRefCount * getCounter() const 
  {
    return myCounter;
  }
  inline T * getRaw() const 
  {
    return myPointer;
  }
  inline T * operator->() const
  {
    return myPointer;
  }
  inline T& operator*() const
  {
    return *myPointer;
  }
  inline bool isNull() const
  {
    return 0 == myPointer;
  }
  inline bool isValid() const 
  {
    return 0 != myPointer;
  }
  inline void invalidate() 
  {
    if (myCounter && myCounter->removeRef()) 
      destroyObject();
    myPointer = 0;
    myCounter = 0;
  }
  inline void setNull() 
  {
    invalidate();
  }
  cPtr<T>& operator=(const cPtr<T> & Other) 
  {
    if (this != &Other)
    {
      //Interesting lifetime issue - for the duration of this function the 'Other' will have a ref count of at least 1 -> thread safe.
      if (myCounter && myCounter->removeRef()) destroyObject();
      myPointer = Other.myPointer;
      myCounter = Other.myCounter;
      if (myCounter) myCounter->addRef();
    }
    return *this;
  }
  //allows cPtr to be added to STL Trees. Makes no demands upon the pointed object.
  inline bool operator < (const cPtr<T> & Other) const 
  {
    return getRaw() < Other.getRaw();
  }
  //the type of the smart-pointable.
  typedef T *Type;
  //}}}
protected :
  //{{{
  // This is seperate function which isn't usually inlined which allows decrementCounter and
  // incrementCounter to be smaller and more likely to be inlined.
  void destroyObject() const 
  {
    delete myPointer;
  }
  //}}}
  //{{{  pointers
  mutable T *         myPointer;
  mutable cRefCount * myCounter;
  //}}}
};

///////////////////////////////////////////////////////////////////////////////
// Support for smart pointer dynamic_cast 

template <class TO, class FROM> TO ptr_cast(const FROM & a)
{
  return TO(dynamic_cast<TO::Type>(a.getRaw()), const_cast<cRefCount*>(a.getCounter()));
}

