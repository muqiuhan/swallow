#include <string>
#include <structopt/app.hpp>
#include "compiler.h"

STRUCTOPT(
  swallow::compiler::CompilerOptions,
  file,
  verbose,
  dump_ast,
  dump_types,
  dump_gmachine_ir);

int main(int argc, char *argv[])
{
  try
    {
      const std::string command = argv[1] == nullptr ? "" : argv[1];
      if (command == "c" || command == "compile")
        {
          return swallow::compiler::Compiler(
            structopt::app(
              "swa compile",
              swallow::compiler::CompilerOptions::VERSION,
              swallow::compiler::CompilerOptions::HELP)
              .parse<swallow::compiler::CompilerOptions>(argc - 1, &argv[1]));
        }
      else if (command == "i" || command == "repl")
        {
          return 0;
        }
      else
        {
          std::cout << "USAGE: swa [command]\n\ncommand:\n\tc, compile\tcall the "
                       "compiler\n\ti, repl\t\tcall the repl interpreter\n";
        }
    }
  catch (const structopt::exception &e)
    {
      std::cout << e.what() << '\n';
      std::cout << e.help() << '\n';
    }
  return 0;
}