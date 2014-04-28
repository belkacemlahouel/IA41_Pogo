#include "board.h"
#include "case.h"

Board::Board(QWidget* parent):QWidget(parent)
{
    this->isWhiteToMove = true;

    /* Creation des cases et de leur version graphique */
    this->board = new Case*[3];
    this->boardGUI = new CaseGUI*[3];

    for (int i = 0; i < 3; ++i)
            board[i] = new Case[3];

    for (int i = 0; i < 3; ++i)
            boardGUI[i] = new CaseGUI[3];

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++){
            this->board[i][j] = Case(j+1 +(i*3));
        }
    }

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++){
            this->boardGUI[i][j] = CaseGUI(j+1 +(i*3),parent);
        }
    }

    /* Ajout de l'image sur les cases, et positionnement */

    QPixmap case_pixmap;

    if(!case_pixmap.load(":/new/images/case150px.png")){
        qWarning("Failed to load case.png");
    }

    /* Création de tous les pions, et ajout au plateau */

    qDebug()<<"Insertion des pions";

    this->insertPawn(0,0,new Pawn(true));
    this->insertPawn(0,0,new Pawn(true));
    this->insertPawn(1,0,new Pawn(true));
    this->insertPawn(1,0,new Pawn(true));
    this->insertPawn(2,0,new Pawn(true));
    this->insertPawn(2,0,new Pawn(true));

    this->insertPawn(0,2,new Pawn(false));
    this->insertPawn(0,2,new Pawn(false));
    this->insertPawn(1,2,new Pawn(false));
    this->insertPawn(1,2,new Pawn(false));
    this->insertPawn(2,2,new Pawn(false));
    this->insertPawn(2,2,new Pawn(false));

    list<PawnLabel*>::iterator it;

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++){

            int x = i*this->boardGUI[i][j].getCaseSize()+i*10+10;
            int y = j*this->boardGUI[i][j].getCaseSize()+j*10+10;

            this->boardGUI[i][j].setParent(parent);
            this->boardGUI[i][j].setPixmap(case_pixmap);
            this->boardGUI[i][j].move(x,y);

            /* Connexion des cases au plateau */
            connect(&(this->boardGUI[i][j]), SIGNAL(caseClicked(CaseGUI*) ), this, SLOT( movePawns(CaseGUI*)));
        }
    }

    /* Création de tous les pions version GUI, et ajout au plateau */

    this->insertPawnLabel(0,0,new PawnLabel(parent,true));
    this->insertPawnLabel(0,0,new PawnLabel(parent,true));
    this->insertPawnLabel(1,0,new PawnLabel(parent,true));
    this->insertPawnLabel(1,0,new PawnLabel(parent,true));
    this->insertPawnLabel(2,0,new PawnLabel(parent,true));
    this->insertPawnLabel(2,0,new PawnLabel(parent,true));

    this->insertPawnLabel(0,2,new PawnLabel(parent,false));
    this->insertPawnLabel(0,2,new PawnLabel(parent,false));
    this->insertPawnLabel(1,2,new PawnLabel(parent,false));
    this->insertPawnLabel(1,2,new PawnLabel(parent,false));
    this->insertPawnLabel(2,2,new PawnLabel(parent,false));
    this->insertPawnLabel(2,2,new PawnLabel(parent,false));

    /* Connexion des pawnLabel à leur pawn associe */
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            list<PawnLabel*>::iterator labelIt = this->boardGUI[i][j].pawnListGUI.begin();
            list<Pawn*>::iterator pawnIt = this->board[i][j].pawnList.begin();

            while(pawnIt != this->board[i][j].pawnList.end())
            {
                Pawn *temp = *pawnIt;
                PawnLabel *tempLabel = *labelIt;

                connect(tempLabel,SIGNAL(clicked()),temp,SLOT(labelClicked()));

                pawnIt++;
                labelIt++;
            }
        }
    }

    /* Connexion des pawnsLabels au plateau */

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {

            for(it = this->boardGUI[i][j].pawnListGUI.begin();it != this->boardGUI[i][j].pawnListGUI.end() ; it++)
            {
                PawnLabel *temp = *it;
                connect(temp,SIGNAL(deselectOthers(PawnLabel*)),this,SLOT(deselectPawnsLabels(PawnLabel*)));
            }
        }
    }

    /* Connexion des pawns au plateau */

    list<Pawn*>::iterator it2;

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            for(it2 = this->board[i][j].pawnList.begin();it2 != this->board[i][j].pawnList.end() ; it2++)
            {
                Pawn *temp = *it2;
                connect(temp,SIGNAL(deselectOthers(Pawn*)),this,SLOT(deselectPawns(Pawn*)));
            }
        }
    }

}

/* Insère un pion dans board[i][j].pawnList */

void Board::insertPawn(int i, int j, Pawn *p)
{
    Case* c= &(this->board[i][j]);

    c->pawnList.push_back(p);
}

/* Idem que insertPawn, mais GRAPHIQUEMENT */

