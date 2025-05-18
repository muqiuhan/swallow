#include "compiler.h"
#include "ast/ast.hpp"
#include "diagnostics/reporter.hpp"
#include "parser/parser.h"
#include <chrono>

using namespace swallow::compiler;

namespace swallow::compiler
{
  auto Compiler(const CompilerOptions &options) -> int
  {
#ifdef TEST_RUNTIME
    auto *result =
      runtime::Runtime::Eval(reinterpret_cast<runtime::node::Base *>(
        runtime::node::Global::Allocate(EntryPoint, 0)));

    std::cout << std::format(
      "test runtime...{}\n",
      reinterpret_cast<swallow::compiler::runtime::node::Int *>(result)->Value);
#endif

    CompileUnit::FILE = new CompileUnit(options.file);
    diagnostics::Reporter::REPORTER = new diagnostics::Reporter();

    std::cout << fmt::format("compiling {}...", options.file);

    const auto  start = std::chrono::system_clock::now();
    const auto &program = parser::Parse();

    if (options.dump_ast)
      ast::Dump(program);

    type::TypeCheck(program, options);
    gmachine::Compile(program, options);

    const auto end = std::chrono::system_clock::now();

    std::cout << fmt::format(
      "ok ({} ms)\n",
      double(std::chrono::duration_cast<std::chrono::microseconds>(end - start)
               .count()));

    delete CompileUnit::FILE;
    delete diagnostics::Reporter::REPORTER;
    return 0;
  }
} // namespace swallow::compiler