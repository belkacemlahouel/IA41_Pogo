#ifndef CASE_H
#define CASE_H

#include <pawnlabel.h>
#include <iostream>
#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QObject>
#include <list>

using namespace std;

class Case : public QLabel
{
Q_OBJECT
private:

    int caseNum;
    int caseSize;

public:
    Case();
    Case(QWidget * parent);
    Case(int num, QWidget *parent);
    ~Case(){}
    Case& operator =(const Case& c);
    std::list<PawnLabel*> pawnList;
    void printContent();
    int getCaseSize();
signals:
    void caseClicked(Case*);

public slots:
    void mousePressEvent ( QMouseEvent * event ) ;

};

#endif // CASE_H
