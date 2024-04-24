%option noyywrap

%{

#include "ast/ast.hpp"
#include "bison_parser.hpp"

#define YY_DECL yy::parser::symbol_type yylex()

static yy::location LOC;

#define YY_USER_ACTION LOC.step(); LOC.columns(yyleng); 

%}

%option yylineno

%%

[ \n]+  {  }
\+      { return yy::parser::make_PLUS(LOC); }
\*      { return yy::parser::make_TIMES(LOC); }
-       { return yy::parser::make_MINUS(LOC); }
\/      { return yy::parser::make_DIVIDE(LOC); }
[0-9]+  { return yy::parser::make_INT(atoi(yytext), LOC); }
let     { return yy::parser::make_FN(LOC); }
data    { return yy::parser::make_DATA(LOC); }
match   { return yy::parser::make_MATCH(LOC); }
with    { return yy::parser::make_WITH(LOC); }
\{      { return yy::parser::make_OCURLY(LOC); }
\}      { return yy::parser::make_CCURLY(LOC); }
\(      { return yy::parser::make_OPAREN(LOC); }
\)      { return yy::parser::make_CPAREN(LOC); }
\[      { return yy::parser::make_OBRACKET(LOC); }
\]      { return yy::parser::make_CBRACKET(LOC); }
,       { return yy::parser::make_COMMA(LOC); }
;       { return yy::parser::make_SEMICOLON(LOC); }
->      { return yy::parser::make_ARROW(LOC); }
\|      { return yy::parser::make_VERTIAL(LOC); }
=>      { return yy::parser::make_DOUBLEARROW(LOC); }
=       { return yy::parser::make_EQUAL(LOC); }

[a-z][a-zA-Z]* { return yy::parser::make_LID(std::string(yytext), LOC); }
[A-Z][a-zA-Z]* { return yy::parser::make_UID(std::string(yytext), LOC); }

<<EOF>> { return yy::parser::make_END(LOC); }

%%