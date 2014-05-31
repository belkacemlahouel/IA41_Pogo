% #############################################################################
% think([ETAT], [cJoueur, lvlAI], [nCaseDepart, nCaseArrivee, indexPionStack]).
% #############################################################################

% -----------------------------------------------------------------------------
% ETAT est codé comme suit :
%		-1 pour dire qu'on passe à la case suivante
%		1  pour désigner un pion blanc
%		0  pour désigner un pion noir
% De cette manière, on a toujours un tableau de 21 charactères.
%
% Chaque tas de pions est considéré comme étant un stack
%		Le premier est celui du haut
% 		Le dernier est celui du bas
% Cela devrait nous arranger lors de la réflexion Prolog.
% -----------------------------------------------------------------------------

% -----------------------------------------------------------------------------
% cJoueur est la couleur joueur
%		1 pour le joueur blanc
%		0 pour le joueur noir
% -----------------------------------------------------------------------------

% -----------------------------------------------------------------------------
% lvlAI est le niveau de l'intelligence artificelle
%		C'est normalement un nombre dans {0, 1, 2, 3}
%		(à vérifier) où 0 => arboresence de recherche vide : hasard ?
% -----------------------------------------------------------------------------

% -----------------------------------------------------------------------------
% nCaseDepart     numéro de la case de départ
% nCaseArrivee    numéro de la case d'arrivée, du pion qu'on jouera
% indexPionStack  index du pion "à la mode stack"
% -----------------------------------------------------------------------------

% -----------------------------------------------------------------------------
% Les cases sont numérotées comme suit :
% 1 2 3
% 4 5 6
% 7 8 9
% -----------------------------------------------------------------------------
















% #############################################################################
% Creation de fonctions d'évaluations - Voir "./idees_evaluation"
% #############################################################################

% -----------------------------------------------------------------------------
% Base de faits : évaluation d'un pion
% evam_pion(Pion, Evaluation)
eval_pion(1, 1).		% pion blanc
eval_pion(0, -1).		% pion noir

% -----------------------------------------------------------------------------
% eval1([ETAT], EVAL).
% -----------------------------------------------------------------------------

% ------------ NIGHLTY ----------------
% eval1(ETAT, E) :- E is 0,
% 					eval1(ETAT, E, 0). % ETAT, EVAL, COMPTEUR (4 premiers pions)
% eval1([], _, 0) :- !.
% eval1([-1|R], E, _) :- eval1(R, E, 0).		% Si on trouve un -1, C <- 0
% eval1([_|R], E, 4) :-	 eval1(R, E, 4).		% Si C = 4, vers la prochaine
% eval1([X|R], E, C) :- eval_pion(X, XE),	% Sinon, on evalue le pion
% 						C1 is C+1,			% On incrémente le compteur
%						E1 is E+XE,			% On mets à jour l'éval
%						eval1(R, E1, C1).	% On évalue le reste

% ------------ Fonctionnel ------------
eval(ETAT, E) :- eval1(ETAT, E, 0). % ETAT, EVAL, COMPTEUR (4 premiers pions)

eval1([], 0, 0) :- !.
eval1([-1|R], E, _) :- 	eval1(R, E, 0), !.		% Si on trouve un -1, C <- 0
eval1([_|R], E, 4) :-	eval1(R, E, 4), !.		% Si C = 4, vers la prochaine

eval1([X|R], E, C) :- 	eval_pion(X, XE),	% Sinon, on evalue le pion
 						C1 is C+1,			% On incrémente le compteur
						eval1(R, E1, C1),	% On évalue le reste
						E is E1+XE.			% On mets à jour l'éval



% #############################################################################
% Fonction qui nous donne le nouvel état en fonction de ce qu'on veut jouer
% #############################################################################

% -----------------------------------------------------------------------------
% nouvel_etat(+[ETAT0], +nCaseDepart, +nCaseArrivee, +indexPion, ?[ETAT1])
% -----------------------------------------------------------------------------

nouvel_etat(E, D, A, I, E1) :-  enleve_pile(E, D, I, E0, P),
								insere_pile(E0, P, A, E1).





