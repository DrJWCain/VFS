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

//#include "stdafx.h"
// This usage smaple file is taken from my test code. It should generate the following output when 
// calling TestedOK(). Dongning, March 2010
      //TestReference()
      //FuncClass::FuncClass() mVal: 1
      //FastCallFreeFunc3(168, 888, 77.77)
      //FastCallFreeFunc2(168, 888     )
      //StdCallFreeFunc1 (168          )
      //FreeFunc3        (168, 888, 77.77)
      //FreeFunc2        (168, 888     )
      //FreeFunc1        (168          )
      //FreeFunc0        (             )
      //FuncClass::StaticFunc3  (168, 888, 77.77)
      //FuncClass::FastCallFunc3(168, 888, 77.77)
      //FuncClass::FastCallFunc2(168, 888     )
      //FuncClass::StdCallFunc1 (168          )
      //FuncClass::Func3        (168, 888, 1.345)
      //FuncClass::FuncCV2      (168, 888     )
      //FuncClass::FuncV1       (168          )
      //FuncClass::FuncC0       (             )
      //FuncClass::~FuncClass() mVal: 1
      //TestAutoPtr() - std::auto_ptr
      //FuncClass::FuncClass() mVal: 1
      //FuncClass::FuncClass() mVal: 1
      //FuncClass::FuncClass() mVal: 1
      //FuncClass::FuncClass() mVal: 1
      //FuncClass::StaticFunc3  (168, 888, 77.77)
      //FuncClass::FastCallFunc3(168, 888, 77.77)
      //FuncClass::FastCallFunc2(168, 888     )
      //FuncClass::StdCallFunc1 (168          )
      //FuncClass::Func3        (168, 888, 77.77)
      //FuncClass::FuncCV2      (168, 888     )
      //FuncClass::FuncV1       (168          )
      //FuncClass::FuncC0       (             )
      //FuncClass::~FuncClass() mVal: 1
      //FuncClass::~FuncClass() mVal: 1
      //FuncClass::~FuncClass() mVal: 1
      //FuncClass::~FuncClass() mVal: 1
      //TestScopedPtr() - boost::scoped_ptr
      //FuncClass::FuncClass() mVal: 1
      //FuncClass::FuncClass() mVal: 1
      //FuncClass::FuncClass() mVal: 1
      //FuncClass::FuncClass() mVal: 1
      //FuncClass::StaticFunc3  (168, 888, 77.77)
      //FuncClass::FastCallFunc3(168, 888, 77.77)
      //FuncClass::FastCallFunc2(168, 888     )
      //FuncClass::StdCallFunc1 (168          )
      //FuncClass::Func3        (168, 888, 77.77)
      //FuncClass::FuncCV2      (168, 888     )
      //FuncClass::FuncV1       (168          )
      //FuncClass::FuncC0       (             )
      //FuncClass::~FuncClass() mVal: 1
      //FuncClass::~FuncClass() mVal: 1
      //FuncClass::~FuncClass() mVal: 1
      //FuncClass::~FuncClass() mVal: 1
      //TestSharedPtr() - boost::shared_ptr
      //FuncClass::FuncClass() mVal: 1
      //FuncClass::StaticFunc3  (168, 888, 77.77)
      //FuncClass::FastCallFunc3(168, 888, 77.77)
      //FuncClass::FastCallFunc2(168, 888     )
      //FuncClass::StdCallFunc1 (168          )
      //FuncClass::Func3        (168, 888, 77.77)
      //FuncClass::FuncCV2      (168, 888     )
      //FuncClass::FuncV1       (168          )
      //FuncClass::FuncC0       (             )
      //FuncClass::~FuncClass() mVal: 1
      //TestQPtr() - vfs::cPtr
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::StaticFunc3  (11, 22, 3.3)
      //QFuncClass::FastCallFunc3(11, 22, 3.3)
      //QFuncClass::FastCallFunc2(11, 22     )
      //QFuncClass::StdCallFunc1 (11          )
      //QFuncClass::Func3        (11, 22, 3.3)
      //QFuncClass::StaticFunc3  (168, 888, 77.77)
      //QFuncClass::FastCallFunc3(168, 888, 77.77)
      //QFuncClass::FastCallFunc2(168, 888     )
      //QFuncClass::StdCallFunc1 (168          )
      //QFuncClass::Func3        (168, 888, 77.77)
      //QFuncClass::FuncCV2      (168, 888     )
      //QFuncClass::FuncV1       (168          )
      //QFuncClass::FuncC0       (             )
      //QFuncClass::~QFuncClass() mVal: 1
      //QFuncClass::~QFuncClass() mVal: 1
      //QFuncClass::~QFuncClass() mVal: 1
      //QFuncClass::~QFuncClass() mVal: 1
      //TestPointerTransform<boost::shared_ptr>()
      //TestPointerTransform()
      //FuncClass::FuncClass() mVal: 1
      //FuncClass::FuncClass() mVal: 2
      //FuncClass::FuncClass() mVal: 3
      //FuncClass::FuncClass() mVal: 4
      //FuncClass::StaticFunc3  (168, 888, 77.77)
      //FuncClass::FastCallFunc3(168, 888, 77.77)
      //FuncClass::FastCallFunc2(168, 888     )
      //FuncClass::StdCallFunc1 (168          )
      //FuncClass::Func3        (168, 888, 77.77)
      //FuncClass::FuncCV2      (168, 888     )
      //FuncClass::FuncV1       (168          )
      //FuncClass::FuncC0       (             )
      //FuncClass::~FuncClass() mVal: 4
      //FuncClass::~FuncClass() mVal: 3
      //FuncClass::~FuncClass() mVal: 2
      //FuncClass::~FuncClass() mVal: 1
      //TestPointerTransform<boost::scoped_ptr>()
      //TestPointerTransform()
      //FuncClass::FuncClass() mVal: 1
      //FuncClass::FuncClass() mVal: 2
      //FuncClass::FuncClass() mVal: 3
      //FuncClass::FuncClass() mVal: 4
      //FuncClass::StaticFunc3  (168, 888, 77.77)
      //FuncClass::FastCallFunc3(168, 888, 77.77)
      //FuncClass::FastCallFunc2(168, 888     )
      //FuncClass::StdCallFunc1 (168          )
      //FuncClass::Func3        (168, 888, 77.77)
      //FuncClass::FuncCV2      (168, 888     )
      //FuncClass::FuncV1       (168          )
      //FuncClass::FuncC0       (             )
      //FuncClass::~FuncClass() mVal: 4
      //FuncClass::~FuncClass() mVal: 3
      //FuncClass::~FuncClass() mVal: 2
      //FuncClass::~FuncClass() mVal: 1
      //TestPointerTransform<std::auto_ptr>()
      //TestPointerTransform()
      //FuncClass::FuncClass() mVal: 1
      //FuncClass::FuncClass() mVal: 2
      //FuncClass::FuncClass() mVal: 3
      //FuncClass::FuncClass() mVal: 4
      //FuncClass::StaticFunc3  (168, 888, 77.77)
      //FuncClass::FastCallFunc3(168, 888, 77.77)
      //FuncClass::FastCallFunc2(168, 888     )
      //FuncClass::StdCallFunc1 (168          )
      //FuncClass::Func3        (168, 888, 77.77)
      //FuncClass::FuncCV2      (168, 888     )
      //FuncClass::FuncV1       (168          )
      //FuncClass::FuncC0       (             )
      //FuncClass::~FuncClass() mVal: 4
      //FuncClass::~FuncClass() mVal: 3
      //FuncClass::~FuncClass() mVal: 2
      //FuncClass::~FuncClass() mVal: 1
      //TestCopyablePtr<boost::shared_ptr>()
      //TestCopyablePtr()
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::FastCallFunc3(168, 888, 77.77)
      //QFuncClass::FastCallFunc2(168, 888     )
      //QFuncClass::StdCallFunc1 (168          )
      //QFuncClass::Func3        (168, 888, 77.77)
      //QFuncClass::FuncCV2      (168, 888     )
      //QFuncClass::FuncV1       (168          )
      //QFuncClass::FuncC0       (             )
      //QFuncClass::~QFuncClass() mVal: 1
      //TestCopyablePtr<vfs::cPtr>()
      //TestCopyablePtr()
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::FastCallFunc3(168, 888, 77.77)
      //QFuncClass::FastCallFunc2(168, 888     )
      //QFuncClass::StdCallFunc1 (168          )
      //QFuncClass::Func3        (168, 888, 77.77)
      //QFuncClass::FuncCV2      (168, 888     )
      //QFuncClass::FuncV1       (168          )
      //QFuncClass::FuncC0       (             )
      //QFuncClass::~QFuncClass() mVal: 1
      //TestQConstPtr() - vfs::cConstPtr
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::FuncCV2      (168, 888     )
      //QFuncClass::FuncC0       (             )
      //QFuncClass::~QFuncClass() mVal: 1
      //QFuncClass::~QFuncClass() mVal: 1
      //QFuncClass::~QFuncClass() mVal: 1
      //QFuncClass::~QFuncClass() mVal: 1
      //TestQSharedPtr() - vfs::cPtr
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::QFuncClass() mVal: 1
      //QFuncClass::Func3        (11, 22, 3.3)
      //QFuncClass::FuncCV2      (11, 22     )
      //QFuncClass::FuncV1       (11          )
      //QFuncClass::FuncC0       (             )
      //QFuncClass::StaticFunc3  (168, 888, 77.77)
      //QFuncClass::FastCallFunc3(168, 888, 77.77)
      //QFuncClass::FastCallFunc2(168, 888     )
      //QFuncClass::StdCallFunc1 (168          )
      //QFuncClass::Func3        (168, 888, 77.77)
      //QFuncClass::FuncCV2      (168, 888     )
      //QFuncClass::FuncV1       (168          )
      //QFuncClass::FuncC0       (             )
      //QFuncClass::~QFuncClass() mVal: 1
      //QFuncClass::~QFuncClass() mVal: 1
      //QFuncClass::~QFuncClass() mVal: 1
      //QFuncClass::~QFuncClass() mVal: 1

