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

#include "type/type.hpp"
#include "ast.hpp"
#include "diagnostics/code.hpp"
#include "diagnostics/reporter.hpp"
#include "type/environment.hpp"
#include <algorithm>
#include <format>
#include <sstream>
#include <vector>

using namespace swallow::compiler::utils;

namespace swallow::compiler::ast
{

  auto Int::TypeCheck(
    type::Manager &typeManager, const type::Environment &typeEnvironment)
    const noexcept -> Result<type::Type::Ptr, Void>
  {
    return Ok(type::Type::Ptr(new type::Base("Int")));
  }

  auto LID::TypeCheck(
    type::Manager &typeManager, const type::Environment &typeEnvironment)
    const noexcept -> Result<type::Type::Ptr, Void>
  {
    return typeEnvironment.Lookup(ID).or_else([&](const auto &err) {
      return diagnostics::Reporter::REPORTER->normal(
        Location,
        std::format("'{}' was not declared", ID),
        "The definition of this identifier cannot be found in the context",
        "Function or Variable is undefined",
        diagnostics::Code::LID_NOT_DECLARED);
    });
  }

  auto UID::TypeCheck(
    type::Manager &typeManager, const type::Environment &typeEnvironment)
    const noexcept -> Result<type::Type::Ptr, Void>
  {
    return typeEnvironment.Lookup(ID).or_else([&](const auto &err) {
      return diagnostics::Reporter::REPORTER->normal(
        Location,
        std::format("'{}' was not declared", ID),
        "The definition of this identifier cannot be found in the context",
        "Type or ConstructorName is undefined",
        diagnostics::UID_NOT_DECLARED);
    });
  }

  auto Binop::TypeCheck(
    type::Manager &typeManager, const type::Environment &typeEnvironment)
    const noexcept -> Result<type::Type::Ptr, Void>
  {
    const std::string operatorName = OperatorToString(Operator);

    type::Type::Ptr leftType =
      Left->CommonTypeCheck(typeManager, typeEnvironment);
    type::Type::Ptr rightType =
      Right->CommonTypeCheck(typeManager, typeEnvironment);

    type::Type::Ptr functionType =
      typeEnvironment.Lookup(operatorName)
        .or_else([&](const auto &err) {
          return diagnostics::Reporter::REPORTER->normal(
            Location,
            std::format("'{}' was not declared", operatorName),
            "The definition of this operator cannot be found in the "
            "context",
            "Operator is undefined",
            diagnostics::BINOP_NOT_DECLARED);
        })
        .unwrap();

    type::Type::Ptr returnType = typeManager.NewType();
    type::Type::Ptr arrowOne =
      type::Type::Ptr(new type::Arrow(rightType, returnType));
    type::Type::Ptr arrowTwo =
      type::Type::Ptr(new type::Arrow(leftType, arrowOne));

    typeManager.Unify(arrowTwo, functionType)
      .or_else([&](const auto &err) {
        std::stringstream expected;
        std::stringstream actual;

        functionType->Dump(typeManager, expected);
        arrowOne->Dump(typeManager, actual);

        return diagnostics::Reporter::REPORTER->normal(
          Location,
          std::format("Type checking failed for '{}'", operatorName),
          std::format(
            "The operator '{}' has type '{}', but here may need a "
            "'{}'",
            operatorName,
            expected.str(),
            actual.str()),
          std::format("'{}' conflicts with '{}'", expected.str(), actual.str()),
          diagnostics::BINOP_TYPE_MISMATCH);
      })
      .ignore();

    return Ok(returnType);
  }

  auto Application::TypeCheck(
    type::Manager &typeManager, const type::Environment &typeEnvironment)
    const noexcept -> Result<type::Type::Ptr, Void>
  {
    type::Type::Ptr leftType =
      Left->CommonTypeCheck(typeManager, typeEnvironment);
    type::Type::Ptr rightType =
      Right->CommonTypeCheck(typeManager, typeEnvironment);

    type::Type::Ptr returnType = typeManager.NewType();
    type::Type::Ptr arrowType =
      type::Type::Ptr(new type::Arrow(rightType, returnType));

    typeManager.Unify(arrowType, leftType)
      .or_else([&](const auto &err) {
        std::stringstream expected;
        std::stringstream actual;

        arrowType->Dump(typeManager, expected);
        leftType->Dump(typeManager, actual);

        return diagnostics::Reporter::REPORTER->normal(
          Location,
          std::format("Type checking failed for function application"),
          std::format(
            "This expression has type '{}', but here may need a '{}'",
            actual.str(),
            expected.str()),
          std::format("'{}' conflicts with '{}'", expected.str(), actual.str()),
          diagnostics::APPLICATION_TYPE_MISMATCH);
      })
      .ignore();

    return Ok(returnType);
  }