% -----------------------------------------------------------------------------
% enleve_pile(+E, +D, +I, ?E0, ?P)
% E0 est l'état E sans la pile P située sur la case D, du 1er au pion #I inclus
% On suppose : D dans [1, 9] (ou l'index de la dernière case) et I dans [1, 3]
% -----------------------------------------------------------------------------

% On ajoute un compteur de cases + un compteur d'index
enleve_pile(E, D, I, E0, P) :-
			enleve_pile(E, D, I, E0, P, 1, 1), !.

% On s'arrête si E vide, donc P et E0 vides
enleve_pile([], _, _, [], [], _, _) :- !.

% On incrémente le compteur de cases, si -1
enleve_pile([-1|E], D, I, [-1|E0], P, CC, CI) :-
			CC1 is CC+1,
			enleve_pile(E, D, I, E0, P, CC1, CI), !.

% On récupère la pile si on est sur la case #D
enleve_pile([X|E], D, I, E0, [X|P], D, CI) :-
			CI =< I,
			CI1 is CI+1,
			enleve_pile(E, D, I, E0, P, D, CI1), !.

% enleve_pile(E, D, I, E0, [], D, CI) :-
%			CI = I+1,
%			enleve_pile(E, D, I, E0, [], D, CI).

% On arrête de récupérer la pile si on est arrivés à l'index #I
% On recopie l'état initial dans l'état final (cas usuel)
enleve_pile([X|E], D, I, [X|E0], P, CC, CI) :-
			enleve_pile(E, D, I, E0, P, CC, CI), !.

% ----------


% Compteur cases
% enleve_pile(E, D, I, E0, P) :- enleve_pile_c(E, D, I, E0, P, 1).

% ----- Chercher la case

% enleve_pile_c([-1|R], D, I, [-1|R0], P, CC) :- CC1 is CC+1,
%										  enleve_pile_c(R, D, I, R0, P, CC1).

% enleve_pile_c(E, D, I, E0, P, D) :- enleve_pile_i(E, D, I, E0, P, D, 1).

% enleve_pile_c([X|R], D, I, [X|R0], P, CC) :-
%										enleve_pile_c(R, D, I, R0, P, CC).

% enleve_pile_c([], _, _, [], _, 10) :- !.
% Quand le parcours est fini, on pointe sur la case après la neuvième case (?)

% ----- Chercher l'index du pion

% Problème si la pile est plus grande que le nombre de pions ?
% Il ne faut pas que ce prédicat recoive des informations incohérentes !
% Genre bouger le pion n°3 alors que la pile est de taille 1
% Checker la couleur aussi, avant de lui demander ça

% Pile ne contient que X, les autres seront ajoutés dessus, lors du retour
% enleve_pile_i([X|R], D, I, E0, [X], CC, I) :-
%										enleve_pile_c(R, D, I, E0, [X], CC).
% enleve_pile_i([X|R], D, I, E0, [X|P], CC, CI) :-
%										CI1 is CI+1,
%										enleve_pile_i(R, D, I, E0, P, CC, CI1).

% ----- Déplacer le pion et ceux qui sont au dessus
% ----- Faut réorganiser l'ordre des priorités...


% -----------------------------------------------
% insere_pile(+[ETAT0], +[PILE], +ARRIVEE, ?[ETAT1]).
% -----------------------------------------------

% Compteur de cases
insere_pile(E0, P, A, E1) :-
			insere_pile(E0, P, A, E1, 1), !.

% On rencontre un -1, synonyme d'une nouvelle case
insere_pile([-1|R0], P, A, [-1|R1], CC) :-
			CC1 is CC+1,
			insere_pile(R0, P, A, R1, CC1), !.

% On insère la pile au dessus de la case #A si on est dessus
insere_pile(E0, [X|R], A, [X|R1], A) :-
			insere_pile(E0, R, A, R1, A), !.

% On peut continuer à recopier l'état initial, indépendamment de la pile
insere_pile([X|R0], P, A, [X|R1], CC) :-
			insere_pile(R0, P, A, R1, CC), !.

% On s'arrête si on a fini de parcourir l'état initial (et la pile est vide)
insere_pile([], [], _, [], _) :- !.
% insere_pile([], [], _, [], 10) :- !.

% ligne(+CASE,-LIGNE).
% renvoie le numero de ligne de CASE (1, 2 ou 3);
ligne(X,1):-member(X,[1,2,3]),!.
ligne(X,2):-member(X,[4,5,6]),!.
ligne(X,3):-member(X,[7,8,9]),!.

% colonne(+CASE,-COL).
% renvoie le numero de colonne de CASE (1, 2 ou 3);
colonne(X,1):-member(X,[1,4,7]),!.
colonne(X,2):-member(X,[2,5,8]),!.
colonne(X,3):-member(X,[3,6,9]),!.

% longueur_deplacement(+DEP,+DEST,-LONG)
% Renvoie la longueur d'un déplacement de la case DEP vers la case DEST
longueur_deplacement(X,X,0):-!.
longueur_deplacement(X,Y,LONG):-colonne(X,CX),colonne(Y,CY),ligne(X,LX),ligne(Y,LY),DC is abs(CX - CY),DL is abs(LX - LY),LONG is DC + DL.

% case(ETAT,CONTENU,SUITEETAT)
% donne le contenu de la prochaine case non vide, ainsi que la suite de l'état à analyser
case([],[],[]).
case([-1|R],[],R):-!.
case([X|LC],[X|R2],R1):-case(LC,R2,R1).

% joueur_case(+CASE,?JOUEUR)
% JOUEUR est le joueur à qui appartient la case CASE (1 ou 0)

joueur_case([X|_],X).
			
% piles_joueur(+ETAT,+JOUEUR,-PILES,-INDEXES)
% Renvoie directement les piles possible pour le joueur JOUEUR, ainsi que les indices (cases) de ces piles
			
piles_joueur(E,J,P,L):- piles_joueur(E,J,P,1,L),!.
piles_joueur([],_,[],_,[]):-!.
piles_joueur([-1],_,[],_,[]):-!.
piles_joueur([-1|LC],J,P,COUNT,IS):-!,
			COUNT<10,
			COUNT1 is COUNT+1,
			piles_joueur(LC,J,P,COUNT1,IS).
piles_joueur(ETAT,J,[PILE|R],COUNT,[COUNT|R1]):-
			case(ETAT,CASE,S),
			joueur_case(CASE,J),
			!,
			pile(CASE,PILE),
			COUNT1 is COUNT+1,
			piles_joueur(S,J,R,COUNT1,R1).
piles_joueur(ETAT,J,PILES,COUNT,INDEXES):-
			case(ETAT,CASE,S),
			not(joueur_case(CASE,J)),
			!,
			COUNT1 is COUNT+1,
			piles_joueur(S,J,PILES,COUNT1,INDEXES).
			
% pile(+CASE,?INDEXPILE)
% renvoie la liste des index du pion à la base de chaque pile possibles pour une case
pile(C,[1,2,3]):-length(C,Y),Y >= 3,!.
pile(C,[1,2]):-length(C,2),!.
pile(C,[1]):-length(C,1),!.
						
% coups_pile(CASE,INDEX,COUPS)
% donnes une liste de tous les coups possibles pour la pile donnée (qui correspond à la case d'index INDEX)

coups_pile(P,I,CO):-findall(COUPS,coups_pile1(P,I,COUPS),CO).

coups_pile1(P,I,[DEP,ARR,IND]):-
						DEP is I,
						member(IND,P),
						member(ARR,[1,2,3,4,5,6,7,8,9]),
						longueur_deplacement(DEP,ARR,IND).
						
coups_pile1(P,I,[DEP,ARR,3]):-
						DEP is I,
						member(3,P),
						member(ARR,[1,2,3,4,5,6,7,8,9]),
						longueur_deplacement(DEP,ARR,1).
						
% coups_possibles_joueur(+ETAT,+JOUEUR,-COUPS)
% Retourne la liste des coups possibles pour le joueur JOUEUR

coups_possibles_joueur(ETAT,JOUEUR,COUPS):-
						piles_joueur(ETAT,JOUEUR,PILES,INDEXES),
						coups_possibles_joueur1(PILES,INDEXES,COUPS).
						
coups_possibles_joueur1([],[],[]).						
coups_possibles_joueur1([P|R1],[I|R2],COUPS):-
						coups_pile(P,I,C1),
						append(C1,C2,COUPS),
						coups_possibles_joueur1(R1,R2,C2).
					
% coup_max(+ETAT,+COUPS,-COUP,-NETAT)
% Cette fonction prend un état, une liste de coups, et ressort le coup COUP qui amène à l'état NETAT évalué le plus haut possible

coup_max(_,[C],C,_).

coup_max(ETAT,[[D,A,I]|R],[DM,AM,IM],MAXETAT):-
					coup_max(ETAT,R,[DM1,AM1,IM1],MAXETAT1),
					nouvel_etat(ETAT,D,A,I,NETAT),
					eval(MAXETAT1,EV1),
					eval(NETAT,EV),
					(EV >= EV1,!, MAXETAT = NETAT, DM is D, AM is A, IM is I;
					 EV < EV1,!, MAXETAT = MAXETAT1,DM is DM1, AM is AM1, IM is IM1).
				
						
% meilleur_coup(+ETAT,+JOUEUR,-COUP,-NETAT)
% La fonction meilleur_coup va ressortir COUP, le coup qui mènera vers le meilleur nouveau etat NETAT pour JOUEUR
% Le joueur 1 va chercher à MAXIMISER l'évaluation de NETAT
% Le joueur 0 va chercher à MINIMISER     ''       ''   ''

meilleur_coup(E,1,C,NE):-
				coups_possibles_joueur(E,1,CS),
				meilleur_coup(E,1,CS,C,NE).
				
meilleur_coup(E,1,CS,C,NE):- coup_max(E,CS,C,NE).
				
% minmax(+ETAT,+JOUEUR,+DEPTH,-COUP)
% minmax prend l'état actuel, ainsi que le joueur qui doit jouer, et ressort le meilleur coup que doit joueur JOUEUR
% la profondeur de la recherche est caractérisée par DEPTH



