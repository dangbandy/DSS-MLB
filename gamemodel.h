/*
 *  Author: Andy Dang
 *  Date: 8/2/2019
 *  Description: GameModel will parse and store the game JSON object values into its member variables.
 */
#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "constants.h"
#include <QJsonObject>

class GameModel
{
public:
    GameModel();
    GameModel(const QJsonObject & json);
    ~GameModel();
    QString getTitle() const;
    QString getThumbNail() const;
    QString getDescription() const;

    void read(const QJsonObject & json);

private:
    QString title, thumbnail, description;
};
#endif // GAMEMODEL_H