#pragma warning( disable : 4819)

#include "scoped_exit_func.h"
#include <boost/smart_ptr.hpp>
#include <memory>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/typeof/typeof.hpp>

#include <KernelSDK\include\QKernelBase.h>

#include <iostream>
#include <boost/format.hpp>

typedef void            tReturn;
typedef int&            tParam0;   
typedef short           tParam1;   
typedef const float&    tParam2;   
typedef volatile char*  tParam3;   

// Member functions
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

// Free functions
int                 FreeFunc0        (                                  ){ std::cout << boost::format("FreeFunc0        (             )\n")               ; return 0;}
tReturn             FreeFunc1        (tParam0 p0                        ){ std::cout << boost::format("FreeFunc1        (%1%          )\n") % p0          ; }
tReturn             FreeFunc2        (tParam0 p0, tParam1 p1            ){ std::cout << boost::format("FreeFunc2        (%1%, %2%     )\n") % p0 % p1     ; }
tReturn             FreeFunc3        (tParam0 p0, tParam1 p1, tParam2 p2){ std::cout << boost::format("FreeFunc3        (%1%, %2%, %3%)\n") % p0 % p1 % p2; }
tReturn __stdcall   StdCallFreeFunc1 (tParam0 p0                        ){ std::cout << boost::format("StdCallFreeFunc1 (%1%          )\n") % p0          ; }
tReturn __fastcall  FastCallFreeFunc2(tParam0 p0, tParam1 p1            ){ std::cout << boost::format("FastCallFreeFunc2(%1%, %2%     )\n") % p0 % p1     ; }
int     __fastcall  FastCallFreeFunc3(tParam0 p0, tParam1 p1, tParam2 p2){ std::cout << boost::format("FastCallFreeFunc3(%1%, %2%, %3%)\n") % p0 % p1 % p2;  return 3;}

