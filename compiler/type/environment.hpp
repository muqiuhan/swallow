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

#ifndef SWALLOW_COMPILER_TYPE_ENVIRONMENT_H
#define SWALLOW_COMPILER_TYPE_ENVIRONMENT_H

#include "type.hpp"
#include <map>
#include <string>

namespace swallow::compiler::type
{
  class Environment
  {
  public:
    std::map<std::string, Type::Ptr> Names;
    Environment const               *Parent = nullptr;

    explicit Environment(Environment const *Parent) : Parent(Parent) {}

    Environment() : Environment(nullptr) {}

    [[nodiscard]] auto Lookup(const std::string &name) const noexcept -> tl::expected<Type::Ptr, std::nullptr_t>;

    [[nodiscard]] auto Scope() const noexcept -> Environment;

    void Bind(const std::string &name, Type::Ptr type) noexcept;
    void Dump(std::ostream &to, const Manager &typeManager) noexcept;
  };

} // namespace swallow::compiler::type

#endif // SWALLOW_COMPILER_TYPE_ENVIRONMENT_H