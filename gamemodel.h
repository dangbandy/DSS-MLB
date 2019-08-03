#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <iostream>
#include "statiddata.h"
#include <QJsonObject>

class GameModel
{
public:
    GameModel();
    GameModel(const QJsonObject &json);
    ~GameModel();
    QString getTitle() const;
    QString getThumbNail() const;
    QString getDescription() const;

    bool read(const QJsonObject &json);

private:
    QString title, thumbnail, description;
};
#endif // GAMEMODEL_H
