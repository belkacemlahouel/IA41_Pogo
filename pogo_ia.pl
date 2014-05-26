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
% E0 est l'état E sans la pile P située sur la case D, du 1er au pion #I (inc)
% -----------------------------------------------------------------------------

enleve_pile(E, D, I, E0, P) :- enleve_pile(E, D, I, E0, P, 1). % Compteur cases

% ----- Chercher la case

enleve_pile_c([-1|R], D, I, E0, P, CC) :- CC1 is CC+1,
										  enleve_pile(R, D, I, E0, P, CC1).

enleve_pile_c(E, D, I, E0, P, D) :- enleve_pile_i(E, D, I, E0, P, D, 1).

enleve_pile_c([X|R], D, I, [X|R0], P, CC) :-
										enleve_pile_c(R, D, I, R0, P, CC).

enleve_pile_c([], _, _, [], _, 10) :- !.
% Quand le parcours est fini, on pointe sur la case après la neuvième case (?)

% ----- Chercher l'index du pion

% Problème si la pile est plus grande que le nombre de pions ?
% Il ne faut pas que ce prédicat recoive des informations incohérentes !
% Genre bouger le pion n°3 alors que la pile est de taille 1
% Checker la couleur aussi, avant de lui demander ça

% Pile ne contient que X, les autres seront ajoutés dessus, lors du retour
enleve_pile_i([X|R], D, I, E0, [X], CC, I) :-
										enleve_pile_c(R, D, I, E0, [X], CC).
enleve_pile_i([X|R], D, I, E0, [X|P], CC, CI) :-
										CI1 is CI+1,
										enleve_pile_i(R, D, I, E0, P, CC, CI1).

% ----- Déplacer le pion et ceux qui sont au dessus
% ----- Faut réorganiser l'ordre des priorités...

% Compteur de cases ajouté
insere_pile(E0, P, A, E1) :- insere_pile(E0, P, A, E1, 1).

% Si plus rien à insérer dans la liste, alors on peut continuer comme avant.
% Voir avec les priorités d'une nouvelle case...
insere_pile([X|R0], [], A, [X|R1], A) :- insere_pile(R0, [], A, R1, A).

% Si il reste des trucs dans la pile, on les insère
% /!\ VERIFIER que l'ordre LIFO de la pile ne change pas...
insere_pile(E0, [X|R], A, [X|R1], A) :- insere_pile(E0, R, A, R1, A).

% Cas usuel
insere_pile([X|R0], _, A, [X|R1], CC) :- insere_pile(R0, _, A, R1, CC).
insere_pile([-1|R0], _, A, [-1|R1], CC) :- 	CC1 is CC+1,
											insere_pile(R0, _, A, R1, CC1).

% Si on arrive sur la case #10 (après la fin)
% Alors les deux états (nouveau et ancien) sont vides, et la pile également
insere_pile([], [], _, [], 10) :- !.










