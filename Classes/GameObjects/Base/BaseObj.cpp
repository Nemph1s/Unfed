/**
* @file GameObjects/Base/BaseObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/Base/BaseObj.h"

#include "Utils/Helpers/Helper.h"

//--------------------------------------------------------------------
BaseObj::BaseObj()
    : mColumn(-1)
    , mRow(-1)
    , mType(CommonTypes::GameObjectType::Unknown)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
BaseObj::~BaseObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
BaseObj * BaseObj::create()
//--------------------------------------------------------------------
{
    BaseObj * ret = new (std::nothrow) BaseObj();
    if (ret && ret->init()) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

BaseObj * BaseObj::create(const CommonTypes::GameObjectInfo & info)
{
    BaseObj * ret = new (std::nothrow) BaseObj();
    if (ret && ret->init(info)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool BaseObj::init()
//--------------------------------------------------------------------
{
    if (!Node::init()) {
        cocos2d::log("BaseObj::init: can't init Node inctance");
        return false;
    }

    return true;
}

//--------------------------------------------------------------------
bool BaseObj::init(const CommonTypes::GameObjectInfo & info)
//--------------------------------------------------------------------
{
    if (!Node::init()) {
        cocos2d::log("BaseObj::init: can't init Node inctance");
        return false;
    }
    mColumn = info.column;
    mRow = info.row;
    mType = info.type;

    return true;
}

//--------------------------------------------------------------------
cocos2d::String& BaseObj::spriteName() const
//--------------------------------------------------------------------
{
    return cocos2d::String();
}

//--------------------------------------------------------------------
int BaseObj::getTypeAsInt() const
//--------------------------------------------------------------------
{
    return Helper::getInstance()->to_underlying(mType);
}

//--------------------------------------------------------------------
bool BaseObj::isMovable() const
//--------------------------------------------------------------------
{
    return false;
}

//--------------------------------------------------------------------
bool BaseObj::isRemovable() const
//--------------------------------------------------------------------
{
    return false;
}

//--------------------------------------------------------------------
bool BaseObj::isPossibleSwap() const
//--------------------------------------------------------------------
{
    return false;
}