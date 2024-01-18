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
#include <cstdint>
#include <format>

namespace swallow::compiler::ast
{
  static void printIndent(uint8_t n, std::ostream & to) noexcept
  {
    while (n--)
      to << "  ";
  }

  void Int::dump(uint8_t indent, std::ostream & to) const noexcept
  {
    printIndent(indent, to);
    to << std::format("<Int: {}>", Value) << std::endl;
  }

  void LID::dump(uint8_t indent, std::ostream & to) const noexcept
  {
    printIndent(indent, to);
    to << std::format("<LID: {}>", ID) << std::endl;
  }

  void UID::dump(uint8_t indent, std::ostream & to) const noexcept
  {
    printIndent(indent, to);
    to << std::format("<UID: {}>", ID) << std::endl;
  }

  void Binop::dump(uint8_t indent, std::ostream & to) const noexcept
  {
    printIndent(indent, to);

    to << std::format("<Binop: {}", operatorsToString(Operator).unwrap())
       << std::endl;

    Left->dump(indent + 1, to);
    Left->dump(indent + 1, to);

    to << ">" << std::endl;
  }

  void Application::dump(uint8_t indent, std::ostream & to) const noexcept
  {
    printIndent(indent, to);

    to << "<Application: " << std::endl;

    Left->dump(indent + 1, to);
    Left->dump(indent + 1, to);
    to << ">" << std::endl;
  }

  void Match::dump(uint8_t indent, std::ostream & to) const noexcept
  {
    printIndent(indent, to);

    to << "<Match: " << std::endl;

    for (const auto & branch : Branches)
      {
        printIndent(indent + 1, to);
        branch->Patt->dump(to);
        to << std::endl;
        branch->Expr->dump(indent + 2, to);
      }

    to << ">" << std::endl;
  }

  void PatternVariable::dump(std::ostream & to) const noexcept
  {
    to << Variable;
  }

  void PatternConstructor::dump(std::ostream & to) const noexcept
  {
    to << Constructor;

    for (const auto & param : Params)
      to << " " << param;
  }

} // namespace swallow::compiler::ast