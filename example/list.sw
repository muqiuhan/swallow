data List = [ Nil, Cons Int List ]

let main argv = {
  match argv with {
  | Nil => { 0 }
  | Cons x xs => { 1 }
  }
}