// Interface
struct iQFuncClass
{
  virtual ~iQFuncClass() = 0 {}
  virtual int                 FuncC0       (                                  )  const          = 0;
  virtual tReturn             FuncV1       (tParam0 p0                        )  volatile       = 0;
  virtual tReturn             FuncCV2      (tParam0 p0, tParam1 p1            )  const volatile = 0;
  virtual tReturn             Func3        (tParam0 p0, tParam1 p1, tParam2 p2)                 = 0;
  virtual tReturn __stdcall   StdCallFunc1 (tParam0 p0                        )                 = 0;
  virtual tReturn __fastcall  FastCallFunc2(tParam0 p0, tParam1 p1            )                 = 0;
  virtual int     __fastcall  FastCallFunc3(tParam0 p0, tParam1 p1, tParam2 p2)                 = 0;
};

// Member functions to be used with cPtr and cConstPtr.
struct QFuncClass : vfs::cRefCount, iQFuncClass
{
  int mVal;
  QFuncClass(int val) : mVal(val) {std::cout << "QFuncClass::QFuncClass() mVal: " << mVal << "\n";}
  virtual ~QFuncClass() {std::cout << "QFuncClass::~QFuncClass() mVal: " << mVal << "\n";}
  int                 FuncC0       (                                  )  const          { std::cout << boost::format("QFuncClass::FuncC0       (             )\n")               ; return mVal;}
  tReturn             FuncV1       (tParam0 p0                        )  volatile       { std::cout << boost::format("QFuncClass::FuncV1       (%1%          )\n") % p0          ; }
  tReturn             FuncCV2      (tParam0 p0, tParam1 p1            )  const volatile { std::cout << boost::format("QFuncClass::FuncCV2      (%1%, %2%     )\n") % p0 % p1     ; }
  tReturn             Func3        (tParam0 p0, tParam1 p1, tParam2 p2)                 { std::cout << boost::format("QFuncClass::Func3        (%1%, %2%, %3%)\n") % p0 % p1 % p2; }
  tReturn __stdcall   StdCallFunc1 (tParam0 p0                        )                 { std::cout << boost::format("QFuncClass::StdCallFunc1 (%1%          )\n") % p0          ; }
  tReturn __fastcall  FastCallFunc2(tParam0 p0, tParam1 p1            )                 { std::cout << boost::format("QFuncClass::FastCallFunc2(%1%, %2%     )\n") % p0 % p1     ; }
  int     __fastcall  FastCallFunc3(tParam0 p0, tParam1 p1, tParam2 p2)                 { std::cout << boost::format("QFuncClass::FastCallFunc3(%1%, %2%, %3%)\n") % p0 % p1 % p2; return mVal; }
  static tReturn      StaticFunc3  (tParam0 p0, tParam1 p1, tParam2 p2)                 { std::cout << boost::format("QFuncClass::StaticFunc3  (%1%, %2%, %3%)\n") % p0 % p1 % p2; }
};

namespace vfs
{
  template<class T> inline T * get_pointer(cPtr<T> const & p)
  {
    return p.getRaw();
  }
  template<class T> inline T * get_pointer(cConstPtr<T> const & p)
  {
    return p.getRaw();
  }
};

