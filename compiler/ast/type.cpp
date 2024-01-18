#include "type.h"
#include "ast.h"
#include "environment.h"
#include "panic/panic.hpp"
#include <algorithm>
#include <cstdlib>
#include <reporter.h>
#include <utils.h>
#include <vector>

using namespace swallow::utils;

namespace swallow::compiler::ast
{

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