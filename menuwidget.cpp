#include "menuwidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include <fstream>
#include <QMessageBox>
#include <QNetworkConfigurationManager>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

MenuWidget::MenuWidget(QWidget *parent) : QWidget(parent)
{
    maxWidth = maxHeight = selectedGameDataIdx = 0;
    head = tail = focusedGame = nullptr;

    menuLayout = new QHBoxLayout;
    networkAccessManager = new QNetworkAccessManager(this);
}

bool MenuWidget::initLayout()
{
    menuLayout->setAlignment(Qt::AlignCenter);

    QRect rec = QApplication::desktop()->screen()->rect();
    maxHeight = rec.height();
    maxWidth = rec.width();

    this->setFixedSize(maxWidth, maxHeight);
    QPixmap bkgnd(bgName.c_str());
    bkgnd = bkgnd.scaled(maxWidth, maxHeight,Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);


    this->setLayout(menuLayout);
    return true;
}

bool MenuWidget::initNetwork()
{
    QNetworkConfigurationManager mgr;
    if(!mgr.isOnline())
    {
        errorMsg("You are not connected to the internet :(");
        return false;
    }

    QUrl url(baseURL.c_str());
    QNetworkRequest request;
    request.setUrl(url);
    networkAccessManager->get(request);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &MenuWidget::onResult);
    return true;
}

MenuWidget::~MenuWidget()
{
    gameDataList.clear();

    while (head != nullptr)
    {
        deleteLast();
    }

    delete menuLayout;
    delete networkAccessManager;
}

void MenuWidget::keyPressEvent(QKeyEvent *event)
{
    if(event == nullptr)
    {
        qDebug() << "Error occurred in keyPressEvent(QKeyEvent *).";
        return;
    }

    switch (event->key()) {
    case Qt::Key_Left:
        qDebug() << "Left";
        if(focusedGame == head)
        {
            if(selectedGameDataIdx > 0)
            {
                focusedGame->setSelected(false);
                selectedGameDataIdx -= 1;
                insertFirst(gameDataList[selectedGameDataIdx]);
                deleteLast();
                focusedGame = head;
            }
        }
        else
        {
            selectedGameDataIdx -= 1;
            focusedGame->setSelected(false);
            focusedGame = focusedGame->getPrev();
            focusedGame->setSelected(true);
        }
        break;

    case Qt::Key_Right:
        qDebug() << "Right";
        if(focusedGame == tail)
        {
            if(selectedGameDataIdx < gameDataList.size()-1)
            {
                focusedGame->setSelected(false);
                selectedGameDataIdx += 1;
                insertLast(gameDataList[selectedGameDataIdx]);
                deleteFirst();
                focusedGame = tail;
            }
        }
        else
        {
            selectedGameDataIdx += 1;
            focusedGame->setSelected(false);
            focusedGame = focusedGame->getNext();
            focusedGame->setSelected(true);
        }
        break;
    default:
        break;
    }
}

//figure out a consistent error handling
void MenuWidget::onResult(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError){
        errorMsg("there was an error");
        return;
    }

    qDebug() << "Got to here";

    QJsonObject jsonObj;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QString(reply->readAll()).toUtf8());
    // check validity of the document
    if(!jsonDoc.isNull())
    {
        if(jsonDoc.isObject())
        {
            jsonObj = jsonDoc.object();
        }
        else
        {
            errorMsg("Invalid URL input");
            return;
        }
    }
    else
    {
        errorMsg("Invalid JSON...");
        return;
    }

    if (jsonObj.contains("dates") && jsonObj["dates"].isArray())
    {
         QJsonArray datesArray = jsonObj["dates"].toArray();
         if(!datesArray.isEmpty())
         {
             QJsonObject dates = datesArray[0].toObject();
             if (dates.contains("games") && dates["games"].isArray())
             {
                 QJsonArray gamesArray = dates["games"].toArray();
                 for (int gameIdx = 0; gameIdx < gamesArray.size(); ++gameIdx)
                 {
                    QJsonObject gameJson = gamesArray[gameIdx].toObject();
                    GameModel game(gameJson);
                    gameDataList.push_back(game);
                 }

                 if(gameDataList.size() == 0)
                 {
                     qDebug() << "Game data list is empty";
                     return;
                 }
                 for(unsigned int i = 0; i < maxGamesDisplay; i++)
                 {
                     insertLast(gameDataList[i]);
                     tail->setSelected(false);
                 }
                 head->setSelected(true);
                 focusedGame = head;

             }
             else
             {
                 qDebug() << "Games JSON does not exist" << endl;
             }
         }
         else
         {
             qDebug() << "Dates is empty" << endl;
         }
    }
    else
    {
        qDebug() << "Dates JSON does not exist " << endl;
    }

    reply->deleteLater();
}

bool MenuWidget::insertFirst(const GameModel & g)
{
    GameWidget * ng = new GameWidget();
    ng->setSelected(true);
    ng->initLayout(maxWidth/maxGamesDisplay, maxHeight/(maxGamesDisplay-1));
    ng->initData(g);

    ng->setNext(head);
    if(head != nullptr)
    {
        head->setPrev(ng);
    }
    else
    {
        tail = ng;
    }
    head = ng;

    menuLayout->insertWidget(0,ng, 0, Qt::AlignLeft);
    return true;
}

bool MenuWidget::insertLast(const GameModel & g)
{
    if(head == nullptr)
    {
        return insertFirst(g);
    }

    GameWidget * ng = new GameWidget();
    ng->setSelected(true);
    ng->initLayout(maxWidth/maxGamesDisplay, maxHeight/(maxGamesDisplay-1));
    ng->initData(g);

    ng->setPrev(tail);
    tail->setNext(ng);
    tail = ng;

    menuLayout->insertWidget(-1,ng, 0, Qt::AlignRight);
    return true;
}

bool MenuWidget::deleteFirst()
{
    if(head == nullptr)
    {
        return false;
    }

    if(head->getNext() == nullptr)
    {
        delete head;
        head = tail = nullptr;
        return true;
    }

    GameWidget * temp = head;
    head = head->getNext();
    delete temp;
    head->setPrev(nullptr);

    return true;
}

bool MenuWidget::deleteLast()
{
    if(tail == nullptr)
    {
        return false;
    }

    if(tail->getPrev() == nullptr)
    {
        return deleteFirst();
    }

    GameWidget * temp = tail;
    tail = tail->getPrev();
    delete temp;
    tail->setNext(nullptr);

    return true;
}


void MenuWidget::errorMsg(const char * msg)
{
    QMessageBox::critical(this, "Info", msg);
}
