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

#include "type.hpp"
#include <memory>
#include <string>
#include <vector>

using namespace swallow::type;

namespace swallow::ast
{
  class AST
  {
  public:
    using Ptr = std::unique_ptr<AST>;
    virtual ~AST() = default;

    // virtual Type::Ptr typecheck(TypeManager &typeManager,
    //                             const TypeEnvironment &typeEnvironment)
    //                             const;
  };

  class Pattern
  {
  public:
    using Ptr = std::unique_ptr<Pattern>;

    virtual ~Pattern() = default;
  };

  class Branch
  {
  public:
    using Ptr = std::unique_ptr<Branch>;

  private:
    const Pattern::Ptr Patt;
    const AST::Ptr Expr;

  public:
    Branch(Pattern::Ptr Patt, AST::Ptr Expr)
      : Patt(std::move(Patt))
      , Expr(std::move(Expr))
    {}
  };

  class Constructor
  {
  public:
    using Ptr = std::unique_ptr<Constructor>;

  private:
    const std::string Name;
    const std::vector<std::string> Types;

  public:
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
  };

  class Int final : public AST
  {
    const int Value;

  public:
    explicit Int(const int V)
      : Value(V)
    {}

    // virtual Type::Ptr typecheck(TypeManager &typeManager,
    //                             const TypeEnvironment &typeEnvironment)
    //                             const;
  };

  class LID final : public AST
  {
    const std::string ID;

  public:
    explicit LID(std::string ID)
      : ID(std::move(ID))
    {}
  };

  class UID final : public AST
  {
    const std::string ID;

  public:
    explicit UID(std::string ID)
      : ID(std::move(ID))
    {}
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
  };

  class Application final : public AST
  {
    const Ptr left;
    const Ptr right;

  public:
    Application(Ptr Left, Ptr Right)
      : left(std::move(Left))
      , right(std::move(Right))
    {}
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
  };

  class PatternVariable final : public Pattern
  {
    const std::string Variable;

  public:
    explicit PatternVariable(std::string Variable)
      : Variable(std::move(Variable))
    {}
  };

  class PatternConstr final : public Pattern
  {
    const std::string Constr;
    const std::vector<std::string> Params;

  public:
    PatternConstr(std::string Constr, std::vector<std::string> Params)
      : Constr(std::move(Constr))
      , Params(std::move(Params))
    {}
  };

  class Fn final : public Definition
  {
    const std::string Name;
    const std::vector<std::string> Params;
    const AST::Ptr Body;

  public:
    Fn(std::string Name, std::vector<std::string> Params, AST::Ptr Body)
      : Name(std::move(Name))
      , Params(std::move(Params))
      , Body(std::move(Body))
    {}
  };

  class Data final : public Definition
  {
    const std::string Name;
    const std::vector<Constructor::Ptr> Constructors;

  public:
    Data(std::string Name, std::vector<Constructor::Ptr> Constructors)
      : Name(std::move(Name))
      , Constructors(std::move(Constructors))
    {}
  };
} // namespace swallow::ast

#endif