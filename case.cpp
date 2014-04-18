#include "case.h"
#define CASE_SIZE 150

Case::Case() : QLabel()
{
    this->caseSize = CASE_SIZE;
}

Case::Case(QWidget * parent) : QLabel(parent)
{
    this->caseSize = CASE_SIZE;
}

Case::Case(int num,QWidget * parent) : QLabel(parent)
{
    this->caseNum = num;
    this->caseSize = CASE_SIZE;
}

Case& Case::operator =(const Case& c)
{
    this->pawnList = c.pawnList;
    this->caseNum = c.caseNum;

    return *this;
}

void Case::printContent()
{
    cout << " " << this->pawnList.size() << endl;
}

int Case::getCaseSize()
{
    return this->caseSize;
}

void Case::mousePressEvent ( QMouseEvent * event )
{
    emit caseClicked(this);
}
