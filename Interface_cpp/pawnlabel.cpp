#include "pawnlabel.h"

PawnLabel::PawnLabel(QWidget * parent,bool isWhite) : QLabel(parent)
{
    QPixmap pn;
    QPixmap pb;
    QPixmap pnSelect;
    QPixmap pbSelect;

    selected = 0; // Ajout ici pour suppression de warnings au lieu du .h

    if(!pn.load(":/new/images/pion_noir.png")){
        qWarning("Failed to load pion_noir.png");
    }
    if(!pb.load(":/new/images/pion_blanc.png")){
        qWarning("Failed to load pio_blanc.png");
    }
    if(!pnSelect.load(":/new/images/pion_noir_select.png")){
        qWarning("Failed to load pion_noir_select.png");
    }
    if(!pbSelect.load(":/new/images/pion_blanc_select.png")){
        qWarning("Failed to load pion_blanc_select.png");
    }

    if(isWhite){
        this->setPixmap(pb);
        this->pic = pb;
        this->selectpic = pbSelect;
    }else{
        this->setPixmap(pn);
        this->pic = pn;
        this->selectpic = pnSelect;
    }

    connect( this, SIGNAL( clicked() ), this, SLOT( pawnClicked() ) );
}

void PawnLabel::pawnClicked()
{
    // gérer la sélection avec une classe plus haute.
    // Quand on clique sur un pion -> parcourir ceux plus loin dans la liste
    // et tous les sélectionner

    if(!selected){
        selected=1;
        this->setPixmap(this->selectpic);
    }else{
        selected=0;
        this->setPixmap(this->pic);
    }

    qDebug()<<"Clicked. Selected : " << selected;

    emit deselectOthers(this);

}

void PawnLabel::mousePressEvent ( QMouseEvent * event )
{
    emit clicked();
}

int PawnLabel::getSelected()
{
    return selected;
}

void PawnLabel::setSelected(int select)
{
    this->selected = select;

    if(select){
        this->setPixmap(this->selectpic);
    }else{
        this->setPixmap(this->pic);
    }
}

// ----------------------------------------------------
void PawnLabel::pawnLabelClicked() {
    setSelected(1);
}
// ----------------------------------------------------
