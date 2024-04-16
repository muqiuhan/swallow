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

using namespace swallow::compiler::gmachine::instruction;
using namespace swallow::compiler::gmachine;

namespace swallow::compiler::ast
{
  void Int::compile(const Environment::Ptr &machineEnvironment,
                    std::vector<Instruction::Ptr> &into) const noexcept
  {
    into.push_back(Instruction::Ptr(new PushInt(Value)));
  }

  void LID::compile(const Environment::Ptr &machineEnvironment,
                    std::vector<Instruction::Ptr> &into) const noexcept
  {
    into.push_back(
      Instruction::Ptr(machineEnvironment->hasVariable(ID)
                         ? dynamic_cast<Instruction *>(new Push(
                             machineEnvironment->getOffset(ID).value()))

                         : dynamic_cast<Instruction *>(new PushGlobal(ID))));
  }

  void UID::compile(const Environment::Ptr &machineEnvironment,
                    std::vector<Instruction::Ptr> &into) const noexcept
  {
    into.push_back(Instruction::Ptr(new PushGlobal(ID)));
  }

  void
    Application::compile(const gmachine::Environment::Ptr &machineEnvironment,
                         std::vector<Instruction::Ptr> &into) const noexcept
  {
    Right->compile(machineEnvironment, into);
    Left->compile(Environment::Ptr(new Offset(1, machineEnvironment)), into);
    into.push_back(Instruction::Ptr(new MakeApplication()));
  }

  void Binop::compile(const gmachine::Environment::Ptr &machineEnvironment,
                      std::vector<Instruction::Ptr> &into) const noexcept
  {
    Right->compile(machineEnvironment, into);
    Left->compile(machineEnvironment, into);

    into.push_back(Instruction::Ptr(
      new PushGlobal(gmachine::Binop::operatorsAction(Operator))));
    into.push_back(Instruction::Ptr(new MakeApplication()));
    into.push_back(Instruction::Ptr(new MakeApplication()));
  }

  void Match::compile(const gmachine::Environment::Ptr &machineEnvironment,
                      std::vector<Instruction::Ptr> &into) const noexcept
  {}
} // namespace swallow::compiler::ast