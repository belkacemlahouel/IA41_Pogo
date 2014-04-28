#include <pogo.h>

PoGo::PoGo(QWidget *parent) : QWidget(parent)
{
    this->b = new Board(this);

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
    this->b = new Board(this);
    QPalette p;
    this->turnLabel = new QLabel("XXXXXXXXXXXXXXX",this);
    this->turnLabel->move(200,500);

    this->setFixedSize(490, 540);

    this->whitePlayer.setIsAI(WhiteIsIa);
    this->blackPlayer.setIsAI(BlackIsIa);
    this->blackIAPower = BlackIaLevel;
    this->whiteIAPower = WhiteIaLevel;

    p = this->palette();
    p.setColor(QPalette::Background, Qt::blue);

    this->setAutoFillBackground(true);
    this->setPalette(p);

    // Affichage de la fenêtre
    this->show();
}

void PoGo::startGame()
{
    bool winner = 0;
    QEventLoop pause;
    connect(b, SIGNAL(moveFinished()), &pause, SLOT(quit()));
    QFont f( "Arial", 16, QFont::Bold);
    turnLabel->setFont(f);

    while(!winner)
    {
        qDebug()<< "Tour des blancs";
        turnLabel->setText("BLANCS");
        turnLabel->setStyleSheet("QLabel { color: white }");
        b->setToMove(true);
        pause.exec();

        qDebug()<< "Les blancs ont joue";

        // vérification si quelqu'un a gagné

        qDebug()<< "Tour des noirs";
        turnLabel->setText("NOIRS");
        turnLabel->setStyleSheet("QLabel { color: black }");
        b->setToMove(false);
        pause.exec();

        qDebug()<< "Les noirs ont joue";

        // vérification si quelqu'un a gagné

    }
}

/* Cette fonction set tous les paramètres du jeu. Elle sera appellee a la cloture de l'option menu */

void PoGo::setAllSettings()
{

}
