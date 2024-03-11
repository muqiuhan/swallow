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

#include "ast.hpp"
#include "code.hpp"
#include "diagnostics/reporter.hpp"
#include "type/environment.hpp"
#include <algorithm>
#include <format>
#include <sstream>
#include <utils.h>
#include <vector>

using namespace swallow::compiler::utils;

namespace swallow::compiler::ast
{

  auto Int::typecheck(type::Manager &typeManager,
                      const type::Environment &typeEnvironment) const noexcept
    -> Result<type::Type::Ptr, Void>
  {
    return Ok(type::Type::Ptr(new type::Base("Int")));
  }

  auto LID::typecheck(type::Manager &typeManager,
                      const type::Environment &typeEnvironment) const noexcept
    -> Result<type::Type::Ptr, Void>
  {
    return typeEnvironment.lookup(ID).or_else([&](const auto &err) {
      return diagnostics::Reporter::REPORTER->normal(
        Location, std::format("'{}' was not declared", ID),
        "The definition of this identifier cannot be found in the context",
        "Function or Variable is undefined",
        diagnostics::Code::LID_NOT_DECLARED);
    });
  }

  auto UID::typecheck(type::Manager &typeManager,
                      const type::Environment &typeEnvironment) const noexcept
    -> Result<type::Type::Ptr, Void>
  {
    return typeEnvironment.lookup(ID).or_else([&](const auto &err) {
      return diagnostics::Reporter::REPORTER->normal(
        Location, std::format("'{}' was not declared", ID),
        "The definition of this identifier cannot be found in the context",
        "Type or Constructor is undefined", diagnostics::UID_NOT_DECLARED);
    });
  }

  auto Binop::typecheck(type::Manager &typeManager,
                        const type::Environment &typeEnvironment) const noexcept
    -> Result<type::Type::Ptr, Void>
  {
    const std::string operatorName = operatorsToString(Operator);

    type::Type::Ptr leftType =
      Left->typecheck(typeManager, typeEnvironment)
        .or_else([&](const auto &err) {
          return diagnostics::Reporter::REPORTER->normal(
            Left->Location, "Type checking failed", "Wrong type here",
            std::format("No more information"),
            diagnostics::EXPR_TYPE_CHECKING_FAILED);
        })
        .unwrap();

    type::Type::Ptr rightType =
      Right->typecheck(typeManager, typeEnvironment)
        .or_else([&](const auto &err) {
          return diagnostics::Reporter::REPORTER->normal(
            Left->Location, "Type checking failed", "Wrong type here",
            std::format("No more information"),
            diagnostics::EXPR_TYPE_CHECKING_FAILED);
        })
        .unwrap();

    type::Type::Ptr functionType =
      typeEnvironment.lookup(operatorName)
        .or_else([&](const auto &err) {
          return diagnostics::Reporter::REPORTER->normal(
            Location, std::format("'{}' was not declared", operatorName),
            "The definition of this operator cannot be found in the "
            "context",
            "Operator is undefined", diagnostics::BINOP_NOT_DECLARED);
        })
        .unwrap();

    type::Type::Ptr returnType = typeManager.newType();
    type::Type::Ptr arrowOne =
      type::Type::Ptr(new type::Arrow(rightType, returnType));
    type::Type::Ptr arrowTwo =
      type::Type::Ptr(new type::Arrow(leftType, arrowOne));

    typeManager.unify(arrowTwo, functionType)
      .or_else([&](const auto &err) {
        std::stringstream expected;
        std::stringstream actual;

        functionType->dump(typeManager, expected);
        arrowOne->dump(typeManager, actual);

        return diagnostics::Reporter::REPORTER->normal(
          Location,
          std::format("Type checking failed for '{}'", operatorName),
          std::format(
            "The operator '{}' has type '{}', but here may need a '{}'",
            operatorName, expected.str(), actual.str()),
          std::format("'{}' conflicts with '{}'", expected.str(), actual.str()),
          diagnostics::BINOP_TYPE_MISMATCH);
      })
      .ignore();

    return Ok(returnType);
  }

