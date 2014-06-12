:-module(mod_ia,[coups_possibles_joueur/3,minmax/5,inverser_joueur/2,cases/3,nouvel_etat/5,eval/3]).
:- use_module('gui.pl').

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


% ------------------------
% ------------------------
% Appel de la fonction d'évaluation en fonction du niveau...
% eval(1, ETAT, E) :- eval1(ETAT, E, 0), !.
% eval(0, ETAT, E) :- eval0(ETAT, E, 0), !.
% eval(2, ETAT, E) :- eval2(ETAT, E, 0), !.
% ... à modifier de toutes façons
% ------------------------
% ------------------------

% ------------ APPEL DE L'EVALUATION DEPUIS LA FONCTION DE JEU ------------
eval(ETAT, E, LEVEL) :- 
	(LEVEL = 0,eval4(ETAT, E),!;
	 LEVEL = 1,eval0(ETAT, E),!;
	 LEVEL = 2,eval2(ETAT, E)). % ETAT, EVAL, COMPTEUR (4 premiers pions)
% ------------------------
% ------------------------

% -----------------------------------------------------------------------------
% -----------------------------------------------------------------------------
% -----------------------------------------------------------------------------
% eval1([ETAT], EVAL) : 4 premiers pions
% -----------------------------------------------------------------------------
eval1([], 0, 0) :- !.
eval1([-1|R], E, _) :- 	eval1(R, E, 0), !.		% Si on trouve un -1, C <- 0
eval1([_|R], E, 4) :-	eval1(R, E, 4), !.		% Si C = 4, vers la prochaine

eval1([X|R], E, C) :- 	eval_pion(X, XE),	% Sinon, on evalue le pion
 						C1 is C+1,			% On incrémente le compteur
						eval1(R, E1, C1),	% On évalue le reste
						E is E1+XE.			% On mets à jour l'éval

% -----------------------------------------------------------------------------
% -----------------------------------------------------------------------------
% -----------------------------------------------------------------------------
% eval0([ETAT], EVAL) : nombre de stacks contrôlés par un joueur
% -----------------------------------------------------------------------------
eval0(ETAT, E):-
	(won(ETAT,0),!,E = -9999; % Si l'état est un état de victoire, c'est directement la meilleure évaluation
	 won(ETAT,1),!,E = 9999).
	 
eval0(ETAT,E):-				  % Sinon, on fait l'évaluation standard
	eval2(ETAT,E,0).

eval0([], 0, 0) :- !.
eval0([-1|R], E, _) :- 	eval0(R, E, 0), !.		% Si on trouve un -1, C <- 0
eval0([_|R], E, 1) :-	eval0(R, E, 1), !.		% Si C = 1, vers la prochaine

eval0([X|R], E, C) :- 	eval_pion(X, XE),	% Sinon, on evalue le pion
 						C1 is C+1,			% On incrémente le compteur
						eval0(R, E1, C1),	% On évalue le reste
						E is E1+XE.			% On mets à jour l'éval
						
% -----------------------------------------------------------------------------
% -----------------------------------------------------------------------------
% -----------------------------------------------------------------------------
% eval2([ETAT], EVAL) : 4 premiers pions + poids QUADRUPLE pour le contrôle
%	MEILLEUR EVAL POUR LE MOMENT !!
% 	A FAIRE : privilégier les coups coups qui créent une tour alliée (encore plus de poids pour le contrôle ?)
% -----------------------------------------------------------------------------

eval2(ETAT,E):-
	(won(ETAT,0),!,E = -9999; % Si l'état est un état de victoire, c'est directement la meilleure évaluation
	 won(ETAT,1),!,E = 9999).
	 
eval2(ETAT,E):-				  % Sinon, on fait l'évaluation standard
	eval2(ETAT,E,0).
	
eval2([], 0, 0) :- !.
eval2([-1|R], E, _) :- 	eval2(R, E, 0), !.		% Si on trouve un -1, C <- 0
eval2([_|R], E, 4) :-	eval2(R, E, 4), !.		% Si C = 1, vers la prochaine

eval2([X|R], E, 0) :- 	eval_pion(X, XE),	% Sinon, on evalue le pion
						XE2 is XE*4,		% Poids plus élevé pour la première pièce d'un stack
						eval2(R, E1, 1),	% On évalue le reste
						E is E1+XE2.		% On mets à jour l'éval

eval2([X|R], E, C) :- 	eval_pion(X, XE),	% Sinon, on evalue le pion
 						C1 is C+1,			% On incrémente le compteur
						eval2(R, E1, C1),	% On évalue le reste
						E is E1+XE.			% On mets à jour l'éval
						
% -----------------------------------------------------------------------------
% -----------------------------------------------------------------------------
% -----------------------------------------------------------------------------
% eval4(+ETAT, -EVAL) : compte le nombre de coups pour chaque joueur, l'évaluation est la différence entre les coups de MAX et ceux de MIN
% -----------------------------------------------------------------------------

