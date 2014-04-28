#include "pawn.h"

Pawn::Pawn(bool _isWhite)
{
    isWhite = _isWhite;
    selected = 0;
}

int Pawn::getSelected()
{
    return this->selected;
}

void Pawn::setSelected(int select)
{
    this->selected = select;
}

bool Pawn::getIsWhite()
{
    return this->isWhite;
}

void Pawn::labelClicked()
{

    if(!selected){
        selected=1;
    }else{
        selected=0;
    }

    qDebug()<<"Pawn selected : "<<selected;

    emit deselectOthers(this);
}
