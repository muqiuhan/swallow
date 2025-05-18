#ifndef SWALLOW_COMPILER_TYPE_ENVIRONMENT_H
#define SWALLOW_COMPILER_TYPE_ENVIRONMENT_H

#include "type.hpp"
#include <map>
#include <string>

namespace swallow::compiler::type
{

  class Environment
  {
  public:
    std::map<std::string, Type::Ptr> Names;
    Environment const               *Parent = nullptr;

    explicit Environment(Environment const *Parent) : Parent(Parent) {}

    Environment() : Environment(nullptr) {}

    [[nodiscard]] auto Lookup(const std::string &name) const -> Type::Ptr;

    [[nodiscard]] auto Scope() const -> Environment;

    void Bind(const std::string &name, Type::Ptr type);
    void Dump(std::ostream &to, const Manager &typeManager);
  };

} // namespace swallow::compiler::type

#endif // SWALLOW_COMPILER_TYPE_ENVIRONMENT_H