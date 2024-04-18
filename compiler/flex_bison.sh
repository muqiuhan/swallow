flex -o ./lexer/flex_lexer.cpp ./lexer/lexer.l
bison -o ./parser/bison_parser.cpp -d ./parser/parser.y