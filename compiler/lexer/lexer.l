%option noyywrap

%{

#include "ast.hpp"
#include "bison_parser.hpp"

#define YY_DECL yy::parser::symbol_type yylex()

%}

%%

[ \n]+  {}
\+      { return yy::parser::make_PLUS(); }
\*      { return yy::parser::make_TIMES(); }
-       { return yy::parser::make_MINUS(); }
\/      { return yy::parser::make_DIVIDE(); }
[0-9]+  { return yy::parser::make_INT(atoi(yytext)); }
let     { return yy::parser::make_FN(); }
data    { return yy::parser::make_DATA(); }
match   { return yy::parser::make_MATCH(); }
with    { return yy::parser::make_WITH(); }
\{      { return yy::parser::make_OCURLY(); }
\}      { return yy::parser::make_CCURLY(); }
\(      { return yy::parser::make_OPAREN(); }
\)      { return yy::parser::make_CPAREN(); }
\[      { return yy::parser::make_OBRACKET(); }
\]      { return yy::parser::make_CBRACKET(); }
,       { return yy::parser::make_COMMA(); }
->      { return yy::parser::make_ARROW(); }
\|      { return yy::parser::make_VERTIAL(); }
=>      { return yy::parser::make_DOUBLEARROW(); }
=       { return yy::parser::make_EQUAL(); }

[a-z][a-zA-Z]* { return yy::parser::make_LID(std::string(yytext)); }
[A-Z][a-zA-Z]* { return yy::parser::make_UID(std::string(yytext)); }

%%