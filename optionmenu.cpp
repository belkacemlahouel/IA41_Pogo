#include "optionmenu.h"

/* La fenêtre d'option est codée et designée en dur, c'est pourquoi le code est long */

OptionMenu::OptionMenu(QWidget *parent) : QWidget(parent)
{    
    this->setFixedSize(490, 250);

    QLabel* menuLabel = new QLabel("POGO V 0.3",this);
    menuLabel->move(210,5);

    QLabel* nivWhiteLabel = new QLabel("NIVEAU SI IA",this);
    nivWhiteLabel->move(10,110);

    QLabel* nivBlackLabel = new QLabel("NIVEAU SI IA",this);
    nivBlackLabel->move(420,110);

    QGroupBox *groupbox1 = new QGroupBox("BLANCS", this);

    whiteChoiceIa = new QRadioButton("IA");
    whiteChoiceHmn = new QRadioButton("Humain");

    whiteChoiceIa->setChecked(true);

    QVBoxLayout *vbox1 = new QVBoxLayout(this);
    vbox1->addWidget(whiteChoiceIa);
    vbox1->addWidget(whiteChoiceHmn);

    groupbox1->setLayout(vbox1);
    groupbox1->move(10,30);

    QGroupBox *groupbox2 = new QGroupBox("NOIRS", this);

    blackChoiceIa = new QRadioButton("IA");
    blackChoiceHmn = new QRadioButton("Humain");

    blackChoiceIa->setChecked(true);

    QVBoxLayout *vbox2 = new QVBoxLayout(this);
    vbox2->addWidget(blackChoiceIa);
    vbox2->addWidget(blackChoiceHmn);

    groupbox2->setLayout(vbox2);
    groupbox2->move(405,30);

    liste1 = new QComboBox(this);
    liste1->addItem("1");
    liste1->addItem("2");
    liste1->addItem("3");

    liste1->move(0,130);

    liste2 = new QComboBox(this);
    liste2->addItem("1");
    liste2->addItem("2");
    liste2->addItem("3");

    liste2->move(405,130);

    QPushButton *validate = new QPushButton("Valider",this);
    validate->move(210,210);

    connect(validate, SIGNAL(released()), this, SLOT(handleButton()));

    this->show();
}

/* Déclenchée lorsque les options sont validées, cette fonction :
 * - récupère les options rentrées
 * - ferme les options
 * - créé le jeu "PoGo" avec les paramètres correspondant
 * - lance le jeu avec PoGo::startGame() */

 void OptionMenu::handleButton()
 {
    //TODO : Gérer les infos des boutons, injecter ça dans PoGo, et démarrer le jeu !!
    qDebug()<<"Bouton presse. Prise en compte des options...";
    this->WhiteIALevel = this->liste1->currentIndex() + 1;
    this->BlackIALevel = this->liste2->currentIndex() + 1;

    this->WhiteIA = whiteChoiceIa->isChecked();
    this->BlackIA = blackChoiceIa->isChecked();

    qDebug()<<"Options du jeu : Blancs IA ? " << WhiteIA << " Noirs IA ? " << BlackIA << "Niveaux respectifs (si IA) : " << WhiteIALevel << " " << BlackIALevel;
    qDebug()<<"Demarrage du jeu !!!";

    PoGo *pg = new PoGo(NULL,WhiteIA,BlackIA,WhiteIALevel,BlackIALevel);
    this->close();
    pg->startGame();
    qDebug()<<"Le jeu tourne.";
 }
