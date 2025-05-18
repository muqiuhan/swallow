#ifndef SWALLOW_COMPILER_G_MACHINE_BINOP_HPP
#define SWALLOW_COMPILER_G_MACHINE_BINOP_HPP

#include "error/errors.hpp"
#include "utils/binop.hpp"
#include <string>

namespace swallow::compiler::gmachine
{
  class Binop
  {
  public:
    [[nodiscard]] static auto ToString(utils::Binop op) -> std::string;
    [[nodiscard]] static auto Action(utils::Binop op) -> std::string;
  };
} // namespace swallow::compiler::gmachine

#endif /* SWALLOW_COMPILER_G_MACHINE_BINOP_HPP */