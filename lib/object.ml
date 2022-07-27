(****************************************************************************)
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

type lobject =
  | Fixnum of int
  | Boolean of bool
  | Symbol of string
  | Nil
  | Pair of lobject * lobject

exception This_can't_happen_exn

let rec is_list = function
  | Nil -> true
  | Pair (_, b) -> is_list b
  | _ -> false
;;

let rec print_sexp sexp =
  match sexp with
  | Fixnum v -> print_int v
  | Boolean b -> print_string (if b then "#t" else "#f")
  | Symbol s -> print_string s
  | Nil -> print_string "nil"
  | Pair (_, _) ->
    print_string "(";
    if is_list sexp then print_list sexp else print_pair sexp;
    print_string ")"

and print_list lst =
  match lst with
  | Pair (a, Nil) -> print_sexp a
  | Pair (a, b) ->
    print_sexp a;
    print_string " ";
    print_list b
  | _ -> raise This_can't_happen_exn

and print_pair pair =
  match pair with
  | Pair (a, b) ->
    print_sexp a;
    print_string " ";
    print_sexp b
  | _ -> raise This_can't_happen_exn
;;
