#include "ast.hpp"
#include "diagnostics/reporter.hpp"
#include "type/environment.hpp"
#include "type/type.hpp"
#include <vector>

using namespace swallow::compiler::utils;

namespace swallow::compiler::ast
{
  auto Binop::OperatorToString(utils::Binop op) -> std::string
  {
    switch (op)
      {
      case utils::Binop::PLUS:
        return {"+"};
      case utils::Binop::MINUS:
        return {"-"};
      case utils::Binop::TIMES:
        return {"*"};
      case utils::Binop::DIVIDE:
        return {"/"};
      }

    throw std::runtime_error("OperatorToString failed!!!");
  }

  void Dump(const std::vector<Definition::Ptr> &Program)
  {
    for (const auto &definition : Program)
      {
        Fn *fn = dynamic_cast<Fn *>(definition.get());

        if (fn == nullptr)
          continue;
        for (const auto &param : fn->Params)
          std::cout << " " << param;

        std::cout << ":" << '\n';
        fn->Body->Dump(1, std::cout);
      }
  }

} // namespace swallow::compiler::ast

namespace swallow::compiler::type
{
  void TypeCheck(
    const std::vector<ast::Definition::Ptr> &program,
    const CompilerOptions                   &options)
  {
    Manager     typeManager;
    Environment typeEnvironment;

    auto intType = Type::Ptr(new Base("Int"));
    auto binopType = Type::Ptr(
      new Arrow(intType, Type::Ptr(new type::Arrow(intType, intType))));

    typeEnvironment.Bind("+", binopType);
    typeEnvironment.Bind("-", binopType);
    typeEnvironment.Bind("*", binopType);
    typeEnvironment.Bind("/", binopType);

    for (const auto &definition : program)
      definition->PreScanTypes(typeManager, typeEnvironment);

    for (const auto &definition : program)
      definition->TypeCheck(typeManager, typeEnvironment);

    for (const auto &definition : program)
      definition->Resolve(typeManager);

    if (options.dump_types.has_value())
      typeEnvironment.Dump(std::cout, typeManager);
  }
} // namespace swallow::compiler::type

namespace swallow::compiler::gmachine
{
  void Compile(
    const std::vector<ast::Definition::Ptr> &program,
    const CompilerOptions                   &options)
  {
    for (const auto &definition : program)
      {
        definition->Compile();
        const auto *fn = dynamic_cast<Fn *>(definition.get());

        if (nullptr == fn)
          continue;

        if (options.dump_gmachine_ir.has_value())
          for (const auto &instruction : fn->Instructions)
            instruction->Dump(0, std::cout);
      }
  }
} // namespace swallow::compiler::gmachine