void TestReference()
{
  std::cout << "TestReference()\n";

  int a = 1;
  tParam1 b = 2;
  tParam2 c = 3;

  FuncClass someObj(1);

  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncC0       ), &FuncClass::FuncC0       > scopedFuncC0       (someObj         );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncV1       ), &FuncClass::FuncV1       > scopedFuncV1       (someObj, a      );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncCV2      ), &FuncClass::FuncCV2      > scopedFuncCV2      (someObj, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::Func3        ), &FuncClass::Func3        > scopedFunc3        (someObj, a, b, 1.345);
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::StdCallFunc1 ), &FuncClass::StdCallFunc1 > scopedStdCallFunc1 (someObj, a      );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FastCallFunc2), &FuncClass::FastCallFunc2> scopedFastCallFunc2(someObj, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FastCallFunc3), &FuncClass::FastCallFunc3> scopedFastCallFunc3(someObj, a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::StaticFunc3  ), &FuncClass::StaticFunc3  > scopedStaticFunc3  (a, b, c         );

  scoped_exit_func<BOOST_TYPEOF(&FreeFunc0        ), &FreeFunc0        > scopedFreeFunc0; //       (       );
  scoped_exit_func<BOOST_TYPEOF(&FreeFunc1        ), &FreeFunc1        > scopedFreeFunc1        (a      );
  scoped_exit_func<BOOST_TYPEOF(&FreeFunc2        ), &FreeFunc2        > scopedFreeFunc2        (a, b   );
  scoped_exit_func<BOOST_TYPEOF(&FreeFunc3        ), &FreeFunc3        > scopedFreeFunc3        (a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&StdCallFreeFunc1 ), &StdCallFreeFunc1 > scopedStdCallFreeFunc1 (a      );
  scoped_exit_func<BOOST_TYPEOF(&FastCallFreeFunc2), &FastCallFreeFunc2> scopedFastCallFreeFunc2(a, b   );
  scoped_exit_func<BOOST_TYPEOF(&FastCallFreeFunc3), &FastCallFreeFunc3> scopedFastCallFreeFunc3(a, b, c);

  a = 168;
  b = 888;
  *const_cast<float*>(&c) = (float) 77.77;
}

void TestAutoPtr()
{
  std::cout << "TestAutoPtr() - std::auto_ptr\n";

  int a = 1;
  tParam1 b = 2;
  tParam2 c = 3;

  std::auto_ptr<                FuncClass> someObj  (new FuncClass(1));
  std::auto_ptr<const           FuncClass> someObjC (new FuncClass(1));
  std::auto_ptr<volatile        FuncClass> someObjV (new FuncClass(1));
  std::auto_ptr<const volatile  FuncClass> someObjCV(new FuncClass(1));

  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncC0       ), &FuncClass::FuncC0       , boost::add_reference<std::auto_ptr<boost::mpl::_> > > scopedFuncC0       (someObjC         );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncV1       ), &FuncClass::FuncV1       , boost::add_reference<std::auto_ptr<boost::mpl::_> > > scopedFuncV1       (someObjV, a      );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncCV2      ), &FuncClass::FuncCV2      , boost::add_reference<std::auto_ptr<boost::mpl::_> > > scopedFuncCV2      (someObjCV, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::Func3        ), &FuncClass::Func3        , boost::add_reference<std::auto_ptr<boost::mpl::_> > > scopedFunc3        (someObj, a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::StdCallFunc1 ), &FuncClass::StdCallFunc1 , boost::add_reference<std::auto_ptr<boost::mpl::_> > > scopedStdCallFunc1 (someObj, a      );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FastCallFunc2), &FuncClass::FastCallFunc2, boost::add_reference<std::auto_ptr<boost::mpl::_> > > scopedFastCallFunc2(someObj, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FastCallFunc3), &FuncClass::FastCallFunc3, boost::add_reference<std::auto_ptr<boost::mpl::_> > > scopedFastCallFunc3(someObj, a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::StaticFunc3  ), &FuncClass::StaticFunc3  , boost::add_reference<std::auto_ptr<boost::mpl::_> > > scopedStaticFunc3  (a, b, c         );

  a = 168;
  b = 888;
  *const_cast<float*>(&c) = (float) 77.77;
}

BOOST_STATIC_ASSERT((boost::is_same<FuncClass, boost::remove_cv<const FuncClass>::type>::value));
BOOST_STATIC_ASSERT((boost::is_same<int, boost::mpl::lambda<boost::remove_cv<boost::mpl::_> >::type::apply<const int>::type >::value));

BOOST_STATIC_ASSERT((boost::is_same<const FuncClass, boost::mpl::lambda<boost::remove_cv<boost::mpl::_>::type>::type::apply<const FuncClass>::type >::value));
BOOST_STATIC_ASSERT((boost::is_same<FuncClass, boost::mpl::lambda<boost::add_const<boost::mpl::_1>::type>::type::apply<FuncClass>::type >::value));

BOOST_STATIC_ASSERT((boost::is_same<FuncClass, boost::mpl::lambda<boost::remove_cv<boost::mpl::_1> >::type::apply<const FuncClass>::type >::value));

