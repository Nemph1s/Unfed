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
#include "Common/GlobalInfo/GlobalInfo.h"

#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/TileObjects/TileObj.h"
#include "Controller/ObjectController/Dude/DudeObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"
#include <random>
#include <cstdlib>

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
cocos2d::String* Helper::getSpriteNameByFieldType(int fieldType)
//--------------------------------------------------------------------
{
    cocos2d::String* str = nullptr;
    auto type = static_cast<FieldType>(fieldType);
    int firstDudeType = Helper::to_underlying(FieldType::DudeFromAToB);
    switch (type)
    {
    case FieldType::Dirt:
        str = &GameResources::s_DirtImg;
        break;
    case FieldType::Dirt_HP2:
    case FieldType::Dirt_HP3:
        str = &GameResources::s_DirtX2Img;
        break;
    case FieldType::Bush:
        str = &GameResources::s_BushCorruptedImg;
        break;
    case FieldType::Bush_HP2:
        str = &GameResources::s_BushNormalImg;
        break;
    case FieldType::RockWall:
        str = &GameResources::s_RockImg;
        break;
    case FieldType::DudeFromAToB:
    case FieldType::DudeFromAToBx3:
    case FieldType::DudeChainX:
    case FieldType::DudeAllOfType:
    case FieldType::DudeSquareBomb:
        str = &GameResources::s_dudeSpriteNames.at(fieldType - firstDudeType);
        break;
    case FieldType::Unknown:
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
    auto tileWidth = GlobalInfo->getTileWidth();
    auto tileHeight = GlobalInfo->getTileHeight();
    return cocos2d::Vec2(offsetX + column * tileWidth + tileWidth / 2, offsetY + (NumRows - row - 1) * tileHeight + tileHeight / 2);
}

//--------------------------------------------------------------------
cocos2d::Vec2 Helper::pointForColumnAndRowWithPriority(int column, int row, int priority)
//--------------------------------------------------------------------
{
    auto pos = pointForColumnAndRow(column, row);
    if (priority > 0) {
        auto tileHeight = GlobalInfo->getTileHeight();
        pos.y += (tileHeight / 4) * priority;
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
    if (point.x >= 0 && point.x < NumColumns * GlobalInfo->getTileWidth() &&
        point.y >= 0 && point.y < NumRows * GlobalInfo->getTileHeight()) {
        column = point.x / GlobalInfo->getTileWidth();
        row = NumColumns - (point.y / GlobalInfo->getTileHeight());
        return true;
    }
    return false;
}

//--------------------------------------------------------------------
Direction Helper::invertDirection(const Direction & direction)
//--------------------------------------------------------------------
{
    auto dir = Direction::Unknown;
    switch (direction)
    {
    case Direction::Up:
        dir = Direction::Left;
        break;
    case Direction::Down:
        dir = Direction::Right;
        break;
    case Direction::Left:
        dir = Direction::Up;
        break;
    case Direction::Right:
        dir = Direction::Down;
        break;
    default:
        break;
    }
    return dir;
}

//--------------------------------------------------------------------
CommonTypes::Direction Helper::invertDirection(int direction)
//--------------------------------------------------------------------
{
    auto dir = static_cast<CommonTypes::Direction>(direction);
    return invertDirection(dir);
}

//--------------------------------------------------------------------
int Helper::getDirectionByTileFromAToB(int oldDirection, int fromCol, int fromRow, int toCol, int toRow)
//--------------------------------------------------------------------
{
    auto dir = Direction::Unknown;
    int8_t xDiff = toCol - fromCol;
    int8_t yDiff = toRow - fromRow;
    if (std::abs(xDiff) != std::abs(yDiff)) {
        if (std::abs(xDiff) > std::abs(yDiff)) {
            dir = xDiff > 0 ? Direction::Left : Direction::Right;
        } else {
            dir = yDiff < 0 ? Direction::Up : Direction::Down;
        }
    } else {
        return oldDirection;
    }
    
    return to_underlying(dir);
}

//--------------------------------------------------------------------
CommonTypes::Direction Helper::getDirectionByTileFromAToB(int oldDirection, BaseObj * from, BaseObj * to)
//--------------------------------------------------------------------
{
    auto direction = getDirectionByTileFromAToB(oldDirection, from->getColumn(), from->getRow(), to->getColumn(), to->getRow());
    return static_cast<CommonTypes::Direction>(direction);
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
            color = getScoreColorForFieldObj(tileObj->getFieldType());
        }
    }
    else if (obj->getType() == BaseObjType::Dude) {
        auto dudeObj = dynamic_cast<DudeObj*>(obj);
        if (dudeObj) {
            color = getScoreColorForDudeObj(dudeObj->getFieldType());
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
cocos2d::Color4B Helper::getScoreColorForFieldObj(CommonTypes::FieldType type)
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

//--------------------------------------------------------------------
cocos2d::Color4B Helper::getScoreColorForDudeObj(CommonTypes::FieldType type)
//--------------------------------------------------------------------
{
    // see hints on http://www.colorhexa.com/color-names
    auto color = cocos2d::Color4B(209, 159, 232, 255); //Bright ube 
    switch (type)
    {
    case FieldType::DudeFromAToB:
        color = cocos2d::Color4B(193, 154, 107, 150); //Desert 
        break;
    case FieldType::DudeFromAToBx3:
        color = cocos2d::Color4B(161, 202, 241, 255); //Baby blue eyes
        break;
    case FieldType::DudeAllOfType:
        color = cocos2d::Color4B(80, 200, 120, 255); // Emerald 
        break;
    case FieldType::DudeChainX:
        color = cocos2d::Color4B(170, 240, 209, 255); //Magic mint 
        break;
    case FieldType::DudeSquareBomb:
        color = cocos2d::Color4B(161, 202, 241, 255); //Baby blue eyes
        break;
    default:
        break;
    }
    return color;
}
