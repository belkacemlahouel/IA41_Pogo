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
// "5" is the number of parameters for "think":
//      nCaseStart, idxPawn, nCaseEnd, Board, colorPlayer
// -------------------------------------------------------------------------
void AIPlayer::think() {
    // hresponse : liste prolog
    // response  : c'est des vectors c++
    // stimulus data
    // think ; etat, action à faire (listes)
    // afficher response[0]

    if (m_PrologInterface.start("think", 5)) {
        // Board creation in Prolog format
        int plBoard[21], n = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                Case* ca = &(board->board[i][j]);

                list<Pawn*>* pawns = &(ca->pawnList);
                list<Pawn*>::iterator it;

                for (it = pawns->end(); it != pawns->begin(); --it) {   // k != ... ?

                    // for (int k = pawns.size-1; k >= 0; --k) {          // Stack style
                    /*if (board[i][j]->pawnList.get(k).getIsWhite())*/

                    Pawn* tmp = *it;
                    if (tmp->getIsWhite())       plBoard[n] = 0; // White pawn
                    else                         plBoard[n] = 1; // Black pawn
                }
                plBoard[n] = -1;                                                   // Next case
                ++ n;
            }
        }

        // Player color in Prolog format

        // The others parameters are (normally) returned by Prolog...
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


























// if (m_PrologInterface.start( "think", 2 )) {
//  term_t stimulus_list = (m_PrologInterface.funcNewTermRef)();
//  term_t hResponse;
//  int32 stimulus_size;
//  hResponse = m_PrologInterface.FirstTerm+1;
//  m_PrologInterface.cleanList( m_PrologInterface.FirstTerm );
//  stimulus_size = 10;
//  m_StimulusRawData = new float64 [stimulus_size];
//
//  for( int32 i = 0; i < stimulus_size; ++i ) {
//      m_StimulusRawData[i] = i;
//  }
//
//  // initialise une liste Prolog à partir d'un tableau de 10 éléments 0,1,...,9
//  m_PrologInterface.putList( stimulus_list, stimulus_size, m_StimulusRawData );
//  // Le premier argument du prédicat “think” est une liste
//  m_PrologInterface.consList( m_PrologInterface.FirstTerm, stimulus_list);
//
//  SafeDeleteArray( m_StimulusRawData );
//
//  if (m_PrologInterface.call()) {
//      std::vector<float64> response;
//      // récupère une liste instanciée par le prédicat en tant que vector<float64>
//      m_PrologInterface.getList( hResponse, response );
//  }
//
//  m_PrologInterface.finish();
// }
