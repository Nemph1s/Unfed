/**
* @file GameObjects/TileObjects/DudeObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/TileObjects/DudeObj.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

using CommonTypes::TileType;

//--------------------------------------------------------------------
DudeObj::DudeObj()
    : TileObj()
    //--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
DudeObj::~DudeObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
DudeObj * DudeObj::create(const CommonTypes::TileInfo & info)
//--------------------------------------------------------------------
{
    DudeObj * ret = new (std::nothrow) DudeObj();
    if (ret && ret->init(info)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool DudeObj::init(const CommonTypes::TileInfo & info)
//--------------------------------------------------------------------
{
    if (!TileObj::init(info)) {
        cocos2d::log("DudeObj::init: can't init TileObj inctance");
        return false;
    }

    mIsRemovable = false;
    mIsMovable = true;
    mIsSwappable = true;
    mIsContainer = false;

    return true;
}

//--------------------------------------------------------------------
cocos2d::String& DudeObj::spriteName() const
//--------------------------------------------------------------------
{
    return GameResources::s_dudeSpriteNames.at(getTypeAsInt() - Helper::to_underlying(TileType::DudeLightr));
}

CommonTypes::DudePathType DudeObj::getPathType()
{
    return CommonTypes::DudePathType();
}
