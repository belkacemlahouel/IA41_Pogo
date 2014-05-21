#include "AIPlayer.h"

// -------------------------------------------------------------------------
// Constructor
// Initialization of the Prolog Interface
// "prgp.exe" is the program we will get after adding Prolog predicates
// -------------------------------------------------------------------------
AIPlayer::AIPlayer(bool isW) {
    m_PrologInterface.init("prgp.exe");
    isWhite = isW;
}

// -------------------------------------------------------------------------
// Using Prolog predicates to think
// "think " is the name of the predicate we will use
// "3" is the number of parameters for "think":
// -------------------------------------------------------------------------
// think([ETAT], cJoueur, [nCaseDepart, nCaseArrivee, indexPionStack]).
// -------------------------------------------------------------------------
void AIPlayer::think() {
    if (m_PrologInterface.start("think", 3)) {
        // Board creation in Prolog format
        int etat[21], n = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                Case* ca = &(board->board[i][j]);
                list<Pawn*>* pawns = &(ca->pawnList);
                list<Pawn*>::iterator it;

                // Ordre stack : top = premier
                for (it = pawns->end(); it != pawns->begin(); --it) {
                    Pawn* tmp = *it;
                    if (tmp->getIsWhite())       etat[n] = 0; // White pawn
                    else                         etat[n] = 1; // Black pawn
                }
                etat[n] = -1;                                 // Next case
                ++ n;
            }
        }
        // On suppose qu'on a l'état comme il faut dans un format C++

        // On crée l'état au format Prolog
        term_t hEtat = (m_PrologInterface.funcNewTermRef)();

        // On met la liste au format C++ dans un format Prolog
        m_PrologInterface.putList(hEtat, 21, etat);

        // On construit la liste
        // et on dit que c'est le premier terme du predicat Prolog
        m_PrologInterface.consList(m_PrologInterface.FirstTerm, hEtat);


        int cPlayer[1];
        cPlayer[0] = 1; /*...Récupération du programme...*/
        term_t hCPlayer = (m_PrologInterface.funcNewTermRef)();
        m_PrologInterface.putList(hCPlayer, 1, cPlayer);
        term_t ptrTerm = m_PrologInterface.FirstTerm+1;
        m_PrologInterface.consList(ptrTerm, hCPlayer);


        term_t hReponse = m_PrologInterface.FirstTerm+2;

        if (m_PrologInterface.call()) {
            std::vector<int> reponse;
            m_PrologInterface.getList(hReponse, reponse);
        }

        m_PrologInterface.finish();

    }
}

// -------------------------------------------------------------------------
// After getting the result from Prolog
// We have the index and the n° of the case of the pawn -> we move it
// We use setSelected and movePawn from Pawn
// And we move it to the case Prolog gave us
// -------------------------------------------------------------------------
void AIPlayer::play(QEventLoop* ev) {

}
