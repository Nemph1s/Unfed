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
#include "Controller/ObjectController/Enemy/EnemyObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"
#include <random>
#include <cstdlib>

using namespace GameResources;
using namespace CT;
using namespace GOT;

//--------------------------------------------------------------------
float Helper::randomFloatBetween(float smallNumber, float bigNumber) 
//--------------------------------------------------------------------
{
    float diff = bigNumber - smallNumber;
    return (((float)rand() / RAND_MAX) * diff) + smallNumber;
}

//--------------------------------------------------------------------
float Helper::rangeRandom(float min, float max)
//--------------------------------------------------------------------
{
    float rnd = ((float)rand() / (float)RAND_MAX);
    return rnd*(max - min) + min;
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
float Helper::getDurationToTile(int8_t startRow, int8_t destinationRow)
//--------------------------------------------------------------------
{
    const float tileOffset = 0.09f;
    float timeToTile = fabs(startRow - destinationRow);
    return (timeToTile * 0.1f) + tileOffset;
}

//--------------------------------------------------------------------
float Helper::getDurationToTile(CT::Cell& startCell, CT::Cell& destinationCell)
//--------------------------------------------------------------------
{
    return getDurationToTile(startCell.row, destinationCell.row);
}

//--------------------------------------------------------------------
int8_t Helper::getDistanceBetweenObjects(CT::Cell& cellPosA, CT::Cell& cellPosB)
//--------------------------------------------------------------------
{
    uint8_t colLength = std::abs(cellPosA.column - cellPosB.column);
    uint8_t rowLength = std::abs(cellPosA.row - cellPosB.row);
    return MAX(colLength, rowLength);
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
cocos2d::String * Helper::getSpriteNameByEnemyType(int enemyType)
//--------------------------------------------------------------------
{
    cocos2d::String* str = nullptr;
    if (enemyType >= to_underlying(EnemyType::Simple) && enemyType < to_underlying(EnemyType::Unknown)) {
        str = &GameResources::s_enemySpriteNames.at(enemyType);
    }
    return str;
}

//--------------------------------------------------------------------
bool Helper::isValidCell(CT::Cell& cell)
//--------------------------------------------------------------------
{
    bool result = true;
    if (cell.column < 0 || cell.column >= _GlobalInfo::NumColumns || cell.row < 0 || cell.row >= _GlobalInfo::NumColumns) {
        result = false;
    }
    return result;
}

//--------------------------------------------------------------------
cocos2d::Vec2 Helper::pointForCell(CT::Cell& cell)
//--------------------------------------------------------------------
{
    float offsetX = 2.5f * cell.column;
    float offsetY = 2.5f * (_GlobalInfo::NumRows - cell.row - 1);
    auto tileWidth = GlobInfo->getTileWidth();
    auto tileHeight = GlobInfo->getTileHeight();
    return cocos2d::Vec2(offsetX + cell.column * tileWidth + tileWidth / 2
        , offsetY + (_GlobalInfo::NumRows - cell.row - 1) * tileHeight + tileHeight / 2);
}

//--------------------------------------------------------------------
cocos2d::Vec2 Helper::pointForCellWithPriority(CT::Cell& cell, int priority)
//--------------------------------------------------------------------
{
    auto pos = pointForCell(cell);
    if (priority > 0) {
        auto tileHeight = GlobInfo->getTileHeight();
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
            pos = pointForCellWithPriority(obj->getCell(), tileObj->getPriority());
        }
    } else {
        pos = pointForCell(obj->getCell());
    }
    return pos;
}

//--------------------------------------------------------------------
CT::Cell Helper::cellFromPoint(const cocos2d::Vec2 & point)
//--------------------------------------------------------------------
{
    auto cell = CT::Cell();
    if (point.x >= 0 && point.x < _GlobalInfo::NumColumns * GlobInfo->getTileWidth() &&
        point.y >= 0 && point.y < _GlobalInfo::NumRows * GlobInfo->getTileHeight()) {
        cell.column = point.x / GlobInfo->getTileWidth();
        cell.row = _GlobalInfo::NumColumns - (point.y / GlobInfo->getTileHeight());
        return cell;
    }
    return cell;
}

//--------------------------------------------------------------------
bool Helper::convertPointToTilePos(cocos2d::Vec2& point, CT::Cell& cell)
//--------------------------------------------------------------------
{
    if (point.x >= 0 && point.x < _GlobalInfo::NumColumns * GlobInfo->getTileWidth() &&
        point.y >= 0 && point.y < _GlobalInfo::NumRows * GlobInfo->getTileHeight()) {
        cell.column = point.x / GlobInfo->getTileWidth();
        cell.row = _GlobalInfo::NumColumns - (point.y / GlobInfo->getTileHeight());
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
CT::Direction Helper::invertDirection(int direction)
//--------------------------------------------------------------------
{
    auto dir = static_cast<CT::Direction>(direction);
    return invertDirection(dir);
}

//--------------------------------------------------------------------
int Helper::getDirectionByTileFromAToB(int oldDirection, CT::Cell& fromCell, CT::Cell& toCell)
//--------------------------------------------------------------------
{
    auto dir = Direction::Unknown;
    int8_t xDiff = toCell.column - fromCell.column;
    int8_t yDiff = toCell.row - fromCell.row;
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
CT::Direction Helper::getDirectionByTileFromAToB(int oldDirection, BaseObj* from, BaseObj* to)
//--------------------------------------------------------------------
{
    auto direction = getDirectionByTileFromAToB(oldDirection, from->getCell(), to->getCell());
    return static_cast<CT::Direction>(direction);
}

//--------------------------------------------------------------------
bool Helper::convertDirectionToSwipeDelta(int dir, int & horzDelta, int & vertDelta)
//--------------------------------------------------------------------
{
    bool result = true;
    auto direction = static_cast<CT::Direction>(dir);
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
    auto type = obj->getTypeAsInt();
    if (obj->getType() == BaseObjType::Cookie) {
        color = getScoreColorByCookieType(type);
    }
    else if (obj->getType() == BaseObjType::Field) {
        color = getScoreColorForFieldObj(type);
    }
    else if (obj->getType() == BaseObjType::Dude) {
        color = getScoreColorForDudeObj(type);
    }
    else if (obj->getType() == BaseObjType::Enemy) {
        color = getScoreColorForEnemyObj(type);
    }

    return color;
}

//--------------------------------------------------------------------
cocos2d::Color4B Helper::getScoreColorByCookieType(int type)
//--------------------------------------------------------------------
{
    auto color = cocos2d::Color4B::WHITE;
    auto cookieType = static_cast<CookieType>(type);
    switch (cookieType)
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
cocos2d::Color4B Helper::getScoreColorForFieldObj(int type)
//--------------------------------------------------------------------
{
    auto color = cocos2d::Color4B::WHITE;
    auto fieldType = static_cast<FieldType>(type);
    switch (fieldType)
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
cocos2d::Color4B Helper::getScoreColorForDudeObj(int type)
//--------------------------------------------------------------------
{
    // see hints on http://www.colorhexa.com/color-names
    auto color = cocos2d::Color4B(209, 159, 232, 255); //Bright ube 
    auto dudeType = static_cast<FieldType>(type);
    switch (dudeType)
    {
    case FieldType::DudeFromAToB:
        color = cocos2d::Color4B(193, 154, 107, 150); //Desert 
        break;
    case FieldType::DudeFromAToBx3:
        color = cocos2d::Color4B(161, 202, 241, 255); //Baby blue eyes
        break;
    case FieldType::DudeAllOfType:
        color = cocos2d::Color4B(161, 202, 241, 255); //Baby blue eyes
        break;
    case FieldType::DudeChainX:
        color = cocos2d::Color4B(170, 240, 209, 255); //Magic mint 
        break;
    case FieldType::DudeSquareBomb:
        color = cocos2d::Color4B(175, 64, 53, 255); // Pale Carmine 
        break;
    default:
        break;
    }
    return color;
}

//--------------------------------------------------------------------
cocos2d::Color4B Helper::getScoreColorForEnemyObj(int type)
//--------------------------------------------------------------------
{
    // see hints on http://www.colorhexa.com/color-names
    auto color = cocos2d::Color4B(0x0f, 0x0f, 0x0f, 255); //onyx
    auto enemyType = static_cast<EnemyType>(type);
    switch (type)
    {
    case EnemyType::Simple:
        color = cocos2d::Color4B(0x3e, 0xb4, 0x89, 255); //mint 
        break;
    case EnemyType::Shielded:
        color = cocos2d::Color4B(0xff, 0x45, 0x00, 255); //orange red
        break;
    default:
        break;
    }
    return color;
}
