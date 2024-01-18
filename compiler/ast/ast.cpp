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
#include <reporter.h>
#include <utils.h>
#include <vector>

using namespace swallow::utils;

namespace swallow::compiler::ast
{

  Result<std::string, Void>
  Binop::operatorsToString(const Operators & op) noexcept
  {
    switch (op)
      {
      case Operators::PLUS:
        return Ok(std::string("+"));
      case Operators::MINUS:
        return Ok(std::string("-"));
      case Operators::TIMES:
        return Ok(std::string("*"));
      case Operators::DIVIDE:
        return Ok(std::string("/"));
      }

    return Err(Void());
  }

  Result<type::Type::Ptr, Void>
  Int::typecheck(type::Manager & typeManager,
                 const type::Environment & typeEnvironment) const noexcept
  {
    return Ok(type::Type::Ptr(new type::Base("Int")));
  }

  Result<type::Type::Ptr, Void>
  LID::typecheck(type::Manager & typeManager,
                 const type::Environment & typeEnvironment) const noexcept
  {
    return typeEnvironment.lookup(ID).or_else([&](const auto & err) {
      return diagnostics::Reporter::REPORTER->normal(
        Location,
        std::format("{} was not declared", ID),
        "The definition of this identifier cannot be found in the context",
        "Function or Variable is undefined",
        0x0002);
    });
  }

  Result<type::Type::Ptr, Void>
  UID::typecheck(type::Manager & typeManager,
                 const type::Environment & typeEnvironment) const noexcept
  {
    return typeEnvironment.lookup(ID).or_else([&](const auto & err) {
      return diagnostics::Reporter::REPORTER->normal(
        Location,
        std::format("{} was not declared", ID),
        "The definition of this identifier cannot be found in the context",
        "Type or Constructor is undefined",
        0x0002);
    });
  }

  Result<type::Type::Ptr, Void>
  Binop::typecheck(type::Manager & typeManager,
                   const type::Environment & typeEnvironment) const noexcept
  {
    const std::string operatorName = operatorsToString(Operator).unwrap();

    type::Type::Ptr leftType =
      Left->typecheck(typeManager, typeEnvironment)
        .or_else([&](const auto & err) {
          return diagnostics::Reporter::REPORTER->normal(
            Left->Location,
            "Type mismatch",
            "This expression has type {}, expect {}",
            "Type of operator {} is {}",
            0x0003);
        })
        .unwrap();

    type::Type::Ptr rightType =
      Right->typecheck(typeManager, typeEnvironment)
        .or_else([&](const auto & err) {
          return diagnostics::Reporter::REPORTER->normal(
            Right->Location,
            "Type mismatch",
            "This expression has type {}, expect {}",
            "Type of operator {} is {}",
            0x0003);
        })
        .unwrap();

    type::Type::Ptr functionType =
      typeEnvironment.lookup(operatorName)
        .or_else([&](const auto & err) {
          return diagnostics::Reporter::REPORTER->normal(
            Location,
            std::format("{} was not declared", operatorName),
            "The definition of this operator cannot be found in the context",
            "Operator is undefined",
            0x0004);
        })
        .unwrap();

    type::Type::Ptr returnType = typeManager.newType();
    type::Type::Ptr arrowOne =
      type::Type::Ptr(new type::Arrow(rightType, returnType));
    type::Type::Ptr arrowTwo =
      type::Type::Ptr(new type::Arrow(leftType, arrowOne));

    typeManager.unify(arrowTwo, functionType)
      .or_else([&](const auto & err) {
        return diagnostics::Reporter::REPORTER->normal(
          Location,
          std::format("Type inference failed for {}", operatorName),
          "The type of this expression cannot be unified",
          "No more information",
          0x0005);
      })
      .unwrap();

    return Ok(returnType);
  }

  Result<type::Type::Ptr, Void> Application::typecheck(
    type::Manager & typeManager,
    const type::Environment & typeEnvironment) const noexcept
  {
    type::Type::Ptr leftType =
      Left->typecheck(typeManager, typeEnvironment)
        .or_else([&](const auto & err) {
          return diagnostics::Reporter::REPORTER->normal(
            Left->Location,
            "Type mismatch",
            "This expression has type {}, expect {}",
            "No more information",
            0x0003);
        })
        .unwrap();

    type::Type::Ptr rightType =
      Right->typecheck(typeManager, typeEnvironment)
        .or_else([&](const auto & err) {
          return diagnostics::Reporter::REPORTER->normal(
            Right->Location,
            "Type mismatch",
            "This expression has type {}, expect {}",
            "No more information",
            0x0003);
        })
        .unwrap();

    type::Type::Ptr returnType = typeManager.newType();
    type::Type::Ptr arrowType =
      type::Type::Ptr(new type::Arrow(rightType, returnType));

    typeManager.unify(arrowType, leftType)
      .or_else([&](const auto & err) {
        return diagnostics::Reporter::REPORTER->normal(
          Location,
          "Type inference failed",
          "The type of this expression cannot be unified",
          "No more information",
          0x0005);
      })
      .unwrap();

    return Ok(returnType);
  }

