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

#ifndef SCOPED_HANDLE_H
#define SCOPED_HANDLE_H

template<class R, class T>
struct stdcall_close_func
{
  typedef R (__stdcall* type)(T);
};

template<class R, class T>
struct cdecl_close_func
{
  typedef R (__cdecl* type)(T);
};

template<class R, class T>
struct default_close_func
{
  typedef R (* type)(T);
};

namespace scoped_handle_priv
{
  template<class R, class T, template<class, class>class CloseFuncType, typename CloseFuncType<R, T>::type F, T invalidHandle>
  struct close_handle
  {
    static R close(T& handle)
    {
      R rc = F(handle); 
      handle = invalidHandle;
      return rc;
    }

  };

  template<class T, template<class, class>class CloseFuncType, typename CloseFuncType<void, T>::type F, T invalidHandle>
  struct close_handle<void, T, CloseFuncType, F, invalidHandle>
  {
    static void close(T& handle)
    {
      F(handle); 
      handle = invalidHandle;
    }
  };

  template<class T> struct ptr_operator; // primary template declaration

  template<class T> struct ptr_operator<T*> // partial specialization
  {
    // We use the following function name so that it gives a meaningful compilation error when instantiating the template by using invalid type
    // for either non pointer types or invalid pointer types such as void*, int*, unsigned*.
    static T* invalid_pointer_access(T* ptr) { return ptr; } 
  };

  // Let the compiler generate error for the specializations of following primitive types 
  template<> struct ptr_operator<void*>;
  template<> struct ptr_operator<char*>; 
  template<> struct ptr_operator<short*>; 
  template<> struct ptr_operator<int*>; 
  template<> struct ptr_operator<long*>; 
  template<> struct ptr_operator<long long*>; 
  template<> struct ptr_operator<unsigned char*>; 
  template<> struct ptr_operator<unsigned short*>; 
  template<> struct ptr_operator<unsigned int*>; 
  template<> struct ptr_operator<unsigned long*>; 
  template<> struct ptr_operator<unsigned long long*>; 
  template<> struct ptr_operator<signed char*>;
  template<> struct ptr_operator<float*>; 
  template<> struct ptr_operator<double*>; 
}

template<class R, class T, template<class, class>class CloseFuncType, typename CloseFuncType<R, T>::type F, T invalidHandle>
class scoped_handle
{
    // Disable copy
    scoped_handle(const scoped_handle&);
    scoped_handle& operator = (const scoped_handle&);
    T m_handle;

public:
    scoped_handle() : m_handle(invalidHandle) {}
    explicit scoped_handle(const T& handle) : m_handle(handle) {};
    ~scoped_handle() 
    {
        reset(invalidHandle);
    }


    R close() 
    {
      return scoped_handle_priv::close_handle<R, T, CloseFuncType, F, invalidHandle>::close(m_handle);
    }

    void reset(const T& handle = invalidHandle) 
    {
        if(m_handle != invalidHandle && m_handle != NULL)
        {
            close();
        }

        m_handle = handle;
    }

    T detach() 
    {
        T rsc = m_handle;
        m_handle = invalidHandle;
        return rsc;
    }

    void operator = (const T handle) { reset(handle); }

    operator T () const { return m_handle;}

    bool operator !() const { return (invalidHandle == m_handle || NULL == m_handle);}
    bool operator == (const T handle) const { return (handle == m_handle);}
    T* operator &() { return &m_handle; }
    T operator->() { return scoped_handle_priv::ptr_operator<T>::invalid_pointer_access(m_handle); }

};

#endif //SCOPED_HANDLE_H