  auto Match::TypeCheck(
    type::Manager &typeManager, const type::Environment &typeEnvironment)
    const noexcept -> Result<type::Type::Ptr, Void>
  {
    type::Variable *var = nullptr;
    type::Type::Ptr matchType = typeManager.Resolve(
      With->CommonTypeCheck(typeManager, typeEnvironment), var);
    type::Type::Ptr branchType = typeManager.NewType();

    for (const auto &branch : Branches)
      {
        type::Environment newEnvironment = typeEnvironment.Scope();
        branch->Patt->Match(matchType, typeManager, newEnvironment);

        type::Type::Ptr currentBranchType =
          branch->Expr->CommonTypeCheck(typeManager, newEnvironment);

        typeManager.Unify(branchType, currentBranchType)
          .or_else([&](const auto &err) {
            std::stringstream expected;
            std::stringstream actual;

            branchType->Dump(typeManager, expected);
            currentBranchType->Dump(typeManager, actual);

            return diagnostics::Reporter::REPORTER->normal(
              branch->Location,
              "Type checking failed for Match expression's branch",
              std::format(
                "This branch has type '{}', but here may need a '{}'",
                actual.str(),
                expected.str()),
              std::format(
                "'{}' conflicts with '{}'", expected.str(), actual.str()),
              diagnostics::MATCH_EXPR_BRANCHE_TYPE_CONFLICTS);
          })
          .ignore();
      }

    matchType = typeManager.Resolve(matchType, var);
    if (nullptr == dynamic_cast<type::Data *>(matchType.get()))
      {
        return diagnostics::Reporter::REPORTER->normal(
          Location,
          "Type checking failed for Match expression",
          "This is not a data type",
          "No more information",
          diagnostics::MATCH_NON_DATA_TYPE);
      }

    return Ok(branchType);
  }

  void VariablePattern::Match(
    type::Type::Ptr    type,
    type::Manager     &typeManager,
    type::Environment &typeEnvironment) const noexcept
  {
    typeManager.Bind(Variable, type);
  }

  void ConstructorPattern::Match(
    type::Type::Ptr    type,
    type::Manager     &typeManager,
    type::Environment &typeEnvironment) const noexcept
  {
    type::Type::Ptr constructorType =
      typeEnvironment.Lookup(ConstructorName)
        .or_else([&](const auto &err) {
          return diagnostics::Reporter::REPORTER->normal(
            Location,
            std::format("'{}' was not declared", ConstructorName),
            "The definition of this constructor cannot be found in the "
            "context",
            "Constructor is undefined",
            diagnostics::CONSTRUCTOR_NOT_DECLARED);
        })
        .unwrap();

    std::for_each(Params.begin(), Params.end(), [&](const std::string &param) {
      auto *arrow = dynamic_cast<type::Arrow *>(constructorType.get());

      if (arrow == nullptr)
        {
          diagnostics::Reporter::REPORTER->normal(
            Location,
            "Illegal Pattern",
            "This identifier is not a constructor",
            "Constructor must be a function type",
            diagnostics::PATTERN_CONSTRUCTOR_IS_NOT_FUNCTION);
        }

      typeEnvironment.Bind(param, arrow->Left);
      constructorType = arrow->Right;
    });

    typeManager.Unify(type, constructorType)
      .or_else([&](const auto &err) {
        std::stringstream expected;
        std::stringstream actual;

        type->Dump(typeManager, expected);
        constructorType->Dump(typeManager, actual);

        return diagnostics::Reporter::REPORTER->normal(
          Location,
          "Type checking failed for pattern",
          std::format(
            "This pattern has type '{}', but here may need a '{}'",
            actual.str(),
            expected.str()),
          std::format("'{}' conflicts with '{}'", expected.str(), actual.str()),
          diagnostics::PATTERN_MISMATCH);
      })
      .ignore();
  }

  void Fn::PreScanTypes(
    type::Manager &typeManager, type::Environment &typeEnvironment) noexcept
  {
    ReturnType = typeManager.NewType();
    type::Type::Ptr fullType = ReturnType;

    std::for_each(
      Params.rbegin(), Params.rend(), [&](const std::string &param) {
        type::Type::Ptr paramType = typeManager.NewType();
        fullType = type::Type::Ptr(new type::Arrow(paramType, fullType));
        ParamTypes.push_back(paramType);
      });

    typeEnvironment.Bind(Name, fullType);
  }

