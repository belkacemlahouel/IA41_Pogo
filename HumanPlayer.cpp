#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(bool isW) {
    qDebug() << "\t\t\t\t\t\tINSTANCIATION HUMAN\n";
    isWhite = isW;
}

void HumanPlayer::play(QEventLoop* pause) {
    qDebug() << "\t\t\t\t\t\tHUMANPLAYER::PLAY";
    pause->exec();
}
