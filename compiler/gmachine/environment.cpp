#include "environment.hpp"

namespace swallow::compiler::gmachine
{

  [[nodiscard]] auto Variable::GetOffset(const std::string &name) const -> int
  {
    if (name == Name)
      return 0;

    if (Parent != nullptr)
      return Parent->GetOffset(name) + 1;

    throw std::runtime_error("Variable not found");
  }

  [[nodiscard]] auto Variable::HasVariable(const std::string &name) const
    -> bool
  {
    if (name == Name)
      return true;

    if (Parent != nullptr)
      return Parent->HasVariable(name);

    return false;
  }

  [[nodiscard]] auto Offset::HasVariable(const std::string &name) const -> bool
  {
    if (Parent != nullptr)
      return Parent->HasVariable(name);

    return false;
  }

  [[nodiscard]] auto Offset::GetOffset(const std::string &name) const -> int
  {
    if (Parent != nullptr)
      return Parent->GetOffset(name) + Value;

    throw std::runtime_error("Variable not found");
  }

} // namespace swallow::compiler::gmachine