void TestSharedPtr()
{
  std::cout << "TestSharedPtr() - boost::shared_ptr\n";

  int a = 1;
  tParam1 b = 2;
  tParam2 c = 3;

  boost::shared_ptr<FuncClass> someObj(new FuncClass(1));

  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncC0       ), &FuncClass::FuncC0       , boost::shared_ptr<typename boost::remove_cv<boost::mpl::_> > > scopedFuncC0       (someObj         );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncV1       ), &FuncClass::FuncV1       , boost::shared_ptr<typename boost::remove_cv<boost::mpl::_> > > scopedFuncV1       (someObj, a      );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncCV2      ), &FuncClass::FuncCV2      , boost::shared_ptr<typename boost::remove_cv<boost::mpl::_> > > scopedFuncCV2      (someObj, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::Func3        ), &FuncClass::Func3        , boost::shared_ptr<typename boost::remove_cv<boost::mpl::_> > > scopedFunc3        (someObj, a, b, c);

  scoped_exit_func<BOOST_TYPEOF(&FuncClass::StdCallFunc1 ), &FuncClass::StdCallFunc1 , boost::shared_ptr<boost::mpl::_> > scopedStdCallFunc1 (someObj, a      );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FastCallFunc2), &FuncClass::FastCallFunc2, boost::shared_ptr<boost::mpl::_> > scopedFastCallFunc2(someObj, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FastCallFunc3), &FuncClass::FastCallFunc3, boost::shared_ptr<boost::mpl::_> > scopedFastCallFunc3(someObj, a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::StaticFunc3  ), &FuncClass::StaticFunc3  , boost::shared_ptr<boost::mpl::_> > scopedStaticFunc3  (a, b, c         );

  a = 168;
  b = 888;
  *const_cast<float*>(&c) = (float) 77.77;
}


void TestScopedPtr()
{
  std::cout << "TestScopedPtr() - boost::scoped_ptr\n";

  int a = 1;
  tParam1 b = 2;
  tParam2 c = 3;

  boost::scoped_ptr<               FuncClass> someObj  (new FuncClass(1));
  boost::scoped_ptr<const          FuncClass> someObjC (new FuncClass(1));
  boost::scoped_ptr<volatile       FuncClass> someObjV (new FuncClass(1));
  boost::scoped_ptr<const volatile FuncClass> someObjCV(new FuncClass(1));

  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncC0       ), &FuncClass::FuncC0       , boost::add_reference<boost::scoped_ptr<boost::mpl::_> > > scopedFuncC0       (someObjC         );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncV1       ), &FuncClass::FuncV1       , boost::add_reference<boost::scoped_ptr<boost::mpl::_> > > scopedFuncV1       (someObjV, a      );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncCV2      ), &FuncClass::FuncCV2      , boost::add_reference<boost::scoped_ptr<boost::mpl::_> > > scopedFuncCV2      (someObjCV, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::Func3        ), &FuncClass::Func3        , boost::add_reference<boost::scoped_ptr<boost::mpl::_> > > scopedFunc3        (someObj, a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::StdCallFunc1 ), &FuncClass::StdCallFunc1 , boost::add_reference<boost::scoped_ptr<boost::mpl::_> > > scopedStdCallFunc1 (someObj, a      );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FastCallFunc2), &FuncClass::FastCallFunc2, boost::add_reference<boost::scoped_ptr<boost::mpl::_> > > scopedFastCallFunc2(someObj, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FastCallFunc3), &FuncClass::FastCallFunc3, boost::add_reference<boost::scoped_ptr<boost::mpl::_> > > scopedFastCallFunc3(someObj, a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::StaticFunc3  ), &FuncClass::StaticFunc3  , boost::add_reference<boost::scoped_ptr<boost::mpl::_> > > scopedStaticFunc3  (a, b, c         );

  a = 168;
  b = 888;
  *const_cast<float*>(&c) = (float) 77.77;
}

template<template <typename T> class PtrTransformClass >
void TestCopyablePtr()
{
  std::cout << "TestCopyablePtr()\n";

  int a = 1;
  tParam1 b = 2;
  tParam2 c = 3;

  PtrTransformClass<iQFuncClass> someObj(new QFuncClass(1));

  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::FuncC0       ), &iQFuncClass::FuncC0       , PtrTransformClass<typename boost::remove_cv<boost::mpl::_> > > scopedFuncC0       (someObj         );
  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::FuncV1       ), &iQFuncClass::FuncV1       , PtrTransformClass<typename boost::remove_cv<boost::mpl::_> > > scopedFuncV1       (someObj, a      );
  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::FuncCV2      ), &iQFuncClass::FuncCV2      , PtrTransformClass<typename boost::remove_cv<boost::mpl::_> > > scopedFuncCV2      (someObj, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::Func3        ), &iQFuncClass::Func3        , PtrTransformClass<typename boost::remove_cv<boost::mpl::_> > > scopedFunc3        (someObj, a, b, c);

  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::StdCallFunc1 ), &iQFuncClass::StdCallFunc1 , PtrTransformClass<boost::mpl::_> > scopedStdCallFunc1 (someObj, a      );
  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::FastCallFunc2), &iQFuncClass::FastCallFunc2, PtrTransformClass<boost::mpl::_> > scopedFastCallFunc2(someObj, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::FastCallFunc3), &iQFuncClass::FastCallFunc3, PtrTransformClass<boost::mpl::_> > scopedFastCallFunc3(someObj, a, b, c);
  //scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::StaticFunc3  ), &iQFuncClass::StaticFunc3  , PtrTransformClass<boost::mpl::_> > scopedStaticFunc3  (a, b, c         );

  a = 168;
  b = 888;
  *const_cast<float*>(&c) = (float) 77.77;
}

