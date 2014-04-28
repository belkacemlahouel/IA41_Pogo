#ifndef CASE_H
#define CASE_H

#include <pawnlabel.h>
#include <pawn.h>
#include <iostream>
#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QObject>
#include <list>

using namespace std;

class Case : public QObject
{
Q_OBJECT
private:
    int caseNum;

public:
    Case ();
    Case(int num);
    ~Case(){}
    Case& operator =(const Case& c);
    int getCaseNum();
    void printContent();
    int getCaseSize();

    // TODO : CHANGE TO PRIVATE
    std::list<Pawn*> pawnList;

};

#endif // CASE_H
