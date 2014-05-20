% thinktmp(nCaseDepart, nCaseArrivee, indexPion, ETAT, cPlayer)
% Avec ETAT codé comme suit : on commence a la case 1
% Jusqu à la case 9
% Avec 1 pour white, 0 pour noir, ordre stack (en haut = premier)
% Et -1 pour dire "on change de case"
% Les pions sont indexés de 1 à n

thinktmp(1, 2, 1, [1, 1, -1, 1, 1, -1, 1, 1, -1, -1, -1, -1, 0, 0 -1, 0, 0, -1, 0, 0, -1], 1).

