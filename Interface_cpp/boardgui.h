#ifndef BOARDGUI_H
#define BOARDGUI_H

#include <QWidget>
#include <casegui.h>
#include <case.h>
#include <QCloseEvent>

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

    signals:
        void moveFinished();  // jamais appelé ? A vérifier

    public slots:
        void movePawnLabels(Case* oldCase, Case* dest); // signal reçu à la fin du movePawns du board, active le déplacement des pawnLabels correspondant
        void deselectPawnsLabels(PawnLabel *p); // lorsqu'un label est clické, automatiquement désélectionne tous les autres

};

#endif // BOARDGUI_H
