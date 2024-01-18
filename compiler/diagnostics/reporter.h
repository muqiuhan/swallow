#ifndef SWALLOW_COMPILER_PARSER_REPORTER_H
#define SWALLOW_COMPILER_PARSER_REPORTER_H

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
    inline static Reporter * REPORTER = nullptr;

    explicit Reporter()
      : Detail({ CompileUnit::FILE->FileValue, CompileUnit::FILE->FilePath })
    {}

    [[noreturn]] utils::Err<utils::Void>
    normal(const yy::parser::location_type & loc,
           const std::string && msg,
           const std::string && labelMsg,
           const std::string && note,
           const std::uint32_t && code);
  };

} // namespace swallow::compiler::diagnostics

#endif /* SWALLOW_COMPILER_PARSER_REPORTER_H */