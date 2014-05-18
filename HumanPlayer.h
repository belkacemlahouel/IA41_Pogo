#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer(bool);
    void play(QEventLoop*);
};

#endif // HUMANPLAYER_H
