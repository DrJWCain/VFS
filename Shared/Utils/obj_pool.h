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

#ifndef BOOST_PP_IS_ITERATING

#  ifndef OBJ_POOL_H_INCLUDED
#    define OBJ_POOL_H_INCLUDED

#    include <boost/preprocessor/repetition.hpp>
#    include <boost/preprocessor/arithmetic/sub.hpp>
#    include <boost/preprocessor/punctuation/comma_if.hpp>
#    include <boost/preprocessor/iteration/iterate.hpp>

#include <boost/function_types/parameter_types.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#include <boost/none_t.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/tuple/tuple.hpp>

#include <vector>

struct multi_thread_safe {};
struct single_thread_safe {};

struct on_empty_return_null {};
struct on_empty_wait {};
struct on_empty_create {};

// namespace obj_pool_priv contains the internal implementation for the policies controlling the pool's behavior
namespace obj_pool_priv
{

template<class tag> struct thread_safety_policy;

struct dummy_mutex : boost::noncopyable
{
  void lock() {}
  void unlock() {}
};

template<>
struct thread_safety_policy<multi_thread_safe>
{
  typedef boost::recursive_mutex type;
};

template<>
struct thread_safety_policy<single_thread_safe>
{
  typedef dummy_mutex type;
};


template<class T, class tag>
struct on_empty_policy;

template<class T>
struct on_empty_policy<T, on_empty_return_null>
{
  on_empty_policy(const typename T::arg_type&) {}
  typename T::ele_type get(T&) { return typename T::ele_type((typename T::obj_type*) NULL); }
  void on_put() {}
};

template<class T>
struct on_empty_policy<T, on_empty_wait>
{
  on_empty_policy(const typename T::arg_type&) {}
  typename T::ele_type get(T& pool) 
  { 
    boost::unique_lock<boost::mutex> lock(m_mutex);
    m_cond.wait(lock);  
    return pool.get_last();  
  }

  void on_put() {m_cond.notify_one(); }
private:
  boost::condition_variable m_cond;
  boost::mutex m_mutex;
};

template<class T>
struct on_empty_policy<T, on_empty_create>
{
  on_empty_policy(const typename T::arg_type& arg) : m_arg(arg) {}
  typename T::ele_type get(T& pool) { return pool.create_obj(m_arg); }
  void on_put() {}
private:
  typename T::arg_type m_arg;
};
} // obj_pool_priv


#    ifndef MAX_NUM_OBJ_POOL_PARAMS
#      define MAX_NUM_OBJ_POOL_PARAMS 4
#    endif

#define MACRO_OBJ_POOL_DEF_PARAMS(z, n, d) BOOST_PP_CAT(class T, n = d) 
#define MACRO_OBJ_POOL_DEF(z, n, d) d 
#define MACRO_OBJ_POOL_TUPLE(z, n, t) boost::tuples::get<n>(t) 
// primary template
template<template<class> class P, class T, class get_on_empty, class thread_safety BOOST_PP_ENUM_TRAILING(MAX_NUM_OBJ_POOL_PARAMS, MACRO_OBJ_POOL_DEF_PARAMS, boost::none_t)>
class obj_pool;

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, 2, "obj_pool.h"))
#include BOOST_PP_ITERATE()

#undef MACRO_OBJ_POOL_DEF_PARAMS
#undef MACRO_OBJ_POOL_DEF
#undef MACRO_OBJ_POOL_TUPLE

#  endif // OBJ_POOL_H_INCLUDED

#else // BOOST_PP_IS_ITERATING

#  define n BOOST_PP_ITERATION()

// specialization pattern
template<template<class> class P, class T, class get_on_empty, class thread_safety BOOST_PP_ENUM_TRAILING_PARAMS(n, class T)>
class obj_pool<P, T, get_on_empty, thread_safety BOOST_PP_ENUM_TRAILING_PARAMS(n, T) 
  BOOST_PP_ENUM_TRAILING(BOOST_PP_SUB(MAX_NUM_OBJ_POOL_PARAMS, n), MACRO_OBJ_POOL_DEF, boost::none_t)> 
{
public:
  typedef typename boost::tuples::tuple<BOOST_PP_ENUM_PARAMS(n, T)> arg_type;
  typedef T obj_type;
  typedef P<T> ele_type;
  typedef obj_pool<P, T, get_on_empty, thread_safety BOOST_PP_ENUM_TRAILING_PARAMS(n, T) 
    BOOST_PP_ENUM_TRAILING(BOOST_PP_SUB(MAX_NUM_OBJ_POOL_PARAMS, n), MACRO_OBJ_POOL_DEF, boost::none_t)> obj_pool_type;
  typedef typename obj_pool_priv::on_empty_policy<obj_pool_type, get_on_empty> on_empty_policy_type;
  typedef typename obj_pool_priv::thread_safety_policy<thread_safety>::type mutex_type;

  obj_pool(size_t numAllocated BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(n, T, p)) 
    : m_on_empty_policy(arg_type(BOOST_PP_ENUM_PARAMS(n, p)))
    , m_numAllocated(numAllocated)
    , m_mutex()
  {
    for(size_t i = 0; i < numAllocated; ++i)
    {
      m_pool.push_back(P<T>(new T(BOOST_PP_ENUM_PARAMS(n, p))));
    }
    //std::cout << "obj_pool::obj_pool() - m_numAllocated: " << m_numAllocated << std::endl;
  }
  ~obj_pool(void) 
  {
    //std::cout << "obj_pool::~obj_pool() - size: " << m_pool.size() << "m_numAllocated: " << m_numAllocated << std::endl;
  };

  ele_type get() 
  {
    ele_type t;
    bool empty = true;
    {
      boost::lock_guard<mutex_type> lock(m_mutex);
      if(!m_pool.empty())
      {
        empty = false;
        t = m_pool.back();
        m_pool.pop_back();
      }
    }

    if(empty)
    {
      t = m_on_empty_policy.get(*this);
    }
    
    //std::cout << "obj_pool::get() - size: " << m_pool.size() << std::endl;
    
    return t;
  }

  void put(const ele_type& t)
  {
    boost::lock_guard<mutex_type> lock(m_mutex);
    m_pool.push_back(t);
    m_on_empty_policy.on_put();
  }


private:
  friend on_empty_policy_type;

  ele_type get_last() 
  {
    ele_type t;
    boost::lock_guard<mutex_type> lock(m_mutex);
    t = m_pool.back();
    m_pool.pop_back();
    
    return t;
  }

  ele_type create_obj(const arg_type& arg)
  {
    boost::lock_guard<mutex_type> lock(m_mutex);
    ++m_numAllocated;
    return ele_type(new T(BOOST_PP_ENUM(n, MACRO_OBJ_POOL_TUPLE, arg)));
  }

  on_empty_policy_type m_on_empty_policy;
  std::vector<ele_type > m_pool;
  size_t m_numAllocated;
  mutex_type m_mutex;
};

#  undef n

#endif // BOOST_PP_IS_ITERATING
