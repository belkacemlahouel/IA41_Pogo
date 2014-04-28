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

    // A METTRE EN PRIVE !!
    Case **board;

signals:
    void moveFinished();
    void movePawnLabelsSignal(Case *oldCase,Case* dest);

public slots:
    void movePawns(CaseGUI *cGUI);
    void deselectPawns(Pawn *p);

};

#endif // BOARD_H
