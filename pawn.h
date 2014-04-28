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

signals:
    void deselectOthers(Pawn *p);

public slots:
    void labelClicked();
};

#endif // PAWN_H
