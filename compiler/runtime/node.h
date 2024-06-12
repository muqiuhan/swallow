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

#ifndef SWALLOW_COMPILER_RUNTIME_NODE_H
#define SWALLOW_COMPILER_RUNTIME_NODE_H

#include <cstdint>
#include <functional>
#include <vector>

namespace swallow::compiler::runtime::node
{
  enum class Tag
  {
    APPLICATION,
    INT,
    GLOBAL,
    IND,
    DATA
  };

  class Base
  {
  public:
    enum Tag Tag;

  public:
    [[nodiscard]] static auto Allocate() noexcept -> class Base *;
  };

  class Application
  {
  public:
    class Base        Base;
    const class Base *Left;
    const class Base *Right;

  public:
    [[nodiscard]] static auto Allocate(
      const class Base *Left, const class Base *Right) noexcept -> class Application *;
  };

  class Int
  {
  public:
    class Base Base;
    int32_t    Value;

  public:
    [[nodiscard]] static auto Allocate(int32_t Value) noexcept -> class Int *;
  };

  class Global
  {
  public:
    class Base Base;
    int32_t    Arity;

    std::function<void(class Stack *)> Function;

  public:
    [[nodiscard]] static auto Allocate(
      const std::function<void(class Stack *)> &Function,
      int32_t                                   Arity) noexcept -> class Global *;
  };

  class Ind
  {
  public:
    class Base        Base;
    const class Base *Next;

  public:
    [[nodiscard]] static auto Allocate(const class Base *Next) noexcept -> class Ind *;
  };

  class Data
  {
  public:
    class Base   Base;
    enum Tag     Tag;
    class Base **Array;
  };

} // namespace swallow::compiler::runtime::node

#endif /* SWALLOW_COMPILER_RUNTIME_HPP*/