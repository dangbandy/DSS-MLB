#include "menuwidget.h"
#include <QPushButton>
#include <fstream>

#include <QDebug>

MenuWidget::MenuWidget(QWidget *parent) : QWidget(parent)
{
    menuLayout = new QHBoxLayout;
    networkManager = new QNetworkAccessManager;
}

bool MenuWidget::initLayout()
{
    this->setWindowState(Qt::WindowMaximized);
    this->setFixedSize(this->maximumSize());
    QPixmap bkgnd(":/bgimg.jpg");
    //bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);


    QPushButton *button1 = new QPushButton("One");
    QPushButton *button2 = new QPushButton("Two");
    QPushButton *button3 = new QPushButton("Three");
    QPushButton *button4 = new QPushButton("Four");
    QPushButton *button5 = new QPushButton("Five");
    menuLayout->addWidget(button1);
    menuLayout->addWidget(button2);
    menuLayout->addWidget(button3);
    menuLayout->addWidget(button4);
    menuLayout->addWidget(button5);
    this->setLayout(menuLayout);
    return true;
}

bool MenuWidget::initNetwork()
{
    networkManager = new QNetworkAccessManager;
    QUrl url("http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1");
    QNetworkRequest request;
    request.setUrl(url);
    networkManager->get(request);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MenuWidget::onResult);

    return true;
}

MenuWidget::~MenuWidget()
{
    delete menuLayout;
    delete networkManager;
}

void MenuWidget::onResult(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError){
        //Error handling
        qDebug() << "there was an error\n";
        return;
    }
    std::string data = QString(reply->readAll()).toUtf8().constData();
    std::ofstream outs("./json.txt",std::ofstream::out);
    outs << data;


    qDebug() << endl <<  "Data size: " << data.length() << endl;


}

void MenuWidget::resize()
{

}
