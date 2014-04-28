#ifndef BOARD_H
#define BOARD_H

#include <case.h>

class Board : public QWidget
{
Q_OBJECT
private:
    Case **board;
    bool isWhiteToMove;

public:
    Board(QWidget* parent);
    ~Board(){}
    void insertPawn(Case* c, Pawn *p);
    void insertPawnLabel(Case* c, PawnLabel *p);
    // this function inserts the pawn into the list of pawns on the case c(i,j)
    void insertPawn(int i, int j, Pawn* p);
    void insertPawnLabel(int i, int j, PawnLabel *p);
    void removePawn(Case* c, PawnLabel *p);
    void printBoard();
    int lengthOfMovement(Case *departure, Case *arrival);
    int lengthOfStack(list<Pawn*>::iterator it, list<Pawn *> &list);
    void setToMove(bool b);
    bool isStackWhite(list<Pawn*> &l);
    void movePawnLabels(Case* oldCase, Case* dest);

signals:
    void moveFinished();

public slots:
    void movePawns(Case *c);
    void deselectPawnsLabels(PawnLabel *p);
    void deselectPawns(Pawn *p);

};

#endif // BOARD_H
