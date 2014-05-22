#ifndef PAWN_H
#define PAWN_H
#include <QObject>
#include <QDebug>

class Pawn : public QObject
{
    Q_OBJECT
private:
    int selected;
    bool isWhite; // S'il n'est pas blanc, il est noir.

public:
    Pawn(bool _isWhite);
    int getSelected();
    void setSelected(int select);
    bool getIsWhite();
    void selectCorrespondingLabel();

signals:
    void deselectOthers(Pawn *p); // lorsqu'un pion est sélectionné, ce signal active un slot de board qui déselectionnera les autres
    void pawnSelected(); // lorsque l'IA sélectionne un pion, ce signal permet de sélectionner le label correspondant

public slots:
    void labelClicked(); // lorsque le pawnLabel est cliqué, ce slot permet de cliquer son équivalent pawn
};

#endif // PAWN_H
