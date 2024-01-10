#include <memory>
#include <vector>

namespace swallow::ast {
class AST {
public:
  using Ptr = std::unique_ptr<AST>;

public:
  virtual ~AST() = default;
};

class Pattern {
public:
  using Ptr = std::unique_ptr<Pattern>;

public:
  virtual ~Pattern() = default;
};

class Branch {
public:
  using Ptr = std::unique_ptr<Branch>;

private:
  const Pattern::Ptr Pattern;
  const AST::Ptr Expr;

public:
  Branch(Pattern::Ptr Pattern, AST::Ptr Expr)
      : Pattern(std::move(Pattern)), Expr(std::move(Expr)) {}
};

class Conclassor {
public:
  using Ptr = std::unique_ptr<Conclassor>;

private:
  const std::string Name;
  const std::vector<std::string> Types;

public:
  Conclassor(std::string Name, std::vector<std::string> Types)
      : Name(std::move(Name)), Types(std::move(Types)) {}
};

class Definition {
public:
  using Ptr = std::unique_ptr<Definition>;

public:
  virtual ~Definition() = default;
};

class Int : public AST {
private:
  const int Value;

public:
  explicit Int(int V) : Value(V) {}
};

class Lid : public AST {
private:
  const std::string ID;

public:
  explicit Lid(std::string ID) : ID(std::move(ID)) {}
};

class Uid : public AST {
private:
  const std::string ID;

public:
  explicit Uid(std::string ID) : ID(std::move(ID)) {}
};

class Binop : public AST {
public:
  enum class Operators { PLUS, MINUS, TIMES, DIVIDE };

private:
  const Operators Operator;
  const AST::Ptr Left;
  const AST::Ptr Right;

public:
  Binop(Operators Operator, AST::Ptr Left, AST::Ptr Right)
      : Operator(Operator), Left(std::move(Left)), Right(std::move(Right)) {}
};

class Application : public AST {
private:
  const AST::Ptr left;
  const AST::Ptr right;

public:
  Application(AST::Ptr Left, AST::Ptr Right)
      : left(std::move(Left)), right(std::move(Right)) {}
};

class Match : public AST {
private:
  const AST::Ptr With;
  const std::vector<Branch::Ptr> Branches;

public:
  Match(AST::Ptr o, std::vector<Branch::Ptr> b)
      : With(std::move(o)), Branches(std::move(b)) {}
};

class PatternVariable : public Pattern {
private:
  const std::string Variable;

public:
  PatternVariable(std::string Variable) : Variable(std::move(Variable)) {}
};

class PatternConstr : public Pattern {
private:
  const std::string Constr;
  const std::vector<std::string> Params;

public:
  PatternConstr(std::string Constr, std::vector<std::string> Params)
      : Constr(std::move(Constr)), Params(std::move(Params)) {}
};

class Let : public Definition {
private:
  const std::string Name;
  const std::vector<std::string> Params;
  const AST::Ptr Body;

public:
  Let(std::string Name, std::vector<std::string> Params, AST::Ptr Body)
      : Name(std::move(Name)), Params(std::move(Params)),
        Body(std::move(Body)) {}
};

class Data : public Definition {
private:
  const std::string Name;
  const std::vector<Conclassor::Ptr> Conclassors;

public:
  Data(std::string Name, std::vector<Conclassor::Ptr> Conclassors)
      : Name(std::move(Name)), Conclassors(std::move(Conclassors)) {}
};
} // namespace swallow::ast