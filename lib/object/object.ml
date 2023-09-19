(****************************************************************************)
(* Swallow                                                                    *)
(* Copyright (C) 2022 Muqiu Han                                             *)
(*                                                                          *)
(* This program is free software: you can redistribute it and/or modify     *)
(* it under the terms of the GNU Affero General Public License as published *)
(* by the Free Software Foundation, either version 3 of the License, or     *)
(* (at your option) any later version.                                      *)
(*                                                                          *)
(* This program is distributed in the hope that it will be useful,          *)
(* but WITHOUT ANY WARRANTY; without even the implied warranty of           *)
(* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *)
(* GNU Affero General Public License for more details.                      *)
(*                                                                          *)
(* You should have received a copy of the GNU Affero General Public License *)
(* along with this program.  If not, see <https://www.gnu.org/licenses/>.   *)
(****************************************************************************)

open Swallow_error

type lobject =
  | Integer of Int64.t
  | Float of float
  | Boolean of bool
  | Symbol of string
  | String of string
  | Char of char
  | Nil
  | Pair of lobject * lobject
  | Primitive of string * (lobject list -> lobject)
  | Quote of value
  | Closure of name * name list * expr * value env

and value = lobject
and name = string

and let_kind =
  | LET
  | LETSTAR
  | LETREC

and expr =
  | Literal of value
  | Var of name
  | If of expr * expr * expr
  | And of expr * expr
  | Or of expr * expr
  | Apply of expr * expr
  | Call of expr * expr list
  | Defexpr of def
  | Lambda of name * name list * expr
  | Let of let_kind * (name * expr) list * expr

and def =
  | Setq of name * expr
  | Defun of name * name list * expr
  | Expr of expr

and 'a env = (string * 'a option ref) list

type t = lobject

let rec is_list = function
    | Nil -> true
    | Pair (_, b) -> is_list b
    | _ -> false

let rec list_of_pair pair =
    match pair with
    | Nil -> []
    | Pair (a, b) -> a :: list_of_pair b
    | _ -> failwith "This can't happen!!!!"

let string_of_char a_char = String.make 1 a_char

let rec lookup = function
    | n, [] -> raise (Errors.Runtime_error_exn (Errors.Not_found n))
    | n, (n', v) :: _ when n = n' -> (
        match !v with
        | Some v' -> v'
        | None -> raise (Errors.Runtime_error_exn (Errors.Unspecified_value n)))
    | n, (_, _) :: bs -> lookup (n, bs)

let bind (name, value, sexpr) = (name, ref (Some value)) :: sexpr
let make_local _ = ref None
let bind_local (n, vor, e) = (n, vor) :: e

let bind_list ns vs env =
    try List.fold_left2 (fun acc n v -> bind (n, v, acc)) env ns vs
    with Invalid_argument _ ->
      raise (Errors.Runtime_error_exn (Errors.Missing_argument ns))

let bind_local_list ns vs env =
    try List.fold_left2 (fun acc n v -> bind_local (n, v, acc)) env ns vs
    with Invalid_argument _ ->
      raise (Errors.Runtime_error_exn (Errors.Missing_argument ns))

let rec env_to_val =
    let b_to_val (n, vor) =
        Pair
          ( Symbol n,
            match !vor with
            | None -> Symbol "unspecified"
            | Some v -> v )
    in
        function
        | [] -> Nil
        | b :: bs -> Pair (b_to_val b, env_to_val bs)
