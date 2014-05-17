#include "Player.h"

void Player::play(QEventLoop* pause) {
    pause->exec();
}

void Player::setLevel(int lvl) {
    level = max(0, min(3, lvl)); // 0 <= level <= 3
}
