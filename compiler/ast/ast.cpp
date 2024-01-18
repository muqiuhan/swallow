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

#include "ast.h"
#include "environment.h"
#include "panic/panic.hpp"
#include "type.h"
#include <algorithm>
#include <cstdlib>
#include <reporter.h>
#include <utils.h>
#include <vector>

using namespace swallow::utils;

namespace swallow::compiler::ast
{
  Result<std::string, Void>
    Binop::operatorsToString(const Operators &op) noexcept
  {
    switch (op)
      {
      case Operators::PLUS:
        return Ok(std::string("+"));
      case Operators::MINUS:
        return Ok(std::string("-"));
      case Operators::TIMES:
        return Ok(std::string("*"));
      case Operators::DIVIDE:
        return Ok(std::string("/"));
      }

    return Err(Void());
  }

  void dump(const std::vector<Definition::Ptr> &Program) noexcept
  {
    for (const auto &definition : Program)
      {
        Fn *fn = dynamic_cast<Fn *>(definition.get());

        if (!fn)
          continue;
        for (const auto &param : fn->Params)
          std::cout << " " << param;

        std::cout << ":" << std::endl;
        fn->Body->dump(1, std::cout);
      }
  }
} // namespace swallow::compiler::ast

namespace swallow::compiler::type
{
  void typecheck(const std::vector<ast::Definition::Ptr> &program) noexcept
  {
    Manager typeManager;
    Environment typeEnvironment;

    auto intType = Type::Ptr(new Base("Int"));
    auto binopType = Type::Ptr(
      new Arrow(intType, Type::Ptr(new type::Arrow(intType, intType))));

    typeEnvironment.bind("+", binopType);
    typeEnvironment.bind("-", binopType);
    typeEnvironment.bind("*", binopType);
    typeEnvironment.bind("/", binopType);

    for (const auto &definition : program)
      definition->scanDefinitionType(typeManager, typeEnvironment);

    for (const auto &definition : program)
      definition->typecheck(typeManager, typeEnvironment);
  }
} // namespace swallow::compiler::type