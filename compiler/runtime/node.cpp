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

#include "node.h"
#include "utils/panic/panic.hpp"
#include <cstdlib>

namespace swallow::compiler::runtime::node
{
  [[nodiscard]] auto Base::Allocate() noexcept -> Base *
  {
    auto *node = reinterpret_cast<Base *>(std::malloc(sizeof(class Application)));

    if (nullptr == node)
      utils::Panic("ICE: Cannot allocate Base node");
    else
      return node;
  }

  [[nodiscard]] auto
    Application::Allocate(Base *Left, Base *Right) noexcept -> Application *
  {
    auto *node = reinterpret_cast<Application *>(Base::Allocate());
    node->Node.Tag = Tag::APPLICATION;
    node->Left = Left;
    node->Right = Right;
    return node;
  }

  [[nodiscard]] auto Int::Allocate(int32_t Value) noexcept -> class Int *
  {
    auto *node = reinterpret_cast<Int *>(Base::Allocate());
    node->Node.Tag = Tag::INT;
    node->Value = Value;
    return node;
  }

  [[nodiscard]] auto Global::Allocate(
    void (*Function)(runtime::stack::Stack *), int32_t Arity) noexcept -> Global *
  {
    auto *node = reinterpret_cast<Global *>(Base::Allocate());
    node->Node.Tag = Tag::GLOBAL;
    node->Arity = Arity;
    node->Function = Function;
    return node;
  }

  [[nodiscard]] auto Ind::Allocate(Base *Next) noexcept -> Ind *
  {
    auto *node = reinterpret_cast<Ind *>(Base::Allocate());
    node->Node.Tag = Tag::IND;
    node->Next = Next;
    return node;
  }

} // namespace swallow::compiler::runtime::node
