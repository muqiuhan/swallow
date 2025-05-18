#include "instruction.hpp"
#include "ast/ast.hpp"
#include "utils/dump.hpp"
#include <format>

using namespace swallow::compiler::utils;

namespace swallow::compiler::gmachine::instruction
{
  void Pop::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << std::format("Pop\t\tCount = {}\n", Count);
  }

  void MakeApplication::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << "MakeApplication\n";
  }

  void Pack::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << std::format("Pack\t\tTag = {}, Size = {}\n", Tag, Size);
  }

  void Split::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << "Split\n";
  }

  void Slide::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << std::format("Slide\t\tOffset = {}\n", Offset);
  }

  void Binop::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << std::format(
      "Binop\t\tOperator = {}\n", ast::Binop::OperatorToString(Operator));
  }

  void Allocation::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << std::format("Allocation\t\tAmount = {}\n", Amount);
  }

  void Unwind::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << "Unwind";
  }

  void Jump::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << "Jump\n\n";

    for (const auto &branch : Branches)
      std::for_each(branch.begin(), branch.end(), [&](const auto &instruction) {
        instruction->Dump(2, to);
      });
    to << "\n";
  }

  void PushInt::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << std::format("PushInt\t\tValue = {}\n", Value);
  }

  void PushGlobal::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << std::format("PushGlobal\tName = {}\n", Name);
  }

  void Push::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << std::format("Push\t\tOffset = {}\n", Offset);
  }

  void Eval::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << "Eval\n";
  }

  void Update::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << std::format("Update\t\tOffset = {}\n", Offset);
  }

} // namespace swallow::compiler::gmachine::instruction
