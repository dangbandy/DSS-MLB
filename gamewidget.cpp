#include "gamewidget.h"
#include <QByteArray>

GameWidget::GameWidget(QWidget * parent) : QWidget(parent)
{

    gameLayout = new QVBoxLayout;
    gameTitle = new QLabel;
    gameThumbnail = new QLabel;
    gameDescription = new QLabel;

    networkAccessManager = new QNetworkAccessManager(this);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &GameWidget::loadThumbnail);

    next = prev = nullptr;

    thumbImg.load(noImgName.c_str());
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
    delete networkAccessManager;
}

/*
 * InitLayout sets the size of the application to the current screen's resolution.
 * Graphical object properties will also be initialized and added to the layout.
 */
void GameWidget::initLayout(int w, int h)
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

    gameThumbnail->setAlignment(Qt::AlignHCenter);

    gameDescription->setAlignment(Qt::AlignCenter);
    gameDescription->setWordWrap(true);
    gameDescription->setFont(QFont("Arial", h/20, QFont::Normal));
    gameDescription->setStyleSheet("QLabel { color : white; }");

    gameLayout->addWidget(gameTitle);
    gameLayout->addWidget(gameThumbnail);
    gameLayout->addWidget(gameDescription);
}

/*
 * InitData determines whether the game data have a valid URL. If so,
 * it will attempt a get request to retrieve the image from the URL.
 */
void GameWidget::initData(const GameModel & g)
{
    gameData = g;
    if(!gameData.getThumbNail().isEmpty())
    {
        QUrl url(gameData.getThumbNail());
        QNetworkRequest request(url);
        networkAccessManager->get(request);
    }
    display();
}

void GameWidget::setSelected(bool value)
{
    selected = value;
    display();
}

void GameWidget::setNext(GameWidget * gw)
{
    next = gw;
}

void GameWidget::setPrev(GameWidget * gw)
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

/*
 * Display decides whether to show the title and description or not based whether
 * object is selected in the MenuWidget. The size of the thumbnail will shrink to 50%
 * if the object is not selected.
 */
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

    gameThumbnail->setPixmap(thumbImg.scaled(thumbnailWidth,thumbnailHeight, Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

/*
 * LoadThumbnail checks whether the image get request was successful. If so, it will load the image as the thumbnail
 * for part of the game display.
 */
void GameWidget::loadThumbnail(QNetworkReply * reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        errorHandler.errorLog("Error retrieving img from " + gameData.getThumbNail().toStdString());
        return;
    }
    QByteArray thumbImgBytes = reply->readAll();
    thumbImg.loadFromData(thumbImgBytes);
    display();
    reply->deleteLater();
}

