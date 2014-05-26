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
eval1(ETAT, E) :- eval1(ETAT, E, 0). % ETAT, EVAL, COMPTEUR (4 premiers pions)

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











