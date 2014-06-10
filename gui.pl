:-module(mod_ui,[main/0,printBoard/1,won/2]).
:-use_module('pogo_ia.pl').

% initial_state(-ETAT)
% renvoie l'état initial d'une partie

initial_state(ETAT):-
		ETAT = [1,1,-1,1,1,-1,1,1,-1,-1,-1,-1,0,0,-1,0,0,-1,0,0,-1].
	

	
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%
%		PARTIE INTERFACE GRAPHIQUE
%
%
%

% main/0
% le prédicat principal du projet, celui à appeler pour démarrer le menu principal

main:-
	write('POGO !! (V 1.0) :\n'),
	write('Que faire ?\n'),
	write('\t0.\tIA vs IA\n'),
	write('\t1.\tIA vs JOUEUR\n'),
	ask_player(REP),
	(REP = 0, 
	write('Choisissez le niveau des blancs :\n'),
	write('\t0.\tFaible\n'),
	write('\t1.\tMoyen\n'),
	write('\t2.\tBon\n'),
	ask_level(LEVEL1),
	write('Choisissez le niveau des noirs :\n'),
	write('\t0.\tFaible\n'),
	write('\t1.\tMoyen\n'),
	write('\t2.\tBon\n'),
	ask_level(LEVEL2),
	play_iaonly(LEVEL1,LEVEL2),!;
	 REP = 1, play).
	

% ask_placement(+ETAT,-COUP,+JOUEUR)
% demande au joueur un déplacement valide

ask_placement(PL, [D,A,I], JOUEUR) :-
	write('\nOrdre des cases :\n'),
	write('\n ---\n|123|\n|456|\n|789|\n ---\n'), % pour montrer au joueur comment son numérotées les cases
	write('\nCase de depart : '),
	read(D),
	write('\nCase d arrivee : '),
	read(A),
	write('\nIndex du pion : '),
	read(I),
	coups_possibles_joueur(PL,JOUEUR,COUPS),
	member([D,A,I],COUPS).

ask_placement(PL, COUP, JOUEUR) :-
	write('Mauvais deplacement... Recommencez.\n'),
	ask_placement(PL, COUP, JOUEUR).	

% play_ia(+ETAT,+JOUEUR)
% fonction de jeu de l'IA

play_ia(ETAT,JOUEUR,LEVEL):-
		minmax(ETAT,JOUEUR,[D,A,I],BESTEVAL,LEVEL),% demander le déplacement
		nouvel_etat(ETAT,D,A,I,NETAT),
		write('\nIA a joué : '), write(D),write(', '),write(A),write(', '),write(I),write('\n'),
		write('Eval etat : '),write(BESTEVAL),write('\n'),
		inverser_joueur(JOUEUR,J1),
		(won(NETAT,JOUEUR),!;
		play_hmn(NETAT,J1,LEVEL)).

% play_hmn(+ETAT,+JOUEUR)
% fonction de jeu du joueur (humain)

play_hmn(ETAT,JOUEUR,LEVEL):-
		printBoard(ETAT),% montrer le plateau
		ask_placement(ETAT,[D,A,I],JOUEUR),% demander le déplacement
		nouvel_etat(ETAT,D,A,I,NETAT),
		printBoard(NETAT),% montrer le nouveau plateau
		inverser_joueur(JOUEUR,J1),
		(won(NETAT,JOUEUR),!;
		play_ia(NETAT,J1,LEVEL)).

% play/0
% fonction sans paramètres qui est appellée au départ. Démarre tout simplement le jeu joueur VS ia

play :-
	write('Choisissez votre camp :\n'),
	write('\t0.\tCROIX (noirs)\n'),
	write('\t1.\tRONDS (blancs)\n'),
	ask_player(JOUEUR),
	write('Choisissez le niveau de IA :\n'),
	write('\t0.\tFaible\n'),
	write('\t1.\tMoyen\n'),
	write('\t2.\tBon\n'),
	ask_level(LEVEL),
	initial_state(E),
	(JOUEUR = 0,play_ia(E,1,LEVEL);		% c'est toujours aux blancs de démarrer
	 JOUEUR = 1,play_hmn(E,JOUEUR,LEVEL)).
	 
% play_iaonly/0
% tout comme play, mais lorsqu'on décide de regarder deux IA jouer l'une contre l'autre

play_iaonly(LEVEL1,LEVEL2) :-
	initial_state(E),
	printBoard(E),
	play_iaonly(E,1,LEVEL1,LEVEL2).
	 
% play_ia(+ETAT,+JOUEUR)
% fonction de jeu de l'IA

play_iaonly(ETAT,JOUEUR,LEVEL1,LEVEL2):-
		JOUEUR = 1,
		minmax(ETAT,JOUEUR,[D,A,I],BESTEVAL,LEVEL1),% demander le déplacement
		nouvel_etat(ETAT,D,A,I,NETAT),
		write('\nIA blancs a joué : '), write(D),write(', '),write(A),write(', '),write(I),write('\n'),
		write('Eval etat : '),write(BESTEVAL),write('\n'),
		printBoard(NETAT),
		inverser_joueur(JOUEUR,J1),
		(won(NETAT,JOUEUR),!;
		play_iaonly(NETAT,J1,LEVEL1,LEVEL2)).
		
