#ifndef PLAYER_H
#define PLAYER_H

class Player
{
private:
    bool isAI;

public:
    Player();
    Player(bool _isAI);
    bool getIsAI();
    void setIsAI(bool _isAI);
};

#endif // PLAYER_H
