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
#include <cstdint>
#include <format>

namespace swallow::compiler::ast
{
  static void printIndent(uint8_t n, std::ostream &to) noexcept
  {
    while ((n--) != 0U)
      to << "  ";
  }

  void Int::Dump(uint8_t indent, std::ostream &to) const noexcept
  {
    printIndent(indent, to);
    to << std::format("<Int: {}>", Value) << '\n';
  }

  void LID::Dump(uint8_t indent, std::ostream &to) const noexcept
  {
    printIndent(indent, to);
    to << std::format("<LID: {}>", ID) << '\n';
  }

  void UID::Dump(uint8_t indent, std::ostream &to) const noexcept
  {
    printIndent(indent, to);
    to << std::format("<UID: {}>", ID) << '\n';
  }

  void Binop::Dump(uint8_t indent, std::ostream &to) const noexcept
  {
    printIndent(indent, to);

    to << std::format("<Binop: {}", OperatorToString(Operator)) << '\n';

    Left->Dump(indent + 1, to);
    Left->Dump(indent + 1, to);

    to << ">" << '\n';
  }

  void Application::Dump(uint8_t indent, std::ostream &to) const noexcept
  {
    printIndent(indent, to);

    to << "<Application: " << '\n';

    Left->Dump(indent + 1, to);
    Left->Dump(indent + 1, to);
    to << ">" << '\n';
  }

  void Match::Dump(uint8_t indent, std::ostream &to) const noexcept
  {
    printIndent(indent, to);

    to << "<Match: " << '\n';

    for (const auto &branch : Branches)
      {
        printIndent(indent + 1, to);
        branch->Patt->Dump(to);
        to << '\n';
        branch->Expr->Dump(indent + 2, to);
      }

    to << ">" << '\n';
  }

  void VariablePattern::Dump(std::ostream &to) const noexcept
  {
    to << Variable;
  }

  void ConstructorPattern::Dump(std::ostream &to) const noexcept
  {
    to << ConstructorName;

    for (const auto &param : Params)
      to << " " << param;
  }

} // namespace swallow::compiler::ast
