# this is the build file for project swallow
# it is autogenerated by the xmake build system.
# do not edit by hand.

ifneq ($(VERBOSE),1)
VV=@
endif

CXX=/usr/bin/gcc
CU=/usr/bin/clang
MXX=/usr/bin/gcc
CC=/usr/bin/gcc
MM=/usr/bin/gcc
RC=/home/muqiu/.cargo/bin/rustc
AS=/usr/bin/gcc

LD=/usr/bin/g++
RCLD=/home/muqiu/.cargo/bin/rustc
SH=/usr/bin/g++
RCSH=/home/muqiu/.cargo/bin/rustc
AR=/usr/bin/ar
RCAR=/home/muqiu/.cargo/bin/rustc

stdlib_SH=/usr/bin/g++
stdlib_CXX=/usr/bin/gcc
stdlib_CXX=/usr/bin/gcc
swi_LD=/usr/bin/g++
swi_CXX=/usr/bin/gcc
swi_CXX=/usr/bin/gcc
swc_LD=/usr/bin/g++
swc_CXX=/usr/bin/gcc
swc_CXX=/usr/bin/gcc

stdlib_CXXFLAGS=-m64 -fPIC -g -O0 -std=c++20
stdlib_CXXFLAGS=-m64 -fPIC -g -O0 -std=c++20
stdlib_SHFLAGS=-shared -m64 -fPIC
swi_CXXFLAGS=-m64 -g -O0 -std=c++20
swi_CXXFLAGS=-m64 -g -O0 -std=c++20
swi_LDFLAGS=-m64 -Lbuild/linux/x86_64/debug -Wl,-rpath=$ORIGIN -lstdlib
swc_CXXFLAGS=-m64 -g -O0 -std=c++20 -Icompiler -Icompiler/ast -Icompiler/type -Icompiler/lexer -Icompiler/parser -Icompiler/diagnostics -Icompiler/utils
swc_CXXFLAGS=-m64 -g -O0 -std=c++20 -Icompiler -Icompiler/ast -Icompiler/type -Icompiler/lexer -Icompiler/parser -Icompiler/diagnostics -Icompiler/utils
swc_LDFLAGS=-m64 -Lbuild/linux/x86_64/debug -Wl,-rpath=$ORIGIN -lc++ -lstdlib

default:  stdlib swi swc

all:  stdlib swi swc

.PHONY: default all  stdlib swi swc

stdlib: build/linux/x86_64/debug/libstdlib.so
build/linux/x86_64/debug/libstdlib.so: build/.objs/stdlib/linux/x86_64/debug/stdlib/stdlib.cpp.o
	@echo linking.debug libstdlib.so
	@mkdir -p build/linux/x86_64/debug
	$(VV)$(stdlib_SH) -o build/linux/x86_64/debug/libstdlib.so build/.objs/stdlib/linux/x86_64/debug/stdlib/stdlib.cpp.o $(stdlib_SHFLAGS)

build/.objs/stdlib/linux/x86_64/debug/stdlib/stdlib.cpp.o: stdlib/stdlib.cpp
	@echo compiling.debug stdlib/stdlib.cpp
	@mkdir -p build/.objs/stdlib/linux/x86_64/debug/stdlib
	$(VV)$(stdlib_CXX) -c $(stdlib_CXXFLAGS) -o build/.objs/stdlib/linux/x86_64/debug/stdlib/stdlib.cpp.o stdlib/stdlib.cpp

swi: build/linux/x86_64/debug/swi
build/linux/x86_64/debug/swi: build/linux/x86_64/debug/swc build/.objs/swi/linux/x86_64/debug/repl/main.cpp.o
	@echo linking.debug swi
	@mkdir -p build/linux/x86_64/debug
	$(VV)$(swi_LD) -o build/linux/x86_64/debug/swi build/.objs/swi/linux/x86_64/debug/repl/main.cpp.o $(swi_LDFLAGS)

build/.objs/swi/linux/x86_64/debug/repl/main.cpp.o: repl/main.cpp
	@echo compiling.debug repl/main.cpp
	@mkdir -p build/.objs/swi/linux/x86_64/debug/repl
	$(VV)$(swi_CXX) -c $(swi_CXXFLAGS) -o build/.objs/swi/linux/x86_64/debug/repl/main.cpp.o repl/main.cpp

swc: build/linux/x86_64/debug/swc
build/linux/x86_64/debug/swc: build/linux/x86_64/debug/libstdlib.so build/.objs/swc/linux/x86_64/debug/compiler/compiler.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/ast/ast.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/ast/dump.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/ast/g-machine.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/ast/type.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/reporter.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/utils.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/diagnostics.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/g-machine/binop.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/g-machine/environment.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/g-machine/instruction.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/lexer/lexer.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/lexer/flex_lexer.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/parser/parser.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/parser/bison_parser.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/type/dump.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/type/environment.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/type/type.cpp.o
	@echo linking.debug swc
	@mkdir -p build/linux/x86_64/debug
	$(VV)$(swc_LD) -o build/linux/x86_64/debug/swc build/.objs/swc/linux/x86_64/debug/compiler/compiler.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/ast/ast.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/ast/dump.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/ast/g-machine.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/ast/type.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/reporter.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/utils.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/diagnostics.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/g-machine/binop.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/g-machine/environment.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/g-machine/instruction.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/lexer/lexer.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/lexer/flex_lexer.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/parser/parser.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/parser/bison_parser.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/type/dump.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/type/environment.cpp.o build/.objs/swc/linux/x86_64/debug/compiler/type/type.cpp.o $(swc_LDFLAGS)

build/.objs/swc/linux/x86_64/debug/compiler/compiler.cpp.o: compiler/compiler.cpp
	@echo compiling.debug compiler/compiler.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/compiler.cpp.o compiler/compiler.cpp

