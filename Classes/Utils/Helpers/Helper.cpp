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
#include "GameObjects/TileObjects/DudeObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"
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
cocos2d::String* Helper::getSpriteNameByTileType(int tileType)
//--------------------------------------------------------------------
{
    cocos2d::String* str = nullptr;
    auto type = static_cast<CommonTypes::TileType>(tileType);
    int firstDudeType = Helper::to_underlying(CommonTypes::TileType::DudeFromAToB);
    switch (type)
    {
    case CommonTypes::TileType::Empty:
    case CommonTypes::TileType::Normal:
        str = &GameResources::s_TileImg;
        break;
    case CommonTypes::TileType::Water:
        str = &GameResources::s_TileImg;
        break;
    case CommonTypes::TileType::Dirt:
        str = &GameResources::s_DirtImg;
        break;
    case CommonTypes::TileType::Dirt_HP2:
    case CommonTypes::TileType::Dirt_HP3:
        str = &GameResources::s_DirtX2Img;
        break;
    case CommonTypes::TileType::Bush:
        str = &GameResources::s_BushCorruptedImg;
        break;
    case CommonTypes::TileType::Bush_HP2:
        str = &GameResources::s_BushNormalImg;
        break;
    case CommonTypes::TileType::RockWall:
        str = &GameResources::s_RockImg;
        break;
    case CommonTypes::TileType::DudeFromAToB:
    case CommonTypes::TileType::DudeFromAToBx3:
    case CommonTypes::TileType::DudeChainX:
    case CommonTypes::TileType::DudeAllOfType:
        str = &GameResources::s_dudeSpriteNames.at(tileType - firstDudeType);
        break;
    case CommonTypes::TileType::Unknown:
        break;
    default:
        break;
    }
    return str;
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
cocos2d::Vec2 Helper::pointForColumnAndRowWithPriority(int column, int row, int priority)
//--------------------------------------------------------------------
{
    auto pos = pointForColumnAndRow(column, row);
    if (priority > 0) {
        pos.y += (GameResources::TileHeight / 4) * priority;
    }
    return pos;
}

//--------------------------------------------------------------------
cocos2d::Vec2 Helper::pointForTile(BaseObj * obj)
//--------------------------------------------------------------------
{
    auto pos = cocos2d::Vec2::ZERO;
    if (obj->getType() == BaseObjType::Field) {
        auto tileObj = dynamic_cast<FieldObj*>(obj);
        if (tileObj) {
            pos = pointForColumnAndRowWithPriority(obj->getColumn(), obj->getRow(), tileObj->getPriority());
        }
    } else {
        pos = pointForColumnAndRow(obj->getColumn(), obj->getRow());
    }
    return pos;
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

    if (obj->getType() == BaseObjType::Cookie) {
        auto cookie = dynamic_cast<CookieObj*>(obj);
        if (cookie) {
            color = getScoreColorByCookieType(cookie->getCookieType());
        }
    }
    else if (obj->getType() == BaseObjType::Field) {
        auto tileObj = dynamic_cast<FieldObj*>(obj);
        if (tileObj) {
            color = getScoreColorByFieldType(tileObj->getFieldType());
        }
    }
    else if (obj->getType() == BaseObjType::Dude) {
        auto dudeObj = dynamic_cast<DudeObj*>(obj);
        if (dudeObj) {
            color = cocos2d::Color4B::MAGENTA;
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
cocos2d::Color4B Helper::getScoreColorByFieldType(CommonTypes::FieldType type)
//--------------------------------------------------------------------
{
    auto color = cocos2d::Color4B::WHITE;
    switch (type)
    {
    case FieldType::Dirt:
    case FieldType::Dirt_HP2:
    case FieldType::Dirt_HP3:
        color = cocos2d::Color4B(156, 102, 31, 255);
        break;
    case FieldType::Bush:
    case FieldType::Bush_HP2:
        color = cocos2d::Color4B::GREEN;
        break;
    case FieldType::RockWall:
        color = cocos2d::Color4B::BLACK;
        break;
    default:
        break;
    }
    return color;
}