play_iaonly(ETAT,JOUEUR,LEVEL1,LEVEL2):-
		JOUEUR = 0,
		minmax(ETAT,JOUEUR,[D,A,I],BESTEVAL,LEVEL2),% demander le déplacement
		nouvel_etat(ETAT,D,A,I,NETAT),
		write('\nIA noirs a joué : '), write(D),write(', '),write(A),write(', '),write(I),write('\n'),
		write('Eval etat : '),write(BESTEVAL),write('\n'),
		printBoard(NETAT),
		inverser_joueur(JOUEUR,J1),
		(won(NETAT,JOUEUR),!;
		play_iaonly(NETAT,J1,LEVEL1,LEVEL2)).

% ask_player(-ID)
% demande au joueur s'il souhaite incarner les noirs (croix) ou les ronds (blancs), avec vérification de la réponse.
	
ask_player(ID) :-
	read(ID),
	integer(ID),
	between(0, 1, ID), !.
	
ask_player(ID) :-
	writeln('Choix invalide. Options : 0 ou 1\n'),
	ask_player(ID).
	
% ask_level(-LEVEL)
% demande au joueur le niveau de l'IA (contre laquelle il va jouer, ou pour l'IA vs IA) 
	
ask_level(LEVEL) :-
	read(LEVEL),
	integer(LEVEL),
	between(0, 2, LEVEL), !.
	
ask_level(LEVEL) :-
	writeln('Choix invalide. Options : 0, 1, ou 2\n'),
	ask_level(LEVEL).

% won(+ETAT,+JOUEUR)
% regarde si JOUEUR est gagnant, à l'état donné ETAT

won(ETAT,JOUEUR):-
		cases(ETAT,CASES,_INDEXES),
		won1(CASES,JOUEUR).
		
won1([],_).

won1([[J|_]|R],J):-
		won1(R,J).
	
% printBoard(+ETAT)
% La fonction printBoard prend en paramètre un état, et en ressort une représentation console de ce dernier

printBoard(ETAT):- % BUG QUAND LA DERNIERE CASE DES VIDE !?
			cases(ETAT,CASES,INDEXES),
			writeline(48),
			printBoard1(CASES,INDEXES,1).
			
printBoard1([],[],10):- % plateau fini d'afficher
		write('\n'),
		writeline(48),!,
		write('\n').
		
printBoard1([],[],NUM):-
		member(NUM,[1,4,7]),
		write('\n'),
		print_case_and_blanks([]),
		NUM1 is NUM+1,
		printBoard1([],[],NUM1),!.
		
printBoard1([],[],NUM):-
		print_case_and_blanks([]),
		NUM1 is NUM+1,
		printBoard1([],[],NUM1),!.
		
printBoard1(CASELIST,[INDEX|R2],NUM):-
		INDEX \= NUM,
		member(NUM,[1,4,7]),
		write('\n'),
		print_case_and_blanks([]),
		NUM1 is NUM+1,
		printBoard1(CASELIST,[INDEX|R2],NUM1),!.
		
printBoard1([CASE|R1],[INDEX|R2],NUM):-
		NUM = INDEX,
		member(NUM,[1,4,7]),
		write('\n'),
		reverse(CASE,RCASE), 		% on inverse le contenu de la case pour une question de logique visuelle : le haut de la pile à droite
		print_case_and_blanks(RCASE),
		NUM1 is NUM+1,
		printBoard1(R1,R2,NUM1),!.
		
printBoard1(CASELIST,[INDEX|R2],NUM):-
		INDEX \= NUM,
		print_case_and_blanks([]),
		NUM1 is NUM+1,
		printBoard1(CASELIST,[INDEX|R2],NUM1),!.
		
printBoard1([CASE|R1],[INDEX|R2],NUM):-
		NUM = INDEX,
		reverse(CASE,RCASE),
		print_case_and_blanks(RCASE),
		NUM1 is NUM+1,
		printBoard1(R1,R2,NUM1),!.
		
% printligne(+CASE1,+CASE2,+CASE3)
% écrite une ligne de 3 cases

print_case_and_blanks(CASE1):-
		write('|'),
		printCase(CASE1),
		length(CASE1,LCASE1),
		NBBLANKS1 is 14 - LCASE1,
		printBlanks(NBBLANKS1),
		write('|').
		
% writeLine()
% La fonction writeline dessine une ligne de 14*3 tirets

writeline(0):-!.

writeline(Length):-
			write(-),
			Length2 is Length-1,
			writeline(Length2).
			
% printCase(+CASE)
% écrit le contenu d'une case. Blanc = O, noir = X

printCase([]):-!.

printCase([PION|R]):-
		(PION = 1,write('O');
		 PION = 0,write('X')),!,
		 printCase(R).
		 
% printBlanks(+LENGTH)
% écrit des espaces sur une longueur LENGTH

printBlanks(0):-!.

printBlanks(Length):-
			write(' '),
			Length2 is Length-1,
			printBlanks(Length2).