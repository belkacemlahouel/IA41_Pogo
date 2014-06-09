#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QWidget>
#include <QPalette>
#include <optionmenu.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    OptionMenu *OM = new OptionMenu();


    return app.exec();
}
