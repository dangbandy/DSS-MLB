#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include "statiddata.h"
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
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();
    bool initLayout();
    bool initNetwork();

protected:
  virtual void  keyPressEvent(QKeyEvent *event);

private:
    void onResult(QNetworkReply* reply);

    bool insertFirst(const GameModel & g);
    bool insertLast(const GameModel & g);

    bool deleteFirst();
    bool deleteLast();

    void errorMsg(const char * msg);

    QHBoxLayout *menuLayout;
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
