#include "binop.hpp"

namespace swallow::compiler::gmachine
{
  [[nodiscard]] auto Binop::ToString(utils::Binop op) -> std::string
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

    Panic("OperatorToString");
  }

  [[nodiscard]] auto Binop::Action(utils::Binop op) -> std::string
  {
    switch (op)
      {
      case utils::Binop::PLUS:
        return {"plus"};
      case utils::Binop::MINUS:
        return {"minus"};
      case utils::Binop::TIMES:
        return {"times"};
      case utils::Binop::DIVIDE:
        return {"divide"};
      }

    Panic("OperatorToString");
  }
} // namespace swallow::compiler::gmachine