// Copyright (c) 2023 Muqiu Han
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//     notice,
//       this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Terifo nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef SWALLOW_TYPE_H
#define SWALLOW_TYPE_H

#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace swallow::type
{
  class Type
  {
  public:
    using Ptr = std::shared_ptr<Type>;

  public:
    virtual ~Type() = default;
  };

  class TypeVar : public Type
  {
  public:
    const std::string Name;

    explicit TypeVar(const std::string Name)
      : Name(std::move(Name))
    {
    }
  };

  class TypeBase : public Type
  {
  public:
    const std::string Name;

    explicit TypeBase(const std::string Name)
      : Name(std::move(Name))
    {
    }
  };

  class TypeArrow : public Type
  {
  public:
    const Type::Ptr Left;
    const Type::Ptr Right;

    TypeArrow(const Type::Ptr Left, const Type::Ptr Right)
      : Left(std::move(Left))
      , Right(std::move(Right))
    {
    }
  };

  class TypeManager
  {
  private:
    int32_t LastID = 0;
    std::map<std::string, Type::Ptr> Types;

  public:
    std::string newTypeName() noexcept;
    Type::Ptr newType() noexcept;
    Type::Ptr newArrowType() noexcept;

    /** Find values for placeholder variables such that they can equal. */
    void unify(Type::Ptr left, Type::Ptr right) noexcept;

    /** Get to the bottom of a chain of equations. */
    Type::Ptr resolve(Type::Ptr type, TypeVar *& var) noexcept;

    /** Map a type variable of some name to a type. */
    void bind(const std::string & name, Type::Ptr type) noexcept;
  };
} // namespace swallow::type

#endif