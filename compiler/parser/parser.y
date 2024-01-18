%{
#include <string>
#include <iostream>
#include "ast.h"
#include "bison_parser.hpp"
#include "reporter.h"

std::vector<swallow::compiler::ast::Definition::Ptr> Program;
extern yy::parser::symbol_type yylex();

%}

%code requires {
    #include "ast.h"
    using namespace swallow::compiler::ast;
}

%token PLUS
%token TIMES
%token MINUS
%token DIVIDE
%token <int> INT
%token FN
%token DATA
%token MATCH
%token WITH
%token OCURLY
%token CCURLY
%token OPAREN
%token CPAREN
%token OBRACKET
%token CBRACKET
%token COMMA
%token ARROW
%token VERTIAL
%token DOUBLEARROW
%token EQUAL
%token <std::string> LID
%token <std::string> UID
%token END 0 "end of file"

%language "c++"
%define api.value.type variant
%define api.token.constructor
%locations

%type <std::vector<std::string>> LowercaseParams UppercaseParams
%type <std::vector<Definition::Ptr>> Program Definitions
%type <std::vector<Branch::Ptr>> Branches
%type <std::vector<Constructor::Ptr>> Constructors
%type <AST::Ptr> Add Mul Match Application ApplicationBase
%type <Definition::Ptr> Definition Fn Data 
%type <Branch::Ptr> Branch
%type <Pattern::Ptr> Pattern
%type <Constructor::Ptr> Constructor

%start Program

%%

Program
    : Definitions { Program = std::move($1); }
    ;

Definitions
    : Definitions Definition { $$ = std::move($1); $$.push_back(std::move($2)); }
    | Definition { $$ = std::vector<Definition::Ptr>(); $$.push_back(std::move($1)); }
    ;

Definition
    : Fn { $$ = std::move($1); }
    | Data { $$ = std::move($1); }
    ;

Fn
    : FN LID LowercaseParams EQUAL OCURLY Add CCURLY
        { $$ = Definition::Ptr(new Fn(@$, std::move($2), std::move($3), std::move($6))); }
    | error {
        swallow::compiler::diagnostics::Reporter::REPORTER->normal(
            @$, "Syntax parsing error", "The function definition syntax error",
            "FN LID LowercaseParams EQUAL OCURLY Add CCURLY",
            0x0001
        );
    }
    ;

LowercaseParams
    : %empty { $$ = std::vector<std::string>(); }
    | LowercaseParams LID { $$ = std::move($1); $$.push_back(std::move($2)); }
    ;

UppercaseParams
    : %empty { $$ = std::vector<std::string>(); }
    | UppercaseParams UID { $$ = std::move($1); $$.push_back(std::move($2)); }
    ;

Add
    : Add PLUS Mul { $$ = AST::Ptr(new Binop(@$, Binop::Operators::PLUS, std::move($1), std::move($3))); }
    | Add MINUS Mul { $$ = AST::Ptr(new Binop(@$, Binop::Operators::MINUS, std::move($1), std::move($3))); }
    | Mul { $$ = std::move($1); }
    ;

Mul
    : Mul TIMES Application { $$ = AST::Ptr(new Binop(@$, Binop::Operators::TIMES, std::move($1), std::move($3))); }
    | Mul DIVIDE Application { $$ = AST::Ptr(new Binop(@$, Binop::Operators::DIVIDE, std::move($1), std::move($3))); }
    | Application { $$ = std::move($1); }
    ;

Application
    : Application ApplicationBase { $$ = AST::Ptr(new Application(@$, std::move($1), std::move($2))); }
    | ApplicationBase { $$ = std::move($1); }
    ;

ApplicationBase
    : INT { $$ = AST::Ptr(new Int(@$, $1)); }
    | LID { $$ = AST::Ptr(new LID(@$, std::move($1))); }
    | UID { $$ = AST::Ptr(new UID(@$, std::move($1))); }
    | OPAREN Add CPAREN { $$ = std::move($2); }
    | Match { $$ = std::move($1); }
    ;

Match
    : MATCH Add WITH OCURLY Branches CCURLY
        { $$ = AST::Ptr(new Match(@$, std::move($2), std::move($5))); }
    ;

Branches
    : Branches Branch { $$ = std::move($1); $1.push_back(std::move($2)); }
    | Branch { $$ = std::vector<Branch::Ptr>(); $$.push_back(std::move($1));}
    ;

Branch
    : VERTIAL Pattern DOUBLEARROW OCURLY Add CCURLY
        {
            $$ = Branch::Ptr(new Branch(@$, std::move($2), std::move($5)));
        }
    | error {
        swallow::compiler::diagnostics::Reporter::REPORTER->normal(
            @$, "Syntax parsing error", "Branch syntax error in match expression",
            "VERTIAL Pattern DOUBLEARROW OCURLY Add CCURLY",
            0x0001
        );
    }
    ;

Pattern
    : LID { $$ = Pattern::Ptr(new PatternVariable(@$, std::move($1))); }
    | UID LowercaseParams
        { $$ = Pattern::Ptr(new PatternConstructor(@$, std::move($1), std::move($2))); }
    ;

Data
    : DATA UID EQUAL OBRACKET Constructors CBRACKET
        { $$ = Definition::Ptr(new Data(@$, std::move($2), std::move($5))); }
    ;

Constructors
    : Constructors COMMA Constructor { $$ = std::move($1); $$.push_back(std::move($3)); }
    | Constructor
        { $$ = std::vector<Constructor::Ptr>(); $$.push_back(std::move($1)); }
    ;

Constructor
    : UID UppercaseParams
        { $$ = Constructor::Ptr(new Constructor(@$, std::move($1), std::move($2))); }
    ;