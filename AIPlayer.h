#ifndef AIPLAYER_H
#define AIPLAYER_H

// -------------------------------------------------------------------------
// Implementation of Prolog Interface
// The AI Player uses Prolog predicates to choose its move
// Using inheritance from abstract class Player
// -------------------------------------------------------------------------

#include "PrologInterface.h"
#include "player.h"

class AIPlayer : public Player {
private:
    PrologInterface m_PrologInterface;
    void think();

public:
    int level;
    AIPlayer(bool);
    void play(QEventLoop*);
};

#endif // AIPLAYER_H
