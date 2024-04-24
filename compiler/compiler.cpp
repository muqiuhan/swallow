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

#include "compiler.h"
#include "ast/ast.hpp"
#include "parser.h"
#include "diagnostics/reporter.hpp"
#include <chrono>

using namespace swallow::compiler;

namespace swallow::compiler
{

  auto Compiler(const CompilerOptions &options) noexcept -> int
  {
    CompileUnit::FILE = new CompileUnit(options.file);
    diagnostics::Reporter::REPORTER = new diagnostics::Reporter();

    const auto start = std::chrono::system_clock::now();
    auto      &program = parser::Parse();
    type::TypeCheck(program, options);
    gmachine::Compile(program, options);
    const auto end = std::chrono::system_clock::now();

    std::cout
      << "compiling " << options.file << " ..."
      << double(
           std::chrono::duration_cast<std::chrono::microseconds>(end - start).count())
      << "ms\n";

    delete CompileUnit::FILE;
    delete diagnostics::Reporter::REPORTER;
    return 0;
  }
} // namespace swallow::compiler