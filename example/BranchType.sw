data Bool = [ True, False ]

let main(argv) = {
    Match argv with {
    | True => { 0 }
    | False => { True }
    }
}
