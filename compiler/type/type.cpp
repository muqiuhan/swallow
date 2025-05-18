#include "type.hpp"
#include <algorithm>

namespace swallow::compiler::type
{

  auto Manager::NewTypeName() -> std::string
  {
    int32_t     currentID = LastID++;
    std::string name;

    while (currentID != -1)
      {
        name += static_cast<char>(('a' + (currentID % 26)));
        currentID = currentID / 26 - 1;
      }

    std::reverse(name.begin(), name.end());
    return name;
  }

  auto Manager::NewType() -> Type::Ptr
  {
    return Type::Ptr(new Variable(NewTypeName()));
  }

  auto Manager::NewArrowType() -> Type::Ptr
  {
    return Type::Ptr(new Arrow(NewType(), NewType()));
  }

  auto Manager::Resolve(Type::Ptr type, Variable *&var) const -> Type::Ptr
  {
    Variable *cast = nullptr;
    var = nullptr;

    while ((cast = dynamic_cast<Variable *>(type.get())) != nullptr)
      {
        const auto it = Types.find(cast->Name);
        if (it == Types.end())
          {
            var = cast;
            break;
          }

        type = it->second;
      }
    return type;
  }

  auto Manager::Unify(Type::Ptr left, Type::Ptr right) -> void
  {
    Variable *leftVar = nullptr;
    Variable *rightVar = nullptr;

    left = Resolve(left, leftVar);
    right = Resolve(right, rightVar);

    if (leftVar != nullptr)
      return Bind(leftVar->Name, right);

    if (rightVar != nullptr)
      return Bind(rightVar->Name, left);

    if (auto *leftArrow = dynamic_cast<Arrow *>(left.get()),
        *rightArrow = dynamic_cast<Arrow *>(right.get());
        leftArrow != nullptr && rightArrow != nullptr)
      {

        Unify(leftArrow->Left, rightArrow->Left);
        return Unify(leftArrow->Right, rightArrow->Right);
      }

    if (auto *leftID = dynamic_cast<Base *>(left.get()),
        *rightID = dynamic_cast<Base *>(right.get());
        leftID != nullptr && rightID != nullptr)
      {
        if (leftID->Name == rightID->Name)
          return;
      }

    throw std::runtime_error("Type mismatch");
  }

  void Manager::Bind(const std::string &name, const Type::Ptr &type)
  {
    if (const auto *other = dynamic_cast<const Variable *>(type.get());
        (other != nullptr) && other->Name == name)
      return;

    Types[name] = type;
  }

} // namespace swallow::compiler::type