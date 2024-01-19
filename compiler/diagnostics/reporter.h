#ifndef HOME_MUQIUHAN_WORKSPACE_SWALLOW_COMPILER_DIAGNOSTICS_REPORTER_H
#define HOME_MUQIUHAN_WORKSPACE_SWALLOW_COMPILER_DIAGNOSTICS_REPORTER_H

#include "bison_parser.hpp"
#include "compiler.h"
#include "diagnostics/diagnostics.h"
#include "result/result.hpp"

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

    [[noreturn]] auto
      normal(const yy::parser::location_type &loc, const std::string &&msg,
             const std::string &&labelMsg, const std::string &&note,
             const std::uint32_t &code) -> utils::Err<utils::Void>;
  };

} // namespace swallow::compiler::diagnostics

#endif // HOME_MUQIUHAN_WORKSPACE_SWALLOW_COMPILER_DIAGNOSTICS_REPORTER_H