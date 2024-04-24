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
//     * Neither the name of Swallow nor the names of its contributors
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

#ifndef SWALLOW_COMPILER_TYPE_TYPE_H
#define SWALLOW_COMPILER_TYPE_TYPE_H

#include "result/result.hpp"
#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace swallow::compiler::type
{
  class Manager;

  class Type
  {
  public:
    using Ptr = std::shared_ptr<Type>;

    virtual ~Type() = default;

    virtual void Dump(const Manager &typeManager, std::ostream &to) const noexcept = 0;
  };

  class Variable final : public Type
  {
  public:
    const std::string Name;

    explicit Variable(std::string Name) : Name(std::move(Name)) {}

    void Dump(const Manager &typeManager, std::ostream &to) const noexcept override;
  };

  class Base : public Type
  {
  public:
    const std::string Name;

    explicit Base(std::string Name) : Name(std::move(Name)) {}

    void Dump(const Manager &typeManager, std::ostream &to) const noexcept override;
  };

  class Data final : public Base
  {
  public:
    struct Constructor
    {
      uint8_t Tag;
    };

    std::map<std::string, Constructor> Constructors;

    explicit Data(std::string Name) : Base(std::move(Name)) {}
  };

  class Arrow final : public Type
  {
  public:
    const Ptr Left;
    const Ptr Right;

    Arrow(Ptr Left, Ptr Right) : Left(std::move(Left)), Right(std::move(Right)) {}

    void Dump(const Manager &typeManager, std::ostream &to) const noexcept override;
  };

  class Manager
  {
  public:
    int32_t                          LastID = 0;
    std::map<std::string, Type::Ptr> Types;

    auto                             NewTypeName() noexcept -> std::string;
    auto                             NewType() noexcept -> Type::Ptr;
    auto                             NewArrowType() noexcept -> Type::Ptr;

    /** Find values for placeholder variables such that they can equal. */
    auto                             Unify(Type::Ptr left, Type::Ptr right) noexcept
      -> utils::Result<utils::Void, utils::Void>;

    /** Get to the bottom of a chain of equations. */
    auto Resolve(Type::Ptr type, Variable *&var) const noexcept -> Type::Ptr;

    /** Map a type variable of some name to a type. */
    void Bind(const std::string &name, const Type::Ptr &type) noexcept;
  };
} // namespace swallow::compiler::type

#endif