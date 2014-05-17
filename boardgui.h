#ifndef BOARDGUI_H
#define BOARDGUI_H

#include <QWidget>
#include <casegui.h>
#include <case.h>

class BoardGUI : public QWidget
{
    Q_OBJECT
    private:

    public:
        BoardGUI(QWidget* parent);
        ~BoardGUI(){}
        void insertPawnLabel(CaseGUI* c, PawnLabel *p);
        // this function inserts the pawn into the list of pawns on the case c(i,j)
        void insertPawnLabel(int i, int j, PawnLabel *p);
        void removePawn(CaseGUI *c, PawnLabel *p);
        BoardGUI& operator =(const BoardGUI& b);

        // A METTRE EN PRIVE :
        CaseGUI **boardGUI;

// ---------------------------------------------------------------------
//        // Closing the programm after closing the main window
//        bool closed = false;

//    protected:
//        // Closing the programm after closing the main window
//        void closeEvent(QCloseEvent*);
// ---------------------------------------------------------------------


    signals:
        void moveFinished();

    public slots:
        void movePawnLabels(Case* oldCase, Case* dest);
        void deselectPawnsLabels(PawnLabel *p);

};

#endif // BOARDGUI_H
