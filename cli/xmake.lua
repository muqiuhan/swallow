
includes("../compiler")
includes("../repl")

package("structopt")
    add_deps("cmake")
    set_sourcedir(path.join(os.scriptdir(), "./utils/structopt"))
    on_install(function (package)
        local configs = {}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
        import("package.tools.cmake").install(package, configs)
    end)
package_end()

add_requires("structopt")

target("swa")
  set_languages("c++20")

  add_files("./*.cpp")
  add_includedirs("./utils/structopt/include", "../compiler")
  add_deps("swc", "swi")
  add_packages("structopt")