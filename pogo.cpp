#include <pogo.h>

PoGo::PoGo(QWidget *parent) : QWidget(parent)
{
    QPalette p;

    this->setFixedSize(490, 490);

    p = this->palette();
    p.setColor(QPalette::Background, Qt::blue);

    this->setAutoFillBackground(true);
    this->setPalette(p);

    // Affichage de la fenêtre
    this->show();
}

PoGo::PoGo(QWidget *parent,bool WhiteIsIa, bool BlackIsIa, int WhiteIaLevel, int BlackIaLevel) : QWidget(parent)
{
    this->b = new Board();
    this->bGUI = new BoardGUI(this);

    makeConnections();

    this->turnLabel = new QLabel("XXXXXXXXXXXXXXX",this);
    this->turnLabel->move(200,500);

    this->setFixedSize(490, 540);

    this->whitePlayer.setIsAI(WhiteIsIa);
    this->blackPlayer.setIsAI(BlackIsIa);
    this->blackIAPower = BlackIaLevel;
    this->whiteIAPower = WhiteIaLevel;

    QPalette p;
    p = this->palette();
    p.setColor(QPalette::Background, Qt::blue);

    this->setAutoFillBackground(true);
    this->setPalette(p);

    this->show();
}

void PoGo::startGame()
{
    int winner = 0;
    QEventLoop pause;
    QMessageBox msgBox;
    connect(b, SIGNAL(moveFinished()), &pause, SLOT(quit()));
    QFont f( "Arial", 16, QFont::Bold);
    turnLabel->setFont(f);

    // ---------------------------------------------------------------------
    //while (winner == 0 && bGUI->closed == false) // while(!winner)
    // ---------------------------------------------------------------------

    while (winner == 0)
    {
        qDebug()<< "Tour des blancs";
        turnLabel->setText("BLANCS");
        turnLabel->setStyleSheet("QLabel { color: white }");
        b->setToMove(true);
        pause.exec();

        qDebug()<< "Les blancs ont joue";

        winner = whoWon();

        if (winner == 0) { // Arrêt dès que besoin
            qDebug()<< "Tour des noirs";
            turnLabel->setText("NOIRS");
            turnLabel->setStyleSheet("QLabel { color: black }");
            b->setToMove(false);
            pause.exec();

            qDebug()<< "Les noirs ont joue";

            winner = whoWon();
        }

        qDebug() << "\t\t\t\tLE GAGNANT EST " << winner;
    }

    if(winner == 2)
    {
        qDebug()<<"\tLes noirs ont gagne !!";
        msgBox.setText("Les noirs ont gagne !!");
    }
    else if(winner == 1)
    {
        qDebug()<<"\tLes blancs ont gagne !!";
        msgBox.setText("Les blancs ont gagne !!");
    }

    msgBox.exec();
}

int PoGo::whoWon()
{
    bool allWhite = true;
    bool allBlack = true;

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if (b->board[i][j].pawnList.size() > 0) {
                if (b->isStackWhite(b->board[i][j].pawnList)) {
                    allBlack = false;
                } else {
                    allWhite = false;
                }
            }

//            if(b->board[i][j].pawnList.size() != 0 && b->isStackWhite(b->board[i][j].pawnList))
//            {
//                allBlack = false;
//            }
//            else if(b->board[i][j].pawnList.size() != 0 && !(b->isStackWhite(b->board[i][j].pawnList)))
//            {
//                allWhite = false;
//            }
        }
    }

    if(allWhite)
    {
        return 1;
    }
    else if(allBlack)
    {
        return 2;
    }

    return 0;
}

/* Cette fonction set tous les paramètres du jeu. Elle sera appellee a la cloture de l'option menu */

void PoGo::setAllSettings()
{

}

void PoGo::makeConnections()
{
    qDebug()<<"Etablissement des connexions necessaires...";

    /* Connexion des pawnLabel à leur pawn associe */
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            list<PawnLabel*>::iterator labelIt = this->bGUI->boardGUI[i][j].pawnListGUI.begin();
            list<Pawn*>::iterator pawnIt = this->b->board[i][j].pawnList.begin();

            while(pawnIt != this->b->board[i][j].pawnList.end())
            {
                Pawn *temp = *pawnIt;
                PawnLabel *tempLabel = *labelIt;

                connect(tempLabel,SIGNAL(clicked()),temp,SLOT(labelClicked()));

                pawnIt++;
                labelIt++;
            }
        }
    }

    /* Connexion de board et boardGUI */

    connect(b,SIGNAL(movePawnLabelsSignal(Case*,Case*)),bGUI,SLOT(movePawnLabels(Case*,Case*)));

    /* Connexion des CaseGUI au plateau (le clic sur les cases déclence la fonction movePawns) */

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            CaseGUI *c = &(bGUI->boardGUI[i][j]);
            connect(c,SIGNAL(caseClicked(CaseGUI*)),b,SLOT(caseGUIClicked(CaseGUI*)));
        }
    }

}
