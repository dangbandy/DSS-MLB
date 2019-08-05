/*
 *  Author: Andy Dang
 *  Date: 8/1/2019
 *  Description: Constants.h contains immutable variables.
 */
#ifndef STATIDDATA_H
#define STATIDDATA_H
#include "errorhandler.h"

const std::string baseURLStart = "http://statsapi.mlb.com/api/v1/schedule?hydrate=game(content(editorial(recap))),decisions&date=";
const std::string baseURLEnd = "&sportId=1";

const std::string bgName = ":/bgimg.jpg";
const std::string noImgName = ":/noimg.png";

const int cutIdx = 10;//The image cut JSON array always contain the same amount of cuts. The 10th cut is 430x242 image.
const int maxGamesDisplay = 4;

static ErrorHandler errorHandler;

#endif // STATIDDATA_H
