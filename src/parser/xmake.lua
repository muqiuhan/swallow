target("parser")
  set_kind("shared")
  
  before_build(function (target)
    os.run("bison -o $(scriptdir)/include/bison_parser.cpp -d $(scriptdir)/parser.y")
    os.run("mv $(scriptdir)/include/bison_parser.cpp $(scriptdir)/src/")
  end)

  add_includedirs("include", "../ast/include")
  add_files("src/*.cpp")

  on_clean(function (target)
    os.run("rm $(scriptdir)/src/bison_parser.cpp")
    os.run("rm $(scriptdir)/include/bison_parser.hpp")
    os.run("rm $(scriptdir)/include/stack.hh")
  end)