  void Fn::TypeCheck(
    type::Manager           &typeManager,
    const type::Environment &typeEnvironment) const noexcept
  {
    type::Environment newEnvironment = typeEnvironment.Scope();

    auto paramIterator = Params.begin();
    auto typeIterator = ParamTypes.rbegin();
    while (paramIterator != Params.end() && typeIterator != ParamTypes.rend())
      {
        newEnvironment.Bind(*paramIterator, *typeIterator);
        paramIterator++;
        typeIterator++;
      }

    auto bodyType = Body->CommonTypeCheck(typeManager, newEnvironment);

    typeManager.Unify(ReturnType, bodyType)
      .or_else([&](const auto &err) {
        std::stringstream expected;
        std::stringstream actual;

        ReturnType->Dump(typeManager, expected);
        bodyType->Dump(typeManager, actual);

        return diagnostics::Reporter::REPORTER->normal(
          Location,
          "Type checking failed for function",
          std::format(
            "This function has type '{}', but the body has type '{}'",
            expected.str(),
            actual.str()),
          std::format("'{}' conflicts with '{}'", expected.str(), actual.str()),
          diagnostics::MATCH_EXPR_BRANCHE_TYPE_CONFLICTS);
      })
      .ignore();
  }

  void Data::PreScanTypes(
    type::Manager &typeManager, type::Environment &typeEnvironment) noexcept
  {
    auto   *thisType = new type::Data(Name);
    auto    returnType = type::Type::Ptr(thisType);
    uint8_t nextTag = 0;

    for (const auto &constructor : Constructors)
      {
        constructor->Tag = nextTag;
        thisType->Constructors[constructor->Name] = {nextTag++};

        auto fullType = returnType;
        std::for_each(
          constructor->Types.rbegin(),
          constructor->Types.rend(),
          [&](const auto &typeName) {
            fullType = type::Type::Ptr(new type::Arrow(
              type::Type::Ptr(new type::Base(typeName)), fullType));
          });

        typeEnvironment.Bind(constructor->Name, fullType);
      }
  }

  void Data::TypeCheck(
    type::Manager           &typeManager,
    const type::Environment &typeEnvironment) const noexcept
  {}

  auto AST::CommonTypeCheck(
    type::Manager           &typeManager,
    const type::Environment &typeEnvironment) noexcept -> type::Type::Ptr
  {
    NodeType = TypeCheck(typeManager, typeEnvironment)
                 .or_else([&](const auto &err) {
                   return diagnostics::Reporter::REPORTER->normal(
                     Location,
                     "Type checking failed",
                     "Wrong type here",
                     std::format("No more information"),
                     diagnostics::EXPR_TYPE_CHECKING_FAILED);
                 })
                 .unwrap();
    ;
    return NodeType;
  }

  void AST::CommonResolve(const type::Manager &typeManager) noexcept
  {
    type::Variable *variable;
    type::Type::Ptr resolveType = typeManager.Resolve(NodeType, variable);

    if (nullptr != variable)
      diagnostics::Reporter::REPORTER->normal(
        Location,
        "Ambiguously type here",
        "Ambiguously typed program",
        "No more information",
        diagnostics::AMBIGUOUSLY_TYPE);

    Resolve(typeManager);
    NodeType = std::move(resolveType);
  }

  void Binop::Resolve(const type::Manager &typeManager) noexcept
  {
    Left->CommonResolve(typeManager);
    Right->CommonResolve(typeManager);
  }

  void Application::Resolve(const type::Manager &typeManager) noexcept
  {
    Left->CommonResolve(typeManager);
    Right->CommonResolve(typeManager);
  }

  void Match::Resolve(const type::Manager &typeManager) noexcept
  {
    With->CommonResolve(typeManager);

    for (const auto &branch : Branches)
      branch->Expr->CommonResolve(typeManager);
  }

  void Data::Resolve(const type::Manager &typeManager) noexcept {}

  void Fn::Resolve(const type::Manager &typeManager) noexcept
  {
    type::Variable *var = nullptr;
    Body->CommonResolve(typeManager);

    ReturnType = typeManager.Resolve(ReturnType, var);
    if (nullptr != var)
      diagnostics::Reporter::REPORTER->normal(
        Location,
        "Ambiguously type here",
        "Ambiguously typed program",
        "No more information",
        diagnostics::AMBIGUOUSLY_TYPE);

    for (auto &paramType : ParamTypes)
      {
        paramType = typeManager.Resolve(paramType, var);

        if (nullptr != var)
          diagnostics::Reporter::REPORTER->normal(
            Location,
            "Ambiguously type here",
            "Ambiguously typed program",
            "No more information",
            diagnostics::AMBIGUOUSLY_TYPE);
      }
  }

  void UID::Resolve(const type::Manager &typeManager) noexcept {}

  void LID::Resolve(const type::Manager &typeManager) noexcept {}

  void Int::Resolve(const type::Manager &typeManager) noexcept {}

} // namespace swallow::compiler::ast
