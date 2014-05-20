% Testing AIPlayer with one fact

% ** ** **      * *** **
%           =>            with * white pawns and - black pawns
% -- -- --      -- -- --

% think(1, 1, 2, [[0, 0], [0, 0], [0, 0], [], [], [], [1, 1], [1, 1], [1, 1]], 0).

% think(nCaseStart, idxPawn, nCaseEnd, Board, colorPlayer)
% We should add another predicate with the current evaluation

% 1 2 3
% 4 5 6
% 7 8 9 : Board configuration here

% idxPawn : [0 1 2 3 ...]

% Colors : White = 1, Black = 0

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% White: 1, Black: 0
% Board is coded as following: -1 means "go to next case"
%				Then we have pawns, from top to bottom
%				and again -1 to move forward
% Normally for the answer: we return the position of the cases/pawns
%			   Beginning with 1 -> n
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

think(1, 1, 2, [1, 1, -1, 1, 1, -1, 1, 1, -1, -1, -1, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1], 0).
% Initial situation description. We move the first pawn in stack #1 to #2.




