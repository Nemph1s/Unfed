/**
* @file Utils/Helpers/Helper.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Utils/Helpers/Helper.h"
#include "Utils/GameResources.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include <random>

using namespace GameResources;
using namespace CommonTypes;

//--------------------------------------------------------------------
float Helper::randomFloatBetween(float smallNumber, float bigNumber) 
//--------------------------------------------------------------------
{
    float diff = bigNumber - smallNumber;
    return (((float)rand() / RAND_MAX) * diff) + smallNumber;
}

//--------------------------------------------------------------------
int Helper::random(int fromRange, int toRange)
//--------------------------------------------------------------------
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uniform_dist(fromRange, toRange);
    return uniform_dist(gen);
}

//--------------------------------------------------------------------
CookieType Helper::randomCookieType(int fromRange, int toRange)
//--------------------------------------------------------------------
{
    int randValue = random(fromRange, toRange);
    return static_cast<CookieType>(randValue);
}

//--------------------------------------------------------------------
cocos2d::Vec2 Helper::pointForColumnAndRow(int column, int row)
//--------------------------------------------------------------------
{
    float offsetX = 2.5f * column;
    float offsetY = 2.5f * (NumRows - row - 1);
    return cocos2d::Vec2(offsetX + column * TileWidth + TileWidth / 2, offsetY + (NumRows - row - 1) * TileHeight + TileHeight / 2);
}

//--------------------------------------------------------------------
cocos2d::Vec2 Helper::pointForCookie(CookieObj * cookie)
//--------------------------------------------------------------------
{
    return pointForColumnAndRow(cookie->getColumn(), cookie->getRow());
}

//--------------------------------------------------------------------
bool Helper::convertPointToTilePos(cocos2d::Vec2& point, int& column, int& row)
//--------------------------------------------------------------------
{
    if (point.x >= 0 && point.x < NumColumns*TileWidth && point.y >= 0 && point.y < NumRows*TileHeight) {
        column = point.x / TileWidth;
        row = NumColumns - (point.y / TileHeight);
        return true;
    }
    return false;
}
