/*
 * Author: Andy Dang
 * Date: 8/1/2019
 * Description: The main function executes and display the full program.
 */

#include "menuwidget.h"
#include <QApplication>

int main(int argc, char * argv[])
{
    QApplication a(argc, argv);

    MenuWidget * menu = new MenuWidget();
    menu->initNetwork();
    menu->initLayout();
    menu->show();

    return a.exec();
}
