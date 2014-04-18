#include "pogo.h"

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

/* FAIRE EN SORTE DE DEMARER CETTE FONCTION SEULEMENT UNE FOIS L'OPTION MENU FERME
 * c'est dans cette méthode que l'IA et le joueur joueront, jusqu'à la fin du jeu */

void PoGo::startGame()
{
    bool winner = 0;

    /*while(!winner)
    {
        // tour des blancs
        // QEventLoop pour attend qu'ils aient joué (si humain) ?

        // vérification si quelqu'un a gagné

        // tour des noirs

        // vérification si quelqu'un a gagné
    }*/
}

/* Cette fonction set tous les paramètres du jeu. Elle sera appellee a la cloture de l'option menu */

void PoGo::setAllSettings()
{

}
