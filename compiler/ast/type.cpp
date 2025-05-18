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
    type::Manager &typeManager, const type::Environment &typeEnvironment) const
    -> type::Type::Ptr
  {
    return type::Type::Ptr(new type::Base("Int"));
  }

  auto LID::TypeCheck(
    type::Manager &typeManager, const type::Environment &typeEnvironment) const
    -> type::Type::Ptr
  {
    type::Type::Ptr type = typeEnvironment.Lookup(ID);

    if (nullptr == type)
      {
        diagnostics::Reporter::REPORTER->normal(
          Location,
          std::format("'{}' was not declared", ID),
          "The definition of this identifier cannot be found in the context",
          "Function or Variable is undefined",
          diagnostics::Code::LID_NOT_DECLARED);
      }

    return type;
  }

  auto UID::TypeCheck(
    type::Manager &typeManager, const type::Environment &typeEnvironment) const
    -> type::Type::Ptr
  {
    const auto type = typeEnvironment.Lookup(ID);

    if (nullptr == type)
      {
        diagnostics::Reporter::REPORTER->normal(
          Location,
          std::format("'{}' was not declared", ID),
          "The definition of this identifier cannot be found in the context",
          "Type or ConstructorName is undefined",
          diagnostics::UID_NOT_DECLARED);
      }

    return type;
  }

  auto Binop::TypeCheck(
    type::Manager &typeManager, const type::Environment &typeEnvironment) const
    -> type::Type::Ptr
  {
    const std::string operatorName = OperatorToString(Operator);

    type::Type::Ptr leftType =
      Left->CommonTypeCheck(typeManager, typeEnvironment);
    type::Type::Ptr rightType =
      Right->CommonTypeCheck(typeManager, typeEnvironment);

    type::Type::Ptr functionType = typeEnvironment.Lookup(operatorName);

    if (nullptr == functionType)
      {
        diagnostics::Reporter::REPORTER->normal(
          Location,
          std::format("'{}' was not declared", operatorName),
          "The definition of this operator cannot be found in the "
          "context",
          "Operator is undefined",
          diagnostics::BINOP_NOT_DECLARED);
      }

    type::Type::Ptr returnType = typeManager.NewType();
    type::Type::Ptr arrowOne =
      type::Type::Ptr(new type::Arrow(rightType, returnType));
    type::Type::Ptr arrowTwo =
      type::Type::Ptr(new type::Arrow(leftType, arrowOne));

    try
      {
        typeManager.Unify(arrowTwo, functionType);
      }
    catch (const std::runtime_error &e)
      {
        std::stringstream expected;
        std::stringstream actual;

        functionType->Dump(typeManager, expected);
        arrowOne->Dump(typeManager, actual);

        diagnostics::Reporter::REPORTER->normal(
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
      }

    return returnType;
  }

  auto Application::TypeCheck(
    type::Manager &typeManager, const type::Environment &typeEnvironment) const
    -> type::Type::Ptr
  {
    type::Type::Ptr leftType =
      Left->CommonTypeCheck(typeManager, typeEnvironment);
    type::Type::Ptr rightType =
      Right->CommonTypeCheck(typeManager, typeEnvironment);
    type::Type::Ptr returnType = typeManager.NewType();
    type::Type::Ptr arrowType =
      type::Type::Ptr(new type::Arrow(rightType, returnType));

    try
      {
        typeManager.Unify(arrowType, leftType);
      }
    catch (const std::runtime_error &e)
      {
        std::stringstream expected;
        std::stringstream actual;

        arrowType->Dump(typeManager, expected);
        leftType->Dump(typeManager, actual);

        diagnostics::Reporter::REPORTER->normal(
          Location,
          std::format("Type checking failed for function application"),
          std::format(
            "This expression has type '{}', but here may need a '{}'",
            actual.str(),
            expected.str()),
          std::format("'{}' conflicts with '{}'", expected.str(), actual.str()),
          diagnostics::APPLICATION_TYPE_MISMATCH);
      }

    return returnType;
  }

  auto Match::TypeCheck(
    type::Manager &typeManager, const type::Environment &typeEnvironment) const
    -> type::Type::Ptr
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

        try
          {
            typeManager.Unify(branchType, currentBranchType);
          }
        catch (const std::runtime_error &e)
          {
            std::stringstream expected;
            std::stringstream actual;

            branchType->Dump(typeManager, expected);
            currentBranchType->Dump(typeManager, actual);

            diagnostics::Reporter::REPORTER->normal(
              branch->Location,
              "Type checking failed for Match expression's branch",
              std::format(
                "This branch has type '{}', but here may need a '{}'",
                actual.str(),
                expected.str()),
              std::format(
                "'{}' conflicts with '{}'", expected.str(), actual.str()),
              diagnostics::MATCH_EXPR_BRANCHE_TYPE_CONFLICTS);
          }
      }

    matchType = typeManager.Resolve(matchType, var);
    if (nullptr == dynamic_cast<type::Data *>(matchType.get()))
      {
        diagnostics::Reporter::REPORTER->normal(
          Location,
          "Type checking failed for Match expression",
          "This is not a data type",
          "No more information",
          diagnostics::MATCH_NON_DATA_TYPE);
      }

    return branchType;
  }

  void VariablePattern::Match(
    type::Type::Ptr    type,
    type::Manager     &typeManager,
    type::Environment &typeEnvironment) const
  {
    typeManager.Bind(Variable, type);
  }

  void ConstructorPattern::Match(
    type::Type::Ptr    type,
    type::Manager     &typeManager,
    type::Environment &typeEnvironment) const
  {
    type::Type::Ptr constructorType = typeEnvironment.Lookup(ConstructorName);

    if (nullptr == constructorType)
      {
        diagnostics::Reporter::REPORTER->normal(
          Location,
          std::format("'{}' was not declared", ConstructorName),
          "The definition of this constructor cannot be found in the "
          "context",
          "Constructor is undefined",
          diagnostics::CONSTRUCTOR_NOT_DECLARED);
      }

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

    try
      {
        typeManager.Unify(type, constructorType);
      }
    catch (const std::runtime_error &e)
      {
        std::stringstream expected;
        std::stringstream actual;

        type->Dump(typeManager, expected);
        constructorType->Dump(typeManager, actual);

        diagnostics::Reporter::REPORTER->normal(
          Location,
          "Type checking failed for pattern",
          std::format(
            "This pattern has type '{}', but here may need a '{}'",
            actual.str(),
            expected.str()),
          std::format("'{}' conflicts with '{}'", expected.str(), actual.str()),
          diagnostics::PATTERN_MISMATCH);
      }
  }

  void Fn::PreScanTypes(
    type::Manager &typeManager, type::Environment &typeEnvironment)
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
    type::Manager &typeManager, const type::Environment &typeEnvironment) const
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

    try
      {
        typeManager.Unify(ReturnType, bodyType);
      }
    catch (const std::runtime_error &e)
      {
        std::stringstream expected;
        std::stringstream actual;

        ReturnType->Dump(typeManager, expected);
        bodyType->Dump(typeManager, actual);

        diagnostics::Reporter::REPORTER->normal(
          Location,
          "Type checking failed for function",
          std::format(
            "This function has type '{}', but the body has type '{}'",
            expected.str(),
            actual.str()),
          std::format("'{}' conflicts with '{}'", expected.str(), actual.str()),
          diagnostics::MATCH_EXPR_BRANCHE_TYPE_CONFLICTS);
      }
  }

  void Data::PreScanTypes(
    type::Manager &typeManager, type::Environment &typeEnvironment)
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
    type::Manager &typeManager, const type::Environment &typeEnvironment) const
  {}

  auto AST::CommonTypeCheck(
    type::Manager &typeManager, const type::Environment &typeEnvironment)
    -> type::Type::Ptr
  {
    try
      {
        NodeType = TypeCheck(typeManager, typeEnvironment);
      }
    catch (const std::runtime_error &e)
      {
        diagnostics::Reporter::REPORTER->normal(
          Location,
          "Type checking failed",
          "Wrong type here",
          std::format("No more information"),
          diagnostics::EXPR_TYPE_CHECKING_FAILED);
      }
    return NodeType;
  }

  void AST::CommonResolve(const type::Manager &typeManager)
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

  void Binop::Resolve(const type::Manager &typeManager)
  {
    Left->CommonResolve(typeManager);
    Right->CommonResolve(typeManager);
  }

  void Application::Resolve(const type::Manager &typeManager)
  {
    Left->CommonResolve(typeManager);
    Right->CommonResolve(typeManager);
  }

  void Match::Resolve(const type::Manager &typeManager)
  {
    With->CommonResolve(typeManager);

    for (const auto &branch : Branches)
      branch->Expr->CommonResolve(typeManager);
  }

  void Data::Resolve(const type::Manager &typeManager) {}

  void Fn::Resolve(const type::Manager &typeManager)
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

  void UID::Resolve(const type::Manager &typeManager) {}

  void LID::Resolve(const type::Manager &typeManager) {}

  void Int::Resolve(const type::Manager &typeManager) {}

} // namespace swallow::compiler::ast
