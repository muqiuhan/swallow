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
#include "panic.hpp"
#include <algorithm>
#include <cstdint>

using namespace swallow::utils;

namespace swallow::type {
  std::string TypeManager::newTypeName() noexcept {
    int32_t currentID = LastID++;
    std::string name;

    while (LastID != -1) {
      name += static_cast<char>(('a' + (currentID % 26)));
      currentID = currentID / 26 - 1;
    }

    std::reverse(name.begin(), name.end());
    return name;
  }

  Type::Ptr TypeManager::newType() noexcept {
    return std::make_shared<TypeVar>(TypeVar(newTypeName()));
  }

  Type::Ptr TypeManager::newArrowType() noexcept {
    return std::make_shared<TypeArrow>(TypeArrow(newType(), newType()));
  }

  Type::Ptr TypeManager::resolve(Type::Ptr type, TypeVar *&var) noexcept {
    TypeVar *cast;
    var = nullptr;

    while ((cast = dynamic_cast<TypeVar *>(type.get()))) {
      const auto it = Types.find(cast->Name);
      if (it == Types.end()) {
        var = cast;
        break;
      }

      type = it->second;
    }

    return type;
  }

  void TypeManager::unify(Type::Ptr left, Type::Ptr right) noexcept {
    TypeVar *leftVar = nullptr;
    TypeVar *rightVar = nullptr;

    left = resolve(left, leftVar);
    right = resolve(right, rightVar);

    if (leftVar)
      bind(leftVar->Name, right);
    else if (rightVar)
      bind(rightVar->Name, left);

    else if (auto *leftArrow = dynamic_cast<TypeArrow *>(left.get()),
             *rightArrow = dynamic_cast<TypeArrow *>(right.get());
             leftArrow && rightArrow) {

      unify(leftArrow->Left, rightArrow->Left);
      unify(leftArrow->Right, rightArrow->Right);

    } else if (auto *leftID = dynamic_cast<TypeBase *>(left.get()),
               *rightID = dynamic_cast<TypeBase *>(right.get());
               leftID && rightID) {

      if (leftID->Name == rightID->Name)
        ;

    } else {
      panic("type checking error!!!");
    }
  }

  void TypeManager::bind(const std::string &name,
                         const Type::Ptr &type) noexcept {
    if (auto *other = dynamic_cast<const TypeVar *>(type.get());
        other->Name == name)
      return;
    Types[name] = type;
  }

} // namespace swallow::type