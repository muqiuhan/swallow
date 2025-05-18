#ifndef SWALLOW_COMPILER_DIAGNOSTICS_REPORTER_H
#define SWALLOW_COMPILER_DIAGNOSTICS_REPORTER_H

#include "compiler.h"
#include "diagnostics/diagnostics.h"
#include "parser/bison_parser.hpp"

namespace swallow::compiler::diagnostics
{
  class Reporter
  {
    Details Detail;

  public:
    inline static Reporter *REPORTER = nullptr;

    explicit Reporter()
      : Detail({CompileUnit::FILE->FileValue, CompileUnit::FILE->FilePath})
    {}

    [[noreturn]] auto normal(
      const yy::parser::location_type &loc,
      const std::string              &&msg,
      const std::string              &&labelMsg,
      const std::string              &&note,
      const std::uint32_t             &code) -> std::nullptr_t;
  };

} // namespace swallow::compiler::diagnostics

#endif // SWALLOW_COMPILER_DIAGNOSTICS_REPORTER_H