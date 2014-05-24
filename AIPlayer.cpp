#include "AIPlayer.h"

// -------------------------------------------------------------------------
// Constructor
// Initialization of the Prolog Interface
// "prgp.exe" is the program we will get after adding Prolog predicates
// -------------------------------------------------------------------------
AIPlayer::AIPlayer(bool isW, int lvl, Board* b) {
    qDebug() << "\t\t\t\t\t\tINSTANCIATION AI\n";
    m_PrologInterface.init("prgp.exe");
    isWhite = isW;
    level = max(0, min(3, lvl)); // 0 <= level <= 3
    board = b;
}

// -------------------------------------------------------------------------
// Using Prolog predicates to think
// "think " is the name of the predicate we will use
// "3" is the number of parameters for "think"
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


        // On pourrait se débrouiller pour mettre ça dans l'état
        // Contient des infos player : couleur, niveau
        int cPlayer[2];

        // Récupération des infos du joueur : couleur
        if (isWhite) cPlayer[0] = 1;
        else         cPlayer[0] = 0;

        cPlayer[1] = level;

        term_t hCPlayer = (m_PrologInterface.funcNewTermRef)();
        m_PrologInterface.putList(hCPlayer, 2, cPlayer);
        term_t ptrTerm = m_PrologInterface.FirstTerm+1;
        m_PrologInterface.consList(ptrTerm, hCPlayer);


        term_t hReponse = m_PrologInterface.FirstTerm+2;

        if (m_PrologInterface.call()) {
            std::vector<int> reponse;
            m_PrologInterface.getList(hReponse, reponse);

            // Enregistrement des infos dans les attributs du joueur
            nCaseDepart = reponse[0];
            nCaseArrivee = reponse[1];
            indexPionStack = reponse[2];
        }

        m_PrologInterface.finish();
        // On a finit de "réfléchir"
    }
}

// -------------------------------------------------------------------------
// After getting the result from Prolog
// We have the index and the n° of the case of the pawn -> we move it
// We use setSelected and movePawn from Pawn
// And we move it to the case Prolog gave us
// -------------------------------------------------------------------------
void AIPlayer::play(QEventLoop* pause) {
    qDebug() << "\t\t\t\t\t\tAIPLAYER::PLAY";

    // pause->exec(); // Mets en pause et attends pour le coup du joueur

    // On appelle la réflexion via Prolog
    // think();

        /* ********************* */
        // Supposons le prédicat Prolog appelé et les résultats obtenus
        // On suppose que le coup est possible
        nCaseDepart = 1;
        nCaseArrivee = 2;
        indexPionStack = 1;
        /* ********************* */

    // On récupère le pion et on le sélectionne ############################
    Pawn* selectedPawn;
    list<Pawn*>::iterator it;
    int k = 1;
    int iD, jD;
    int iA, jA; // Coordonnées des cases de Départ/Arrivée, à la mode c++

    qDebug() << "\t\t\t\t\t\t\tCALCULS COORD";

    // Attention2 : Les index des cases commencent à 1
    // Calcul des coordonnées iD, jD
    jD = (nCaseDepart-1)/3;
    iD = (nCaseDepart-1)%3;

    // Calcul des coordonnées iA, jA
    jA = (nCaseArrivee-1)/3;
    iA = (nCaseArrivee-1)%3;

    qDebug() << "\t\t\t\t\t\t\tCOORD: " << iD << ", " << jD << " --> "
                                        << iA << ", " << jA;


    qDebug() << "\t\t\t\t\t\t\tSEARCHING PAWN";

    // Attention : l'index du prédicat est en mode Stack
    //      Dans le code c++, on utilise des listes
    //      Par exemple, le pion 1 (Prolog) -> dernier pion de la liste
    // for (it = board->board[iD][jD].pawnList.end(); k < indexPionStack ; --it) {
    it = board->board[iD][jD].pawnList.end();
    --it;
    qDebug() << "\t\t\t\t\t\t\tLAUCHING SEARCH";
    while (k < indexPionStack) {
        ++k;
        --it;
        qDebug() << "NEXT";
    }

    qDebug() << "\t\t\t\t\t\t\tFOUND PAWN";

    selectedPawn = *it; // Pas sur de cette ligne

    qDebug() << "\t\t\t\t\t\t\tSELECT PION";

    selectedPawn->setSelected(1);
    selectedPawn->pawnSelected(); // on envoie un signal au pawnLabel correspondant
    // Il faut mettre à jour pawnlabel

    // Puis on le fait bouger là où il faut ################################
    // On lui indique le pointeur sur la nouvelle case
    qDebug() << "\t\t\t\t\t\t\tMOVE PION";


    // void BoardGUI::movePawnLabels(Case* oldCase,Case* dest) ?
    board->movePawns(&(board->board[iA][jA]));

    // On déselectionne ? Ou est ce que c'est fait auto ?
    selectedPawn->setSelected(0);
}
