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
#include "Common/GameObjTypes.h"
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
    static GOT::CookieType randomCookieType(int fromRange, int toRange);

    /**
    * Calculate duration from startRow to destinationRow
    * Likewise, the duration of the animation is based on how far the cookie has to fall (0.1 seconds per tile). 
    * You can tweak these numbers to change the feel of the animation.
    * @param startRow The row from which object start fall down.
    * @param destinationRow The destination row of the falling down object.
    */
    static float getDurationToTile(int8_t startRow, int8_t destinationRow);
    /**
    * Calculate duration from startRow to destinationRow
    * @param startCell The cell from which object start fall down.
    * @param destinationCell The destination cell of the falling down object.
    */
    static float getDurationToTile(CT::Cell& startCell, CT::Cell& destinationCell);

    static int8_t getDistanceBetweenObjects(CT::Cell& cellPosA, CT::Cell& cellPosB);

    static cocos2d::String* getSpriteNameByFieldType(int fieldType);
    static cocos2d::String* getSpriteNameByEnemyType(int enemyType);

    static bool isValidCell(CT::Cell& cell);

    static cocos2d::Vec2 pointForCell(CT::Cell& cell);
    static cocos2d::Vec2 pointForCellWithPriority(CT::Cell& cell, int priority);
    static cocos2d::Vec2 pointForTile(BaseObj* obj);

    static cocos2d::Vec2 pointForGoalSprite(int currGoal, int goalsCount, const cocos2d::Size& spriteSize);

    static CT::Cell cellFromPoint(const cocos2d::Vec2& point);
    static bool convertPointToTilePos(cocos2d::Vec2& point, CT::Cell& cell);

    static CT::Direction invertDirection(const CT::Direction& direction);
    static CT::Direction invertDirection(int direction);

    static int getDirectionByTileFromAToB(int oldDirection, CT::Cell& fromCell, CT::Cell& toCell);
    static CT::Direction getDirectionByTileFromAToB(int oldDirection, BaseObj* from, BaseObj* to);
    
    static bool convertDirectionToSwipeDelta(int dir, int& horzDelta, int& vertDelta);

    static cocos2d::Color4B getScoreColorByObj(BaseObj* obj);

private:
    static cocos2d::Color4B getScoreColorByCookieType(int type);
    static cocos2d::Color4B getScoreColorForFieldObj(int type);
    static cocos2d::Color4B getScoreColorForDudeObj(int type);
    static cocos2d::Color4B getScoreColorForEnemyObj(int type);
};