template<template <typename T> class PtrTransformClass>
void TestPointerTransform()
{
  std::cout << "TestPointerTransform()\n";

  int a = 1;
  tParam1 b = 2;
  tParam2 c = 3;

  PtrTransformClass<const volatile FuncClass> someObjCV(new FuncClass(1));
  PtrTransformClass<const          FuncClass> someObjC (new FuncClass(2));
  PtrTransformClass<volatile       FuncClass> someObjV (new FuncClass(3));
  PtrTransformClass<               FuncClass> someObj  (new FuncClass(4));

  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncC0       ), &FuncClass::FuncC0       , boost::add_reference<PtrTransformClass<boost::mpl::_> > > scopedFuncC0       (someObjC         );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncV1       ), &FuncClass::FuncV1       , boost::add_reference<PtrTransformClass<boost::mpl::_> > > scopedFuncV1       (someObjV, a      );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FuncCV2      ), &FuncClass::FuncCV2      , boost::add_reference<PtrTransformClass<boost::mpl::_> > > scopedFuncCV2      (someObjCV, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::Func3        ), &FuncClass::Func3        , boost::add_reference<PtrTransformClass<boost::mpl::_> > > scopedFunc3        (someObj, a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::StdCallFunc1 ), &FuncClass::StdCallFunc1 , boost::add_reference<PtrTransformClass<boost::mpl::_> > > scopedStdCallFunc1 (someObj, a      );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FastCallFunc2), &FuncClass::FastCallFunc2, boost::add_reference<PtrTransformClass<boost::mpl::_> > > scopedFastCallFunc2(someObj, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::FastCallFunc3), &FuncClass::FastCallFunc3, boost::add_reference<PtrTransformClass<boost::mpl::_> > > scopedFastCallFunc3(someObj, a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&FuncClass::StaticFunc3  ), &FuncClass::StaticFunc3  , boost::add_reference<PtrTransformClass<boost::mpl::_> > > scopedStaticFunc3  (a, b, c         );

  a = 168;
  b = 888;
  *const_cast<float*>(&c) = (float) 77.77;


}

void TestQSharedPtr()
{
  std::cout << "TestQSharedPtr() - vfs::cPtr\n";

  int a = 1;
  tParam1 b = 2;
  tParam2 c = 3;

  vfs::cPtr<               QFuncClass> someObj(new QFuncClass(1));
  vfs::cPtr<const          QFuncClass> someObjC (new QFuncClass(1));
  vfs::cPtr<volatile       QFuncClass> someObjV (new QFuncClass(1));
  vfs::cPtr<const volatile QFuncClass> someObjCV(new QFuncClass(1));

  {
    scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncC0       ), &QFuncClass::FuncC0       , vfs::cPtr<typename boost::remove_cv<boost::mpl::_> > > scopedFuncC0       (someObj         );
    scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncV1       ), &QFuncClass::FuncV1       , vfs::cPtr<typename boost::remove_cv<boost::mpl::_> > > scopedFuncV1       (someObj, a      );
    scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncCV2      ), &QFuncClass::FuncCV2      , vfs::cPtr<typename boost::remove_cv<boost::mpl::_> > > scopedFuncCV2      (someObj, a, b   );
    scoped_exit_func<BOOST_TYPEOF(&QFuncClass::Func3        ), &QFuncClass::Func3        , vfs::cPtr<typename boost::remove_cv<boost::mpl::_> > > scopedFunc3        (someObj, a, b, c);
    a = 11;
    b = 22;
    *const_cast<float*>(&c) = (float) 3.3;

    BOOST_STATIC_ASSERT((boost::is_same<boost::fusion::result_of::at_c<
      scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncC0       ), &QFuncClass::FuncC0       , vfs::cPtr<typename boost::remove_cv<boost::mpl::_> > >::param_type, 
      0>::type, vfs::cPtr<QFuncClass> >::value));

    BOOST_STATIC_ASSERT((boost::is_same<boost::fusion::result_of::at_c<
      scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncC0       ), &QFuncClass::FuncC0       , vfs::cPtr<typename boost::remove_cv<boost::mpl::_> > >::ref_param_type, 
      0>::type, vfs::cPtr<QFuncClass> >::value));

    BOOST_STATIC_ASSERT((boost::is_same<boost::fusion::result_of::at_c<
      scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncC0       ), &QFuncClass::FuncC0       , vfs::cPtr<typename boost::remove_cv<boost::mpl::_> > >::call_param_type, 
      0>::type, vfs::cPtr<QFuncClass>& >::value));

    BOOST_STATIC_ASSERT((boost::is_same<boost::mpl::at_c<
      scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncC0       ), &QFuncClass::FuncC0       , vfs::cPtr<typename boost::remove_cv<boost::mpl::_> > >::call_param_type, 
      0>::type, vfs::cPtr<QFuncClass> >::value));

    BOOST_STATIC_ASSERT((boost::is_same<boost::mpl::at_c<
      scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncCV2       ), &QFuncClass::FuncCV2       , vfs::cPtr<typename boost::remove_cv<boost::mpl::_> > >::call_param_type, 
      2>::type, short&>::value));
  }

  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncC0       ), &QFuncClass::FuncC0       , vfs::cPtr<boost::mpl::_> > scopedFuncC0       (someObjC         );
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncV1       ), &QFuncClass::FuncV1       , vfs::cPtr<boost::mpl::_> > scopedFuncV1       (someObjV, a      );
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncCV2      ), &QFuncClass::FuncCV2      , vfs::cPtr<boost::mpl::_> > scopedFuncCV2      (someObjCV, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::Func3        ), &QFuncClass::Func3        , vfs::cPtr<boost::mpl::_> > scopedFunc3        (someObj, a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::StdCallFunc1 ), &QFuncClass::StdCallFunc1 , vfs::cPtr<boost::mpl::_> > scopedStdCallFunc1 (someObj, a      );
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FastCallFunc2), &QFuncClass::FastCallFunc2, vfs::cPtr<boost::mpl::_> > scopedFastCallFunc2(someObj, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FastCallFunc3), &QFuncClass::FastCallFunc3, vfs::cPtr<boost::mpl::_> > scopedFastCallFunc3(someObj, a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::StaticFunc3  ), &QFuncClass::StaticFunc3  , vfs::cPtr<boost::mpl::_> > scopedStaticFunc3  (a, b, c         );

  a = 168;
  b = 888;
  *const_cast<float*>(&c) = (float) 77.77;
}


