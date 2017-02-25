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
#include "GameObjects/TileObjects/TileObj.h"
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
cocos2d::Vec2 Helper::pointForTile(BaseObj * obj)
//--------------------------------------------------------------------
{
    return pointForColumnAndRow(obj->getColumn(), obj->getRow());
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

//--------------------------------------------------------------------
bool Helper::convertDirectionToSwipeDelta(int dir, int & horzDelta, int & vertDelta)
//--------------------------------------------------------------------
{
    bool result = true;
    auto direction = static_cast<CommonTypes::Direction>(dir);
    switch (direction)
    {
    case Direction::Up:
        vertDelta = -1;
        break;
    case Direction::Down:
        vertDelta = 1;
        break;
    case Direction::Left:
        horzDelta = -1;
        break;
    case Direction::Right:
        horzDelta = 1;
        break;
    default:
        result = false;
        break;
    }
    return result;
}

//--------------------------------------------------------------------
cocos2d::Color4B Helper::getScoreColorByObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    auto color = cocos2d::Color4B::WHITE;

    if (!obj) {
        return color;
    }

    if (obj->getType() == BaseObjectType::CookieObj) {
        auto cookie = dynamic_cast<CookieObj*>(obj);
        if (cookie) {
            color = getScoreColorByCookieType(cookie->getCookieType());
        }
    }
    else if (obj->getType() == BaseObjectType::FieldObj) {
        auto tileObj = dynamic_cast<TileObj*>(obj);
        if (tileObj) {
            color = getScoreColorByTileType(tileObj->getTileType());
        }
    }

    return color;
}

//--------------------------------------------------------------------
cocos2d::Color4B Helper::getScoreColorByCookieType(CommonTypes::CookieType type)
//--------------------------------------------------------------------
{
    auto color = cocos2d::Color4B::WHITE;
    switch (type)
    {
    case CookieType::Croissant:
        color = cocos2d::Color4B::ORANGE;
        break;
    case CookieType::Cupcake:
        color = cocos2d::Color4B::RED;
        break;
    case CookieType::Danish:
        color = cocos2d::Color4B::BLUE;
        break;
    case CookieType::Donut:
        color = cocos2d::Color4B::MAGENTA;
        break;
    case CookieType::Macaron:
        color = cocos2d::Color4B::GREEN;
        break;
    case CookieType::SugarCookie:
        color = cocos2d::Color4B::YELLOW;
        break;
    default:
        break;
    }
    return color;
}

//--------------------------------------------------------------------
cocos2d::Color4B Helper::getScoreColorByTileType(CommonTypes::TileType type)
//--------------------------------------------------------------------
{
    auto color = cocos2d::Color4B::WHITE;
    switch (type)
    {
    case TileType::Dirt:
    case TileType::Dirt_HP2:
    case TileType::Dirt_HP3:
        color = cocos2d::Color4B(156, 102, 31, 255);
        break;
    case TileType::Bush:
    case TileType::Bush_HP2:
        color = cocos2d::Color4B::GREEN;
        break;
    case TileType::RockWall:
        color = cocos2d::Color4B::BLACK;
        break;
    default:
        break;
    }
    return color;
}
