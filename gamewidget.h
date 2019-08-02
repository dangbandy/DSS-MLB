#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H
#include <QWidget>

#include <QVBoxLayout>


class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

private:
    QHBoxLayout *gameLayout;
};
#endif // GAMEWIDGET_H
