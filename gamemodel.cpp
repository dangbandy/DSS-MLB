#include "gamemodel.h"
#include <QJsonArray>

GameModel::GameModel()
{
    title = "Title missing";
    description = "Description missing";
}

GameModel::GameModel(const QJsonObject &json)
{
    title = "Title missing";
    description = "Description missing";
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

/*
 * Function: Read
 * Description:
 */
void GameModel::read(const QJsonObject &json)
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
                errorHandler.errorLog("Home->Team JSON does not exist");
            }
        }
        else {
            errorHandler.errorLog("Teams->Home JSON does not exist");
        }

        if(teamsJson.contains("away") && teamsJson["away"].isObject())
        {
            QJsonObject awayJson = teamsJson["away"].toObject();
            if(awayJson.contains("team") && awayJson["team"].isObject()){
                QJsonObject awayTeamJson = awayJson["team"].toObject();
                title += awayTeamJson["name"].toString();
            }
            else {
                errorHandler.errorLog("Away->Team JSON does not exist");
            }
        }
        else {
            errorHandler.errorLog("Teams->Away JSON does not exist");
        }
    }
    else
    {
        errorHandler.errorLog("Teams JSON does not exist");
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
                            errorHandler.errorLog("Content->Editorial->Recap->Mlb->Image->Cuts JSON array does not exist");
                        }
                    }
                    else
                    {
                        errorHandler.errorLog("Content->Editorial->Recap->Mlb->Image JSON does not exist");
                    }
                    //Parse description
                    if(mlbJson.contains("headline") && mlbJson["headline"].isString())
                    {
                        description = mlbJson["headline"].toString();
                    }

                }
                else
                {
                    errorHandler.errorLog("Content->Editorial->Recap->Mlb JSON does not exist");
                }
            }
            else
            {
                errorHandler.errorLog("Content->Editorial->Recap JSON does not exist");
            }
        }
        else
        {
            errorHandler.errorLog("Content->Editorial JSON does not exist");
        }

    }
    else
    {
        errorHandler.errorLog("Content JSON does not exist.");
    }
}

