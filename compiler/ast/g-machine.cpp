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
#include "g-machine/binop.hpp"
#include "g-machine/instruction.hpp"
#include "diagnostics/reporter.hpp"
#include "code.hpp"
#include "utils/panic/panic.hpp"
#include <format>

using swallow::compiler::gmachine::instruction::Instruction;
using namespace swallow::compiler::gmachine;
using namespace swallow::compiler::utils;

namespace swallow::compiler::ast
{
  void Int::Compile(const gmachine::Environment::Ptr &machineEnvironment,
                    std::vector<Instruction::Ptr> &into) const noexcept
  {
    into.push_back(Instruction::Ptr(new instruction::PushInt(Value)));
  }

  void LID::Compile(const gmachine::Environment::Ptr &machineEnvironment,
                    std::vector<Instruction::Ptr> &into) const noexcept
  {
    into.push_back(Instruction::Ptr(
      machineEnvironment->HasVariable(ID)
        ? dynamic_cast<Instruction *>(
            new instruction::Push(machineEnvironment->GetOffset(ID).value()))

        : dynamic_cast<Instruction *>(new instruction::PushGlobal(ID))));
  }

  void UID::Compile(const gmachine::Environment::Ptr &machineEnvironment,
                    std::vector<Instruction::Ptr> &into) const noexcept
  {
    into.push_back(Instruction::Ptr(new instruction::PushGlobal(ID)));
  }

  void Application::Compile(const gmachine::Environment::Ptr &machineEnvironment,
                            std::vector<Instruction::Ptr> &into) const noexcept
  {
    Right->Compile(machineEnvironment, into);
    Left->Compile(Environment::Ptr(new Offset(1, machineEnvironment)), into);
    into.push_back(Instruction::Ptr(new instruction::MakeApplication()));
  }

  void Binop::Compile(const gmachine::Environment::Ptr &machineEnvironment,
                      std::vector<Instruction::Ptr> &into) const noexcept
  {
    Right->Compile(machineEnvironment, into);
    Left->Compile(machineEnvironment, into);

    into.push_back(Instruction::Ptr(
      new instruction::PushGlobal(gmachine::Binop::Action(Operator))));
    into.push_back(Instruction::Ptr(new instruction::MakeApplication()));
    into.push_back(Instruction::Ptr(new instruction::MakeApplication()));
  }

  void Match::Compile(const gmachine::Environment::Ptr &machineEnvironment,
                      std::vector<Instruction::Ptr> &into) const noexcept
  {
    auto *type = dynamic_cast<type::Data *>(With->NodeType.get());
    auto *jump = new instruction::Jump();

    With->Compile(machineEnvironment, into);
    into.push_back(Instruction::Ptr(new instruction::Eval()));
    into.push_back(Instruction::Ptr(jump));

    for (const auto &branch : Branches)
      {
        std::vector<Instruction::Ptr> branchInstructions;
        auto *variablePattern =
          dynamic_cast<VariablePattern *>(branch->Patt.get());
        auto *constructorPattern =
          dynamic_cast<ConstructorPattern *>(branch->Patt.get());

        if (nullptr != variablePattern)
          {
            branch->Expr->Compile(
              Environment::Ptr(new Offset(1, machineEnvironment)),
              branchInstructions);

            for (const auto &constructorPair : type->Constructors)
              {
                if (jump->TagMappings.find(constructorPair.second.Tag)
                    != jump->TagMappings.end())
                  break;

                jump->TagMappings[constructorPair.second.Tag] =
                  jump->Branches.size();
              }
            jump->Branches.push_back(std::move(branchInstructions));
          }
        else if (nullptr != constructorPattern)
          {
            auto newEnvironment = machineEnvironment;

            std::for_each(
              constructorPattern->Params.rbegin(),
              constructorPattern->Params.rend(), [&](const auto &param) {
                newEnvironment =
                  Environment::Ptr(new Variable(param, newEnvironment));
              });

            branchInstructions.push_back(
              Instruction::Ptr(new instruction::Split()));
            branch->Expr->Compile(newEnvironment, branchInstructions);
            branchInstructions.push_back(Instruction::Ptr(
              new instruction::Slide(constructorPattern->Params.size())));

            uint8_t newTag =
              type->Constructors[constructorPattern->ConstructorName].Tag;
            if (jump->TagMappings.find(newTag) != jump->TagMappings.end())
              {
                diagnostics::Reporter::REPORTER->normal(
                  Location,
                  std::format("Duplicate pattern {}",
                              constructorPattern->ConstructorName),
                  "This constructor already exists in context",
                  "Consider remove this pattern",
                  diagnostics::PATTERN_CONSTRUCTOR_IS_DUPLICATED);
              }

            jump->TagMappings[newTag] = jump->Branches.size();
            jump->Branches.push_back(std::move(branchInstructions));
          }

        for (const auto &constructorPair : type->Constructors)
          {
            if (jump->TagMappings.find(constructorPair.second.Tag)
                == jump->TagMappings.end())
              diagnostics::Reporter::REPORTER->normal(
                Location, "This pattern-matching is not exhaustive",
                "There may be other patterns", "Please try to match all cases",
                diagnostics::MATCH_EXPR_IS_NON_EXHAUSTIVE);
          }
      }
  }
} // namespace swallow::compiler::ast
