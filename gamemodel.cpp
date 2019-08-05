#include "gamemodel.h"
#include <QJsonArray>

GameModel::GameModel()
{
    title = "Title missing";
    description = "Description missing";
}

GameModel::GameModel(const QJsonObject & json)
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
 * Read will parse the json object and initialize the game title, thumbnail URL, and description.
 * Title will be the name of the two teams. Thumbnail URL will be from the MLB editorial recap content
 * image. Description will be the the MLB editorial recap content headline.
 */
void GameModel::read(const QJsonObject & json)
{
    const char * teams = "teams";
    const char * home = "home";
    const char * team = "team";
    const char * name = "name";
    const char * away = "away";

    if(json.contains(teams) && json[teams].isObject())
    {
        title.clear();
        QJsonObject teamsJson = json[teams].toObject();

        if(teamsJson.contains(home) && teamsJson[home].isObject())
        {
            QJsonObject homeJson = teamsJson[home].toObject();
            if(homeJson.contains(team) && homeJson[team].isObject()){
                QJsonObject homeTeamJson = homeJson[team].toObject();
                title += homeTeamJson[name].toString() + " vs ";
            }
            else {
                errorHandler.errorLog("Home->Team JSON does not exist");
            }
        }
        else {
            errorHandler.errorLog("Teams->Home JSON does not exist");
        }

        if(teamsJson.contains(away) && teamsJson[away].isObject())
        {
            QJsonObject awayJson = teamsJson[away].toObject();
            if(awayJson.contains(team) && awayJson[team].isObject()){
                QJsonObject awayTeamJson = awayJson[team].toObject();
                title += awayTeamJson[name].toString();
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


    const char * content = "content";
    const char * editorial = "editorial";
    const char * recap = "recap";
    const char * mlb = "mlb";
    const char * image = "image";
    const char * cuts = "cuts";
    const char * src = "src";
    const char * headline = "headline";

    if(json.contains(content) && json[content].isObject())
    {
        QJsonObject contentJson = json[content].toObject();
        if(contentJson.contains(editorial) && contentJson[editorial].isObject())
        {
            QJsonObject editorialJson = contentJson[editorial].toObject();
            if(editorialJson.contains(recap) && editorialJson[recap].isObject())
            {
                QJsonObject recapJson = editorialJson[recap].toObject();
                if(recapJson.contains(mlb) && recapJson[mlb].isObject())
                {
                    QJsonObject mlbJson = recapJson[mlb].toObject();
                    if(mlbJson.contains(image) && mlbJson[image].isObject())
                    {
                        QJsonObject imageJson = mlbJson[image].toObject();
                        if(imageJson.contains(cuts) && imageJson[cuts].isArray())
                        {
                            QJsonArray cutsJsonArray = imageJson[cuts].toArray();
                            thumbnail = cutsJsonArray[cutIdx].toObject()[src].toString();
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
                    if(mlbJson.contains(headline) && mlbJson[headline].isString())
                    {
                        description = mlbJson[headline].toString();
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

