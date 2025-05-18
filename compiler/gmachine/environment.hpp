#ifndef SWALLOW_COMPILER_G_MACHINE_ENVIRONMENT_HPP
#define SWALLOW_COMPILER_G_MACHINE_ENVIRONMENT_HPP

#include "error/errors.hpp"
#include <memory>
#include <string>

namespace swallow::compiler::gmachine
{

  class Environment
  {
  public:
    using Ptr = std::shared_ptr<Environment>;

    virtual ~Environment() = default;

    [[nodiscard]] virtual auto GetOffset(const std::string &name) const
      -> int = 0;

    [[nodiscard]] virtual auto HasVariable(const std::string &name) const
      -> bool = 0;
  };

  class Variable : public Environment
  {
  public:
    std::string Name;
    Ptr         Parent;

    Variable(std::string Name, Ptr Parent)
      : Name(std::move(Name)), Parent(std::move(Parent))
    {}

    [[nodiscard]] auto GetOffset(const std::string &name) const -> int override;

    [[nodiscard]] auto HasVariable(const std::string &name) const
      -> bool override;
  };

  class Offset : public Environment
  {
  public:
    uint32_t Value;
    Ptr      Parent;

    Offset(uint32_t Value, Ptr Parent) : Value(Value), Parent(std::move(Parent))
    {}

    [[nodiscard]] auto GetOffset(const std::string &name) const -> int override;

    [[nodiscard]] auto HasVariable(const std::string &name) const
      -> bool override;
  };

} // namespace swallow::compiler::gmachine

#endif /* SWALLOW_COMPILER_G_MACHINE_ENVIRONMENT_HPP */
