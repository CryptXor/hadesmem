// Copyright Joshua Boyce 2010-2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// This file is part of HadesMem.
// <http://www.raptorfactor.com/> <raptorfactor@raptorfactor.com>

#pragma once

#include <type_traits>

#include "hadesmem/error.hpp"
#include "hadesmem/config.hpp"

namespace hadesmem
{

namespace detail
{

template <typename T>
class Optional
{
public:
  Optional() HADESMEM_NOEXCEPT
    : valid_(false), 
    t_()
  { }

  explicit Optional(T const& t)
    : valid_(true), 
    t_()
  {
    Create(t);
  }

  Optional(Optional const& other)
    : valid_(false), 
    t_()
  {
    if (other.IsValid())
    {
      Create(*other);
    }
  }

  Optional& operator=(Optional const& other)
  {
    Destroy();

    if (other.IsValid())
    {
      Create(*other);
    }

    return *this;
  }

  Optional& operator=(T const& t)
  {
    Destroy();
    Create(t);

    return *this;
  }

  ~Optional()
  {
    Destroy();
  }

  bool IsValid() const HADESMEM_NOEXCEPT
  {
    return valid_;
  }

  T& operator*() HADESMEM_NOEXCEPT
  {
    return const_cast<T&>(static_cast<Optional const*>(this)->operator*());
  }

  T const& operator*() const HADESMEM_NOEXCEPT
  {
    if (IsValid())
    {
      return *static_cast<T const*>(static_cast<void const*>(&t_));
    }

    HADESMEM_THROW_EXCEPTION(Error() << 
      ErrorString("Type is invalid."));
  }

  T* operator->() HADESMEM_NOEXCEPT
  {
    return const_cast<T*>(static_cast<Optional const*>(this)->operator->());
  }

  T const* operator->() const HADESMEM_NOEXCEPT
  {
    if (IsValid())
    {
      return static_cast<T const*>(static_cast<void const*>(&t_));
    }

    HADESMEM_THROW_EXCEPTION(Error() << 
      ErrorString("Type is invalid."));
  }

private:
  void Destroy()
  {
    if (IsValid())
    {
      static_cast<T*>(static_cast<void*>(&t_))->~T();
      valid_ = false;
    }
  }

  void Create(T const& t)
  {
    new (&t_) T(t);
    valid_ = true;
  }

  bool valid_;
  typename std::aligned_storage<sizeof(T), 
    std::alignment_of<T>::value>::type t_;
};

}

}