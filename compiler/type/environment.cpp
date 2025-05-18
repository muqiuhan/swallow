#include "environment.hpp"
#include "ast/ast.hpp"
#include "diagnostics/code.hpp"
#include "diagnostics/reporter.hpp"
#include <stdexcept>
#include <utility>

namespace swallow::compiler::type
{

  auto Environment::Lookup(const std::string &name) const -> Type::Ptr
  {
    if (const auto it = Names.find(name); it != Names.end())
      return it->second;

    if (Parent != nullptr)
      return Parent->Lookup(name);

    return nullptr;
  }

  void Environment::Bind(const std::string &name, Type::Ptr type)
  {
    Names[name] = std::move(type);
  }

  auto Environment::Scope() const -> Environment { return Environment(this); }

} // namespace swallow::compiler::type
