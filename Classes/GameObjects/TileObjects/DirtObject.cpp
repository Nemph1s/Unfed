#include "DirtObject.h"



/**
* @file GameObjects/TileObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/TileObjects/TileObj.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

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
 
    if (info.tileType == CommonTypes::TileType::Dirt) {
        mHP = 1;
    }
    mIsRemovable = true;

    return true;
}

//--------------------------------------------------------------------
cocos2d::String& DirtObject::spriteName() const
//--------------------------------------------------------------------
{
    return GameResources::s_DirtImg;
}

//--------------------------------------------------------------------
void DirtObject::clear()
//--------------------------------------------------------------------
{
    TileObj::clear();
    mHP = 0;
}