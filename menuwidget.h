/*
 *  Author: Andy Dang
 *  Date: 8/1/2019
 *  Description: MenuWidget populates game models from the fetched JSON. It acts as a specialized doubly linkedlist.
 *                  It will also handles user inputs and what to display.
 */

#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include "constants.h"
#include "gamewidget.h"
#include "gamemodel.h"

#include <QWidget>

#include <QHBoxLayout>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QKeyEvent>

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget * parent = nullptr);
    ~MenuWidget();
    void initLayout();
    void initNetwork();

protected:
  virtual void  keyPressEvent(QKeyEvent * event);

private:
    void onResult(QNetworkReply * reply);
    void initData(const QJsonObject & json);

    bool insertFirst(const GameModel & g);
    bool insertLast(const GameModel & g);

    bool deleteFirst();
    bool deleteLast();

    std::string getDate();

    QHBoxLayout * menuLayout;
    QNetworkAccessManager * networkAccessManager;
    std::vector <GameModel> gameDataList;

    GameWidget * head;
    GameWidget * tail;

    GameWidget * focusedGame;
    unsigned int selectedGameDataIdx;

    int maxHeight;
    int maxWidth;

};
#endif // MENUWIDGET_H
