#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"
#include <QEventLoop>

class Player {
protected:
    bool isWhite;
    Board* board;
    int level;

public:
    virtual void play(QEventLoop*) = 0;
    void setLevel(int);
};

#endif // PLAYER_H
