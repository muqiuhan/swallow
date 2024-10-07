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

#include "stack.h"
#include <cstdlib>
#include <cstring>

namespace swallow::compiler::runtime::stack
{
  void Stack::Initialize(Stack *stack) noexcept
  {
    stack->Size = 4;
    stack->Count = 0;
    stack->Data = reinterpret_cast<node::Base **>(std::malloc(sizeof(*(stack->Data)) * stack->Size));

    if (nullptr == stack->Data)
      Panic("ICE: Cannot initialize runtime stack");
  }

  void Stack::Free(Stack *stack) noexcept { free(stack->Data); }

  void Stack::Push(Stack *stack, node::Base *node) noexcept
  {
    while (stack->Count >= stack->Size)
      {
        stack->Data =
          reinterpret_cast<node::Base **>(realloc(stack->Data, (sizeof(*(stack->Data)) * (stack->Size *= 2))));

        if (nullptr == stack->Data)
          Panic("ICE: Unable to expand runtime stack");
      }

    stack->Data[stack->Count++] = node;
  }

  [[nodiscard]] auto Stack::Pop(Stack *stack) noexcept -> node::Base *
  {
    if (stack->Count < 0)
      Panic("ICE: Cannot pop element from empty runtime stack");

    return stack->Data[--(stack->Count)];
  }

  [[nodiscard]] auto Stack::Peek(Stack *stack, uint64_t o) noexcept -> node::Base *
  {
    if (stack->Count < o)
      Panic("ICE: peek exceeds the number of existing elements in the "
            "runtime stack");

    return stack->Data[(stack->Count) - o - 1];
  }

  void Stack::PopN(Stack *stack, uint64_t n) noexcept
  {
    if (stack->Count <= n)
      Panic("ICE: pop exceeds the number of existing elements in the "
            "runtime stack");

    stack->Count -= n;
  }

  void Stack::Slide(Stack *stack, uint64_t n) noexcept
  {
    if (stack->Count < n)
      Panic("ICE: slide exceeds the number of existing elements in the "
            "runtime stack");

    stack->Data[stack->Count - n - 1] = stack->Data[stack->Count - 1];
    stack->Count -= n;
  }

  void Stack::Update(Stack *stack, uint64_t o) noexcept
  {
    if (stack->Count < o + 1)
      Panic("ICE: update exceeds the number of existing elements in the "
            "runtime stack");

    auto *ind = reinterpret_cast<node::Ind *>(stack->Data[stack->Count - o - 2]);
    ind->Node.Tag = node::Tag::IND;
    ind->Next = stack->Data[stack->Count -= 1];
  }

  void Stack::Allocate(Stack *stack, uint64_t o) noexcept
  {
    while (o--)
      Stack::Push(stack, reinterpret_cast<node::Base *>(node::Ind::Allocate(nullptr)));
  }

  void Stack::Pack(Stack *stack, uint64_t n, node::Tag tag) noexcept
  {
    if (stack->Count <= n)
      Panic("ICE: update exceeds the number of existing elements in the "
            "runtime stack");

    auto **data = reinterpret_cast<node::Base **>(std::malloc(sizeof(*stack->Data) * n));

    if (data == nullptr)
      Panic("ICE: Cannot allocate data when pack stack");

    std::memcpy(data, &stack->Data[stack->Count - n - 1], n * sizeof(*data));

    auto *node = reinterpret_cast<node::Data *>(node::Base::Allocate());
    node->Array = data;
    node->Node.Tag = node::Tag::DATA;
    node->Tag = tag;

    stack->PopN(stack, n);
    stack->Push(stack, reinterpret_cast<node::Base *>(node));
  }

  void Stack::Split(Stack *stack, uint64_t n) noexcept
  {
    auto *node = reinterpret_cast<node::Data *>(Stack::Pop(stack));

    for (size_t i = 0; i < n; i++)
      Stack::Push(stack, node->Array[i]);
  }
} // namespace swallow::compiler::runtime::stack