data List = [ Cons Int List ]

let main(argv) = {
  Match argv with {
  | Nil => { }
  | Cons x xs => { 0 }
  }
}
