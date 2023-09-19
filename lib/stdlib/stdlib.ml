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

open Swallow_object
open Swallow_error
open Swallow_eval
open Swallow_lexer
open Swallow_ast
open Swallow_utils
open Swallow_primitives

let eval env e =
    match e with
    | Object.Defexpr d -> Eval.eval_def d env
    | _ ->
        raise
          (Errors.Parse_error_exn
             (Errors.Type_error "Can only have definitions in stdlib"))

let rec slurp stm env =
    try stm |> Lexer.read_sexpr |> Ast.build_ast |> eval env |> snd |> slurp stm
    with Stream.Failure -> env

let stdlib =
    let stm = Stream_wrapper.make_stringstream Stdlib_string.stdlib_string in
        slurp stm Basis.basis