  Result<type::Type::Ptr, Void>
  Match::typecheck(type::Manager & typeManager,
                   const type::Environment & typeEnvironment) const noexcept
  {
    type::Type::Ptr matchType =
      With->typecheck(typeManager, typeEnvironment)
        .or_else([&](const auto & err) {
          return diagnostics::Reporter::REPORTER->normal(
            With->Location,
            "Type mismatch",
            "Cannot use pattern matching on this expression",
            "No more information",
            0x0003);
        })
        .unwrap();
    ;
    type::Type::Ptr branchType = typeManager.newType();

    for (auto & branch : Branches)
      {
        type::Environment newEnvironment = typeEnvironment.scope();
        branch->Patt->match(matchType, typeManager, newEnvironment);

        type::Type::Ptr currentBranchType =
          branch->Expr->typecheck(typeManager, newEnvironment)
            .or_else([&](const auto & err) {
              return diagnostics::Reporter::REPORTER->normal(
                branch->Location,
                "Type mismatch",
                "This expression has type {}, expect {}",
                "The return type of all branches of the pattern match must be "
                "the same",
                0x0003);
            })
            .unwrap();

        typeManager.unify(branchType, currentBranchType)
          .or_else([&](const auto & err) {
            return diagnostics::Reporter::REPORTER->normal(
              Location,
              "Type inference failed",
              "The type of this expression cannot be unified",
              "No more information",
              0x0005);
          })
          .unwrap();
      }

    return Ok(branchType);
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
    type::Type::Ptr constructorType =
      typeEnvironment.lookup(Constructor)
        .or_else([&](const auto & err) {
          return diagnostics::Reporter::REPORTER->normal(
            Location,
            std::format("{} was not declared", Constructor),
            "The definition of this constructor cannot be found in the context",
            "Constructor is undefined",
            0x0002);
        })
        .unwrap();

    std::for_each(Params.begin(), Params.end(), [&](const std::string & param) {
      auto * arrow = dynamic_cast<type::Arrow *>(constructorType.get());

      if (!arrow)
        diagnostics::Reporter::REPORTER->normal(
          Location,
          "Type mismatch",
          "This identifier is not a constructor",
          "Constructor must be a function type",
          0x0003);

      typeEnvironment.bind(param, arrow->Left);
      constructorType = arrow->Right;
    });

    typeManager.unify(type, constructorType)
      .or_else([&](const auto & err) {
        return diagnostics::Reporter::REPORTER->normal(
          Location,
          "Type inference failed",
          "The type of this expression cannot be unified",
          "No more information",
          0x0005);
      })
      .unwrap();

    if (const auto resultType =
          dynamic_cast<type::Base *>(constructorType.get());
        !resultType)
      diagnostics::Reporter::REPORTER->normal(
        Location,
        "Type inference failed",
        "Unable to infer return value type",
        "Constructor must be a function type",
        0x0005);
  }

  void Fn::scanDefinitionType(type::Manager & typeManager,
                              type::Environment & typeEnvironment) noexcept
  {
    ReturnType = typeManager.newType();
    type::Type::Ptr fullType = ReturnType;
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

    typeManager
      .unify(ReturnType,
             Body->typecheck(typeManager, newEnvironment)
               .or_else([&](const auto & err) {
                 return diagnostics::Reporter::REPORTER->normal(
                   Location,
                   "Type mismatch",
                   "This function has type {}, expect {}",
                   "Final type of function body does not match expected",
                   0x0003);
               })
               .unwrap())
      .or_else([&](const auto & err) {
        return diagnostics::Reporter::REPORTER->normal(
          Location,
          "Type inference failed",
          "The type of this expression cannot be unified",
          "No more information",
          0x0003);
      })
      .unwrap();
  }

  void Data::scanDefinitionType(type::Manager & typeManager,
                                type::Environment & typeEnvironment) noexcept
  {
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

} // namespace swallow::compiler::ast

namespace swallow::compiler::type
{
  void typecheck(const std::vector<ast::Definition::Ptr> & program) noexcept
  {
    Manager typeManager;
    Environment typeEnvironment;

    auto intType = Type::Ptr(new Base("Int"));
    auto binopType = Type::Ptr(
      new Arrow(intType, Type::Ptr(new type::Arrow(intType, intType))));

    typeEnvironment.bind("+", binopType);
    typeEnvironment.bind("-", binopType);
    typeEnvironment.bind("*", binopType);
    typeEnvironment.bind("/", binopType);

    for (const auto & definition : program)
      definition->scanDefinitionType(typeManager, typeEnvironment);

    for (const auto & definition : program)
      definition->typecheck(typeManager, typeEnvironment);
  }
} // namespace swallow::compiler::type