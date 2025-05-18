#include "utils/dump.hpp"
#include "ast.hpp"
#include <cstdint>
#include <format>

namespace swallow::compiler::ast
{
  using namespace swallow::compiler::utils;

  void Int::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << std::format("<Int: {}>", Value) << '\n';
  }

  void LID::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << std::format("<LID: {}>", ID) << '\n';
  }

  void UID::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);
    to << std::format("<UID: {}>", ID) << '\n';
  }

  void Binop::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);

    to << std::format("<Binop: {}", OperatorToString(Operator)) << '\n';

    Left->Dump(indent + 1, to);
    Left->Dump(indent + 1, to);

    to << ">" << '\n';
  }

  void Application::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);

    to << "<Application: " << '\n';

    Left->Dump(indent + 1, to);
    Left->Dump(indent + 1, to);
    to << ">" << '\n';
  }

  void Match::Dump(uint8_t indent, std::ostream &to) const
  {
    dump::DumpIndent(indent, to);

    to << "<Match: " << '\n';

    for (const auto &branch : Branches)
      {
        dump::DumpIndent(indent + 1, to);
        branch->Patt->Dump(to);
        to << '\n';
        branch->Expr->Dump(indent + 2, to);
      }

    to << ">" << '\n';
  }

  void VariablePattern::Dump(std::ostream &to) const { to << Variable; }

  void ConstructorPattern::Dump(std::ostream &to) const
  {
    to << ConstructorName;

    for (const auto &param : Params)
      to << " " << param;
  }

} // namespace swallow::compiler::ast
