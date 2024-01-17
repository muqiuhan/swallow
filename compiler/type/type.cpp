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

#include "type.h"
#include "../utils/panic/panic.hpp"
#include <algorithm>
#include <cstdint>
#include <format>
#include <iterator>
#include <ostream>

using namespace swallow::utils;

namespace swallow::type
{
  std::string Manager::newTypeName() noexcept
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

  Type::Ptr Manager::newType() noexcept
  {
    return Type::Ptr(new Variable(newTypeName()));
  }

  Type::Ptr Manager::newArrowType() noexcept
  {
    return Type::Ptr(new Arrow(newType(), newType()));
  }

  Type::Ptr Manager::resolve(Type::Ptr type, Variable *& var) noexcept
  {
    Variable * cast;
    var = nullptr;

    while ((cast = dynamic_cast<Variable *>(type.get())))
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

  void Manager::unify(Type::Ptr left, Type::Ptr right) noexcept
  {
    Variable * leftVar;
    Variable * rightVar;

    left = resolve(left, leftVar);
    right = resolve(right, rightVar);

    if (leftVar)
      bind(leftVar->Name, right);
    else if (rightVar)
      bind(rightVar->Name, left);

    else if (auto * leftArrow = dynamic_cast<Arrow *>(left.get()),
             *rightArrow = dynamic_cast<Arrow *>(right.get());
             leftArrow && rightArrow)
      {
        unify(leftArrow->Left, rightArrow->Left);
        unify(leftArrow->Right, rightArrow->Right);
      }
    else if (auto * leftID = dynamic_cast<Base *>(left.get()),
             *rightID = dynamic_cast<Base *>(right.get());
             leftID && rightID)
      {
        if (leftID->Name == rightID->Name)
          return;
      }
    else
      panic("type checking failed: unification failed");
  }

  void Manager::bind(const std::string & name, const Type::Ptr & type) noexcept
  {
    // BUG: other->Name == nullptr
    if (auto * other = dynamic_cast<const Variable *>(type.get());
        other->Name == name)
      return;
    Types[name] = type;
  }

} // namespace swallow::type