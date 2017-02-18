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
#include "GameObjects/TileObjects/TileObj.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

using CommonTypes::TileType;

//--------------------------------------------------------------------
DirtObject::DirtObject()
    : TileObj()
    , mHP(0)
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
    return GameResources::s_DirtImg;
}

//--------------------------------------------------------------------
cocos2d::String& DirtObject::description() const
//--------------------------------------------------------------------
{
    return *cocos2d::String::createWithFormat("type:%d square:(%d,%d)", getTypeAsInt(), mColumn, mRow);
}

//--------------------------------------------------------------------
void DirtObject::match()
//--------------------------------------------------------------------
{
    mHP--;
}

//--------------------------------------------------------------------
bool DirtObject::isReadyToRemove() const
//--------------------------------------------------------------------
{
    bool result = false;
    if (getIsRemovable()) {
        result = (mHP <= 0);
    }
    return result;
}

//--------------------------------------------------------------------
void DirtObject::clear()
//--------------------------------------------------------------------
{
    TileObj::clear();
    mHP = 0;
}