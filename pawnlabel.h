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
    int selected = 0;
    QPixmap pic;
    QPixmap selectpic;

public:
    PawnLabel(QWidget * parent = 0 , bool isWhite = false);
    ~PawnLabel(){}
    int getSelected();
    void setSelected(int select);
    bool getIsWhite();


signals:
    void clicked();
    void deselectOthers(PawnLabel *p);

public slots:
    void pawnClicked();

protected:
    void mousePressEvent ( QMouseEvent * event ) ;

};

#endif // PAWNLABEL_H
