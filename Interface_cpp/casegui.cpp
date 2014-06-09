#include "casegui.h"

#define CASE_SIZE 150

CaseGUI::CaseGUI() : QLabel()
{
    this->caseSize = CASE_SIZE;
}

CaseGUI::CaseGUI(QWidget *parent) : QLabel(parent)
{
    this->caseSize = CASE_SIZE;
}

CaseGUI::CaseGUI(int num,QWidget * parent) : QLabel(parent)
{
    this->caseNum = num;
    this->caseSize = CASE_SIZE;
}

CaseGUI& CaseGUI::operator =(const CaseGUI& c)
{
    this->pawnListGUI = c.pawnListGUI;
    this->caseNum = c.caseNum;

    return *this;
}

int CaseGUI::getCaseSize()
{
    return this->caseSize;
}

int CaseGUI::getCaseNum()
{
    return this->caseNum;
}

void CaseGUI::mousePressEvent ( QMouseEvent * event )
{
    emit caseClicked(this);
}


