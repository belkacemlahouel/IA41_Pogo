#include "AIPlayer.h"

// -------------------------------------------------------------------------
// Constructor
// Initialization of the Prolog Interface
// "prgp.exe" is the program we will get after adding Prolog predicates
// -------------------------------------------------------------------------
AIPlayer::AIPlayer(bool isW, int lvl, Board* b) {
    qDebug() << "\t\t\t\t\t\tINSTANCIATION AI\n";
    m_PrologInterface.init("pogo.exe");
    isWhite = isW;
    level = max(0, min(3, lvl)); // 0 <= level <= 3
    board = b;
}

/* deuxième version du prédicat think, juste pour le test*/
void AIPlayer::think3(){

    if (m_PrologInterface.start( "think", 2 )){
        term_t stimulus_list = (m_PrologInterface.funcNewTermRef)();
        term_t hResponse;
        int32 stimulus_size;
        int rep[3];

        hResponse = m_PrologInterface.FirstTerm+1;
        m_PrologInterface.cleanList( m_PrologInterface.FirstTerm );
        stimulus_size = 3;
        float64* m_StimulusRawData = new float64 [stimulus_size];

        for( int32 i = 0; i < stimulus_size; ++i )
        {
            m_StimulusRawData[i] = (float64) i;
        }

        m_PrologInterface.putList( stimulus_list, stimulus_size,
        m_StimulusRawData ); // initialise une liste Prolog à partir d'un tableau de 10éléments 0,1,...,9
        m_PrologInterface.consList( m_PrologInterface.FirstTerm, stimulus_list
        ); // Le premier argument du prédicat “think” est une liste
        delete( m_StimulusRawData );
        std::vector<float64> response;
        if (m_PrologInterface.call())
        {

            m_PrologInterface.getList( hResponse, response ); // récupère uneliste instanciée par le prédicat en tant que vector<float64>
        }

        rep[0] = response[0];
        rep[1] = response[1];
        rep[2] = response[2];

        m_PrologInterface.finish();
    }

}

/* deuxième version du prédicat think, juste pour le test*/
void AIPlayer::think2(){

    if (m_PrologInterface.start( "think", 2 ))
    {
        term_t stimulus_list = (m_PrologInterface.funcNewTermRef)();
        term_t hResponse;
        int32 stimulus_size_etat;
        hResponse = m_PrologInterface.FirstTerm+1;
        m_PrologInterface.cleanList( m_PrologInterface.FirstTerm );
        stimulus_size_etat = 22;
        int32 etat[22];
        float64 etatDebug[22];

        if(isWhite) etat[0] = (int32) 1;
        else        etat[0] = (int32) 0;
        if(isWhite) etatDebug[0] = (float64) 1;
        else        etatDebug[0] = (float64) 0;

        int n = 1;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                Case* ca = &(board->board[j][i]);
                list<Pawn*>& pawns = ca->pawnList;
                list<Pawn*>::reverse_iterator it;

                // Ordre stack : top = premier
                int length = pawns.size();
                for (it = pawns.rbegin(); it != pawns.rend(); ++it) {
                    Pawn* tmp = *it;
                    if (tmp->getIsWhite())       etat[n] = (int32) 1; // White pawn
                    else                         etat[n] = (int32) 0; // Black pawn
                    if (tmp->getIsWhite())       etatDebug[n] = (float64) 1; // White pawn
                    else                         etatDebug[n] = (float64) 0; // Black pawn
                    ++ n;
                }
                // Next case
                etat[n] = (int32) -1;
                etatDebug[n] = -1;
                ++n;
            }
        }

        m_PrologInterface.putList( stimulus_list, stimulus_size_etat, etat ); // initialise une liste Prolog à partir d'un tableau de 10 éléments 0,1,...,9

        m_PrologInterface.consList( m_PrologInterface.FirstTerm, stimulus_list); // Le premier argument du prédicat “think” est une liste

        delete(etat);

        if (m_PrologInterface.call()){
            std::vector<int32> response;
            if(m_PrologInterface.getList( hResponse, response )){ // récupère une liste instanciée par le prédicat en tant que vector<float64>
                nCaseDepart = response[0];
                nCaseArrivee = response[1];
                indexPionStack = response[2];
            } else {
                qDebug()<<"Impossible d'appeller le predicat...\n";
            }
        }
        m_PrologInterface.finish();
    }
}

