#ifndef BOARD_H
#define BOARD_H

#include <case.h>

class Board : public QWidget
{
Q_OBJECT
private:
    Case **board;


public:
    Board(QWidget* parent);
    ~Board(){}
    void insertPawn(Case* c, PawnLabel *p);
    // this function inserts the pawn into the list of pawns on the case c(i,j)
    void insertPawn(int i, int j, PawnLabel* p);
    void removePawn(Case* c, PawnLabel *p);
    void printBoard();
    int lengthOfMovement(Case *departure, Case *arrival);
    int lengthOfStack(list<PawnLabel*>::iterator it, list<PawnLabel *> &list);

public slots:
    void movePawns(Case *c);

};

#endif // BOARD_H
