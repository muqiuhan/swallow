fmt:
	find . -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' -o -iname '*.hh' | xargs clang-format -i

check:
	xmake check clang.tidy --configfile=.clang-tidy

fix:
	xmake check clang.tidy --configfile=.clang-tidy --fix

build:
	xmake f -m debug && xmake build -w

release:
	xmake f -m release && xmake build -w

.PHONY: fmt check fix build release