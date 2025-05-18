#ifndef SWALLOW_COMPILER_PARSER_H
#define SWALLOW_COMPILER_PARSER_H

#include "ast/ast.hpp"

namespace swallow::compiler::parser
{

  auto Parse() -> std::vector<swallow::compiler::ast::Definition::Ptr> &;

} // namespace swallow::compiler::parser

#endif