#include "Player.h"

void Player::setLevel(int lvl) {
    level = max(0, min(3, lvl)); // 0 <= level <= 3
}
