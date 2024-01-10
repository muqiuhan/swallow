target("lexer")
  set_kind("shared")
  
  before_build(function (target)
    os.run("flex -o $(scriptdir)/src/flex_lexer.cpp $(scriptdir)/lexer.l")
  end)

  add_includedirs("../ast/include", "../parser/include")
  add_files("src/*.cpp")

  on_clean(function (target)
    os.run("rm $(scriptdir)/src/flex_lexer.cpp")
  end)