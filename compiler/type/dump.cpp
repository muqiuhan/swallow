#include "environment.hpp"
#include "type.hpp"
#include <ostream>

namespace swallow::compiler::type
{
  void Variable::Dump(const Manager &typeManager, std::ostream &to) const
  {
    const auto it = typeManager.Types.find(Name);

    if (it != typeManager.Types.end())
      it->second->Dump(typeManager, to);
    else
      to << Name;
  }

  void Arrow::Dump(const Manager &typeManager, std::ostream &to) const
  {
    Left->Dump(typeManager, to);
    to << " -> ";
    Left->Dump(typeManager, to);
  }

  void Base::Dump(const Manager &typeManager, std::ostream &to) const
  {
    to << Name;
  }

  void Environment::Dump(std::ostream &to, const Manager &typeManager)
  {
    for (const auto &[name, type] : Names)
      {
        to << name << ": ";
        type->Dump(typeManager, to);
        to << '\n';
      }
  }
} // namespace swallow::compiler::type