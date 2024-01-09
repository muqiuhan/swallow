target("lexer")
  set_kind("shared")
  
  before_build(function (target)
    os.run("flex -o %s/src/lexer.cpp %s/lexer.l", os.scriptdir(), os.scriptdir())
    os.run("clang-format -i %s/src/lexer.cpp", os.scriptdir())
  end)

  add_files("src/*.cpp")