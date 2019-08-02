#include "menuwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MenuWidget * menu = new MenuWidget();
    menu->initNetwork();
    menu->initLayout();
    menu->show();

    return a.exec();
}
