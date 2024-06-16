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

#include "instruction.hpp"
#include "ast/ast.hpp"
#include "utils/dump.hpp"
#include <format>

using namespace swallow::compiler::utils;

namespace swallow::compiler::gmachine::instruction
{
  void Pop::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << std::format("Pop\t\tCount = {}\n", Count);
  }

  void MakeApplication::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << "MakeApplication\n";
  }

  void Pack::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << std::format("Pack\t\tTag = {}, Size = {}\n", Tag, Size);
  }

  void Split::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << "Split\n";
  }

  void Slide::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << std::format("Slide\t\tOffset = {}\n", Offset);
  }

  void Binop::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << std::format("Binop\t\tOperator = {}\n", ast::Binop::OperatorToString(Operator));
  }

  void Allocation::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << std::format("Allocation\t\tAmount = {}\n", Amount);
  }

  void Unwind::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << "Unwind";
  }

  void Jump::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << "Jump\n\n";

    for (const auto& branch : Branches)
      std::for_each(branch.begin(), branch.end(), [&](const auto& instruction) { instruction->Dump(2, to); });
    to << "\n";
  }

  void PushInt::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << std::format("PushInt\t\tValue = {}\n", Value);
  }

  void PushGlobal::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << std::format("PushGlobal\tName = {}\n", Name);
  }

  void Push::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << std::format("Push\t\tOffset = {}\n", Offset);
  }

  void Eval::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << "Eval\n";
  }

  void Update::Dump(uint8_t indent, std::ostream& to) const noexcept
  {
    dump::DumpIndent(indent, to);
    to << std::format("Update\t\tOffset = {}\n", Offset);
  }

} // namespace swallow::compiler::gmachine::instruction
