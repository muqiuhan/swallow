data Bool = [ True, False ]
data List = [ Nil, Cons Int List ]

let main(argv) = {
    Match argv with {
    | True => { 0 }
    | Nil => { 1 }
    }
}
