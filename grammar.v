Require Import Bool.Bool.
Require Import Init.Nat.
Require Import Arith.EqNat.
Require Import Lists.List.
Require Import Strings.String.
Require Import ZArith.
Require Import Coq.Program.Wf.
Import ListNotations.

Module MML_Grammar.

(* Macros, implemented as total maps, as described in "Maps" of Software Foundations*)

Definition Macro (A : Type) :=
  string -> A.

(* Lambdas and conditionals *)

Inductive command :=
  | Unit
  | True
  | False
  | Eq (num1 num2 : command)
  | Num (n : Z)
  | Plus (num1 num2 : command)
  | Minus (num1 num2 : command)
  | Multiply (num1 num2 : command)
  | Divide (num1 num2 : command)
  | Empty
  | Singleton (x : command)
  | Pair (member1 member2 : command)
  | Lambda (arg : string) (c : command) (*c could be implemented as command -> command*)
  | If (b : command) (c1 c2 : command).

Inductive value : Set :=
  | VUnit
  | VBool (b : bool)
  | VNum (n : Z)
  | VList (l : list value)
  | VLambda (s : string) (c : command)
  | VError.

(* write eqbval list *)

Fixpoint eqbval (v1 v2 : value) : bool :=
  let eqbValList := (fix eqbValList (l1 l2 : list value) : bool :=
      match l1, l2 with
      | [], [] => true
      | (h_1 :: t_1), (h_2 :: t_2) => eqbval h_1 h_2 && eqbValList t_1 t_2
      | _, _ => false
      end) in
  match v1, v2 with
  | VUnit, VUnit => true
  | (VBool b1), (VBool b2) => Bool.eqb b1 b2
  | (VNum n1), (VNum n2) => Z.eqb n1 n2
  | (VList l1), (VList l2) => eqbValList l1 l2
  | VLambda s1 c1, VLambda s2 c2 => false 
  (* If someone wants to prove extensional functionality, make a separate proof *)
  | _, _ => false
  end.

Fixpoint evalCom (c : command) : value :=
  match c with
  | Unit => VUnit
  | True => VBool true
  | False => VBool false
  | Eq num1 num2 => VBool (eqbval (evalCom num1) (evalCom num2))
  | Num n => VNum n
  | Plus (Num n1) (Num n2) => VNum (n1 + n2)
  | Minus (Num n1) (Num n2) => VNum (n1 - n2)
  | Divide (Num n1) (Num n2) => VNum (n1 * n2)
  | Multiply (Num n1) (Num n2) => VNum (Z.div n1 n2)
  | Empty => VList []
  | Singleton x => VList [(evalCom x)]
  | Pair member1 member2 => VList [(evalCom member1); (evalCom member2)]
  | Lambda arg c => VLambda arg c(*c could be implemented as command -> command*)
  | If b c1 c2 => match (evalCom b) with
                | VBool true => evalCom c1
                | VBool false => evalCom c2
                | _ => VError
                end
  | _ => VError
  end.

End MML_Grammar.

