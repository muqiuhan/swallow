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

#ifndef SWALLOW_UTILS_PANIC_H
#define SWALLOW_UTILS_PANIC_H

#include <exception>
#include <format>
#include <iostream>
#include <source_location>

namespace swallow::compiler::utils
{
  template <class... Args> struct PanicFormat
  {
    template <class T>
    consteval PanicFormat(

      const T &s,
      std::source_location loc = std::source_location::current()) noexcept
      : fmt{s}, loc{loc}
    {}

    std::format_string<Args...> fmt;
    std::source_location loc;
  };

  template <class... Args>
  [[noreturn]] void Panic(PanicFormat<std::type_identity_t<Args>...> fmt,
                          Args &&...args) noexcept
  {
    auto msg =
      std::format("{}:{} Panic: {}\n", fmt.loc.file_name(), fmt.loc.line(),
                  std::format(fmt.fmt, std::forward<Args>(args)...));
    std::cout << msg.c_str() << std::endl;
    std::terminate();
  }
} // namespace swallow::compiler::utils

#endif