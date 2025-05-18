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

    auto append(std::string append_message) const -> void
    {
      spdlog::error(
        "{} at ({}:{})", append_message, location.file_name(), location.line());
    }

    [[noreturn]] auto panic() const -> void
    {
      spdlog::error(
        "{} at ({}:{})", message, location.file_name(), location.line());
      std::terminate();
    }
  };

#define _Error(message)                                                        \
  swallow::compiler::error::Error { message, std::source_location::current() }

#define Unimplemented()                                                        \
  _Error(fmt::format(                                                          \
           "unimplemented function: {}",                                       \
           std::source_location::current().function_name()))                   \
    .panic()

#define Panic(message)                                                         \
  _Error(fmt::format("Internal Compiler Error: {}", message))

#define Err(message) tl::make_unexpected(_Error(message))
}; // namespace swallow::compiler::error

#endif /* SWALLOW_COMPILER_ERRORS_HPP */
