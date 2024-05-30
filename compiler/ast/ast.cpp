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

#include "ast.hpp"
#include "diagnostics/reporter.hpp"
#include "panic/panic.hpp"
#include "type/environment.hpp"
#include "type/type.hpp"
#include <vector>

using namespace swallow::compiler::utils;

namespace swallow::compiler::ast
{
  auto Binop::OperatorToString(utils::Binop op) noexcept -> std::string
  {
    switch (op)
      {
      case utils::Binop::PLUS:
        return {"+"};
      case utils::Binop::MINUS:
        return {"-"};
      case utils::Binop::TIMES:
        return {"*"};
      case utils::Binop::DIVIDE:
        return {"/"};
      }

    utils::Panic("OperatorToString failed!!!");
  }

  void Dump(const std::vector<Definition::Ptr> &Program) noexcept
  {
    for (const auto &definition : Program)
      {
        Fn *fn = dynamic_cast<Fn *>(definition.get());

        if (fn == nullptr)
          continue;
        for (const auto &param : fn->Params)
          std::cout << " " << param;

        std::cout << ":" << '\n';
        fn->Body->Dump(1, std::cout);
      }
  }

} // namespace swallow::compiler::ast

namespace swallow::compiler::type
{
  void TypeCheck(
    const std::vector<ast::Definition::Ptr> &program,
    const CompilerOptions                   &options) noexcept
  {
    Manager     typeManager;
    Environment typeEnvironment;

    auto intType = Type::Ptr(new Base("Int"));
    auto binopType =
      Type::Ptr(new Arrow(intType, Type::Ptr(new type::Arrow(intType, intType))));

    typeEnvironment.Bind("+", binopType);
    typeEnvironment.Bind("-", binopType);
    typeEnvironment.Bind("*", binopType);
    typeEnvironment.Bind("/", binopType);

    for (const auto &definition : program)
      definition->PreScanTypes(typeManager, typeEnvironment);

    for (const auto &definition : program)
      definition->TypeCheck(typeManager, typeEnvironment);

    for (const auto &definition : program)
      definition->Resolve(typeManager);

    if (options.dump_types.has_value())
      typeEnvironment.Dump(std::cout, typeManager);
  }
} // namespace swallow::compiler::type

namespace swallow::compiler::gmachine
{
  void Compile(
    const std::vector<ast::Definition::Ptr> &program,
    const CompilerOptions                   &options) noexcept
  {
    for (const auto &definition : program)
      {
        definition->Compile();
        const auto *fn = dynamic_cast<Fn *>(definition.get());

        if (nullptr == fn)
          continue;

        if (options.dump_gmachine_ir.has_value())
          for (const auto &instruction : fn->Instructions)
            instruction->Dump(0, std::cout);
      }
  }
} // namespace swallow::compiler::gmachine
