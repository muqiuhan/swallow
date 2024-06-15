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

namespace swallow::compiler::runtime::stack
{
  class Stack;
} // namespace swallow::compiler::runtime::stack

namespace swallow::compiler::runtime::node
{
  enum class Tag : int8_t
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
    [[nodiscard]] static auto Allocate() noexcept -> Base *;
  };

  class Application
  {
  public:
    Base  Node;
    Base *Left;
    Base *Right;

  public:
    [[nodiscard]] static auto Allocate(Base *Left, Base *Right) noexcept -> Application *;
  };

  class Int
  {
  public:
    Base    Node;
    int32_t Value;

  public:
    [[nodiscard]] static auto Allocate(int32_t Value) noexcept -> Int *;
  };

  class Global
  {
  public:
    Base    Node;
    int32_t Arity;
    void (*Function)(runtime::stack::Stack *);

  public:
    [[nodiscard]] static auto Allocate(void (*Function)(runtime::stack::Stack *), int32_t Arity) noexcept -> Global *;
  };

  class Ind
  {
  public:
    Base *Next;
    Base  Node;

  public:
    [[nodiscard]] static auto Allocate(Base *Next) noexcept -> Ind *;
  };

  class Data
  {
  public:
    enum Tag Tag;
    Base   **Array;
    Base     Node;
  };

} // namespace swallow::compiler::runtime::node

#endif /* SWALLOW_COMPILER_RUNTIME_HPP*/