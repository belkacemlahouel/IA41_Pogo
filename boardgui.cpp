#include "boardgui.h"

BoardGUI::BoardGUI(QWidget* parent):QWidget(parent)
{
    /* Creation des cases et de leur version graphique */

    this->boardGUI = new CaseGUI*[3];

    for (int i = 0; i < 3; ++i)
            boardGUI[i] = new CaseGUI[3];

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


    list<PawnLabel*>::iterator it;

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++){

            int x = i*this->boardGUI[i][j].getCaseSize()+i*10+10;
            int y = j*this->boardGUI[i][j].getCaseSize()+j*10+10;

            this->boardGUI[i][j].setParent(parent);
            this->boardGUI[i][j].setPixmap(case_pixmap);
            this->boardGUI[i][j].move(x,y);
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

    /* Connexion des pawnsLabels au plateau GUI */

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

}

/* Idem que insertPawn, mais GRAPHIQUEMENT */

void BoardGUI::insertPawnLabel(int i, int j, PawnLabel *p)
{
    CaseGUI* c= &(this->boardGUI[i][j]);

    int x = c->x() + c->getCaseSize()/2 - 25;
    int y = c->y() + c->getCaseSize() - 15 - c->pawnListGUI.size()*10;
    c->pawnListGUI.push_back(p);
    p->move(x,y);
}


void BoardGUI::insertPawnLabel(CaseGUI* c, PawnLabel *p)
{
    int x = c->x() + c->getCaseSize()/2 - 25;
    int y = c->y() + c->getCaseSize() - 15 - c->pawnListGUI.size()*10;
    c->pawnListGUI.push_back(p);
    p->move(x,y);
}

void BoardGUI::removePawn(CaseGUI* c, PawnLabel *p)
{
    c->pawnListGUI.remove(p);
}

// Se déclenche a la fin de Board::movePawns.
void BoardGUI::movePawnLabels(Case* oldCase,Case* dest)
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

void BoardGUI::deselectPawnsLabels(PawnLabel *p)
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

BoardGUI& BoardGUI::operator =(const BoardGUI& b){
    this->boardGUI = b.boardGUI;

    return *this;
}
