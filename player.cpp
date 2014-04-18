#include "player.h"

Player::Player()
{
    this->isAI = false;
}

Player::Player(bool _isAI)
{
    this->isAI = _isAI;
}

bool Player::getIsAI()
{
    return this->isAI;
}


void Player::setIsAI(bool _isAI)
{
    this->isAI;
}
