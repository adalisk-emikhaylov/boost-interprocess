//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gazta�aga 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_MOVE_HPP
#define BOOST_INTERPROCESS_MOVE_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>
#include <boost/mpl/if.hpp>

/*!\file
   Describes a function and a type to emulate move semantics.
*/
namespace boost {
namespace interprocess {

/*!Trait class to detect if a type is movable*/
template <class T>
struct is_movable
{
   enum {  value = false };
};

namespace detail {

/*!An object that represents a moved object.*/
template<class T>
struct moved_object
{  
   moved_object(const T &obj)
      :  m_obj(const_cast<T *>(&obj))
   {}

   T &get() const
   {  return *m_obj;  }

   private:
   T *m_obj; 
};

// Metafunction that, given moveable T, provides move_source<T>, else T&.
template <typename T>
struct move_type
{
   public: // metafunction result
   typedef typename mpl::if_<
      is_movable<T>
      , moved_object<T>
      , T&
      >::type type;
};

template <typename T> 
class move_return
{
   private:
   T m_moved;

   public:
   typedef T type;

   move_return(T& returned)
      : m_moved(moved_object<T>(returned))
   {}

   move_return(const move_return& operand)
      : m_moved(const_cast<move_return&>(operand))
   {}

   operator moved_object<T>()
   {  return moved_object<T>(m_moved);  }
};

template <typename T>
struct return_type
{
   public: // metafunction result

   typedef typename boost::mpl::if_
      <is_movable<T>
      , move_return<T>
      , T
      >::type type;
};

}  //namespace detail {

/*!A function that converts an object to a moved object so that 
   it can match a function taking a detail::moved_object object.*/
template<class Object>
typename detail::move_type<Object>::type move
   (const Object &lock)
{  
   typedef typename detail::move_type<Object>::type type;
   return type(lock);   
}

}  //namespace interprocess {
}  //namespace boost {

#include <boost/interprocess/detail/config_end.hpp>

#endif   //#ifndef BOOST_INTERPROCESS_MOVE_HPP