/**
* @file GameObjects/TileObjects/DudeObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ObjectController/Dude/DudeObj.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

using CommonTypes::FieldType;

//--------------------------------------------------------------------
DudeObj::DudeObj()
    : FieldObj()
    , mIsActivated(false)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
DudeObj::~DudeObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
DudeObj * DudeObj::create(const CommonTypes::FieldInfo & info)
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
bool DudeObj::init(const CommonTypes::FieldInfo & info)
//--------------------------------------------------------------------
{
    if (!FieldObj::init(info)) {
        cocos2d::log("DudeObj::init: can't init FieldObj inctance");
        return false;
    }

    mIsRemovable = false;
    mIsMovable = true;
    mIsSwappable = true;
    mIsContainer = false;
    mScoreValue = 500;

    return true;
}

//--------------------------------------------------------------------
cocos2d::String& DudeObj::spriteName() const
//--------------------------------------------------------------------
{
    return GameResources::s_dudeSpriteNames.at(getTypeAsInt() - Helper::to_underlying(FieldType::DudeFromAToB));
}

//--------------------------------------------------------------------
void DudeObj::activate()
//--------------------------------------------------------------------
{
    mIsActivated = true;
}

//--------------------------------------------------------------------
void DudeObj::deactivate()
//--------------------------------------------------------------------
{
    mIsActivated = false;
}