eval4(ETAT,E):-
	(won(ETAT,0),!,E = -9999; % Si l'état est un état de victoire, c'est directement la meilleure évaluation
	 won(ETAT,1),!,E = 9999).

eval4(ETAT, EVAL) :- 	coups_possibles_joueur(ETAT, 1, COUPSW),
						length(COUPSW,W),
						coups_possibles_joueur(ETAT, 0, COUPSB),
						length(COUPSB,B),
						EVAL is W-B.

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

% cases(+ETAT,-CASES,-INDEXES)
% Renvoie 2 Lists : CASES, la liste du contenu de chaque cases du plateau
% INDEXES, la liste des index de chaque case du plateau, correspondant aux cases dans CASES (dans le même ordre)
	
cases(E,C,L):-cases(E,C,1,L),!.
cases([],[],_,[]):-!.
cases([-1],[],_,[]):-!.
cases([-1|LC],C,COUNT,IS):-!,
COUNT < 10,
COUNT1 is COUNT+1,
cases(LC,C,COUNT1,IS).
cases(ETAT,[CASE|R],COUNT,[COUNT|R1]):-
case(ETAT,CASE,S),
		!,
		COUNT1 is COUNT+1,
		cases(S,R,COUNT1,R1).

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
						
% coups_pile(+PILE,+INDEX,COUPS)
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
					
% etats_pile(+ETAT,+PILE,+INDEX,-ETATS)
% retourne tous les états ETATS possibles amenés par le mouvement des piles PILE, à partir d'un état ETAT

etats_pile(ETAT,PILE,INDEX,ETATS):-findall(ETS,etats_pile1(ETAT,PILE,INDEX,ETS),ETATS).

etats_pile1(ETAT,P,I,NE):-
						DEP is I,
						member(IND,P),
						member(ARR,[1,2,3,4,5,6,7,8,9]),
						longueur_deplacement(DEP,ARR,IND),
						nouvel_etat(ETAT,DEP,ARR,IND,NE).
						
etats_pile1(ETAT,P,I,NE):-
						DEP is I,
						member(3,P),
						member(ARR,[1,2,3,4,5,6,7,8,9]),
						longueur_deplacement(DEP,ARR,1),
						nouvel_etat(ETAT,DEP,ARR,3,NE).
						
% etats_possibles_joueur(+ETAT,+JOUEUR,-NETATS)
% retourne tous les états possibles à partir d'un état (ces états sont
% amenés grâce aux coups possibles du joueur

etats_possibles_joueur(ETAT,JOUEUR,NETATS):-
						piles_joueur(ETAT,JOUEUR,PILES,INDEXES),
						etats_possibles_joueur1(ETAT,PILES,INDEXES,NETATS).

etats_possibles_joueur1(_,[],[],[]).
etats_possibles_joueur1(ETAT,[P|R1],[I|R2],NETATS):-
						etats_pile(ETAT,P,I,E1),
						append(E1,E2,NETATS),
						etats_possibles_joueur1(ETAT,R1,R2,E2).
				
% minmax(+ETAT,+JOUEUR,+DEPTH,-COUP, -EVALETAT +LEVEL)
% minmax prend l'état actuel, ainsi que le joueur qui doit jouer, et ressort le meilleur coup que doit joueur JOUEUR
% la profondeur de la recherche est caractérisée par DEPTH. On donne des profondeurs différentes en fonction du niveau du joueur.
% EVALETAT est là en particulier pour le debug

minmax(ETAT,JOUEUR,BESTCOUP,EVALETAT,LEVEL):-
				(LEVEL = 0,!, DEPTH = 3, alphabeta(ETAT,JOUEUR,LEVEL,-10000,10000,BESTCOUP,EVALETAT,DEPTH);
				LEVEL = 1,!, DEPTH = 3, alphabeta(ETAT,JOUEUR,LEVEL,-10000,10000,BESTCOUP,EVALETAT,DEPTH);
				LEVEL = 2, DEPTH = 4, alphabeta(ETAT,JOUEUR,LEVEL,-10000,10000,BESTCOUP,EVALETAT,DEPTH)).
				 % -10000 et 10000 sont des valeurs excessivement grandes pour simuler +inf et -inf

% alphabeta(+ETAT,+JOUEUR,+ALPHA,+BETA,?BESTCOUP,?BESTEVAL,+DEPTH)
% effectue un minmax avec élagage alpha-beta
% la décrémentation de la profondeur, recherche des coups possibles pour le joueur, et on
% continue en profondeur tant qu'on peut
				
alphabeta(ETAT, JOUEUR, LEVEL, Alpha, Beta, BESTCOUP, BESTEVAL, Depth) :-
	Depth > 0,
	OneDeeper is Depth - 1,
	coups_possibles_joueur(ETAT, JOUEUR, L),
	length(L,LMOVES),
	LMOVES > 0,
	!,
	boundedbest(ETAT,L,LEVEL, Alpha, Beta, JOUEUR, OneDeeper, BESTCOUP, BESTEVAL).

