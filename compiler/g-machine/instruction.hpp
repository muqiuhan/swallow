// Copyright (c) 2023 Muqiu Han
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//     notice,
//       this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Swallow nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef SWALLOW_COMPILER_G_MACHINE_INSTRUCTION_HPP
#define SWALLOW_COMPILER_G_MACHINE_INSTRUCTION_HPP

#include "binop.hpp"
#include "g-machine/environment.hpp"
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

    virtual void Dump(uint8_t indent, std::ostream& to) const noexcept = 0;
  };

  class PushInt : public Instruction
  {
  public:
    const int Value;

    explicit PushInt(int Value) : Value(Value) {}

    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class PushGlobal : public Instruction
  {
  public:
    const std::string Name;

    explicit PushGlobal(std::string Name) : Name(std::move(Name)) {}

    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class Push : public Instruction
  {
  public:
    uint32_t Offset;

    explicit Push(uint32_t Offset) : Offset(Offset) {}

    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class Pop : public Instruction
  {
  public:
    const uint32_t Count;

    explicit Pop(uint32_t Count) : Count(Count) {}

    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class MakeApplication : public Instruction
  {
  public:
    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class Update : public Instruction
  {
  public:
    uint32_t Offset;

    explicit Update(uint32_t Offset) : Offset(Offset) {}

    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class Pack : public Instruction
  {
  public:
    int Tag;
    int Size;

    Pack(int Tag, int Size) : Tag(Tag), Size(Size) {}

    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class Split : public Instruction
  {
  public:
    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class Slide : public Instruction
  {
  public:
    uint32_t Offset;

    explicit Slide(uint32_t Offset) : Offset(Offset) {}

    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class Binop : public Instruction
  {
  public:
    utils::Binop Operator;

    explicit Binop(utils::Binop Operator) : Operator(Operator) {}

    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class Eval : public Instruction
  {
  public:
    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class Allocation : public Instruction
  {
  public:
    int Amount;

    explicit Allocation(int Amount) : Amount(Amount) {}

    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class Unwind : public Instruction
  {
  public:
    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };

  class Jump : public Instruction
  {
  public:
    std::vector<std::vector<Instruction::Ptr>> Branches;
    std::map<int, int> TagMappings;

    void Dump(uint8_t indent, std::ostream& to) const noexcept override;
  };
} // namespace swallow::compiler::gmachine::instruction

#endif /* SWALLOW_COMPILER_G_MACHINE_INSTRUCTION_HPP */
