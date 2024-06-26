# this is the build file for project swallow
# it is autogenerated by the xmake build system.
# do not edit by hand.

ifneq ($(VERBOSE),1)
VV=@
endif

CXX=/usr/bin/clang-18
MXX=/usr/bin/clang-18
AS=/usr/bin/clang-18
CC=/usr/bin/clang-18
MM=/usr/bin/clang-18

AR=/usr/bin/ar
LD=/usr/bin/clang++-18
SH=/usr/bin/clang++-18

swc_LD=/usr/bin/clang++-18
swc_CXX=/usr/bin/clang-18
swc_CXX=/usr/bin/clang-18
base_SH=/usr/bin/clang++-18
base_CXX=/usr/bin/clang-18
base_CXX=/usr/bin/clang-18
swi_LD=/usr/bin/clang++-18
swi_CXX=/usr/bin/clang-18
swi_CXX=/usr/bin/clang-18

swc_CXXFLAGS=-Qunused-arguments -m64 -I/usr/lib/llvm-18/include/c++/v1 -fvisibility=hidden -fvisibility-inlines-hidden -O3 -std=c++20 -Icompiler -Icompiler/ast -Icompiler/type -Icompiler/lexer -Icompiler/parser -Icompiler/diagnostics -Icompiler/utils -stdlib=libc++ -DNDEBUG
swc_CXXFLAGS=-Qunused-arguments -m64 -I/usr/lib/llvm-18/include/c++/v1 -fvisibility=hidden -fvisibility-inlines-hidden -O3 -std=c++20 -Icompiler -Icompiler/ast -Icompiler/type -Icompiler/lexer -Icompiler/parser -Icompiler/diagnostics -Icompiler/utils -stdlib=libc++ -DNDEBUG
swc_LDFLAGS=-m64 --stdlib=libc++ -Lbuild/linux/x86_64/release -Wl,-rpath=$ORIGIN -s -lc++ -lbase
base_CXXFLAGS=-Qunused-arguments -m64 -fPIC -I/usr/lib/llvm-18/include/c++/v1 -O3 -std=c++20 -DNDEBUG
base_CXXFLAGS=-Qunused-arguments -m64 -fPIC -I/usr/lib/llvm-18/include/c++/v1 -O3 -std=c++20 -DNDEBUG
base_SHFLAGS=-shared -m64 -fPIC -s
swi_CXXFLAGS=-Qunused-arguments -m64 -I/usr/lib/llvm-18/include/c++/v1 -fvisibility=hidden -fvisibility-inlines-hidden -O3 -std=c++20 -DNDEBUG
swi_CXXFLAGS=-Qunused-arguments -m64 -I/usr/lib/llvm-18/include/c++/v1 -fvisibility=hidden -fvisibility-inlines-hidden -O3 -std=c++20 -DNDEBUG
swi_LDFLAGS=-m64 --stdlib=libc++ -Lbuild/linux/x86_64/release -Wl,-rpath=$ORIGIN -s -lbase

default:  swc base swi

all:  swc base swi

.PHONY: default all  swc base swi

swc: build/linux/x86_64/release/swc
build/linux/x86_64/release/swc: build/linux/x86_64/release/libbase.so build/.objs/swc/linux/x86_64/release/compiler/compiler.cpp.o build/.objs/swc/linux/x86_64/release/compiler/parser/parser.cpp.o build/.objs/swc/linux/x86_64/release/compiler/parser/bison_parser.cpp.o build/.objs/swc/linux/x86_64/release/compiler/type/dump.cpp.o build/.objs/swc/linux/x86_64/release/compiler/type/environment.cpp.o build/.objs/swc/linux/x86_64/release/compiler/type/type.cpp.o build/.objs/swc/linux/x86_64/release/compiler/diagnostics/reporter.cpp.o build/.objs/swc/linux/x86_64/release/compiler/diagnostics/diagnostics.cpp.o build/.objs/swc/linux/x86_64/release/compiler/diagnostics/utils.cpp.o build/.objs/swc/linux/x86_64/release/compiler/g-machine/instruction.cpp.o build/.objs/swc/linux/x86_64/release/compiler/g-machine/binop.cpp.o build/.objs/swc/linux/x86_64/release/compiler/g-machine/environment.cpp.o build/.objs/swc/linux/x86_64/release/compiler/lexer/flex_lexer.cpp.o build/.objs/swc/linux/x86_64/release/compiler/lexer/lexer.cpp.o build/.objs/swc/linux/x86_64/release/compiler/ast/ast.cpp.o build/.objs/swc/linux/x86_64/release/compiler/ast/g-machine.cpp.o build/.objs/swc/linux/x86_64/release/compiler/ast/dump.cpp.o build/.objs/swc/linux/x86_64/release/compiler/ast/type.cpp.o
	@echo linking.release swc
	@mkdir -p build/linux/x86_64/release
	$(VV)$(swc_LD) -o build/linux/x86_64/release/swc build/.objs/swc/linux/x86_64/release/compiler/compiler.cpp.o build/.objs/swc/linux/x86_64/release/compiler/parser/parser.cpp.o build/.objs/swc/linux/x86_64/release/compiler/parser/bison_parser.cpp.o build/.objs/swc/linux/x86_64/release/compiler/type/dump.cpp.o build/.objs/swc/linux/x86_64/release/compiler/type/environment.cpp.o build/.objs/swc/linux/x86_64/release/compiler/type/type.cpp.o build/.objs/swc/linux/x86_64/release/compiler/diagnostics/reporter.cpp.o build/.objs/swc/linux/x86_64/release/compiler/diagnostics/diagnostics.cpp.o build/.objs/swc/linux/x86_64/release/compiler/diagnostics/utils.cpp.o build/.objs/swc/linux/x86_64/release/compiler/g-machine/instruction.cpp.o build/.objs/swc/linux/x86_64/release/compiler/g-machine/binop.cpp.o build/.objs/swc/linux/x86_64/release/compiler/g-machine/environment.cpp.o build/.objs/swc/linux/x86_64/release/compiler/lexer/flex_lexer.cpp.o build/.objs/swc/linux/x86_64/release/compiler/lexer/lexer.cpp.o build/.objs/swc/linux/x86_64/release/compiler/ast/ast.cpp.o build/.objs/swc/linux/x86_64/release/compiler/ast/g-machine.cpp.o build/.objs/swc/linux/x86_64/release/compiler/ast/dump.cpp.o build/.objs/swc/linux/x86_64/release/compiler/ast/type.cpp.o $(swc_LDFLAGS)

