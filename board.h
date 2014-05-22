#ifndef BOARD_H
#define BOARD_H

#include <case.h>
#include <casegui.h>

class Board : public QObject
{
Q_OBJECT
private:
    bool isWhiteToMove;

public:
    Board();
    ~Board(){}
    void insertPawn(Case* c, Pawn *p);
    // this function inserts the pawn into the list of pawns on the case c(i,j)
    void insertPawn(int i, int j, Pawn* p);
    int lengthOfMovement(Case *departure, Case *arrival);
    int lengthOfStack(list<Pawn*>::iterator it, list<Pawn *> &list);
    void setToMove(bool b);
    bool isStackWhite(list<Pawn*> &l);
    void movePawns(Case *c);

    // A METTRE EN PRIVE !!
    Case **board;

signals:
    void moveFinished(); // Signal envoyé pour sortir de l'attente d'un mouvement (pause.exec())
    void movePawnLabelsSignal(Case *oldCase,Case* dest); // signal envoyé à la fin de movePawns activer movePawnLabel du boardGUI

public slots:
    void caseGUIClicked(CaseGUI* cGUI); // s'active lorsqu'on clique sur une caseGUI (active un mouvement)
    void deselectPawns(Pawn *p); // envoyé par pawn, permet de déselectionner tous les pions différents du pion nouvellement sélectionné

};

#endif // BOARD_H
