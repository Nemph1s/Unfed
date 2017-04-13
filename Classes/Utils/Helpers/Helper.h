/**
* @file Utils/Helpers/Helper.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"
#include "Common/CommonTypes.h"
#include "Utils/PlatformMacros.h"

class CookieObj;
class BaseObj;
class Helper 
{
    CREATE_SINGLETON(Helper);

public:

    template <typename E>
    static constexpr int to_underlying(E e) noexcept {

        return static_cast<std::underlying_type_t<E>>(e);
    }

    static float randomFloatBetween(float smallNumber, float bigNumber);
    static float rangeRandom(float min, float max);
    static int random(int fromRange, int toRange);
    static CommonTypes::CookieType randomCookieType(int fromRange, int toRange);

    static float getDurationToTile(int8_t startRow, int8_t destinationRow);
    static int8_t getDistanceBetweenObjects(CommonTypes::CellPos cellPosA, CommonTypes::CellPos cellPosB);

    static cocos2d::String* getSpriteNameByFieldType(int fieldType);

    static bool isValidColumnAndRow(int column, int row);

    static cocos2d::Vec2 pointForColumnAndRow(int column, int row);
    static cocos2d::Vec2 pointForColumnAndRowWithPriority(int column, int row, int priority);
    static cocos2d::Vec2 pointForTile(BaseObj* obj);

    static bool convertPointToTilePos(cocos2d::Vec2& point, int& column, int& row);

    static CommonTypes::Direction invertDirection(const CommonTypes::Direction& direction);
    static CommonTypes::Direction invertDirection(int direction);

    static int getDirectionByTileFromAToB(int oldDirection, int fromCol, int fromRow, int toCol, int toRow);
    static CommonTypes::Direction getDirectionByTileFromAToB(int oldDirection, BaseObj* from, BaseObj* to);
    
    static bool convertDirectionToSwipeDelta(int dir, int& horzDelta, int& vertDelta);

    static cocos2d::Color4B getScoreColorByObj(BaseObj* obj);

private:
    static cocos2d::Color4B getScoreColorByCookieType(CommonTypes::CookieType type);
    static cocos2d::Color4B getScoreColorForFieldObj(CommonTypes::FieldType type);
    static cocos2d::Color4B getScoreColorForDudeObj(CommonTypes::FieldType type);
};