void TestQPtr()
{
  std::cout << "TestQPtr() - vfs::cPtr\n";

  int a = 1;
  tParam1 b = 2;
  tParam2 c = 3;

  vfs::cPtr<               QFuncClass> someObj  (new QFuncClass(1));
  vfs::cPtr<const          QFuncClass> someObjC (new QFuncClass(1));
  vfs::cPtr<volatile       QFuncClass> someObjV (new QFuncClass(1));
  vfs::cPtr<const volatile QFuncClass> someObjCV(new QFuncClass(1));

  // the construction of someObjC, someObjV and someObjCV in the following way won't compile.
  //const          QFuncClass*  pC (new QFuncClass(1));
  //volatile       QFuncClass*  pV (new QFuncClass(1));
  //const volatile QFuncClass*  pCV(new QFuncClass(1));
  //vfs::cPtr<const          QFuncClass> someObjC (pC );
  //vfs::cPtr<volatile       QFuncClass> someObjV (pV );
  //vfs::cPtr<const volatile QFuncClass> someObjCV(pCV);


  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncC0       ), &QFuncClass::FuncC0       , boost::add_reference<vfs::cPtr<boost::mpl::_> > > scopedFuncC0       (someObjC         );
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncV1       ), &QFuncClass::FuncV1       , boost::add_reference<vfs::cPtr<boost::mpl::_> > > scopedFuncV1       (someObjV, a      );
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FuncCV2      ), &QFuncClass::FuncCV2      , boost::add_reference<vfs::cPtr<boost::mpl::_> > > scopedFuncCV2      (someObjCV, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::Func3        ), &QFuncClass::Func3        , boost::add_reference<vfs::cPtr<boost::mpl::_> > > scopedFunc3        (someObj, a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::StdCallFunc1 ), &QFuncClass::StdCallFunc1 , boost::add_reference<vfs::cPtr<boost::mpl::_> > > scopedStdCallFunc1 (someObj, a      );
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FastCallFunc2), &QFuncClass::FastCallFunc2, boost::add_reference<vfs::cPtr<boost::mpl::_> > > scopedFastCallFunc2(someObj, a, b   );
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FastCallFunc3), &QFuncClass::FastCallFunc3, boost::add_reference<vfs::cPtr<boost::mpl::_> > > scopedFastCallFunc3(someObj, a, b, c);
  scoped_exit_func<BOOST_TYPEOF(&QFuncClass::StaticFunc3  ), &QFuncClass::StaticFunc3  , boost::add_reference<vfs::cPtr<boost::mpl::_> > > scopedStaticFunc3  (a, b, c         );

  {
    scoped_exit_func<BOOST_TYPEOF(&QFuncClass::Func3        ), &QFuncClass::Func3        , vfs::cPtr<boost::mpl::_> > scopedFunc3        (someObj, a, b, c);
    scoped_exit_func<BOOST_TYPEOF(&QFuncClass::StdCallFunc1 ), &QFuncClass::StdCallFunc1 , vfs::cPtr<boost::mpl::_> > scopedStdCallFunc1 (someObj, a      );
    scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FastCallFunc2), &QFuncClass::FastCallFunc2, vfs::cPtr<boost::mpl::_> > scopedFastCallFunc2(someObj, a, b   );
    scoped_exit_func<BOOST_TYPEOF(&QFuncClass::FastCallFunc3), &QFuncClass::FastCallFunc3, vfs::cPtr<boost::mpl::_> > scopedFastCallFunc3(someObj, a, b, c);
    scoped_exit_func<BOOST_TYPEOF(&QFuncClass::StaticFunc3  ), &QFuncClass::StaticFunc3  , vfs::cPtr<boost::mpl::_> > scopedStaticFunc3  (a, b, c         );
    a = 11;
    b = 22;
    *const_cast<float*>(&c) = (float) 3.3;
  }

  a = 168;
  b = 888;
  *const_cast<float*>(&c) = (float) 77.77;
}