  auto Application::typecheck(type::Manager &typeManager,
                              const type::Environment &typeEnvironment)
    const noexcept -> Result<type::Type::Ptr, Void>
  {
    type::Type::Ptr leftType =
      Left->typecheck(typeManager, typeEnvironment)
        .or_else([&](const auto &err) {
          return diagnostics::Reporter::REPORTER->normal(
            Left->Location, "Type checking failed", "Wrong type here",
            std::format("No more information"),
            diagnostics::EXPR_TYPE_CHECKING_FAILED);
        })
        .unwrap();

    type::Type::Ptr rightType =
      Right->typecheck(typeManager, typeEnvironment)
        .or_else([&](const auto &err) {
          return diagnostics::Reporter::REPORTER->normal(
            Left->Location, "Type checking failed", "Wrong type here",
            std::format("No more information"),
            diagnostics::EXPR_TYPE_CHECKING_FAILED);
        })
        .unwrap();

    type::Type::Ptr returnType = typeManager.newType();
    type::Type::Ptr arrowType =
      type::Type::Ptr(new type::Arrow(rightType, returnType));

    typeManager.unify(arrowType, leftType)
      .or_else([&](const auto &err) {
        std::stringstream expected;
        std::stringstream actual;

        arrowType->dump(typeManager, expected);
        leftType->dump(typeManager, actual);

        return diagnostics::Reporter::REPORTER->normal(
          Location,
          std::format("Type checking failed for function application"),
          std::format("This expression has type '{}', but here may need a '{}'",
                      actual.str(), expected.str()),
          std::format("'{}' conflicts with '{}'", expected.str(), actual.str()),
          diagnostics::APPLICATION_TYPE_MISMATCH);
      })
      .ignore();

    return Ok(returnType);
  }

  auto Match::typecheck(type::Manager &typeManager,
                        const type::Environment &typeEnvironment) const noexcept
    -> Result<type::Type::Ptr, Void>
  {
    type::Variable *var = nullptr;
    type::Type::Ptr matchType = typeManager.resolve(
      With->typecheck(typeManager, typeEnvironment)
        .or_else([&](const auto &err) {
          return diagnostics::Reporter::REPORTER->normal(
            With->Location, "Type checking failed", "Wrong type here",
            std::format("No more information"),
            diagnostics::MATCH_EXPR_TYPE_CHECKING_FAILED);
        })
        .unwrap(),
      var);

    type::Type::Ptr branchType = typeManager.newType();

    for (const auto &branch : Branches)
      {
        type::Environment newEnvironment = typeEnvironment.scope();
        branch->Patt->match(matchType, typeManager, newEnvironment);

        type::Type::Ptr currentBranchType =
          branch->Expr->typecheck(typeManager, newEnvironment)
            .or_else([&](const auto &err) {
              return diagnostics::Reporter::REPORTER->normal(
                branch->Location, "Type checking failed", "Wrong type here",
                "No more information",
                diagnostics::MATCH_EXPR_CURRENT_BRANCHE_TYPE_CHECKING_FAILED);
            })
            .unwrap();

        typeManager.unify(branchType, currentBranchType)
          .or_else([&](const auto &err) {
            std::stringstream expected;
            std::stringstream actual;

            branchType->dump(typeManager, expected);
            currentBranchType->dump(typeManager, actual);

            return diagnostics::Reporter::REPORTER->normal(
              branch->Location,
              "Type checking failed for match expression's branch",
              std::format("This branch has type '{}', but here may need a '{}'",
                          actual.str(), expected.str()),
              std::format("'{}' conflicts with '{}'", expected.str(),
                          actual.str()),
              diagnostics::MATCH_EXPR_BRANCHE_TYPE_CONFLICTS);
          })
          .ignore();
      }

    matchType = typeManager.resolve(matchType, var);
    if (nullptr == dynamic_cast<type::Base *>(matchType.get()))
      {
        return diagnostics::Reporter::REPORTER->normal(
          Location, "Type checking failed for match expression",
          "This is not a data type", "No more information",
          diagnostics::MATCH_NON_DATA_TYPE);
      }

    return Ok(branchType);
  }

  void PatternVariable::match(type::Type::Ptr type, type::Manager &typeManager,
                              type::Environment &typeEnvironment) const noexcept
  {
    typeManager.bind(Variable, type);
  }

