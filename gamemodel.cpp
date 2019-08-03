#include "gamemodel.h"
#include <QJsonArray>

GameModel::GameModel()
{
    title = "No Title";
    description = "No Description";
}

GameModel::GameModel(const QJsonObject &json)
{
    title = "No Title";
    description = "No Description";
    read(json);
}

GameModel::~GameModel()
{

}

QString GameModel::getTitle() const
{
    return title;
}

QString GameModel::getThumbNail() const
{
    return thumbnail;
}

QString GameModel::getDescription() const
{
    return description;
}

bool GameModel::read(const QJsonObject &json)
{
    //Parse Title
    if(json.contains("teams") && json["teams"].isObject())
    {
        title.clear();
        QJsonObject teamsJson = json["teams"].toObject();

        if(teamsJson.contains("home") && teamsJson["home"].isObject())
        {
            QJsonObject homeJson = teamsJson["home"].toObject();
            if(homeJson.contains("team") && homeJson["team"].isObject()){
                QJsonObject homeTeamJson = homeJson["team"].toObject();
                title += homeTeamJson["name"].toString() + " vs ";
            }
            else {
                qDebug() << "Home Team JSON does not exist" << endl;
            }
        }
        else {
            qDebug() << "Teams Home JSON does not exist" << endl;
        }

        if(teamsJson.contains("away") && teamsJson["away"].isObject())
        {
            QJsonObject awayJson = teamsJson["away"].toObject();
            if(awayJson.contains("team") && awayJson["team"].isObject()){
                QJsonObject awayTeamJson = awayJson["team"].toObject();
                title += awayTeamJson["name"].toString();
            }
            else {
                qDebug() << "Away Team JSON does not exist" << endl;
            }
        }
        else {
            qDebug() << "Teams Away JSON does not exist" << endl;
        }
    }
    else
    {
        qDebug() << "Teams JSON does not exist" << endl;
    }


    if(json.contains("content") && json["content"].isObject())
    {
        QJsonObject contentJson = json["content"].toObject();
        if(contentJson.contains("editorial") && contentJson["editorial"].isObject())
        {
            QJsonObject editorialJson = contentJson["editorial"].toObject();
            if(editorialJson.contains("recap") && editorialJson["recap"].isObject())
            {
                QJsonObject recapJson = editorialJson["recap"].toObject();
                if(recapJson.contains("mlb") && recapJson["mlb"].isObject())
                {
                    QJsonObject mlbJson = recapJson["mlb"].toObject();

                    //Parse thumbnail url
                    if(mlbJson.contains("image") && mlbJson["image"].isObject())
                    {
                        QJsonObject imageJson = mlbJson["image"].toObject();
                        if(imageJson.contains("cuts") && imageJson["cuts"].isArray())
                        {
                            QJsonArray cutsJsonArray = imageJson["cuts"].toArray();
                            thumbnail = cutsJsonArray[cutIdx].toObject()["src"].toString();
                        }
                        else
                        {
                            qDebug() << "Content Editorial Recap Mlb Image Cuts JSON array does not exist" << endl;
                        }
                    }
                    else
                    {
                        qDebug() << "Content Editorial Recap Mlb Image JSON does not exist" << endl;
                    }
                    //Parse description
                    if(mlbJson.contains("headline") && mlbJson["headline"].isString())
                    {
                        description = mlbJson["headline"].toString();
                    }

                }
                else
                {
                    qDebug() << "Content Editorial Recap Mlb JSON does not exist" << endl;
                }
            }
            else
            {
                qDebug() << "Content Editorial Recap JSON does not exist" << endl;
            }
        }
        else
        {
            qDebug() << "Content Editorial JSON does not exist" << endl;
        }

    }
    else
    {
        qDebug() << "Content JSON does not exist." << endl;
    }

    qDebug() << "Title: " << title;
    qDebug() << "Thumbnail: " << thumbnail;
    qDebug() << "Description: " << description << endl;
    return true;
}
