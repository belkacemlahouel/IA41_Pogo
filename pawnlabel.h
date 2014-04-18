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
    bool isWhite; // S'il n'est pas blanc, il est noir.

public:
    PawnLabel(QWidget * parent = 0 );
    PawnLabel(QWidget * parent, bool _isWhite);
    ~PawnLabel(){}
    int getSelected();
    void setSelected(int select);
    bool getIsWhite();


signals:
    void clicked();

public slots:
    void pawnClicked();

protected:
    void mousePressEvent ( QMouseEvent * event ) ;

};

#endif // PAWNLABEL_H
