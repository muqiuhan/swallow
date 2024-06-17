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

#include "parser.h"
#include "bison_parser.hpp"
#include "compiler.h"
#include "diagnostics/code.hpp"
#include "diagnostics/reporter.hpp"
#include "location.hh"
#include "utils/panic.hpp"
#include <cstdio>

namespace yy
{

  void parser::error(const location_type &loc, const std::string &msg)
  {
    swallow::compiler::diagnostics::Reporter::REPORTER->normal(
      location{loc.begin, loc.begin},
      "Syntax error",
      "There is a syntax problem here",
      "No more information",
      swallow::compiler::diagnostics::Code::PARSING);
  }

} // namespace yy

extern std::vector<swallow::compiler::ast::Definition::Ptr> Program;

extern FILE *yyin, *yyout;

namespace swallow::compiler::parser
{

  auto Parse() noexcept -> std::vector<ast::Definition::Ptr> &
  {
    FILE *file = std::fopen(compiler::CompileUnit::FILE->FilePath.c_str(), "r");

    if (file == nullptr)
      utils::Panic("Cannot open file {}", compiler::CompileUnit::FILE->FilePath);

    yyin = file;
    yy::parser().parse();

    fclose(file);
    return Program;
  }

} // namespace swallow::compiler::parser