build/.objs/swc/linux/x86_64/release/compiler/compiler.cpp.o: compiler/compiler.cpp
	@echo compiling.release compiler/compiler.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/compiler.cpp.o compiler/compiler.cpp

build/.objs/swc/linux/x86_64/release/compiler/parser/parser.cpp.o: compiler/parser/parser.cpp
	@echo compiling.release compiler/parser/parser.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/parser
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/parser/parser.cpp.o compiler/parser/parser.cpp

build/.objs/swc/linux/x86_64/release/compiler/parser/bison_parser.cpp.o: compiler/parser/bison_parser.cpp
	@echo compiling.release compiler/parser/bison_parser.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/parser
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/parser/bison_parser.cpp.o compiler/parser/bison_parser.cpp

build/.objs/swc/linux/x86_64/release/compiler/type/dump.cpp.o: compiler/type/dump.cpp
	@echo compiling.release compiler/type/dump.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/type
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/type/dump.cpp.o compiler/type/dump.cpp

build/.objs/swc/linux/x86_64/release/compiler/type/environment.cpp.o: compiler/type/environment.cpp
	@echo compiling.release compiler/type/environment.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/type
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/type/environment.cpp.o compiler/type/environment.cpp

build/.objs/swc/linux/x86_64/release/compiler/type/type.cpp.o: compiler/type/type.cpp
	@echo compiling.release compiler/type/type.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/type
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/type/type.cpp.o compiler/type/type.cpp

build/.objs/swc/linux/x86_64/release/compiler/diagnostics/reporter.cpp.o: compiler/diagnostics/reporter.cpp
	@echo compiling.release compiler/diagnostics/reporter.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/diagnostics
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/diagnostics/reporter.cpp.o compiler/diagnostics/reporter.cpp

build/.objs/swc/linux/x86_64/release/compiler/diagnostics/diagnostics.cpp.o: compiler/diagnostics/diagnostics.cpp
	@echo compiling.release compiler/diagnostics/diagnostics.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/diagnostics
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/diagnostics/diagnostics.cpp.o compiler/diagnostics/diagnostics.cpp

build/.objs/swc/linux/x86_64/release/compiler/diagnostics/utils.cpp.o: compiler/diagnostics/utils.cpp
	@echo compiling.release compiler/diagnostics/utils.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/diagnostics
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/diagnostics/utils.cpp.o compiler/diagnostics/utils.cpp

build/.objs/swc/linux/x86_64/release/compiler/g-machine/instruction.cpp.o: compiler/g-machine/instruction.cpp
	@echo compiling.release compiler/g-machine/instruction.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/g-machine
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/g-machine/instruction.cpp.o compiler/g-machine/instruction.cpp

build/.objs/swc/linux/x86_64/release/compiler/g-machine/binop.cpp.o: compiler/g-machine/binop.cpp
	@echo compiling.release compiler/g-machine/binop.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/g-machine
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/g-machine/binop.cpp.o compiler/g-machine/binop.cpp

