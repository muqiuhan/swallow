#ifndef SWALLOW_COMPILER_G_MACHINE_INSTRUCTION_HPP
#define SWALLOW_COMPILER_G_MACHINE_INSTRUCTION_HPP

#include "binop.hpp"
#include "gmachine/environment.hpp"
#include <cstdint>
#include <map>
#include <memory>
#include <vector>

namespace swallow::compiler::gmachine::instruction
{

  class Instruction
  {
  public:
    using Ptr = std::unique_ptr<Instruction>;

    virtual ~Instruction() = default;

    virtual void Dump(uint8_t indent, std::ostream &to) const = 0;
  };

  class PushInt : public Instruction
  {
  public:
    const int Value;

    explicit PushInt(int Value) : Value(Value) {}

    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class PushGlobal : public Instruction
  {
  public:
    const std::string Name;

    explicit PushGlobal(std::string Name) : Name(std::move(Name)) {}

    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class Push : public Instruction
  {
  public:
    uint32_t Offset;

    explicit Push(uint32_t Offset) : Offset(Offset) {}

    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class Pop : public Instruction
  {
  public:
    const uint32_t Count;

    explicit Pop(uint32_t Count) : Count(Count) {}

    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class MakeApplication : public Instruction
  {
  public:
    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class Update : public Instruction
  {
  public:
    uint32_t Offset;

    explicit Update(uint32_t Offset) : Offset(Offset) {}

    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class Pack : public Instruction
  {
  public:
    int Tag;
    int Size;

    Pack(int Tag, int Size) : Tag(Tag), Size(Size) {}

    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class Split : public Instruction
  {
  public:
    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class Slide : public Instruction
  {
  public:
    uint32_t Offset;

    explicit Slide(uint32_t Offset) : Offset(Offset) {}

    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class Binop : public Instruction
  {
  public:
    utils::Binop Operator;

    explicit Binop(utils::Binop Operator) : Operator(Operator) {}

    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class Eval : public Instruction
  {
  public:
    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class Allocation : public Instruction
  {
  public:
    int Amount;

    explicit Allocation(int Amount) : Amount(Amount) {}

    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class Unwind : public Instruction
  {
  public:
    void Dump(uint8_t indent, std::ostream &to) const override;
  };

  class Jump : public Instruction
  {
  public:
    std::vector<std::vector<Instruction::Ptr>> Branches;
    std::map<uint8_t, uint8_t>                 TagMappings;

    void Dump(uint8_t indent, std::ostream &to) const override;
  };
} // namespace swallow::compiler::gmachine::instruction

#endif /* SWALLOW_COMPILER_G_MACHINE_INSTRUCTION_HPP */
