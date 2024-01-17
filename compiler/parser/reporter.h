#ifndef SWALLOW_COMPILER_PARSER_REPORTER_H
#define SWALLOW_COMPILER_PARSER_REPORTER_H

#include "bison_parser.hpp"
#include "compiler.h"
#include "diagnostics/diagnostics.h"

namespace swallow::compiler::parser
{
  class ParserReporter
  {
    diagnostics::Details Details;

  public:
    inline static ParserReporter * REPORTER = nullptr;

    explicit ParserReporter()
      : Details({ swallow::compiler::CompileUnit::FILE->FileValue,
                  swallow::compiler::CompileUnit::FILE->FilePath })
    {}

    void normal(const yy::parser::location_type & loc,
                const std::string && msg,
                const std::string && labelMsg,
                const std::string && note);
  };

} // namespace swallow::compiler::parser

#endif /* SWALLOW_COMPILER_PARSER_REPORTER_H */