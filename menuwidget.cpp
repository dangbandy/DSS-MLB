#include "menuwidget.h"

#include <ctime>
#include <sstream>

#include <QDesktopWidget>
#include <QApplication>
#include <QNetworkConfigurationManager>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

MenuWidget::MenuWidget(QWidget * parent) : QWidget(parent)
{
    maxWidth = maxHeight = selectedGameDataIdx = 0;
    head = tail = focusedGame = nullptr;

    menuLayout = new QHBoxLayout;
    networkAccessManager = new QNetworkAccessManager(this);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &MenuWidget::onResult);
}

/*
 * InitLayout sets the size of the application to the current resolution of the screen.
 * It will also setup the layout alignment property as well as the background image of the program.
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
 * InitNetwork checks for internet connection. If there is connection then
 * it will build the mlb stats URL and perform a get request. Otherwise,
 * an error box will pop up and the program will terminate.
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
 * KeyPressEvent listens for the left and right arrow keys input. If the input is left,
 * the following logic will determine the key's functionality:
 *      If the current focused/selected GameWidget is the very left/head and the next
 * game data is within the boundary of the GameDataList indices, then insert a new
 * GameWidget with the next game data onto the doubly linked list. Afterward, remove
 * the most right/tail out of the list and display.
 *
 *      Otherwise, if the current focused/selected GameWidget is not the very left/head
 * and the next game data is within the boundary of the GameDataList indices, then move
 * the focused GameWidget to its next.
 *
 * If the input is right, then the same logic applies to insert to the most right/tail,
 * remove the most left/head, and set the new focused GameWidget.
 *
 */
void MenuWidget::keyPressEvent(QKeyEvent * event)
{
    if(event == nullptr)
    {
        errorHandler.errorLog("Error occurred in keyPressEvent(QKeyEvent *).");
        return;
    }

    if(head == nullptr)
    {
        errorHandler.errorLog("User attempted to move focused game before the InitData function was called.");
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
 * OnResult retrieves the URL get request response. If the response is not an error and is a JSON
 * document, it will be converted into a JSON object for later parsing. The function will then
 * call the InitData function to parse and push each game data onto a vector.
 */
void MenuWidget::onResult(QNetworkReply * reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
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

    initData(jsonObj);

    reply->deleteLater();
}

/*
 * InitData parses the JSON and populate the GameDataList vector with GameModel objects.
 * It will also populize the doubly linked list to contain the max amount of games to display
 * on the screen. The head of the list will become the first to be selected/focused.
 */
void MenuWidget::initData(const QJsonObject & json)
{
    const char * dates = "dates";
    const char * games = "games";

    if (json.contains(dates) && json[dates].isArray())
    {
         QJsonArray datesArray = json[dates].toArray();
         if(!datesArray.isEmpty())
         {
             QJsonObject dates = datesArray[0].toObject();
             if (dates.contains(games) && dates[games].isArray())
             {
                 QJsonArray gamesArray = dates[games].toArray();
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
}

/*
 * InsertFirst will insert a new GameWidget to the head of the list. If the
 * list is currently empty, then a new GameWidget is allocated and set to become
 * the head and the tail of the list. Otherwise, the new GameWidget's next
 * will reference the current head, and the current head's previous will reference
 * the new GameWidget. The new GameWidget will become the new head.
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
 * InsertLast will insert a new GameWidget to the tail of the list. If the
 * list is currently empty, then InsertFirst will handle the insertion. Otherwise,
 * a new GameWidget object is allocated and initialized. The current tail's next
 * will reference the new GameWidget, and the new GameWidget's previous will reference
 * the current tail. The new GameWidget will become the new tail.
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
 * DeleteFirst will deallocate the head of the doubly linkedlist. If
 * there is only one GameWidget in the list, then it will free single
 * GameWidget and delete all references to it. Otherwise, all references
 * to the current head will be cut and the head's memory will be deallocated.
 * The node next to the current head will become the new head.
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
 * DeleteLast will deallocate the tail of the doubly linkedlist. If there
 * is only one GameWidget in the list, then DeleteFirst will handle the
 * deletion. Otherwise, all references to the current tail will be cut
 * and the current tail memory will be free. The node previous to the current
 * tail will then become the new tail.
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
 * GetDate will calculate yesterday's date, utilizing the ctime library,
 * and return it as a string in a YY-MM-DD format.
 */
std::string MenuWidget::getDate()
{
    time_t now = time(nullptr);
    tm *date = localtime(&now); //Get current date.
    --date->tm_mday; //Move back 1 day
    std::stringstream ss;

    int year = date->tm_year+1900;
    int month = date->tm_mon+1;
    int day= date->tm_mday;


    ss << year << '-' << month << '-' << day;
    return ss.str();
}



