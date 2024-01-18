#include "compiler.h"
#include "ast.h"
#include "parser.h"

#include "diagnostics.h"

using namespace swallow::compiler;

auto main(int argc, char ** argv) -> int
{
  CompileUnit::FILE = new CompileUnit(argv[1]);

  auto & program = parser::parse();
  type::typecheck(program);

  delete CompileUnit::FILE;
  return 0;
}
