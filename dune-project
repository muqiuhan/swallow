(lang dune 3.3)

(name swallow)

(generate_opam_files true)

(source
 (github muqiuhan/Swallow))

(authors "Muqiu Han")

(maintainers "Muqiu Han")

(license AGPL-3.0-only)

(documentation https://github.com/muqiuhan/Swallow)

(package
 (name swallow)
 (synopsis "A Lisp implementation in OCaml")
 (description "A Lisp implementation in OCaml")
 (depends ocaml dune core ocolor camlp-streams)
 (tags
  (lisp interpreter compiler)))

; See the complete stanza docs at https://dune.readthedocs.io/en/stable/dune-files.html#dune-project
