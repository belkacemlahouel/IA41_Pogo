#ifndef PAWNLABEL_H
#define PAWNLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QWidget>
#include <QDebug>

class PawnLabel : public QLabel
{
Q_OBJECT
private:
    int selected;
    QPixmap pic;
    QPixmap selectpic;

public:
    PawnLabel(QWidget * parent = 0 , bool isWhite = false);
    ~PawnLabel(){}
    int getSelected();
    void setSelected(int select);
    bool getIsWhite();


signals:
    void clicked(); // signal qui s'envoie avec le clic de la souris
    void deselectOthers(PawnLabel *p); // signal envoyé au boardGUI afin de déselectionner tous les labels ne
                                       // correspondant pas au label venant d'être cliqué

public slots:
    void pawnClicked(); // slot qui s'active par la réception du signal lors du clic de la souris
    void pawnLabelClicked(); // met le label à seleted = 1, appelé par le pawn correspondant

protected:
    void mousePressEvent ( QMouseEvent * event ) ;

};

#endif // PAWNLABEL_H
