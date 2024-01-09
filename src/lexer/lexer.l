%option noyywrap

%{
#include <iostream>
%}

%%

[ \n]+ {}
\+ { std::cout << "PLUS" << std::endl; }
\* { std::cout << "TIMES" << std::endl; }
- { std::cout << "MINUS" << std::endl; }
\/ { std::cout << "DIVIDE" << std::endl; }
[0-9]+ { std::cout << "NUMBER: " << yytext << std::endl; }
Fn { std::cout << "KEYWORD: defn" << std::endl; }
Data { std::cout << "KEYWORD: data" << std::endl; }
Match { std::cout << "KEYWORD: case" << std::endl; }
of { std::cout << "KEYWORD: of" << std::endl; }
\{ { std::cout << "OPEN CURLY" << std::endl; }
\} { std::cout << "CLOSED CURLY" << std::endl; }
\( { std::cout << "OPEN PARENTH" << std::endl; }
\) { std::cout << "CLOSE PARENTH" << std::endl; }
,  { std::cout << "COMMA" << std::endl; }
-> { std::cout << "PATTERN ARROW" << std::endl; }
= { std::cout << "EQUAL" << std::endl; }
[a-z][a-zA-Z]* { std::cout << "LOWERCASE IDENTIFIER: " << yytext << std::endl; }
[A-Z][a-zA-Z]* { std::cout << "UPPERCASE IDENTIFIER: " << yytext << std::endl; }

%%

int main() {
    yylex();
}
