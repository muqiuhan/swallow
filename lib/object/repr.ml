open Object

let rec to_string e =
    match e with
    | Integer v -> Int64.to_string v
    | Boolean b ->
        if b then
          "#t"
        else
          "#f"
    | Float f -> Float.to_string f
    | Char c -> Core.Char.to_string c
    | Symbol s -> s
    | Nil -> "nil"
    | String s -> Format.sprintf {|"%s"|} s
    | Pair _ ->
        Format.sprintf "(%s)\n"
          (if is_list e then
             string_of_list e
           else
             string_of_pair e)
    | Primitive (name, _) -> Format.sprintf "#<primitive:%s" name
    | Quote expr -> Format.sprintf "'%s" (to_string expr)
    | Closure (name, name_list, _, _) ->
        Format.sprintf "#<%s:(%s)>" name (String.concat "" name_list)

and string_of_list l =
    match l with
    | Pair (a, Nil) -> to_string a
    | Pair (a, b) -> Format.sprintf "(%s %s)" (to_string a) (string_of_list b)
    | _ -> failwith "This can't happen!!!!"

and string_of_pair p =
    match p with
    | Pair (a, b) -> Format.sprintf "(%s . %s)" (to_string a) (to_string b)
    | _ -> failwith "This can't happen!!!!"