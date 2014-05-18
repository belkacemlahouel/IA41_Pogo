% Testing AIPlayer with one fact

% ** ** **      * *** **
%           =>            with * white pawns and - black pawns
% -- -- --      -- -- --

think(1, 1, 2, [[0, 0], [0, 0], [0, 0], [], [], [], [1, 1], [1, 1], [1, 1]], 0).

% think(nCaseStart, idxPawn, nCaseEnd, Board, colorPlayer)
% We should add another predicate with the current evaluation

% 1 2 3
% 4 5 6
% 7 8 9 : Board configuration here

% idxPawn : [0 1 2 3 ...]

% Colors : White = 1, Black = 0

