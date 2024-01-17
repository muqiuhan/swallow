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
//     * Neither the name of Terifo nor the names of its contributors
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

#ifndef SWALLOW_AST_HPP
#define SWALLOW_AST_HPP

#include "environment.h"
#include "type.h"
#include "type.hpp"
#include <memory>
#include <string>
#include <vector>

namespace swallow::ast
{
  class AST
  {
  public:
    using Ptr = std::unique_ptr<AST>;
    virtual ~AST() = default;

    virtual type::Type::Ptr
    typecheck(type::Manager & typeManager,
              const type::Environment & typeEnvironment) const noexcept = 0;
  };

  class Pattern
  {
  public:
    using Ptr = std::unique_ptr<Pattern>;

    virtual ~Pattern() = default;

    virtual void match(type::Type::Ptr type,
                       type::Manager & typeManager,
                       type::Environment & typeEnvironment) const noexcept = 0;
  };

  class Branch
  {
  public:
    using Ptr = std::unique_ptr<Branch>;

    const Pattern::Ptr Patt;
    const AST::Ptr Expr;

    Branch(Pattern::Ptr Patt, AST::Ptr Expr)
      : Patt(std::move(Patt))
      , Expr(std::move(Expr))
    {}
  };

  class Constructor
  {
  public:
    using Ptr = std::unique_ptr<Constructor>;

    const std::string Name;
    const std::vector<std::string> Types;

    Constructor(std::string Name, std::vector<std::string> Types)
      : Name(std::move(Name))
      , Types(std::move(Types))
    {}
  };

  class Definition
  {
  public:
    using Ptr = std::unique_ptr<Definition>;

    virtual ~Definition() = default;

    virtual void
    scanDefinitionType(type::Manager & typeManager,
                       type::Environment & typeEnvironment) noexcept = 0;
    virtual void
    typecheck(type::Manager & typeManager,
              const type::Environment & typeEnvironment) const noexcept = 0;
  };

  class Int final : public AST
  {
    const int Value;

  public:
    explicit Int(const int V)
      : Value(V)
    {}

    virtual type::Type::Ptr typecheck(
      type::Manager & typeManager,
      const type::Environment & typeEnvironment) const noexcept override;
  };

  class LID final : public AST
  {
    const std::string ID;

  public:
    explicit LID(std::string ID)
      : ID(std::move(ID))
    {}

    virtual type::Type::Ptr typecheck(
      type::Manager & typeManager,
      const type::Environment & typeEnvironment) const noexcept override;
  };

  class UID final : public AST
  {
    const std::string ID;

  public:
    explicit UID(std::string ID)
      : ID(std::move(ID))
    {}

    virtual type::Type::Ptr typecheck(
      type::Manager & typeManager,
      const type::Environment & typeEnvironment) const noexcept override;
  };

  class Binop final : public AST
  {
  public:
    enum class Operators
    {
      PLUS,
      MINUS,
      TIMES,
      DIVIDE
    };

  private:
    const Operators Operator;
    const Ptr Left;
    const Ptr Right;

  public:
    Binop(Operators Operator, Ptr Left, Ptr Right)
      : Operator(Operator)
      , Left(std::move(Left))
      , Right(std::move(Right))
    {}

    static std::string operatorsToString(const Operators op) noexcept;

    virtual type::Type::Ptr typecheck(
      type::Manager & typeManager,
      const type::Environment & typeEnvironment) const noexcept override;
  };

  class Application final : public AST
  {
    const Ptr Left;
    const Ptr Right;

  public:
    Application(Ptr Left, Ptr Right)
      : Left(std::move(Left))
      , Right(std::move(Right))
    {}

    virtual type::Type::Ptr typecheck(
      type::Manager & typeManager,
      const type::Environment & typeEnvironment) const noexcept override;
  };

  class Match final : public AST
  {
    const Ptr With;
    const std::vector<Branch::Ptr> Branches;

  public:
    Match(Ptr o, std::vector<Branch::Ptr> b)
      : With(std::move(o))
      , Branches(std::move(b))
    {}

    virtual type::Type::Ptr typecheck(
      type::Manager & typeManager,
      const type::Environment & typeEnvironment) const noexcept override;
  };

  class PatternVariable final : public Pattern
  {
    const std::string Variable;

  public:
    explicit PatternVariable(std::string Variable)
      : Variable(std::move(Variable))
    {}

    void match(type::Type::Ptr type,
               type::Manager & typeManager,
               type::Environment & typeEnvironment) const noexcept override;
  };

  class PatternConstructor final : public Pattern
  {
  public:
    const std::string Constructor;
    const std::vector<std::string> Params;

    PatternConstructor(std::string Constructor, std::vector<std::string> Params)
      : Constructor(std::move(Constructor))
      , Params(std::move(Params))
    {}

    virtual void
    match(type::Type::Ptr type,
          type::Manager & typeManager,
          type::Environment & typeEnvironment) const noexcept override;
  };

  class Fn final : public Definition
  {
  public:
    const std::string Name;
    const std::vector<std::string> Params;
    const AST::Ptr Body;

    std::vector<type::Type::Ptr> ParamTypes;
    type::Type::Ptr returnType;

    Fn(std::string Name, std::vector<std::string> Params, AST::Ptr Body)
      : Name(std::move(Name))
      , Params(std::move(Params))
      , Body(std::move(Body))
    {}

    virtual void
    scanDefinitionType(type::Manager & typeManager,
                       type::Environment & typeEnvironment) noexcept override;
    virtual void typecheck(
      type::Manager & typeManager,
      const type::Environment & typeEnvironment) const noexcept override;
  };

  class Data final : public Definition
  {
  public:
    const std::string Name;
    const std::vector<Constructor::Ptr> Constructors;

    Data(std::string Name, std::vector<Constructor::Ptr> Constructors)
      : Name(std::move(Name))
      , Constructors(std::move(Constructors))
    {}

    virtual void
    scanDefinitionType(type::Manager & typeManager,
                       type::Environment & typeEnvironment) noexcept override;
    virtual void typecheck(
      type::Manager & typeManager,
      const type::Environment & typeEnvironment) const noexcept override;
  };
} // namespace swallow::ast

namespace swallow::type
{
  void typecheck(const std::vector<ast::Definition::Ptr> & program) noexcept;
} // namespace swallow::type

#endif