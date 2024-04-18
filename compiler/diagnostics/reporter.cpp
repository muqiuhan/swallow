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

#include "reporter.hpp"
#include <cstdlib>

using namespace swallow::compiler::utils;

namespace swallow::compiler::diagnostics
{

  [[noreturn]] auto Reporter::normal(
    const yy::parser::location_type &loc,
    const std::string              &&msg,
    const std::string              &&labelMsg,
    const std::string              &&note,
    const std::uint32_t             &code) -> Err<Void>
  {

    ReportBuilder()
      .with_type(ReportType::ERROR)
      .with_message(msg)
      .with_code(code)
      .add_label(LabelBuilder()
                   .with_message(labelMsg)
                   .with_span(
                     {&Detail,
                      static_cast<size_t>(loc.begin.column - 1),
                      static_cast<size_t>(loc.end.column - 1)})
                   .with_color(ColorType::RED)
                   .build())
      .with_note(note)
      .build()
      .print(std::cout);

    exit(EXIT_FAILURE);
  }

} // namespace swallow::compiler::diagnostics