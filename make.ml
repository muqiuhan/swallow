#!/bin/env ocaml
let _ = 
  begin
    match Sys.argv |> Array.to_list with
    | _ :: "fmt" :: _ -> "find . -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' -o -iname '*.hh' | xargs clang-format -i"
    | _ :: "check" :: _ -> "xmake check clang.tidy --configfile=.clang-tidy"
    | _ :: "fix" :: _ -> "xmake check clang.tidy --configfile=.clang-tidy --fix"
    | _ :: "build" :: _ -> "xmake f --toolchain=gcc -m debug -y && xmake build -j$(expr $(nproc) / 2)"
    | _ :: "release" :: _ -> "xmake f --toolchain=gcc -m release -y && xmake build -j$(expr $(nproc) / 2)"
    | _ :: "build.win" :: _ -> "xmake f --toolchain=mingw -m debug -y && xmake build -j$(expr $(nproc) / 2)"
    | _ :: "release.win" :: _ -> "xmake f --toolchain=mingw -m release -y && xmake build -j$(expr $(nproc) / 2)"
    | _ -> ""
  end
  |> Sys.command
