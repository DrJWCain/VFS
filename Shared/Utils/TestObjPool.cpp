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

#include "stdafx.h"

#include "obj_pool.h"

#include <process.h>
#include "Z:\kernel\Shared\Utils\WinScopedHandleDef.h"
#include <iostream>
#include <boost/format.hpp>

typedef void            tReturn;
typedef int&            tParam0;   
typedef short           tParam1;   
typedef const float&    tParam2;   
typedef volatile char*  tParam3;   

struct FuncClass
{
  int mVal;
  FuncClass(int val) : mVal(val) {std::cout << "FuncClass::FuncClass() mVal: " << mVal << "\n";}
  ~FuncClass() {std::cout << "FuncClass::~FuncClass() mVal: " << mVal << "\n";}
  int                 FuncC0       (                                  )  const          { std::cout << boost::format("FuncClass::FuncC0       (             )\n")               ; return mVal;}
  tReturn             FuncV1       (tParam0 p0                        )  volatile       { std::cout << boost::format("FuncClass::FuncV1       (%1%          )\n") % p0          ; }
  tReturn             FuncCV2      (tParam0 p0, tParam1 p1            )  const volatile { std::cout << boost::format("FuncClass::FuncCV2      (%1%, %2%     )\n") % p0 % p1     ; }
  tReturn             Func3        (tParam0 p0, tParam1 p1, tParam2 p2)                 { std::cout << boost::format("FuncClass::Func3        (%1%, %2%, %3%)\n") % p0 % p1 % p2; }
  tReturn __stdcall   StdCallFunc1 (tParam0 p0                        )                 { std::cout << boost::format("FuncClass::StdCallFunc1 (%1%          )\n") % p0          ; }
  tReturn __fastcall  FastCallFunc2(tParam0 p0, tParam1 p1            )                 { std::cout << boost::format("FuncClass::FastCallFunc2(%1%, %2%     )\n") % p0 % p1     ; }
  int     __fastcall  FastCallFunc3(tParam0 p0, tParam1 p1, tParam2 p2)                 { std::cout << boost::format("FuncClass::FastCallFunc3(%1%, %2%, %3%)\n") % p0 % p1 % p2; return mVal; }
  static tReturn      StaticFunc3  (tParam0 p0, tParam1 p1, tParam2 p2)                 { std::cout << boost::format("FuncClass::StaticFunc3  (%1%, %2%, %3%)\n") % p0 % p1 % p2; }
};


typedef obj_pool<boost::shared_ptr, FuncClass, on_empty_return_null, single_thread_safe, int> obj_pool_st_null_t;
typedef obj_pool<boost::shared_ptr, FuncClass, on_empty_wait, single_thread_safe, int> obj_pool_st_block_t;
typedef obj_pool<boost::shared_ptr, FuncClass, on_empty_create, single_thread_safe, int> obj_pool_st_create_t;

typedef obj_pool<boost::shared_ptr, FuncClass, on_empty_return_null, multi_thread_safe, int> obj_pool_mt_null_t;
typedef obj_pool<boost::shared_ptr, FuncClass, on_empty_wait, multi_thread_safe, int> obj_pool_mt_block_t;
typedef obj_pool<boost::shared_ptr, FuncClass, on_empty_create, multi_thread_safe, int> obj_pool_mt_create_t;

template<class T, int n>
void Test()
{
  T objPool(n, 7);
  objPool.get();
  objPool.get();
}

unsigned __stdcall ThreadProc(void* param)
{
  obj_pool_mt_block_t* pool = (obj_pool_mt_block_t*) param;
  obj_pool_mt_block_t::ele_type ele = pool->get();
  Sleep(2000);
  std::cout << GetTickCount() << " : Return an obj\n";  
  pool->put(ele);
  return 1;
}


template<int n>
void TestMt()
{
  obj_pool_mt_block_t objPool(n, 7);
  unsigned threadId;
  tKernelHandle threadHandle((HANDLE)_beginthreadex(NULL, 0, ThreadProc, &objPool, 0, &threadId));
  Sleep(200);
  std::cout << GetTickCount() << " : Get first obj\n";
  objPool.get();
  std::cout << GetTickCount() << " : Waiting for returned obj\n";
  objPool.get();
}

void TestObjPool()
{
  Test<obj_pool_st_null_t, 2>();
  //Test<obj_pool_st_block_t, 1>();
  Test<obj_pool_st_create_t, 1>();
  
  Test<obj_pool_mt_null_t, 2>();
  Test<obj_pool_mt_create_t, 1>(); 
  TestMt<2>();
 
}