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

#ifndef SWALLOW_COMPILER_RUNTIME_STACK_H
#define SWALLOW_COMPILER_RUNTIME_STACK_H

#include "runtime/node.h"
#include <cstdint>

namespace swallow::compiler::runtime::stack
{
  class Stack
  {
  public:
    uint64_t           Size;
    uint64_t           Count;
    class node::Base **Data;

  public:
    static void Initialize(class Stack *stack) noexcept;
    static void Free(class Stack *stack) noexcept;
    static void Push(class Stack *stack, class node::Base *node) noexcept;

    [[nodiscard]]
    static auto Pop(class Stack *stack) noexcept -> class node::Base *;
    [[nodiscard]]
    static auto Peek(class Stack *stack, uint64_t o) noexcept -> class node::Base *;

    static void PopN(class Stack *stack, uint64_t n) noexcept;
    static void Slide(class Stack *stack, uint64_t n) noexcept;
    static void Update(class Stack *stack, uint64_t o) noexcept;
    static void Allocate(class Stack *stack, uint64_t o) noexcept;
    static void Pack(class Stack *stack, uint64_t n, node::Tag) noexcept;
    static void Split(class Stack *stack, uint64_t n) noexcept;
  };
} // namespace swallow::compiler::runtime::stack

#endif /* SWALLOW_COMPILER_RUNTIME_STACK_H */