build/.objs/swc/linux/x86_64/release/compiler/g-machine/environment.cpp.o: compiler/g-machine/environment.cpp
	@echo compiling.release compiler/g-machine/environment.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/g-machine
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/g-machine/environment.cpp.o compiler/g-machine/environment.cpp

build/.objs/swc/linux/x86_64/release/compiler/lexer/flex_lexer.cpp.o: compiler/lexer/flex_lexer.cpp
	@echo compiling.release compiler/lexer/flex_lexer.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/lexer
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/lexer/flex_lexer.cpp.o compiler/lexer/flex_lexer.cpp

build/.objs/swc/linux/x86_64/release/compiler/lexer/lexer.cpp.o: compiler/lexer/lexer.cpp
	@echo compiling.release compiler/lexer/lexer.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/lexer
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/lexer/lexer.cpp.o compiler/lexer/lexer.cpp

build/.objs/swc/linux/x86_64/release/compiler/ast/ast.cpp.o: compiler/ast/ast.cpp
	@echo compiling.release compiler/ast/ast.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/ast
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/ast/ast.cpp.o compiler/ast/ast.cpp

build/.objs/swc/linux/x86_64/release/compiler/ast/g-machine.cpp.o: compiler/ast/g-machine.cpp
	@echo compiling.release compiler/ast/g-machine.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/ast
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/ast/g-machine.cpp.o compiler/ast/g-machine.cpp

build/.objs/swc/linux/x86_64/release/compiler/ast/dump.cpp.o: compiler/ast/dump.cpp
	@echo compiling.release compiler/ast/dump.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/ast
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/ast/dump.cpp.o compiler/ast/dump.cpp

build/.objs/swc/linux/x86_64/release/compiler/ast/type.cpp.o: compiler/ast/type.cpp
	@echo compiling.release compiler/ast/type.cpp
	@mkdir -p build/.objs/swc/linux/x86_64/release/compiler/ast
	$(VV)$(swc_CXX) -c $(swc_CXXFLAGS) -o build/.objs/swc/linux/x86_64/release/compiler/ast/type.cpp.o compiler/ast/type.cpp

base: build/linux/x86_64/release/libbase.so
build/linux/x86_64/release/libbase.so: build/.objs/base/linux/x86_64/release/base/base.cpp.o
	@echo linking.release libbase.so
	@mkdir -p build/linux/x86_64/release
	$(VV)$(base_SH) -o build/linux/x86_64/release/libbase.so build/.objs/base/linux/x86_64/release/base/base.cpp.o $(base_SHFLAGS)

build/.objs/base/linux/x86_64/release/base/base.cpp.o: base/base.cpp
	@echo compiling.release base/base.cpp
	@mkdir -p build/.objs/base/linux/x86_64/release/base
	$(VV)$(base_CXX) -c $(base_CXXFLAGS) -o build/.objs/base/linux/x86_64/release/base/base.cpp.o base/base.cpp

swi: build/linux/x86_64/release/swi
build/linux/x86_64/release/swi: build/linux/x86_64/release/swc build/.objs/swi/linux/x86_64/release/repl/main.cpp.o
	@echo linking.release swi
	@mkdir -p build/linux/x86_64/release
	$(VV)$(swi_LD) -o build/linux/x86_64/release/swi build/.objs/swi/linux/x86_64/release/repl/main.cpp.o $(swi_LDFLAGS)

build/.objs/swi/linux/x86_64/release/repl/main.cpp.o: repl/main.cpp
	@echo compiling.release repl/main.cpp
	@mkdir -p build/.objs/swi/linux/x86_64/release/repl
	$(VV)$(swi_CXX) -c $(swi_CXXFLAGS) -o build/.objs/swi/linux/x86_64/release/repl/main.cpp.o repl/main.cpp

clean:  clean_swc clean_base clean_swi

clean_swc:  clean_base
	@rm -rf build/linux/x86_64/release/swc
	@rm -rf build/linux/x86_64/release/swc.sym
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/compiler.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/parser/parser.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/parser/bison_parser.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/type/dump.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/type/environment.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/type/type.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/diagnostics/reporter.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/diagnostics/diagnostics.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/diagnostics/utils.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/g-machine/instruction.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/g-machine/binop.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/g-machine/environment.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/lexer/flex_lexer.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/lexer/lexer.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/ast/ast.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/ast/g-machine.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/ast/dump.cpp.o
	@rm -rf build/.objs/swc/linux/x86_64/release/compiler/ast/type.cpp.o

clean_base: 
	@rm -rf build/linux/x86_64/release/libbase.so
	@rm -rf build/linux/x86_64/release/base.sym
	@rm -rf build/.objs/base/linux/x86_64/release/base/base.cpp.o

clean_swi:  clean_swc
	@rm -rf build/linux/x86_64/release/swi
	@rm -rf build/linux/x86_64/release/swi.sym
	@rm -rf build/.objs/swi/linux/x86_64/release/repl/main.cpp.o

