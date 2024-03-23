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

#include "type.hpp"
#include <algorithm>

using namespace swallow::compiler::utils;

namespace swallow::compiler::type
{
  auto Manager::newTypeName() noexcept -> std::string
  {
    int32_t currentID = LastID++;
    std::string name;

    while (currentID != -1)
      {
        name += static_cast<char>(('a' + (currentID % 26)));
        currentID = currentID / 26 - 1;
      }

    std::reverse(name.begin(), name.end());
    return name;
  }

  auto Manager::newType() noexcept -> Type::Ptr
  {
    return Type::Ptr(new Variable(newTypeName()));
  }

  auto Manager::newArrowType() noexcept -> Type::Ptr
  {
    return Type::Ptr(new Arrow(newType(), newType()));
  }

  auto Manager::resolve(Type::Ptr type, Variable *&var) noexcept -> Type::Ptr
  {
    Variable *cast = nullptr;
    var = nullptr;

    while ((cast = dynamic_cast<Variable *>(type.get())) != nullptr)
      {
        const auto it = Types.find(cast->Name);
        if (it == Types.end())
          {
            var = cast;
            break;
          }

        type = it->second;
      }
    return type;
  }

  auto Manager::unify(Type::Ptr left,
                      Type::Ptr right) noexcept -> Result<Void, Void>
  {
    Variable *leftVar = nullptr;
    Variable *rightVar = nullptr;

    left = resolve(left, leftVar);
    right = resolve(right, rightVar);

    if (leftVar != nullptr)
      {
        bind(leftVar->Name, right);
        return Ok(Void());
      }

    if (rightVar != nullptr)
      {
        bind(rightVar->Name, left);
        return Ok(Void());
      }

    if (auto *leftArrow = dynamic_cast<Arrow *>(left.get()),
        *rightArrow = dynamic_cast<Arrow *>(right.get());
        leftArrow != nullptr && rightArrow != nullptr)
      {
        return unify(leftArrow->Left, rightArrow->Left)
          .and_then([&](const auto &ok) {
            return unify(leftArrow->Right, rightArrow->Right);
          });
      }

    if (auto *leftID = dynamic_cast<Base *>(left.get()),
        *rightID = dynamic_cast<Base *>(right.get());
        leftID != nullptr && rightID != nullptr)
      {
        if (leftID->Name == rightID->Name)
          return Ok(Void());
      }

    return Err(Void());
  }

  void Manager::bind(const std::string &name, const Type::Ptr &type) noexcept
  {
    if (const auto *other = dynamic_cast<const Variable *>(type.get());
        (other != nullptr) && other->Name == name)
      return;

    Types[name] = type;
  }

} // namespace swallow::compiler::type