  void
    PatternConstructor::match(type::Type::Ptr type, type::Manager &typeManager,
                              type::Environment &typeEnvironment) const noexcept
  {
    type::Type::Ptr constructorType =
      typeEnvironment.lookup(Constructor)
        .or_else([&](const auto &err) {
          return diagnostics::Reporter::REPORTER->normal(
            Location, std::format("'{}' was not declared", Constructor),
            "The definition of this constructor cannot be found in the "
            "context",
            "Constructor is undefined", diagnostics::CONSTRUCTOR_NOT_DECLARED);
        })
        .unwrap();

    std::for_each(Params.begin(), Params.end(), [&](const std::string &param) {
      auto *arrow = dynamic_cast<type::Arrow *>(constructorType.get());

      if (arrow == nullptr)
        {
          diagnostics::Reporter::REPORTER->normal(
            Location, "Illegal Pattern", "This identifier is not a constructor",
            "Constructor must be a function type",
            diagnostics::PATTERN_CONSTRUCTOR_IS_NOT_FUNCTION);
        }

      typeEnvironment.bind(param, arrow->Left);
      constructorType = arrow->Right;
    });

    typeManager.unify(type, constructorType)
      .or_else([&](const auto &err) {
        std::stringstream expected;
        std::stringstream actual;

        type->dump(typeManager, expected);
        constructorType->dump(typeManager, actual);

        return diagnostics::Reporter::REPORTER->normal(
          Location, "Type checking failed for pattern",
          std::format("This pattern has type '{}', but here may need a '{}'",
                      actual.str(), expected.str()),
          std::format("'{}' conflicts with '{}'", expected.str(), actual.str()),
          diagnostics::PATTERN_MISMATCH);
      })
      .ignore();

    if (auto *const resultType =
          dynamic_cast<type::Base *>(constructorType.get());
        resultType == nullptr)
      {
        diagnostics::Reporter::REPORTER->normal(
          Location, "Illegal return type", "Unable to return this value",
          "No more information", diagnostics::UNKNOWN);
      }
  }

  void Fn::scanDefinitionType(type::Manager &typeManager,
                              type::Environment &typeEnvironment) noexcept
  {
    ReturnType = typeManager.newType();
    type::Type::Ptr fullType = ReturnType;
    std::for_each(
      Params.rbegin(), Params.rend(), [&](const std::string &param) {
        type::Type::Ptr paramType = typeManager.newType();
        fullType = type::Type::Ptr(new type::Arrow(paramType, fullType));
        ParamTypes.push_back(paramType);
      });

    typeEnvironment.bind(Name, fullType);
  }

  void Fn::typecheck(type::Manager &typeManager,
                     const type::Environment &typeEnvironment) const noexcept
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

    auto bodyType = Body->typecheck(typeManager, newEnvironment)
                      .or_else([&](const auto &err) {
                        return diagnostics::Reporter::REPORTER->normal(
                          Location, "Type checking failed for function body",
                          "Wrong type here", "No more information",
                          diagnostics::FUNCTION_BODY_TYPE_CHECKING_FAILED);
                      })
                      .unwrap();

    typeManager.unify(ReturnType, bodyType)
      .or_else([&](const auto &err) {
        std::stringstream expected;
        std::stringstream actual;

        ReturnType->dump(typeManager, expected);
        bodyType->dump(typeManager, actual);

        return diagnostics::Reporter::REPORTER->normal(
          Location, "Type checking failed for function",
          std::format("This function has type '{}', but the body has type '{}'",
                      expected.str(), actual.str()),
          std::format("'{}' conflicts with '{}'", expected.str(), actual.str()),
          diagnostics::MATCH_EXPR_BRANCHE_TYPE_CONFLICTS);
      })
      .ignore();
  }

  void Data::scanDefinitionType(type::Manager &typeManager,
                                type::Environment &typeEnvironment) noexcept
  {
    auto fullType = type::Type::Ptr(new type::Base(Name));
    for (const auto &constructor : Constructors)
      {
        std::for_each(constructor->Types.rbegin(), constructor->Types.rend(),
                      [&](const auto &typeName) {
                        fullType = type::Type::Ptr(new type::Arrow(
                          type::Type::Ptr(new type::Base(typeName)), fullType));
                      });

        typeEnvironment.bind(constructor->Name, fullType);
      }
  }

  void Data::typecheck(type::Manager &typeManager,
                       const type::Environment &typeEnvironment) const noexcept
  {}

} // namespace swallow::compiler::ast
