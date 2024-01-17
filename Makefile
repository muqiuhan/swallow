fmt:
	find . -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' -o -iname '*.hh' | xargs clang-format