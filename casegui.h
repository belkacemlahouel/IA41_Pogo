#ifndef CASEGUI_H
#define CASEGUI_H

#include <QWidget>
#include <QLabel>
#include <pawnlabel.h>

class CaseGUI : public QLabel
{
    Q_OBJECT
    private:
        int caseNum;
        int caseSize;

    public:
        CaseGUI();
        CaseGUI(QWidget* parent);
        ~CaseGUI(){}
        CaseGUI(int num,QWidget * parent);
        int getCaseSize();
        int getCaseNum();
        CaseGUI& operator =(const CaseGUI& c);

        // TODO : CHANGE TO PRIVATE
        std::list<PawnLabel*> pawnListGUI;

    signals:
        void caseClicked(CaseGUI*);

    public slots:
        void mousePressEvent ( QMouseEvent * event ) ;

};

#endif // CASEGUI_H
