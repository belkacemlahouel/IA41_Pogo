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
    void deselectOthers(Pawn *p);
    void pawnSelected();

public slots:
    void labelClicked();
};

#endif // PAWN_H
