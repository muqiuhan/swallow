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

#include "runtime.h"
#include "panic/panic.hpp"
#include "runtime/node.h"
#include "runtime/stack.h"

namespace swallow::compiler::runtime
{
  [[nodiscard]] auto Runtime::Eval(node::Base *node) noexcept -> node::Base *
  {
    Stack::Initialize(&Stack);
    Stack::Push(&Stack, node);
    Unwind();

    auto *result = Stack::Pop(&Stack);
    Stack::Free(&Stack);
    return result;
  }

  void Runtime::Unwind() noexcept
  {
    while (true)
      {
        auto *peek = Stack::Peek(&Stack, 0);

        if (peek->Tag == node::Tag::APPLICATION)
          Stack::Push(&Stack, reinterpret_cast<node::Application *>(peek)->Left);
        else if (peek->Tag == node::Tag::GLOBAL)
          {
            auto *node = reinterpret_cast<node::Global *>(peek);

            if (Stack.Count < node->Arity)
              utils::Panic("ICE: Stack->Count > node->Arity");

            for (size_t i = 1; i <= node->Arity; i++)
              Stack.Data[Stack.Count - i] =
                reinterpret_cast<node::Application *>(Stack.Data[Stack.Count - i - 1])
                  ->Right;

            node->Function(&Stack);
          }
        else if (peek->Tag == node::Tag::IND)
          {
            const auto *_ = Stack::Pop(&Stack);
            Stack::Push(&Stack, reinterpret_cast<node::Ind *>(peek)->Next);
          }
        else
          break;
      }
  }

} // namespace swallow::compiler::runtime

void AddFunction(swallow::compiler::runtime::stack::Stack *stack) noexcept
{
  swallow::compiler::runtime::stack::Stack::Push(
    stack,
    reinterpret_cast<swallow::compiler::runtime::node::Base *>(
      swallow::compiler::runtime::node::Int::Allocate(
        reinterpret_cast<swallow::compiler::runtime::node::Int *>(
          swallow::compiler::runtime::Runtime::Eval(
            swallow::compiler::runtime::stack::Stack::Peek(stack, 0)))
          ->Value
        + reinterpret_cast<swallow::compiler::runtime::node::Int *>(
            swallow::compiler::runtime::Runtime::Eval(
              swallow::compiler::runtime::stack::Stack::Peek(stack, 1)))
            ->Value)));
}

void EntryPoint(swallow::compiler::runtime::stack::Stack *stack) noexcept
{
  swallow::compiler::runtime::stack::Stack::Push(
    stack,
    reinterpret_cast<swallow::compiler::runtime::node::Base *>(
      swallow::compiler::runtime::node::Int::Allocate(10)));

  swallow::compiler::runtime::stack::Stack::Push(
    stack,
    reinterpret_cast<swallow::compiler::runtime::node::Base *>(
      swallow::compiler::runtime::node::Int::Allocate(20)));

  swallow::compiler::runtime::stack::Stack::Push(
    stack,
    reinterpret_cast<swallow::compiler::runtime::node::Base *>(
      swallow::compiler::runtime::node::Global::Allocate(AddFunction, 2)));

  swallow::compiler::runtime::stack::Stack::Push(
    stack,
    reinterpret_cast<swallow::compiler::runtime::node::Base *>(
      swallow::compiler::runtime::node::Application::Allocate(
        swallow::compiler::runtime::stack::Stack::Pop(stack),
        swallow::compiler::runtime::stack::Stack::Pop(stack))));

  swallow::compiler::runtime::stack::Stack::Push(
    stack,
    reinterpret_cast<swallow::compiler::runtime::node::Base *>(
      swallow::compiler::runtime::node::Application::Allocate(
        swallow::compiler::runtime::stack::Stack::Pop(stack),
        swallow::compiler::runtime::stack::Stack::Pop(stack))));
}