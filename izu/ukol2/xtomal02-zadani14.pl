uloha14([], _) :- write("ERROR: Empty list").
uloha14(List, Limit, Shodnota) :-
    suma( List, Limit, Sum ),
    counter( List, Limit, Count ),
    Count \= 0,
    Shodnota is Sum / Count, !.
uloha14(_) :-
    write("No number in list equal or lower than limit").

suma(L, Max, Sum) :-
   suma(L, Max, 0, Sum).        %Volani funkce s akumulatorem pro ulozeni sumy pri pruchodu 
suma([H|T], Max, Acc, Sum) :-
    absolute(H,A),
    A =< Max,                   %Hlavicka seznamu je mensi nebo se rovna limitu               
    Summ is Acc + H,            %Aktualizujeme akumulator navysenim o hodnotu H      
    suma(T, Max, Summ, Sum).    %Volame znova funkci pro zbytek seznamu
suma([H|T], Max, Acc, Sum) :-
    absolute(H,A),
    A > Max,                     %Hlavicka seznamu je vetsi nez limit
    suma(T, Max, Acc, Sum).     %Nepricitame, volame pro zbytek seznamu
suma([], _, Sum, Sum).          %Ulozeni akumulatoru do sumy

counter(L, Max, Count) :-
    counter(L, Max, 0, Count).
counter([H|T], Max, Acc, Count) :-
    absolute(H,A),
    A =< Max,                            
    Accu is Acc + 1,                  
    counter(T, Max, Accu, Count).   
counter([H|T], Max, Acc, Count) :-
    absolute(H,A),
    A > Max,                           
    counter(T, Max, Acc, Count).   
counter([], _, Count, Count).     

absolute(X,X) :- X >= 0, !.
absolute(X,Y) :- Y is -X.