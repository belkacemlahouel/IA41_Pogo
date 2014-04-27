#include "board.h"
#include "case.h"

Board::Board(QWidget* parent):QWidget(parent)
{
    this->isWhiteToMove = true;
    this->board = new Case*[3];
    for (int i = 0; i < 3; ++i)
            board[i] = new Case[3];

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++){
            this->board[i][j] = Case(j+1 +(i*3),parent);
        }
    }

    /* Ajout de l'image sur les cases, et positionnement */

    QPixmap case_pixmap;

    if(!case_pixmap.load(":/new/images/case150px.png")){
        qWarning("Failed to load case.png");
    }

    list<PawnLabel*>::iterator it;

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++){

            int x = i*this->board[i][j].getCaseSize()+i*10+10;
            int y = j*this->board[i][j].getCaseSize()+j*10+10;

            this->board[i][j].setParent(parent);
            this->board[i][j].setPixmap(case_pixmap);
            this->board[i][j].move(x,y);
            connect(&(this->board[i][j]), SIGNAL(caseClicked(Case*) ), this, SLOT( movePawns(Case*)));

            for(it = this->board[i][j].pawnList.begin();it != this->board[i][j].pawnList.end() ; it++)
            {
                PawnLabel *temp = *it;
                connect(temp,SIGNAL(deselectOthers(PawnLabel*)),this,SLOT(deselectPawns(PawnLabel*)));
            }// connexion de toutes les cases au plateau.
        }
    }

    /* Création de tous les pions, et positionnement */

    /*PawnLabel* black_label =new PawnLabel(parent,false);
    PawnLabel* white_label =new PawnLabel(parent,true);*/

    qDebug()<<"Insertion des pions";

    this->insertPawn(0,0,new PawnLabel(parent,true));
    this->insertPawn(0,0,new PawnLabel(parent,true));
    this->insertPawn(1,0,new PawnLabel(parent,true));
    this->insertPawn(1,0,new PawnLabel(parent,true));
    this->insertPawn(2,0,new PawnLabel(parent,true));
    this->insertPawn(2,0,new PawnLabel(parent,true));

    this->insertPawn(0,2,new PawnLabel(parent,false));
    this->insertPawn(0,2,new PawnLabel(parent,false));
    this->insertPawn(1,2,new PawnLabel(parent,false));
    this->insertPawn(1,2,new PawnLabel(parent,false));
    this->insertPawn(2,2,new PawnLabel(parent,false));
    this->insertPawn(2,2,new PawnLabel(parent,false));

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            for(it = this->board[i][j].pawnList.begin();it != this->board[i][j].pawnList.end() ; it++)
            {
                PawnLabel *temp = *it;
                connect(temp,SIGNAL(deselectOthers(PawnLabel*)),this,SLOT(deselectPawns(PawnLabel*)));
            }// connexion de toutes les cases au plateau.
        }
    }

}

/* Insère un pion dans board[i][j].pawnList, ainsi que graphiquement */

void Board::insertPawn(int i, int j, PawnLabel *p)
{
    Case* c= &(this->board[i][j]);

    int x = c->x() + c->getCaseSize()/2 - 25;
    int y = c->y() + c->getCaseSize() - 15 - c->pawnList.size()*10;
    c->pawnList.push_back(p);
    p->move(x,y);
}

/* Insère un pion dans c.pawnList, ainsi que graphiquement */

void Board::insertPawn(Case* c, PawnLabel *p)
{
    int x = c->x() + c->getCaseSize()/2 - 25;
    int y = c->y() + c->getCaseSize() - 15 - c->pawnList.size()*10;
    c->pawnList.push_back(p);
    p->move(x,y);
}

void Board::removePawn(Case* c, PawnLabel *p)
{
    c->pawnList.remove(p);
}

void Board::printBoard()
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            cout << "i : " << i << " j : " << j;
            this->board[i][j].printContent();
        }

    }
}

/* Pour que l'IA puisse bouger un pion, il faut qu'elle setSelected du/des pions à bouger à 1, et qu'elle sélectionne sa case.
 Pour finir, on appellera movePaws, qui fera directement le lien entre la case choisie et le pion auparavent sélectionné*/