alphabeta(ETAT, _,LEVEL, _, _, _, Val, 0) :- % Profondeur atteinte, on évalue la feuille
		eval(ETAT,Val,LEVEL),!.
		
alphabeta(ETAT, _,LEVEL, _, _, _, Val, _) :- % Si plus de coup avant d'avoir atteint la profondeur 0
		eval(ETAT,Val,LEVEL).

% boundedbest(+ETAT,+COUPLISTE,+LEVEL, +Alpha, +Beta, +JOUEUR, +Depth, ?BESTCOUP, ?BESTVAL)
% la fonction boundedbest va s'occuper de faire le lien entre alphabeta (qui évalue les branches)
% et goodenough (qui est chargée de comparer ces branches)
		
boundedbest(ETAT,[[D,A,I]|NEXTCOUPS],LEVEL, Alpha, Beta, JOUEUR, Depth, BESTCOUP, BESTVAL) :-
  inverser_joueur(JOUEUR, J2),
  nouvel_etat(ETAT,D,A,I,NEXTETAT),
  alphabeta(NEXTETAT, J2,LEVEL, Alpha, Beta, _BESTCOUP, Val, Depth),
  goodenough(ETAT,NEXTCOUPS,LEVEL,Depth, Alpha, Beta, JOUEUR, [D,A,I], Val, BESTCOUP, BESTVAL).

% goodenough(+ETAT,+MOVELIST,+LEVEL,+Depth, +Alpha, +Beta, +JOUEUR, +COUP, Val, BESTCOUP, BESTEVAL)
% goodenough est le prédicat chargé d'évaluer de comparer les évaluations qu'il reçoit avec
% alpha et beta. C'est lui qui se charge de cut (de l'élagage)
  
goodenough(_,[],_,_, _, _, _, COUP, Val, COUP, Val) :-!.   % On a fini la liste de coups

goodenough(_,_,_,_, Alpha, Beta, JOUEUR, COUP, Val, COUP, Val) :- % Cas de cut
  JOUEUR = 0, Val > Beta, !                 % MIN a dépassé beta
  ;
  JOUEUR = 1, Val < Alpha, !.               % MAX est passé sous alpha

goodenough(ETAT,MOVELIST,LEVEL,Depth, Alpha, Beta, JOUEUR, COUP, Val, BESTCOUP, BESTEVAL)  :-
  newbounds(Alpha, Beta, JOUEUR, Val, NewAlpha, NewBeta),
  boundedbest(ETAT,MOVELIST,LEVEL, NewAlpha, NewBeta, JOUEUR, Depth, COUP1, Val1),
  betterof(JOUEUR, COUP, Val, COUP1, Val1, BESTCOUP, BESTEVAL).

% newbounds(+Alpha, +Beta, +JOUEUR, +Val, -NewAlpha, -NewBeta)
% ce prédicat est chargé de mettre à jour Alpha ou Beta selon les conditions rencontrées
 
newbounds(Alpha, Beta, JOUEUR, Val, Val, Beta)  :-
  JOUEUR = 0, Val > Alpha, !.               % Pour MIN, Lower bond augmente

newbounds(Alpha, Beta, JOUEUR, Val, Alpha, Val)  :-
  JOUEUR = 1, Val < Beta, !.                % Pour MAX, upper bond diminue

newbounds(Alpha, Beta, _, _, Alpha, Beta).           % Rien ne change

% betterof(+JOUEUR, +COUP1, +Val1, +COUP2, +Val2, -BESTCOUP, -BESTEVAL)
% Choisit le meilleur coup entre COUP1 et COUP2, et le stocke dans BESTCOUP (et BESTEVAL)
% Si les coups sont égaux, on choisit l'un des deux au hasard.

betterof(JOUEUR, COUP1, Val1, _, Val2, COUP1, Val1)  :-  % COUP1 est meilleur que COUP2
  JOUEUR = 1, Val1 > Val2, !		% rappel : pour MAX (1), on cherche la valeur la plus HAUTE !!
  ;
  JOUEUR = 0, Val1 < Val2, !.
  
betterof(_, COUP1, Val1, COUP2, Val2, RANDCOUP, RANDVAL)  :-  % COUP1 et COUP2 sont égaux : on fait en random
  Val1 = Val2, 
  random(0,2,R),
  (R = 0, RANDCOUP = COUP1, RANDVAL = Val1,!;
   R = 1, RANDCOUP = COUP2, RANDVAL = Val2).

betterof(_, _, _, COUP2, Val2, COUP2, Val2).     % sinon COUP2 est meilleur

% inverser_joueur(+J1,-J2).
% transforme J1 = 1 en J2 = 0 et vice versa

inverser_joueur(1,0).
inverser_joueur(0,1).

