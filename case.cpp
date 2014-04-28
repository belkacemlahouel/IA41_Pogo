#include "case.h"

Case::Case()
{
}

Case::Case(int num)
{
    this->caseNum = num;
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

int Case::getCaseNum()
{
    return this->caseNum;
}