void Board::movePawns(Case* c)
{
    qDebug()<<"Case clicked !";

    int trouve = 0;
    int i;
    int j;

    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++){

            if(&(this->board[i][j]) == c)
               break;
        }
        if(&(this->board[i][j]) == c)
           break;
    }

    qDebug()<<"Case clicked ! Position : " << i << " " << j;

    /* On cherche pour toutes les cases, la première qui contient un pion à la caractéristique "selected" à 1. Quand on le trouve, on
     * prend automatiquement tous les pions qui sont au dessus de lui dans la liste, sans se soucier de leur statut */

    PawnLabel* toMove;

    list<PawnLabel*>::iterator it;
    list<PawnLabel*>::iterator toMove_iterator;
    Case* oldCase;

        for(i=0;i<3;i++)
        {
            for(j=0;j<3;j++){

                    for (it=this->board[i][j].pawnList.begin(); it != this->board[i][j].pawnList.end(); ++it)
                    {
                        if((*it)->getSelected() == 1)
                        {
                            toMove_iterator = it;
                            trouve = 1;
                            oldCase = &(this->board[i][j]);
                        }
                    }

            }
        }

        /* On verifie les 3 règles principales : que c'est bien au noir ou au blanc de jouer, qu'il n'y a pas plus de 3 pions pris,
         * et que la taille du mouvement est aussi grand que le nombre de pions pris */

        if(trouve != 0)
        {
            if(this->isWhiteToMove != this->isStackWhite(oldCase->pawnList))
            {
                qDebug()<<"Ce n'est pas a ton tour de jouer";
            }
            else
            {
                if(oldCase == c)
                {
                    qDebug()<<"Impossible : pion deja sur la case";
                }
                else
                {
                    if(lengthOfStack(toMove_iterator,oldCase->pawnList) > 3)
                    {
                        qDebug()<<"Nombre de pieces maximum autorisees : 3";
                    }
                    else
                    {
                        if(lengthOfMovement(oldCase,c) != lengthOfStack(toMove_iterator,oldCase->pawnList))
                        {
                            qDebug()<<"Mouvement impossible (mouvement different du nombre de pieces prises)";
                        }
                        else
                        {
                            qDebug()<<"Taille de la pile : " << std::distance(toMove_iterator,oldCase->pawnList.end());
                            qDebug()<<"Taille de la pile : " << lengthOfStack(toMove_iterator,oldCase->pawnList);
                            qDebug()<<"Distance : " << lengthOfMovement(oldCase,c);

                            while(toMove_iterator != oldCase->pawnList.end())
                            {
                                toMove = *toMove_iterator;
                                insertPawn(c,toMove);
                                toMove_iterator = oldCase->pawnList.erase(toMove_iterator);
                                toMove->setSelected(0);
                            }
                            emit moveFinished();
                        }
                    }
                }
            }
        }
}

/* Cette fonction regarde la taille de la pile. Elle sera utilisée pour déterminer la validité du
 * mouvement */

int Board::lengthOfStack(list<PawnLabel*>::iterator it,list<PawnLabel*>& list)
{
    int i=0;
    while(it != list.end() )
    {
        i++;
        it++;
    }
    return i;
}

/* Cette fonction regarde la taille du mouvement entre deux cases. Elle sera utilisée pour déterminer la validité de
 * ce dernier */

int Board::lengthOfMovement(Case* departure,Case* arrival)
{
    int id = 0;
    int jd = 0;

    int ia = 0;
    int ja = 0;

    int dist_i = 0;
    int dist_j = 0;

    for(int i = 0;i<3;i++)
    {
        for(int j = 0;j<3;j++)
        {
            if(&(this->board[i][j]) == departure)
            {
                id = i;
                jd = j;
            }
        }
    }

    for(int i = 0;i<3;i++)
    {
        for(int j = 0;j<3;j++)
        {
            if(&(this->board[i][j]) == arrival)
            {
                ia = i;
                ja = j;
            }
        }
    }

    if(id > ia) dist_i = (id - ia);
    else dist_i = (ia - id);

    if(jd > ja) dist_j = (jd - ja);
    else dist_j = (ja - jd);

    return dist_i + dist_j;
}

void Board::setToMove(bool b)
{
    this->isWhiteToMove = b;
}

/* Cette fonction détermine si la pile appartient au joueur noir ou au blanc */

bool Board::isStackWhite(list<PawnLabel*> &l)
{
    list<PawnLabel*>::iterator it;

    it = l.end();
    it--;
    PawnLabel *lastPawn = *it;

    return lastPawn->getIsWhite();

}

/* Lorsqu'un pion est cliqué, cette fonction désélectionne tous les autres */

void Board::deselectPawns(PawnLabel *p)
{
    list<PawnLabel*>::iterator it;

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            for(it = this->board[i][j].pawnList.begin();it != this->board[i][j].pawnList.end() ; it++)
            {
                PawnLabel *temp = *it;
                if(temp != p)
                {
                    temp->setSelected(false);
                }
            }
        }
    }
}