void TestQConstPtr()
{
  std::cout << "TestQConstPtr() - vfs::cConstPtr\n";

  int a = 1;
  tParam1 b = 2;
  tParam2 c = 3;

  vfs::cConstPtr<               iQFuncClass> someObj  (new QFuncClass(1));
  vfs::cConstPtr<const          iQFuncClass> someObjC (new QFuncClass(1));
  vfs::cConstPtr<volatile       iQFuncClass> someObjV (new QFuncClass(1));
  vfs::cConstPtr<const volatile iQFuncClass> someObjCV(new QFuncClass(1));

  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::FuncC0       ), &iQFuncClass::FuncC0       , boost::add_reference<vfs::cConstPtr<boost::mpl::_> > > scopedFuncC0       (someObjC         );
  //scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::FuncV1       ), &iQFuncClass::FuncV1       , boost::add_reference<vfs::cConstPtr<boost::mpl::_> > > scopedFuncV1       (someObjV, a      );
  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::FuncCV2      ), &iQFuncClass::FuncCV2      , boost::add_reference<vfs::cConstPtr<boost::mpl::_> > > scopedFuncCV2      (someObjCV, a, b   );
  //scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::Func3        ), &iQFuncClass::Func3        , boost::add_reference<vfs::cConstPtr<boost::mpl::_> > > scopedFunc3        (someObjC, a, b, c);
  //scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::StdCallFunc1 ), &iQFuncClass::StdCallFunc1 , boost::add_reference<vfs::cConstPtr<boost::mpl::_> > > scopedStdCallFunc1 (someObj, a      );
  //scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::FastCallFunc2), &iQFuncClass::FastCallFunc2, boost::add_reference<vfs::cConstPtr<boost::mpl::_> > > scopedFastCallFunc2(someObj, a, b   );
  //scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::FastCallFunc3), &iQFuncClass::FastCallFunc3, boost::add_reference<vfs::cConstPtr<boost::mpl::_> > > scopedFastCallFunc3(someObj, a, b, c);
  ////scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::StaticFunc3  ), &iQFuncClass::StaticFunc3  , boost::add_reference<vfs::cConstPtr<boost::mpl::_> > > scopedStaticFunc3  (a, b, c         );

  //{
  //  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::Func3        ), &iQFuncClass::Func3        , vfs::cConstPtr<boost::mpl::_> > scopedFunc3        (someObj, a, b, c);
  //  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::StdCallFunc1 ), &iQFuncClass::StdCallFunc1 , vfs::cConstPtr<boost::mpl::_> > scopedStdCallFunc1 (someObj, a      );
  //  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::FastCallFunc2), &iQFuncClass::FastCallFunc2, vfs::cConstPtr<boost::mpl::_> > scopedFastCallFunc2(someObj, a, b   );
  //  scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::FastCallFunc3), &iQFuncClass::FastCallFunc3, vfs::cConstPtr<boost::mpl::_> > scopedFastCallFunc3(someObj, a, b, c);
  //  //scoped_exit_func<BOOST_TYPEOF(&iQFuncClass::StaticFunc3  ), &iQFuncClass::StaticFunc3  , vfs::cConstPtr<boost::mpl::_> > scopedStaticFunc3  (a, b, c         );
  //  a = 11;
  //  b = 22;
  //  *const_cast<float*>(&c) = (float) 3.3;
  //}

  a = 168;
  b = 888;
  *const_cast<float*>(&c) = (float) 77.77;
}


void TestedOK()
{
  TestReference();
  TestAutoPtr();
  TestScopedPtr();
  TestSharedPtr();
#ifndef _DEBUG 
  //TestPointerTransform<vfs::cPtr>(); // Cannot use this template since the object needs to be derived from cRefCount to use cPtr
  TestQPtr();
#endif


  std::cout << "TestPointerTransform<boost::shared_ptr>()\n";
  TestPointerTransform<boost::shared_ptr>();

  std::cout << "TestPointerTransform<boost::scoped_ptr>()\n";
  TestPointerTransform<boost::scoped_ptr>();

  std::cout << "TestPointerTransform<std::auto_ptr>()\n";
  TestPointerTransform<std::auto_ptr>();

  std::cout << "TestCopyablePtr<boost::shared_ptr>()\n";
  TestCopyablePtr<boost::shared_ptr>();

  std::cout << "TestCopyablePtr<vfs::cPtr>()\n";
  TestCopyablePtr<vfs::cPtr>();

  TestQConstPtr();

  TestQSharedPtr();
}

