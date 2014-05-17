#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"

class Player {
protected:
    bool color;
    Board* board;
    bool isAI;

public:
    void play();
    Player();

    // Deprecated: you should use inheritance
    Player(bool _isAI);
    bool getIsAI();
    void setIsAI(bool _isAI);
};

#endif // PLAYER_H
