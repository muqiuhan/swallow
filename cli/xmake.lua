
includes("../compiler")
includes("../repl")

target("swa")
  set_languages("c++20")

  add_files("./*.cpp")
  add_includedirs("../compiler")
  add_deps("swc", "swi")