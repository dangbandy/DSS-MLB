/*
 *  Author: Andy Dang
 *  Date: 8/1/2019
 *  Description:
 */
#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "statiddata.h"
#include "gamemodel.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    void initLayout(int w, int h);
    void initData(const GameModel & g);

    void setSelected(bool value);

    void setNext(GameWidget *gw);
    void setPrev(GameWidget *gw);

    GameWidget *getPrev() const;
    GameWidget *getNext() const;

private:
    void display();
    void loadThumbnail(QNetworkReply *reply);

    QVBoxLayout *gameLayout;
    QLabel * gameTitle;
    QLabel * gameThumbnail;
    QLabel * gameDescription;

    QNetworkAccessManager* networkAccessManager;

    GameModel gameData;
    GameWidget *next;
    GameWidget *prev;

    QPixmap thumbImg;
    int thumbnailWidth;
    int thumbnailHeight;
    bool selected;

};
#endif // GAMEWIDGET_H
