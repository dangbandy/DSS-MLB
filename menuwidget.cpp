#include "menuwidget.h"

#include <ctime>
#include <sstream>

#include <QDesktopWidget>
#include <QApplication>
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
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &MenuWidget::onResult);
}

/*
 * Function: Init Layout
 * Description:
 */
void MenuWidget::initLayout()
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
}

/*
 * Function: Init Network
 * Description:
 */
void MenuWidget::initNetwork()
{
    QNetworkConfigurationManager mgr;
    if(!mgr.isOnline())
    {
        errorHandler.errorBox(this,"Internet connection is not established.");
        exit(EXIT_FAILURE);
    }

    std::string completeURL = baseURLStart + getDate() + baseURLEnd;
    QUrl url(completeURL.c_str());
    QNetworkRequest request(url);
    networkAccessManager->get(request);
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

/*
 * Function: Key Press Event
 * Description:
 */
void MenuWidget::keyPressEvent(QKeyEvent *event)
{
    if(event == nullptr)
    {
        errorHandler.errorLog("Error occurred in keyPressEvent(QKeyEvent *).");
        return;
    }

    switch (event->key()) {
    case Qt::Key_Left:
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

/*
 * Function: On Result
 * Description:
 */
void MenuWidget::onResult(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError){
        errorHandler.errorBox(this,"HTTP Request failed.");
        exit(EXIT_FAILURE);
    }

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
            errorHandler.errorBox(this, "Invalid URL input");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        errorHandler.errorBox(this, "Invalid JSON...");
        exit(EXIT_FAILURE);
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
                     errorHandler.errorLog("Game data list is empty.");
                     return;
                 }
                 for(unsigned int i = 0; i < maxGamesDisplay; i++)
                 {
                     insertLast(gameDataList[i]);
                     if(i != 0)
                     {
                         tail->setSelected(false);
                     }
                 }
                 focusedGame = head;

             }
             else
             {
                 errorHandler.errorLog("Dates->Games JSON does not exist.");
             }
         }
         else
         {
             errorHandler.errorLog("Dates Array JSON is empty.");
         }
    }
    else
    {
        errorHandler.errorLog("Dates JSON does not exist.");
    }

    reply->deleteLater();
}

/*
 * Function: Insert First
 * Description:
 */
bool MenuWidget::insertFirst(const GameModel & g)
{
    GameWidget * ng = new GameWidget();
    ng->initLayout(maxWidth/maxGamesDisplay, maxHeight/(maxGamesDisplay-1));
    ng->initData(g);
    ng->setSelected(true);


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

/*
 * Function: Insert Last
 * Description:
 */
bool MenuWidget::insertLast(const GameModel & g)
{
    if(head == nullptr)
    {
        return insertFirst(g);
    }

    GameWidget * ng = new GameWidget();
    ng->initLayout(maxWidth/maxGamesDisplay, maxHeight/(maxGamesDisplay-1));
    ng->initData(g);
    ng->setSelected(true);

    ng->setPrev(tail);
    tail->setNext(ng);
    tail = ng;

    menuLayout->insertWidget(-1,ng, 0, Qt::AlignRight);
    return true;
}

/*
 * Function: Delete First
 * Description:
 */
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

/*
 * Function: Delete Last
 * Description:
 */
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

/*
 * Function: Get Date
 * Description:
 */
std::string MenuWidget::getDate()
{
    time_t tt;
    time( &tt );
    tm TM = *localtime( &tt );
    std::stringstream ss;

    int year = TM.tm_year+1900;
    int month = TM.tm_mon+1;
    int day= TM.tm_mday;

    if(day == 1)
        {
            //months which have 30 days in previous month
            if(month == 4|| month == 6 || month == 9|| month == 11)
            {
                day = 31;
                month--;
            }
            //for MARCH, to define february last day
            else if(month==3)
            {
                if(year%4==0)
                    day=29;
                else
                    day=28;

                month--;
            }
            //for January, to define December last day
            else if(month==1)
            {
                day=31;
                month = 12;
                year--;
            }
            //for Feb, to define January last day
            else if(month==2)
            {
                day=31;
                month--;
            }
            //for other months
            else
            {
                day=30;
                month--;
            }
        }
    else
    {
        day--;
    }
    ss << year << '-' << month << '-' << day;
    return ss.str();
}



