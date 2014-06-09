:-module(mod_ui,[play/0,printBoard/1]).
:-use_module('pogo_ia.pl').


% Pour sauvegarder l'�tat du jeu
:- dynamic actualState/1.

% actualState(-ETAT)
% conserve l'�tat actuel du plateau. Ce pr�dicat est dynamique, on y ins�re sans arr�t le nouveau �tat, et on supprime l'ancien
% il ne contient donc pas de clause � proprement parler

% initial_state(-ETAT)
% renvoie l'�tat initial d'une partie

initial_state(ETAT):-
		ETAT = [1,1,-1,1,1,-1,1,1,-1,-1,-1,-1,0,0,-1,0,0,-1,0,0,-1].

% set_initial_state/0
% ce pr�dicat enregistre l'�tat initial en tant qu'�tat actuel. N�cessaire pour le tout d�but du jeu.
		
set_initial_state:-
    retractall(actualState(_)),
    initial_state(E),
    assert(actualState(E)).
		
% saveState(+COUP)
% Sauvegarde le coup jou�
saveState([D,A,I]) :-
    retract(actualState(E)),
    nouvel_etat(E,D,A,I,E1),
    assert(actualState(E1)).
	
	

	
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%
%		PARTIE INTERFACE GRAPHIQUE
%
%
%

% ask_placement(+ETAT,-COUP,+JOUEUR)
% demande au joueur un d�placement valide

ask_placement(PL, [D,A,I], JOUEUR) :-
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
play_ia(P1, JOUEUR) :-
	actualState(PL),
	Level1 is (Level + 1) * 2,
	minmax(PL,+JOUEUR,+DEPTH,[D,A,I]), !,
	nouvel_etat(PL,D,A,I,P1),
	saveState([D,A,I]),
	actualState(NPL),
	pintBoard(NPL),
	not(won),
	play(PL, JOUEUR).

% play(+ETAT,+JOUEUR)
% fonction de jeu du joueur (non IA)

play(LastBoard, JOUEUR) :-
	actualState(PL),
	ask_placement(PL, [D,A,I], JOUEUR),
	saveState([D,A,I]),
	actualState(NPL),
	printBoard(NPL),
	not(won),
	play_ia(LastBoard, JOUEUR).

play :-
	write('Quels pions jouer ? :\n'),
	write('\t0.\tCROIX (noirs)\n'),
	write('\t1.\tRONDS (blancs)\n'),
	ask_player(JOUEUR),
	initial_state(E),
	play(E, JOUEUR).


% play/0
% fonction sans param�tres qui est appell�e au d�part. D�marre tout simplement le jeu.
% AJOUTER UNE FONCTION PLAY et PLAY_IA pour demander au joueur et � l'IA le coup qu'ils veulent jouer.

play :-
	write('Niveau (IA) :\n'),
	write('\t0.\tCROIX (noirs)\n'),
	write('\t1.\tRONDS (blancs)\n'),
	ask_player(JOUEUR),
	initial_state(E),
	play(E, JOUEUR).

% ask_player(-ID)
% demande au joueur s'il souhaite incarner les noirs (croix) ou les ronds (blancs), avec v�rification de la r�ponse.
	
ask_player(ID) :-
	read(ID),
	integer(ID),
	between(0, 1, ID), !.
	
ask_player(ID) :-
	writeln('Choix invalide. Options : 0 ou 1\n'),
	ask_id(ID).

% printBoard(+ETAT)
% La fonction printBoard prend en param�tre un �tat, et en ressort une repr�sentation console de ce dernier

printBoard(ETAT):- % BUG QUAND LA DERNIERE CASE DES VIDE !?
			cases(ETAT,CASES,INDEXES),
			writeline(48),
			printBoard1(CASES,INDEXES,1).
			
printBoard1([],[],10):- % plateau fini d'afficher
		write('\n'),
		writeline(48),!.
		
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
		print_case_and_blanks(CASE),
		NUM1 is NUM+1,
		printBoard1(R1,R2,NUM1),!.
		
printBoard1(CASELIST,[INDEX|R2],NUM):-
		INDEX \= NUM,
		print_case_and_blanks([]),
		NUM1 is NUM+1,
		printBoard1(CASELIST,[INDEX|R2],NUM1),!.
		
printBoard1([CASE|R1],[INDEX|R2],NUM):-
		NUM = INDEX,
		print_case_and_blanks(CASE),
		NUM1 is NUM+1,
		printBoard1(R1,R2,NUM1),!.
		
% printligne(+CASE1,+CASE2,+CASE3)
% �crite une ligne de 3 cases

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
% �crit le contenu d'une case. Blanc = O, noir = X

printCase([]):-!.

printCase([PION|R]):-
		(PION = 1,write('O');
		 PION = 0,write('X')),!,
		 printCase(R).
		 
% printBlanks(+LENGTH)
% �crit des espaces sur une longueur LENGTH

printBlanks(0):-!.

printBlanks(Length):-
			write(' '),
			Length2 is Length-1,
			printBlanks(Length2).