/**
* @file GameObjects/TileObjects/Obstacles/BushObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/TileObjects/Obstacles/RockObj.h"

#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

using CommonTypes::TileType;

//--------------------------------------------------------------------
RockObj::RockObj()
    : TileObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
RockObj::~RockObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
RockObj* RockObj::create(const CommonTypes::TileInfo & info)
//--------------------------------------------------------------------
{
    RockObj* ret = new (std::nothrow) RockObj();
    if (ret && ret->init(info)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool RockObj::init(const CommonTypes::TileInfo & info)
//--------------------------------------------------------------------
{
    if (!TileObj::init(info)) {
        cocos2d::log("RockObj::init: can't init TileObj inctance");
        return false;
    }

    if (info.tileType >= TileType::RockWall && info.tileType <= TileType::RockWall) {
        mHP = (getTypeAsInt() - Helper::to_underlying(TileType::RockWall)) + 1;
    }
    mIsRemovable = true;
    mIsMovable = true;
    mIsContainer = false;
    mScoreValue = 30;

    return true;
}

//--------------------------------------------------------------------
cocos2d::String& RockObj::spriteName() const
//--------------------------------------------------------------------
{
    const int weakState = 1;
    cocos2d::String* str = nullptr;
    switch (mHP)
    {
    case weakState:
    default:
        str = &GameResources::s_RockImg;
        break;
    }
    return *str;
}

//--------------------------------------------------------------------
bool RockObj::checkMatchingCondition(int column, int row)
//--------------------------------------------------------------------
{
    if (column < 0 || column >= CommonTypes::NumColumns || row < 0 || row >= CommonTypes::NumColumns) {
        return false;
    }
    bool objectOnTop = (mColumn == column && mRow == row - 1);
    bool objectOnBot = (mColumn == column && mRow == row + 1);
    bool objectOnLeft = (mColumn == column - 1 && mRow == row);
    bool objectOnRight = (mColumn == column + 1 && mRow == row);
    return objectOnTop || objectOnBot || objectOnLeft || objectOnRight;
}
