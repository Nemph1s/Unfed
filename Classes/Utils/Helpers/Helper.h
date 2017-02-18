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
    static int random(int fromRange, int toRange);
    static CommonTypes::CookieType randomCookieType(int fromRange, int toRange);

    static cocos2d::Vec2 pointForColumnAndRow(int column, int row);
    static cocos2d::Vec2 pointForTile(BaseObj* obj);

    static bool convertPointToTilePos(cocos2d::Vec2& point, int& column, int& row);
};