void Board::insertPawnLabel(int i, int j, PawnLabel *p)
{
    CaseGUI* c= &(this->boardGUI[i][j]);

    int x = c->x() + c->getCaseSize()/2 - 25;
    int y = c->y() + c->getCaseSize() - 15 - c->pawnListGUI.size()*10;
    c->pawnListGUI.push_back(p);
    p->move(x,y);
}

/* Insère un pion dans c.pawnList */

void Board::insertPawn(Case* c, Pawn *p)
{
    c->pawnList.push_back(p);
}

void Board::insertPawnLabel(CaseGUI* c, PawnLabel *p)
{
    int x = c->x() + c->getCaseSize()/2 - 25;
    int y = c->y() + c->getCaseSize() - 15 - c->pawnListGUI.size()*10;
    c->pawnListGUI.push_back(p);
    p->move(x,y);
}

void Board::removePawn(CaseGUI* c, PawnLabel *p)
{
    c->pawnListGUI.remove(p);
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

void Board::movePawns(CaseGUI* cGUI)
{
    qDebug()<<"Case clicked !";

    int trouve = 0;
    int i;
    int j;

    Case* c;
    /* Recherche de la case correspondant a caseGUI */

    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            if(board[i][j].getCaseNum() == cGUI->getCaseNum())
            {
               c = &board[i][j];
            }
        }
    }

    /* On cherche pour toutes les cases, la première qui contient un pion à la caractéristique "selected" à 1. Quand on le trouve, on
     * prend automatiquement tous les pions qui sont au dessus de lui dans la liste, sans se soucier de leur statut */

    Pawn* toMove;

    list<Pawn*>::iterator it;
    list<Pawn*>::iterator toMove_iterator;
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
                    if(std::distance(toMove_iterator,oldCase->pawnList.end()) > 3)
                    {
                        qDebug()<<"Nombre de pieces maximum autorisees : 3";
                    }
                    else
                    {
                        if(lengthOfMovement(oldCase,c) != std::distance(toMove_iterator,oldCase->pawnList.end()))
                        {
                            qDebug()<<"Mouvement impossible (mouvement different du nombre de pieces prises)";
                        }
                        else
                        {
                            qDebug()<<"Taille de la pile : " << std::distance(toMove_iterator,oldCase->pawnList.end());
                            qDebug()<<"Distance : " << lengthOfMovement(oldCase,c);

                            while(toMove_iterator != oldCase->pawnList.end())
                            {
                                toMove = *toMove_iterator;
                                insertPawn(c,toMove);
                                toMove_iterator = oldCase->pawnList.erase(toMove_iterator);
                                toMove->setSelected(0);
                            }

                            /* Une fois le déplacement du modèle fini, on déplace la version graphique */
                            movePawnLabels(oldCase,c);

                            emit moveFinished();
                        }
                    }
                }
            }
        }
}



void Board::movePawnLabels(Case* oldCase,Case* dest)
{
    PawnLabel* toMove;
    list<PawnLabel*>::iterator it;
    list<PawnLabel*>::iterator toMove_iterator;

    CaseGUI* oldCaseGUI;
    CaseGUI* destGUI;

    /* On recherche les versions GUI des cases oldCase et dest */

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(boardGUI[i][j].getCaseNum() == oldCase->getCaseNum())
            {
               oldCaseGUI = &boardGUI[i][j];
            }
        }
    }

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(boardGUI[i][j].getCaseNum() == dest->getCaseNum())
            {
               destGUI = &boardGUI[i][j];
            }
        }
    }


    for (it=oldCaseGUI->pawnListGUI.begin(); it != oldCaseGUI->pawnListGUI.end(); ++it)
    {
        if((*it)->getSelected() == 1)
        {
            toMove_iterator = it;
        }
    }

    while(toMove_iterator != oldCaseGUI->pawnListGUI.end())
    {
        toMove = *toMove_iterator;
        insertPawnLabel(destGUI,toMove);
        toMove_iterator = oldCaseGUI->pawnListGUI.erase(toMove_iterator);
        toMove->setSelected(0);
    }
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

bool Board::isStackWhite(list<Pawn*> &l)
{
    list<Pawn*>::iterator it;

    it = l.end();
    it--;
    Pawn *lastPawn = *it;

    return lastPawn->getIsWhite();

}

/* Lorsqu'un pion est cliqué, cette fonction désélectionne tous les autres */

void Board::deselectPawnsLabels(PawnLabel *p)
{
    list<PawnLabel*>::iterator it;

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            for(it = this->boardGUI[i][j].pawnListGUI.begin();it != this->boardGUI[i][j].pawnListGUI.end() ; it++)
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

/* Idem que deselectPawnsLabels, mais pour la version "modèle" de ces pions */

void Board::deselectPawns(Pawn *p)
{
    list<Pawn*>::iterator it;

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            for(it = this->board[i][j].pawnList.begin();it != this->board[i][j].pawnList.end() ; it++)
            {
                Pawn *temp = *it;
                if(temp != p)
                {
                    temp->setSelected(false);
                }
            }
        }
    }
}
