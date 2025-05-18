#include <format>
#include <sstream>

#include "ast.hpp"
#include "diagnostics/code.hpp"
#include "diagnostics/reporter.hpp"
#include "gmachine/binop.hpp"
#include "gmachine/instruction.hpp"

using swallow::compiler::gmachine::instruction::Instruction;
using namespace swallow::compiler::gmachine;
using namespace swallow::compiler::utils;

namespace swallow::compiler::ast
{
  void Int::Compile(
    const gmachine::Environment::Ptr &machineEnvironment,
    std::vector<Instruction::Ptr>    &into) const
  {
    into.push_back(Instruction::Ptr(new instruction::PushInt(Value)));
  }

  void LID::Compile(
    const gmachine::Environment::Ptr &machineEnvironment,
    std::vector<Instruction::Ptr>    &into) const
  {
    into.push_back(Instruction::Ptr(
      machineEnvironment->HasVariable(ID)
        ? dynamic_cast<Instruction *>(
            new instruction::Push(machineEnvironment->GetOffset(ID)))

        : dynamic_cast<Instruction *>(new instruction::PushGlobal(ID))));
  }

  void UID::Compile(
    const gmachine::Environment::Ptr &machineEnvironment,
    std::vector<Instruction::Ptr>    &into) const
  {
    into.push_back(Instruction::Ptr(new instruction::PushGlobal(ID)));
  }

  void Application::Compile(
    const gmachine::Environment::Ptr &machineEnvironment,
    std::vector<Instruction::Ptr>    &into) const
  {
    Right->Compile(machineEnvironment, into);
    Left->Compile(Environment::Ptr(new Offset(1, machineEnvironment)), into);
    into.push_back(Instruction::Ptr(new instruction::MakeApplication()));
  }

  void Binop::Compile(
    const gmachine::Environment::Ptr &machineEnvironment,
    std::vector<Instruction::Ptr>    &into) const
  {
    Right->Compile(machineEnvironment, into);
    Left->Compile(machineEnvironment, into);

    into.push_back(Instruction::Ptr(
      new instruction::PushGlobal(gmachine::Binop::Action(Operator))));
    into.push_back(Instruction::Ptr(new instruction::MakeApplication()));
    into.push_back(Instruction::Ptr(new instruction::MakeApplication()));
  }

  static auto CompileVariablePattern(
    instruction::Jump      *jump,
    const type::Data       *type,
    const Branch::Ptr      &branch,
    const Environment::Ptr &machineEnvironment) -> std::vector<Instruction::Ptr>
  {
    std::vector<Instruction::Ptr> branchInstructions;

    branch->Expr->Compile(
      Environment::Ptr(new Offset(1, machineEnvironment)), branchInstructions);

    for (const auto &constructorPair : type->Constructors)
      {
        if (
          jump->TagMappings.find(constructorPair.second.Tag)
          != jump->TagMappings.end())
          break;

        jump->TagMappings[constructorPair.second.Tag] = jump->Branches.size();
      }
    jump->Branches.push_back(std::move(branchInstructions));
    return branchInstructions;
  }

  static auto CompileConstructorPattern(
    instruction::Jump        *jump,
    type::Data               *type,
    const Branch::Ptr        &branch,
    const Environment::Ptr   &machineEnvironment,
    const ConstructorPattern *constructorPattern,
    const yy::location       &Location) -> std::vector<Instruction::Ptr>
  {
    auto                          newEnvironment = machineEnvironment;
    std::vector<Instruction::Ptr> branchInstructions;

    std::for_each(
      constructorPattern->Params.rbegin(),
      constructorPattern->Params.rend(),
      [&](const auto &param) {
        newEnvironment = Environment::Ptr(new Variable(param, newEnvironment));
      });

    branchInstructions.push_back(Instruction::Ptr(new instruction::Split()));
    branch->Expr->Compile(newEnvironment, branchInstructions);
    branchInstructions.push_back(Instruction::Ptr(
      new instruction::Slide(constructorPattern->Params.size())));

    uint8_t newTag =
      type->Constructors[constructorPattern->ConstructorName].Tag;
    if (jump->TagMappings.find(newTag) != jump->TagMappings.end())
      {
        diagnostics::Reporter::REPORTER->normal(
          Location,
          std::format(
            "Duplicate pattern {}", constructorPattern->ConstructorName),
          "This constructor already exists in context",
          "Consider remove this pattern",
          diagnostics::PATTERN_CONSTRUCTOR_IS_DUPLICATED);
      }

    jump->TagMappings[newTag] = jump->Branches.size();
    jump->Branches.push_back(std::move(branchInstructions));
    return branchInstructions;
  }

  static void CheckCompileResult(
    const instruction::Jump *jump,
    const type::Data        *type,
    const yy::location      &Location)
  {
    for (const auto &constructorPair : type->Constructors)
      {
        if (
          jump->TagMappings.find(constructorPair.second.Tag)
          == jump->TagMappings.end())
          {
            diagnostics::Reporter::REPORTER->normal(
              Location,
              "This pattern-matching is not exhaustive",
              "There may be other possible patterns for this expression",
              "Please try to match all cases",
              diagnostics::MATCH_EXPR_IS_NON_EXHAUSTIVE);
          }
      }
  }

  static void CompileBranch(
    const Branch::Ptr      &branch,
    instruction::Jump      *jump,
    type::Data             *type,
    const Environment::Ptr &machineEnvironment,
    const yy::location     &Location)
  {

    auto *variablePattern = dynamic_cast<VariablePattern *>(branch->Patt.get());
    auto *constructorPattern =
      dynamic_cast<ConstructorPattern *>(branch->Patt.get());

    if (nullptr != variablePattern)
      CompileVariablePattern(jump, type, branch, machineEnvironment);
    else if (nullptr != constructorPattern)
      CompileConstructorPattern(
        jump, type, branch, machineEnvironment, constructorPattern, Location);
    else
      Panic("Cannot compile pattern-matching branch");
  }

  void Match::Compile(
    const gmachine::Environment::Ptr &machineEnvironment,
    std::vector<Instruction::Ptr>    &into) const
  {
    auto *type = dynamic_cast<type::Data *>(With->NodeType.get());

    if (nullptr == type)
      {
        std::stringstream with;
        With->Dump(0, with);
        Panic(fmt::format(
          "Cannot compile expression to gmachine instruction for match "
          "expr {} at ({}:{} - {}:{})",
          with.str(),
          Location.begin.line,
          Location.begin.column,
          Location.end.line,
          Location.begin.column));
      }

    auto *jump = new instruction::Jump();

    With->Compile(machineEnvironment, into);
    into.push_back(Instruction::Ptr(new instruction::Eval()));
    into.push_back(Instruction::Ptr(jump));

    for (const auto &branch : Branches)
      CompileBranch(
        branch, jump, type, machineEnvironment, this->With->Location);

    CheckCompileResult(jump, type, this->With->Location);
  }

  void Fn::Compile()
  {
    auto newEnvironment = Environment::Ptr(new Offset(0, nullptr));
    std::for_each(Params.rbegin(), Params.rend(), [&](const auto &param) {
      newEnvironment = Environment::Ptr(new Variable(param, newEnvironment));
    });

    Body->Compile(newEnvironment, Instructions);
    Instructions.push_back(
      Instruction::Ptr(new instruction::Update(Params.size())));
    Instructions.push_back(
      Instruction::Ptr(new instruction::Pop(Params.size())));
  }

  void Data::Compile() {}

} // namespace swallow::compiler::ast
