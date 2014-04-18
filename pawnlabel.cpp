#include "pawnlabel.h"

PawnLabel::PawnLabel(QWidget * parent ) : QLabel(parent)
{
    selected = 0;
    connect( this, SIGNAL( clicked() ), this, SLOT( pawnClicked() ) );
}

PawnLabel::PawnLabel(QWidget * parent, bool _isWhite ) : QLabel(parent)
{
    QPixmap pn;
    QPixmap pb;

    if(!pn.load(":/new/images/pion_noir.png")){
        qWarning("Failed to load pion_noir.png");
    }

    if(!pb.load(":/new/images/pion_blanc.png")){
        qWarning("Failed to load pio_blanc.png");
    }

    isWhite = _isWhite;
    selected = 0;

    if(isWhite)
    {
        this->setPixmap(pb);
    }
    else
    {
        this->setPixmap(pn);
    }


    connect( this, SIGNAL( clicked() ), this, SLOT( pawnClicked() ) );
}

void PawnLabel::pawnClicked()
{
    // gérer la sélection avec une classe plus haute. Quand on clique sur un pion -> parcourir
    // ceux plus loin dans la liste et tous les sélectionner

    if(!selected)
        selected=1;
    else
        selected=0;

    qDebug()<<"Clicked. Selected : " << selected;

}

void PawnLabel::mousePressEvent ( QMouseEvent * event )
{
    emit clicked();
}

int PawnLabel::getSelected()
{
    return this->selected;
}

void PawnLabel::setSelected(int select)
{
    this->selected = select;
}

bool PawnLabel::getIsWhite()
{
    return this->isWhite;
}
