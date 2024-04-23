#!/bin/env ocaml

let _ = 
  begin
    match Sys.argv |> Array.to_list with
    | _ :: "fmt" :: _ -> "find . -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' -o -iname '*.hh' | xargs clang-format -i"
    | _ :: "check" :: _ -> "xmake check clang.tidy --configfile=.clang-tidy"
    | _ :: "fix" :: _ -> "xmake check clang.tidy --configfile=.clang-tidy --fix"
    | _ :: "build" :: _ -> "xmake f -m debug && xmake build -w"
    | _ :: "release" :: _ -> "xmake f -m release && xmake build -w"
    | _ :: "build.win" :: _ -> "xmake f --toolchain=mingw -m debug && xmake build -w"
    | _ :: "release.win" :: _ -> "xmake f --toolchain=mingw -m release && xmake build -w"
    | _ -> ""
  end
  |> Sys.command