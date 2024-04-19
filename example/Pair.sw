data Pair = [ Pair Int Int ]

let first(pair) = {
    match pair with {
    | Pair(x, y) => { x }
    }
}

let second(pair) = {
    match pair with {
    | Pair(x, y) => { y }
    }
}

let slow(x) = {
    returns x after waiting for 1 second
}

let main(argv) = {
    first (Pair (slow 320) (slow 6))
}