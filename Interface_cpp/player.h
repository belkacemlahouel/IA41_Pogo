#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"
#include <QEventLoop>

class Player {
protected:
    bool isWhite;
    Board* board;

public:
    virtual void play(QEventLoop*) = 0;
};

#endif // PLAYER_H
