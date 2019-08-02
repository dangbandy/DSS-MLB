#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>

#include <QHBoxLayout>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QFrame>

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    bool initLayout();
    bool initNetwork();
    ~MenuWidget();

private:
    void onResult(QNetworkReply* reply);
    void resize();

    QHBoxLayout *menuLayout;
    QFrame * menuFrame;
    QNetworkAccessManager * networkManager;
};
#endif // MENUWIDGET_H