build/.objs/swc/linux/x86_64/debug/compiler/ast/ast.cpp.o: compiler/ast/ast.cpp
	@echo compiling.debug compiler/ast/ast.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/ast
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/ast/ast.cpp.o compiler/ast/ast.cpp

build/.objs/swc/linux/x86_64/debug/compiler/ast/dump.cpp.o: compiler/ast/dump.cpp
	@echo compiling.debug compiler/ast/dump.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/ast
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/ast/dump.cpp.o compiler/ast/dump.cpp

build/.objs/swc/linux/x86_64/debug/compiler/ast/g-machine.cpp.o: compiler/ast/g-machine.cpp
	@echo compiling.debug compiler/ast/g-machine.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/ast
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/ast/g-machine.cpp.o compiler/ast/g-machine.cpp

build/.objs/swc/linux/x86_64/debug/compiler/ast/type.cpp.o: compiler/ast/type.cpp
	@echo compiling.debug compiler/ast/type.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/ast
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/ast/type.cpp.o compiler/ast/type.cpp

build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/reporter.cpp.o: compiler/diagnostics/reporter.cpp
	@echo compiling.debug compiler/diagnostics/reporter.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/diagnostics
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/reporter.cpp.o compiler/diagnostics/reporter.cpp

build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/utils.cpp.o: compiler/diagnostics/utils.cpp
	@echo compiling.debug compiler/diagnostics/utils.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/diagnostics
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/utils.cpp.o compiler/diagnostics/utils.cpp

build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/diagnostics.cpp.o: compiler/diagnostics/diagnostics.cpp
	@echo compiling.debug compiler/diagnostics/diagnostics.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/diagnostics
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/diagnostics.cpp.o compiler/diagnostics/diagnostics.cpp

build/.objs/swc/linux/x86_64/debug/compiler/g-machine/binop.cpp.o: compiler/g-machine/binop.cpp
	@echo compiling.debug compiler/g-machine/binop.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/g-machine
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/g-machine/binop.cpp.o compiler/g-machine/binop.cpp

build/.objs/swc/linux/x86_64/debug/compiler/g-machine/environment.cpp.o: compiler/g-machine/environment.cpp
	@echo compiling.debug compiler/g-machine/environment.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/g-machine
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/g-machine/environment.cpp.o compiler/g-machine/environment.cpp

build/.objs/swc/linux/x86_64/debug/compiler/g-machine/instruction.cpp.o: compiler/g-machine/instruction.cpp
	@echo compiling.debug compiler/g-machine/instruction.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/g-machine
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/g-machine/instruction.cpp.o compiler/g-machine/instruction.cpp

build/.objs/swc/linux/x86_64/debug/compiler/lexer/lexer.cpp.o: compiler/lexer/lexer.cpp
	@echo compiling.debug compiler/lexer/lexer.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/lexer
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/lexer/lexer.cpp.o compiler/lexer/lexer.cpp

build/.objs/swc/linux/x86_64/debug/compiler/lexer/flex_lexer.cpp.o: compiler/lexer/flex_lexer.cpp
	@echo compiling.debug compiler/lexer/flex_lexer.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/lexer
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/lexer/flex_lexer.cpp.o compiler/lexer/flex_lexer.cpp

build/.objs/swc/linux/x86_64/debug/compiler/parser/parser.cpp.o: compiler/parser/parser.cpp
	@echo compiling.debug compiler/parser/parser.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/parser
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/parser/parser.cpp.o compiler/parser/parser.cpp

build/.objs/swc/linux/x86_64/debug/compiler/parser/bison_parser.cpp.o: compiler/parser/bison_parser.cpp
	@echo compiling.debug compiler/parser/bison_parser.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/parser
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/parser/bison_parser.cpp.o compiler/parser/bison_parser.cpp

build/.objs/swc/linux/x86_64/debug/compiler/type/dump.cpp.o: compiler/type/dump.cpp
	@echo compiling.debug compiler/type/dump.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/type
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/type/dump.cpp.o compiler/type/dump.cpp

build/.objs/swc/linux/x86_64/debug/compiler/type/environment.cpp.o: compiler/type/environment.cpp
	@echo compiling.debug compiler/type/environment.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/type
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/type/environment.cpp.o compiler/type/environment.cpp

build/.objs/swc/linux/x86_64/debug/compiler/type/type.cpp.o: compiler/type/type.cpp
	@echo compiling.debug compiler/type/type.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/debug/compiler/type
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/debug/compiler/type/type.cpp.o compiler/type/type.cpp

clean:  clean_stdlib clean_swi clean_swc

clean_stdlib: 
	@rm -rf build/linux/x86_64/debug/libstdlib.so
	@rm -rf build/linux/x86_64/debug/stdlib.sym
	@rm -rf build/.objs/stdlib/linux/x86_64/debug/stdlib/stdlib.cpp.o

clean_swi:  clean_swc
	@rm -rf build/linux/x86_64/debug/swi
	@rm -rf build/linux/x86_64/debug/swi.sym
	@rm -rf build/.objs/swi/linux/x86_64/debug/repl/main.cpp.o

clean_swc:  clean_stdlib
	@rm -rf build/linux/x86_64/debug/swc
	@rm -rf build/linux/x86_64/debug/swc.sym
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/compiler.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/ast/ast.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/ast/dump.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/ast/g-machine.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/ast/type.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/reporter.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/utils.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/diagnostics/diagnostics.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/g-machine/binop.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/g-machine/environment.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/g-machine/instruction.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/lexer/lexer.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/lexer/flex_lexer.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/parser/parser.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/parser/bison_parser.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/type/dump.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/type/environment.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/debug/compiler/type/type.cpp.o

