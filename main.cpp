#include "menuwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MenuWidget * menu = new MenuWidget();
    if(!menu->initNetwork()){
        a.quit();
        return 0;
    }
    if(!menu->initLayout()){
        a.quit();
        return 0;
    }
    menu->show();

    return a.exec();
}
