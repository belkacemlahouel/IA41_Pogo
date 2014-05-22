#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <pogo.h>

class OptionMenu : public QWidget
{
    Q_OBJECT
private:
    int BlackIALevel;
    int WhiteIALevel;
    bool WhiteIA;
    bool BlackIA;
    QRadioButton *whiteChoiceIa;
    QRadioButton *whiteChoiceHmn;
    QRadioButton *blackChoiceIa;
    QRadioButton *blackChoiceHmn;
    QComboBox *liste1;
    QComboBox *liste2;

public:
    explicit OptionMenu(QWidget *parent = 0);

signals:

private slots:
    void handleButton(); // lorsqu'on valide les options, ce slot reçoit un signal et gère toutes les infos rentrées dans les options

};

#endif // OPTIONMENU_H
