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

#ifndef SWALLOW_COMPILER_ERRORS_HPP
#define SWALLOW_COMPILER_ERRORS_HPP

#include <exception>
#include <source_location>
#include <spdlog/spdlog.h>
#include <string>

namespace swallow::compiler::error
{
  class Error
  {
  public:
    const std::string          message;
    const std::source_location location;

    auto append(std::string append_message) const noexcept -> void
    {
      spdlog::error("{} at ({}:{})", append_message, location.file_name(), location.line());
    }

    [[noreturn]] auto panic() const noexcept -> void
    {
      spdlog::error("{} at ({}:{})", message, location.file_name(), location.line());
      std::terminate();
    }
  };

#define _Error(message)                                                                                                \
  swallow::compiler::error::Error { message, std::source_location::current() }

#define Unimplemented()                                                                                                \
  _Error(fmt::format("unimplemented function: {}", std::source_location::current().function_name())).panic()

#define Panic(message) _Error(fmt::format("{}", message)).panic()

#define Err(message)   tl::make_unexpected(_Error(message))
}; // namespace swallow::compiler::error

#endif /* SWALLOW_COMPILER_ERRORS_HPP */
