type ('nonterminal, 'terminal) symbol =
  | N of 'nonterminal
  | T of 'terminal
  
let subset a b = List.for_all(fun i -> List.mem i b) a;;

let equal_sets a b = subset a b && subset b a;;

let rec computed_fixed_point eq f x =
if eq x (f x) then
x
else
computed_fixed_point eq f (f x);;

let start_list g = [(N (fst g) )];; (*gets the start symbol of the grammar*)

let rules g = snd g;;         (*gets the rules from the grammar*)

let decide a = match a with   (*decides if it is a nonterminal or a terminal*)
| N symbol -> true
| T symbol -> false;;

let rec create rules results =  (*creates a list that only contains the results from a list of rules*)
match rules with 
| [] -> []
| h::t -> if List.exists ( (=) (N(fst h)) ) results then h::(create t results) else (create t results);;

let start g =
N (fst g) ;;


let create_rule g accessible = List.filter decide ( List.flatten(List.map snd (create (rules g) accessible ) ) ) ;;
let create_rules g accessible = (start g)::(create_rule g accessible);;
(*filters what is reachable*)


let reachable_rules g =  create (rules g) (computed_fixed_point (equal_sets) (create_rules g) (start_list g) );; (*creates all the reachable rules*)

let filter_reachable g = ( fst g , reachable_rules g);; (*the final product*)







