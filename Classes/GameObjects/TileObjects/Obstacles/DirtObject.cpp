/**
* @file GameObjects/TileObjects/Obstacles/DirtObject.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/TileObjects/Obstacles/DirtObject.h"

#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

using CommonTypes::TileType;

//--------------------------------------------------------------------
DirtObject::DirtObject()
    : TileObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
DirtObject::~DirtObject()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
DirtObject * DirtObject::create(const CommonTypes::TileInfo & info)
//--------------------------------------------------------------------
{
    DirtObject * ret = new (std::nothrow) DirtObject();
    if (ret && ret->init(info)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool DirtObject::init(const CommonTypes::TileInfo & info)
//--------------------------------------------------------------------
{
    if (!TileObj::init(info)) {
        cocos2d::log("TileObj::init: can't init Node inctance");
        return false;
    }
 
    if (info.tileType >= TileType::Dirt && info.tileType <= TileType::Dirt_HP3) {
        mHP = (getTypeAsInt() - Helper::to_underlying(TileType::Dirt)) + 1;
    }
    mIsRemovable = true;
    mIsContainer = true;

    return true;
}

//--------------------------------------------------------------------
cocos2d::String& DirtObject::spriteName() const
//--------------------------------------------------------------------
{
    const int x2State = 2;
    const int x1State = 1;
    cocos2d::String* str = nullptr;
    switch (mHP)
    {
    case x1State:
        str = &GameResources::s_DirtX2Img;
        break;
    case x2State:
    default:
        str = &GameResources::s_DirtImg;
        break;
    }
    return *str;
}

//--------------------------------------------------------------------
bool DirtObject::checkMatchingCondition(int column, int row)
//--------------------------------------------------------------------
{
    if (column < 0 || column >= CommonTypes::NumColumns || row < 0 || row >= CommonTypes::NumColumns) {
        return false;
    }
    return (mColumn == column && mRow == row);
}
