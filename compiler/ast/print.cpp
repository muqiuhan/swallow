#include "ast.h"
#include <cstdint>
#include <format>

namespace swallow::compiler::ast
{
  static void printIndent(uint8_t n, std::ostream & to) noexcept
  {
    while (n--)
      to << "  ";
  }

  void Int::print(uint8_t indent, std::ostream & to) const noexcept
  {
    printIndent(indent, to);
    to << std::format("<Int: {}>", Value) << std::endl;
  }

  void LID::print(uint8_t indent, std::ostream & to) const noexcept
  {
    printIndent(indent, to);
    to << std::format("<LID: {}>", ID) << std::endl;
  }

  void UID::print(uint8_t indent, std::ostream & to) const noexcept
  {
    printIndent(indent, to);
    to << std::format("<UID: {}>", ID) << std::endl;
  }

  void Binop::print(uint8_t indent, std::ostream & to) const noexcept
  {
    printIndent(indent, to);

    to << std::format("<Binop: {}", operatorsToString(Operator).unwrap())
       << std::endl;

    Left->print(indent + 1, to);
    Left->print(indent + 1, to);

    to << ">" << std::endl;
  }

  void Application::print(uint8_t indent, std::ostream & to) const noexcept
  {
    printIndent(indent, to);

    to << "<Application: " << std::endl;

    Left->print(indent + 1, to);
    Left->print(indent + 1, to);
    to << ">" << std::endl;
  }

  void Match::print(uint8_t indent, std::ostream & to) const noexcept
  {
    printIndent(indent, to);

    to << "<Match: " << std::endl;

    for (const auto & branch : Branches)
      {
        printIndent(indent + 1, to);
        branch->Patt->print(to);
        to << std::endl;
        branch->Expr->print(indent + 2, to);
      }

    to << ">" << std::endl;
  }

  void PatternVariable::print(std::ostream & to) const noexcept
  {
    to << Variable;
  }

  void PatternConstructor::print(std::ostream & to) const noexcept
  {
    to << Constructor;

    for (const auto & param : Params)
      to << " " << param;
  }

} // namespace swallow::compiler::ast