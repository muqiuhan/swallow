data List = [ Nil; Cons Int List ]

let length(l) = {
    match l with {
    | Nil => { 0 }
    | Cons(x, xs) => { 1 + length xs }
    }
}
