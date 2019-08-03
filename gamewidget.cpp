#include "gamewidget.h"
#include <QByteArray>


GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{

    gameLayout = new QVBoxLayout;
    gameTitle = new QLabel;
    gameThumbnail = new QLabel;
    gameDescription = new QLabel;

    networkAccessManager = new QNetworkAccessManager(this);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &GameWidget::loadThumbnail);

    next = prev = nullptr;

    thumbnailWidth = 0;
    thumbnailHeight = 0;
    selected = false;
}

GameWidget::~GameWidget()
{
    delete gameLayout;
    delete gameTitle;
    delete gameThumbnail;
    delete gameDescription;
}

bool GameWidget::initLayout(int w, int h)
{
    gameLayout->setAlignment(Qt::AlignVCenter);
    this->setLayout(gameLayout);
    this->setFixedSize(w, h);
    thumbnailWidth = w;
    thumbnailHeight = h/2;

    gameTitle->setAlignment(Qt::AlignCenter);
    gameTitle->setWordWrap(true);
    gameTitle->setFont(QFont("Arial", h/15, QFont::Bold));
    gameTitle->setStyleSheet("QLabel { color : white; }");

    gameThumbnail->setAlignment(Qt::AlignCenter);

    gameDescription->setAlignment(Qt::AlignCenter);
    gameDescription->setWordWrap(true);
    gameDescription->setFont(QFont("Arial", h/20, QFont::Normal));
    gameDescription->setStyleSheet("QLabel { color : white; }");

    gameLayout->addWidget(gameTitle);
    gameLayout->addWidget(gameThumbnail);
    gameLayout->addWidget(gameDescription);

    return true;
}

bool GameWidget::initData(const GameModel & g)
{
    gameData = g;
    display();

    return true;
}


void GameWidget::setSelected(bool value)
{
    selected = value;
    display();
}

void GameWidget::setNext(GameWidget *gw)
{
    next = gw;
}

void GameWidget::setPrev(GameWidget *gw)
{
    prev = gw;
}

GameWidget *GameWidget::getPrev() const
{
    return prev;
}

GameWidget *GameWidget::getNext() const
{
    return next;
}

void GameWidget::display()
{
    if(selected)
    {
        gameTitle->setText(gameData.getTitle());
        gameDescription->setText(gameData.getDescription());
        thumbnailWidth = this->width();
        thumbnailHeight = this->height()/2;
    }
    else
    {
        gameTitle->setText("");
        gameDescription->setText("");
        thumbnailWidth = int(this->width() * .5);
        thumbnailHeight = int((this->height()/2) * .5);
    }


    if(gameData.getThumbNail().isEmpty())
    {
        QPixmap thumbImg;
        thumbImg.load(noImgName.c_str());
        gameThumbnail->setPixmap(thumbImg.scaled(thumbnailWidth,thumbnailHeight, Qt::KeepAspectRatio));
    }
    else
    {
        //qDebug() << "URL: " << gameData.getThumbNail();
        QUrl url;
        url = QUrl::fromEncoded(gameData.getThumbNail().toStdString().c_str());
        QNetworkRequest request;
        request.setUrl(url);
        networkAccessManager->get(request);
    }

}

bool GameWidget::loadThumbnail(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error loading img from URL.";
        return false;
    }
    /*
    QByteArray thumbImgBytes = reply->readAll();
    QPixmap thumbImg;
    thumbImg.loadFromData(thumbImgBytes);
    gameThumbnail->setPixmap(thumbImg.scaled(thumbnailWidth,thumbnailHeight, Qt::KeepAspectRatio));
    */
    qDebug() << "";
    reply->deleteLater();
    return true;
}

