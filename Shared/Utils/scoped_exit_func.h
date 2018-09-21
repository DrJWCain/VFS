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

// Description:
// scoped_exit_func is a template that allows you to set up a function call by reference parameters 
// and call the function at the end of the scrope of the object of its initiated type.
// More details are described in vfs's R&D Wiki under "Automactically Call a Function at the End of Scope"

// Usage:
// Instead of explaining its usage, which has includes the scenarios of calling member functions by 
// using various smart pointers of its object and oject reference,  a sample file TestScopedExitFunc.cpp 
// illustrating its usage is added to the same directory.


#ifndef BOOST_PP_IS_ITERATING

#  ifndef SCOPED_EXIT_FUNC_INCLUDED
#    define SCOPED_EXIT_FUNC_INCLUDED

#pragma warning( disable : 4819)

#define BOOST_FT_CC_NAMES 
#define BOOST_FT_AUTODETECT_CALLING_CONVENTIONS

#include <boost/noncopyable.hpp>

#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/functional/invocation/invoke_procedure.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/transform.hpp>

#include <boost/function_types/config/config.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>

#include <boost/preprocessor/repetition/enum.hpp> 
#include <boost/preprocessor/cat.hpp> 
#include <boost/preprocessor/repetition/enum_params.hpp> 

#define MAX_NUM_PARAMS 6

template<class tFunct, tFunct func, class ClassTransform = boost::add_reference<boost::mpl::_>, int n = boost::function_types::function_arity<tFunct>::value>
class scoped_exit_func;

#define MACRO_CALL_PARAMS(z, n, p) typename boost::mpl::at_c<ref_param_type, n>::type BOOST_PP_CAT(p, n)

template<class tFunct, tFunct func, class ClassTransform>                                                 
class scoped_exit_func<tFunct, func, ClassTransform, 0> : boost::noncopyable                                     
{                                                                                                         
public:                                                                                                   
  scoped_exit_func() {}        
  ~scoped_exit_func() {func(); }                                                  
};


// generate specializations
#    define BOOST_PP_ITERATION_LIMITS (1, MAX_NUM_PARAMS - 1)
#    define BOOST_PP_FILENAME_1       "scoped_exit_func.h" // this file
#    include BOOST_PP_ITERATE()

#undef MAX_NUM_PARAMS
#undef MACRO_CALL_PARAMS

#  endif // SCOPED_EXIT_FUNC_INCLUDED

#else // BOOST_PP_IS_ITERATING

#  define n BOOST_PP_ITERATION()


template<class tFunct, tFunct func, class ClassTransform>                                                 
class scoped_exit_func<tFunct, func, ClassTransform, n> : boost::noncopyable                                     
{                                                                                                         
public:                                                                                                   
  typedef typename boost::function_types::parameter_types<tFunct, ClassTransform>::type param_type;                              
  
  typedef typename boost::mpl::if_< 
    boost::function_types::is_member_function_pointer<tFunct>,                                                                                                  
    typename boost::mpl::push_front<                                                                             
        typename boost::mpl::transform<                                                                          
            typename boost::mpl::pop_front<param_type>::type,                                                    
            boost::add_reference<boost::mpl::_1>                                                                             
          >::type,                                                                                        
        typename boost::mpl::deref<typename boost::mpl::begin<param_type>::type>::type                                  
    >::type,                                                                                              
    typename boost::mpl::transform<param_type, boost::add_reference<boost::mpl::_1> >::type 
  >::type ref_param_type;                                                                                 

  typedef typename boost::fusion::result_of::as_vector<ref_param_type>::type call_param_type;                    
                                                                                                          
  scoped_exit_func(BOOST_PP_ENUM(n, MACRO_CALL_PARAMS, p)) : mParms(BOOST_PP_ENUM_PARAMS(n, p)) {}        
  ~scoped_exit_func() {boost::fusion::invoke_procedure(func, mParms); }                                                  
private:                                                                                                  
  call_param_type mParms;                                                                                 
};

#  undef n

#endif // BOOST_PP_IS_ITERATING
