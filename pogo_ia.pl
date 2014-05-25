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

eval1(ETAT, E) :- 	E is 0,
			eval1(ETAT, E, 0). % ETAT, EVAL, COMPTEUR (4 premiers pions)

eval1([], _, 0).
eval1([_|R], E, 4) :-	eval1(R, E, 4).		% Si C = 4, vers la prochaine
eval1([-1|R], E, _) :- 	eval1(R, E, 0).		% Si on trouve un -1, C <- 0
eval1([X|R], E, C) :- 	eval_pion(X, XE),	% Sinon, on evalue le pion
			C1 is C+1,		% On incrémente le compteur
			E1 is E+XE,		% On mets à jour l'éval
			eval1(R, E1, C1).	% On évalue le reste







