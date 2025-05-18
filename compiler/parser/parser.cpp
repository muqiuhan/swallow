#include "parser.h"
#include "bison_parser.hpp"
#include "compiler.h"
#include "diagnostics/code.hpp"
#include "diagnostics/reporter.hpp"
#include "error/errors.hpp"
#include "location.hh"
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

  auto Parse() -> std::vector<ast::Definition::Ptr> &
  {
    FILE *file = std::fopen(compiler::CompileUnit::FILE->FilePath.c_str(), "r");

    if (file == nullptr)
      Panic(fmt::format(
        "Cannot open file {}", compiler::CompileUnit::FILE->FilePath));

    yyin = file;
    yy::parser().parse();

    fclose(file);
    return Program;
  }

} // namespace swallow::compiler::parser