// -------------------------------------------------------------------------
// Using Prolog predicates to think
// "think " is the name of the predicate we will use
// "3" is the number of parameters for "think"
// -------------------------------------------------------------------------
// think([ETAT], [cJoueur, lvlAI], [nCaseDepart, nCaseArrivee, indexPionStack]).
// -------------------------------------------------------------------------
void AIPlayer::think() {
    if (m_PrologInterface.start("think", 3)) {

        // Board creation in Prolog format
        int32 stimulus_size_etat = 21;
        int32 stimulus_size_player = 2;
        float64* etat = new float64[stimulus_size_etat];

        int n = 0;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                Case* ca = &(board->board[j][i]);
                list<Pawn*>& pawns = ca->pawnList;
                list<Pawn*>::reverse_iterator it;

                // Ordre stack : top = premier
                int length = pawns.size();
                for (it = pawns.rbegin(); it != pawns.rend(); ++it) {
                    Pawn* tmp = *it;
                    if (tmp->getIsWhite())       etat[n] = 1; // White pawn
                    else                         etat[n] = 0; // Black pawn
                    ++ n;
                }
                // Next case
                etat[n] = -1;
                ++n;
            }
        }
        // On suppose qu'on a l'Ã©tat comme il faut dans un format C++

        // On crÃ©e l'Ã©tat au format Prolog
        term_t hEtat = (m_PrologInterface.funcNewTermRef)();
        m_PrologInterface.cleanList(m_PrologInterface.FirstTerm);
        // On met la liste au format C++ dans un format Prolog
        m_PrologInterface.putList(hEtat, stimulus_size_etat, etat);
        delete(etat);

        /* LE BUG PROVIENT D'ENTRE ICI <<<<<<<<<<<<<<<<<<< */

        // On construit la liste
        // et on dit que c'est le premier terme du predicat Prolog
        m_PrologInterface.consList(m_PrologInterface.FirstTerm, hEtat);

        // On pourrait se dÃ©brouiller pour mettre Ã§a dans l'Ã©tat
        // Contient des infos player : couleur, niveau
        float64* cPlayer = new float64[stimulus_size_player];

        // RÃ©cupÃ©ration des infos du joueur : couleur
        if (isWhite) cPlayer[0] = 1;
        else         cPlayer[0] = 0;

        cPlayer[1] = level;

        term_t hCPlayer = (m_PrologInterface.funcNewTermRef)();
        m_PrologInterface.putList(hCPlayer, stimulus_size_player, cPlayer);
        delete(cPlayer);

        term_t ptrTerm = m_PrologInterface.FirstTerm+1;
        m_PrologInterface.cleanList(ptrTerm);
        m_PrologInterface.consList(ptrTerm, hCPlayer);

        /* ET ICI >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

        term_t hReponse = m_PrologInterface.FirstTerm+2;

        if (m_PrologInterface.call()) {
            std::vector<int32> reponse;
            m_PrologInterface.getList(hReponse, reponse);

            // Enregistrement des infos dans les attributs du joueur
            nCaseDepart = reponse[0];
            nCaseArrivee = reponse[1];
            indexPionStack = reponse[2];
        }

        m_PrologInterface.finish();
        // On a finit de "rÃ©flÃ©chir"
    }
}

// -------------------------------------------------------------------------
// After getting the result from Prolog
// We have the index and the nÂ° of the case of the pawn -> we move it
// We use setSelected and movePawn from Pawn
// And we move it to the case Prolog gave us
// -------------------------------------------------------------------------
void AIPlayer::play(QEventLoop* pause) {
    qDebug() << "\t\t\t\t\t\tAIPLAYER::PLAY";

    // pause->exec(); // Mets en pause et attends pour le coup du joueur

    // On appelle la rÃ©flexion via Prolog
    // think();

        /* ********************* */
        // Supposons le prÃ©dicat Prolog appelÃ© et les rÃ©sultats obtenus
        // On suppose que le coup est possible
        //nCaseDepart = 1;
        //nCaseArrivee = 2;
        //indexPionStack = 1;
        /* ********************* */

    this->think3();

    // On rÃ©cupÃ¨re le pion et on le sÃ©lectionne ############################
    Pawn* selectedPawn;
    list<Pawn*>::iterator it;
    int k = 1;
    int iD, jD;
    int iA, jA; // CoordonnÃ©es des cases de DÃ©part/ArrivÃ©e, Ã  la mode c++

    qDebug() << "\t\t\t\t\t\t\tCALCULS COORD";

    // Attention2 : Les index des cases commencent Ã  1
    // Calcul des coordonnÃ©es iD, jD
    jD = (nCaseDepart-1)/3;
    iD = (nCaseDepart-1)%3;

    // Calcul des coordonnÃ©es iA, jA
    jA = (nCaseArrivee-1)/3;
    iA = (nCaseArrivee-1)%3;

    qDebug() << "\t\t\t\t\t\t\tCOORD: " << iD << ", " << jD << " --> "
                                        << iA << ", " << jA;


    qDebug() << "\t\t\t\t\t\t\tSEARCHING PAWN";

    // Attention : l'index du prÃ©dicat est en mode Stack
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
    selectedPawn->selectCorrespondingLabel(); // on envoie un signal au pawnLabel correspondant
    // Il faut mettre Ã  jour pawnlabel

    // Puis on le fait bouger lÃ  oÃ¹ il faut ################################
    // On lui indique le pointeur sur la nouvelle case
    qDebug() << "\t\t\t\t\t\t\tMOVE PION";


    // void BoardGUI::movePawnLabels(Case* oldCase,Case* dest) ?
    board->movePawns(&(board->board[iA][jA]));

    // On dÃ©selectionne ? Ou est ce que c'est fait auto ?
    selectedPawn->setSelected(0);
}
