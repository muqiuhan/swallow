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

#include "ast.h"
#include "environment.h"
#include "panic/panic.hpp"
#include "type.h"
#include <algorithm>
#include <vector>

using namespace swallow::utils;

namespace swallow::ast
{

  std::string Binop::operatorsToString(const Operators op) noexcept
  {
    switch (op)
      {
      case Operators::PLUS:
        return "+";
      case Operators::MINUS:
        return "-";
      case Operators::TIMES:
        return "*";
      case Operators::DIVIDE:
        return "/";
      }

    panic("Unknown operator");
  }

  type::Type::Ptr
  Int::typecheck(type::Manager & typeManager,
                 const type::Environment & typeEnvironment) const noexcept
  {
    return type::Type::Ptr(new type::Base("Int"));
  }

  type::Type::Ptr
  LID::typecheck(type::Manager & typeManager,
                 const type::Environment & typeEnvironment) const noexcept
  {
    return typeEnvironment.lookup(ID);
  }

  type::Type::Ptr
  UID::typecheck(type::Manager & typeManager,
                 const type::Environment & typeEnvironment) const noexcept
  {
    return typeEnvironment.lookup(ID);
  }

  type::Type::Ptr
  Binop::typecheck(type::Manager & typeManager,
                   const type::Environment & typeEnvironment) const noexcept
  {
    const std::string operatorName = operatorsToString(Operator);

    type::Type::Ptr leftType = Left->typecheck(typeManager, typeEnvironment);
    type::Type::Ptr rightType = Right->typecheck(typeManager, typeEnvironment);
    type::Type::Ptr functionType = typeEnvironment.lookup(operatorName);

    if (!functionType)
      panic("Typecheck failed: cannot find the operator declaration `{}`",
            operatorName);

    type::Type::Ptr returnType = typeManager.newType();
    type::Type::Ptr arrowOne =
      type::Type::Ptr(new type::Arrow(rightType, returnType));
    type::Type::Ptr arrowTwo =
      type::Type::Ptr(new type::Arrow(leftType, arrowOne));

    typeManager.unify(arrowTwo, functionType);
    return returnType;
  }

  type::Type::Ptr Application::typecheck(
    type::Manager & typeManager,
    const type::Environment & typeEnvironment) const noexcept
  {
    type::Type::Ptr leftType = Left->typecheck(typeManager, typeEnvironment);
    type::Type::Ptr rightType = Right->typecheck(typeManager, typeEnvironment);
    type::Type::Ptr returnType = typeManager.newType();
    type::Type::Ptr arrowType =
      type::Type::Ptr(new type::Arrow(rightType, returnType));

    typeManager.unify(arrowType, leftType);
    return returnType;
  }

  type::Type::Ptr
  Match::typecheck(type::Manager & typeManager,
                   const type::Environment & typeEnvironment) const noexcept
  {
    type::Type::Ptr matchType = With->typecheck(typeManager, typeEnvironment);
    type::Type::Ptr branchType = typeManager.newType();

    for (auto & branch : Branches)
      {
        type::Environment newEnvironment = typeEnvironment.scope();
        branch->Patt->match(matchType, typeManager, newEnvironment);

        type::Type::Ptr currentBranchType =
          branch->Expr->typecheck(typeManager, newEnvironment);
        typeManager.unify(branchType, currentBranchType);
      }

    return branchType;
  }

  void
  PatternVariable::match(type::Type::Ptr type,
                         type::Manager & typeManager,
                         type::Environment & typeEnvironment) const noexcept
  {
    typeManager.bind(Variable, type);
  }

  void
  PatternConstructor::match(type::Type::Ptr type,
                            type::Manager & typeManager,
                            type::Environment & typeEnvironment) const noexcept
  {
    type::Type::Ptr constructorType = typeEnvironment.lookup(Constructor);

    if (!constructorType)
      panic("type checking failed: no declartion of constructor '{}'",
            Constructor);

    std::for_each(Params.begin(), Params.end(), [&](const std::string & param) {
      type::Arrow * arrow = dynamic_cast<type::Arrow *>(constructorType.get());

      if (!arrow)
        panic("type checking failed: constructor '{}' must be a function type",
              Constructor);

      typeEnvironment.bind(param, arrow->Left);
      constructorType = arrow->Right;
    });

    typeManager.unify(type, constructorType);
    if (const auto resultType =
          dynamic_cast<type::Base *>(constructorType.get());
        !resultType)
      panic("type checking failed: return value is not unique");
  }

  void Fn::scanDefinitionType(type::Manager & typeManager,
                              type::Environment & typeEnvironment) noexcept
  {
    std::cout << std::format("scan function '{}' definition", Name)
              << std::endl;
    const type::Type::Ptr returnType = typeManager.newType();
    type::Type::Ptr fullType = returnType;
    std::for_each(Params.rbegin(),
                  Params.rend(),
                  [&](const std::string & param) {
                    type::Type::Ptr paramType = typeManager.newType();
                    fullType =
                      type::Type::Ptr(new type::Arrow(paramType, fullType));

                    ParamTypes.push_back(paramType);
                  });

    typeEnvironment.bind(Name, fullType);
  }

  void Fn::typecheck(type::Manager & typeManager,
                     const type::Environment & typeEnvironment) const noexcept
  {
    type::Environment newEnvironment = typeEnvironment.scope();

    auto paramIterator = Params.begin();
    auto typeIterator = ParamTypes.rbegin();
    while (paramIterator != Params.end() && typeIterator != ParamTypes.rend())
      {
        newEnvironment.bind(*paramIterator, *typeIterator);
        paramIterator++;
        typeIterator++;
      }

    typeManager.unify(returnType, Body->typecheck(typeManager, newEnvironment));
  }

  void Data::scanDefinitionType(type::Manager & typeManager,
                                type::Environment & typeEnvironment) noexcept
  {
    std::cout << std::format("scan data '{}' definition", Name) << std::endl;
    auto fullType = type::Type::Ptr(new type::Base(Name));
    for (const auto & constructor : Constructors)
      {
        for (const auto & typeName : constructor->Types)
          fullType = type::Type::Ptr(
            new type::Arrow(type::Type::Ptr(new type::Base(typeName)),
                            fullType));

        typeEnvironment.bind(constructor->Name, fullType);
      }
  }

  void Data::typecheck(type::Manager & typeManager,
                       const type::Environment & typeEnvironment) const noexcept
  {}

} // namespace swallow::ast

namespace swallow::type
{
  void typecheck(const std::vector<ast::Definition::Ptr> & program) noexcept
  {
    std::cout << "Type checking..." << std::endl;
    type::Manager typeManager;
    type::Environment typeEnvironment;

    auto intType = type::Type::Ptr(new type::Base("Int"));
    auto binopType = type::Type::Ptr(
      new type::Arrow(intType,
                      type::Type::Ptr(new type::Arrow(intType, intType))));

    std::cout << "Bind binary operators..." << std::endl;
    typeEnvironment.bind("+", binopType);
    typeEnvironment.bind("-", binopType);
    typeEnvironment.bind("*", binopType);
    typeEnvironment.bind("/", binopType);

    std::cout << "scan definitions type..." << std::endl;
    for (const auto & definition : program)
      definition->scanDefinitionType(typeManager, typeEnvironment);

    std::cout << "type checking..." << std::endl;
    for (const auto & definition : program)
      definition->typecheck(typeManager, typeEnvironment);
  }
} // namespace swallow::type