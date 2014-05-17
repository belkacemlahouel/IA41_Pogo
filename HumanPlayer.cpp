#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(bool isW) {
    isWhite = isW;
}

void HumanPlayer::play(QEventLoop* pause) {
    pause->exec();
}
