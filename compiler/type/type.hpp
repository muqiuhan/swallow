#ifndef SWALLOW_COMPILER_TYPE_H
#define SWALLOW_COMPILER_TYPE_H

#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace swallow::compiler::type
{
  class Manager;

  class Type
  {
  public:
    using Ptr = std::shared_ptr<Type>;

    virtual ~Type() = default;

    virtual void Dump(const Manager &typeManager, std::ostream &to) const = 0;
  };

  class Variable final : public Type
  {
  public:
    const std::string Name;

    explicit Variable(std::string Name) : Name(std::move(Name)) {}

    void Dump(const Manager &typeManager, std::ostream &to) const override;
  };

  class Base : public Type
  {
  public:
    const std::string Name;

    explicit Base(std::string Name) : Name(std::move(Name)) {}

    void Dump(const Manager &typeManager, std::ostream &to) const override;
  };

  class Data final : public Base
  {
  public:
    struct Constructor
    {
      uint8_t Tag;
    };

    std::map<std::string, Constructor> Constructors;

    explicit Data(std::string Name) : Base(std::move(Name)) {}
  };

  class Arrow final : public Type
  {
  public:
    const Ptr Left;
    const Ptr Right;

    Arrow(Ptr Left, Ptr Right) : Left(std::move(Left)), Right(std::move(Right))
    {}

    void Dump(const Manager &typeManager, std::ostream &to) const override;
  };

  class Manager
  {
  public:
    int32_t                          LastID = 0;
    std::map<std::string, Type::Ptr> Types;

    auto NewTypeName() -> std::string;
    auto NewType() -> Type::Ptr;
    auto NewArrowType() -> Type::Ptr;

    /** Find values for placeholder variables such that they can equal. */
    auto Unify(Type::Ptr left, Type::Ptr right) -> void;

    /** Get to the bottom of a chain of equations. */
    auto Resolve(Type::Ptr type, Variable *&var) const -> Type::Ptr;

    /** Map a type variable of some name to a type. */
    void Bind(const std::string &name, const Type::Ptr &type);
  };
} // namespace swallow::compiler::type

#endif