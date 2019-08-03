#ifndef STATIDDATA_H
#define STATIDDATA_H
#include <iostream>
#include <QDebug>

const std::string baseURL = "http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1";
const std::string bgName = ":/bgimg.jpg";
const std::string noImgName = ":/noimg.png";
const int cutIdx = 19;
const int maxGamesDisplay = 4;
#endif // STATIDDATA_H
