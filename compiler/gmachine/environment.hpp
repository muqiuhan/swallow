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

#ifndef SWALLOW_COMPILER_G_MACHINE_ENVIRONMENT_HPP
#define SWALLOW_COMPILER_G_MACHINE_ENVIRONMENT_HPP

#include <memory>
#include "error/errors.hpp"
#include <tl/expected.hpp>
#include <tl/optional.hpp>
#include <string>

namespace swallow::compiler::gmachine
{

  class Environment
  {
  public:
    using Ptr = std::shared_ptr<Environment>;

    virtual ~Environment() = default;

    [[nodiscard]] virtual auto GetOffset(const std::string &name) const noexcept -> tl::optional<int> = 0;

    [[nodiscard]] virtual auto HasVariable(const std::string &name) const noexcept -> bool = 0;
  };

  class Variable : public Environment
  {
  public:
    std::string Name;
    Ptr         Parent;

    Variable(std::string Name, Ptr Parent) : Name(std::move(Name)), Parent(std::move(Parent)) {}

    [[nodiscard]] auto GetOffset(const std::string &name) const noexcept -> tl::optional<int> override;

    [[nodiscard]] auto HasVariable(const std::string &name) const noexcept -> bool override;
  };

  class Offset : public Environment
  {
  public:
    uint32_t Value;
    Ptr      Parent;

    Offset(uint32_t Value, Ptr Parent) : Value(Value), Parent(std::move(Parent)) {}

    [[nodiscard]] auto GetOffset(const std::string &name) const noexcept -> tl::optional<int> override;

    [[nodiscard]] auto HasVariable(const std::string &name) const noexcept -> bool override;
  };

} // namespace swallow::compiler::gmachine

#endif /* SWALLOW_COMPILER_G_MACHINE_ENVIRONMENT_HPP */
