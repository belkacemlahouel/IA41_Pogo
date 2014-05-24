#ifndef POGO_H
#define POGO_H

#include <QEventLoop>
#include <QMessageBox>
#include <QPalette>

#include "board.h"
#include "boardgui.h"
#include "player.h"
#include "optionmenu.h"
#include "Player.h"
#include "AIPlayer.h"
#include "HumanPlayer.h"

/* La classe PoGo est la classe maîtresse du jeu, c'est elle qui determine qui joue a quel moment, et quand le jeu se finit */

class PoGo : public QWidget
{
    Q_OBJECT
private:
    Board *b;
    BoardGUI *bGUI;
    Player* whitePlayer;
    Player* blackPlayer;
    int whiteIAPower; // 1 = nul, 2 = bon, 3 = très bon
    int blackIAPower;
    int whoWon();
    QLabel* turnLabel;
    void makeConnections();

public:
    PoGo(QWidget *parent=0);
    PoGo(QWidget *parent=0, bool WhiteIsIa = false, bool BlackIsIa = false, int WhiteIaLevel = 1, int BlackIaLevel = 1);
    void startGame();
    void setAllSettings();
};

#endif // POGO_H
