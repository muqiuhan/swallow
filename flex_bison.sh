flex -o $(scriptdir)/lexer/flex_lexer.cpp $(scriptdir)/lexer/lexer.l
bison -o $(scriptdir)/parser/bison_parser.cpp -d $(scriptdir)/parser/parser.y