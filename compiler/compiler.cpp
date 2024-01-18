#include "compiler.h"
#include "ast.h"
#include "parser.h"

#include "reporter.h"

using namespace swallow::compiler;

auto main(int argc, char ** argv) -> int
{
  CompileUnit::FILE = new CompileUnit(argv[1]);
  diagnostics::Reporter::REPORTER = new diagnostics::Reporter();

  auto & program = parser::parse();
  type::typecheck(program);

  delete CompileUnit::FILE;
  delete diagnostics::Reporter::REPORTER;
  return 0;
}
