#include "menuwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "SslSupport: " << QSslSocket::supportsSsl();
    qDebug() << "SslLibraryBuildVersion: " << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "SslLibraryRuntimeVersion: " << QSslSocket::sslLibraryVersionString();

    MenuWidget * menu = new MenuWidget();
    menu->initNetwork();
    menu->initLayout();
    menu->show();

    return a.exec();
}
