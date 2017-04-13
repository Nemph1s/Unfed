/**
* @file GameObjects/TileObjects/Base/BaseObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/TileObjects/Base/BaseObj.h"

#include "Common/Factory/SpritesFactory.h"
#include "Utils/Helpers/Helper.h"

//--------------------------------------------------------------------
BaseObj::BaseObj()
    : mColumn(-1)
    , mRow(-1)
    , mType(CT::BaseObjType::Unknown)
    , mSpriteNode(nullptr)
    , mDummyString(nullptr)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
BaseObj::~BaseObj()
//--------------------------------------------------------------------
{
    CC_SAFE_RELEASE(mDummyString);
    clear();
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

//--------------------------------------------------------------------
BaseObj * BaseObj::create(const CT::BaseObjInfo & info)
//--------------------------------------------------------------------
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
bool BaseObj::init(const CT::BaseObjInfo & info)
//--------------------------------------------------------------------
{
    if (!Node::init()) {
        cocos2d::log("BaseObj::init: can't init Node inctance");
        return false;
    }
    mColumn = info.column;
    mRow = info.row;
    mType = info.type;
    mDummyString = cocos2d::String::create("");
    CC_SAFE_RETAIN(mDummyString);

    return true;
}

//--------------------------------------------------------------------
CT::Cell BaseObj::getCell() const
//--------------------------------------------------------------------
{
    return CT::Cell(mColumn, mRow);
}

//--------------------------------------------------------------------
cocos2d::String& BaseObj::spriteName() const
//--------------------------------------------------------------------
{
    return *mDummyString;
}

cocos2d::String & BaseObj::description() const
{
    return *mDummyString;
}

//--------------------------------------------------------------------
int BaseObj::getTypeAsInt() const
//--------------------------------------------------------------------
{
    return Helper::getInstance()->to_underlying(mType);
}

//--------------------------------------------------------------------
void BaseObj::match()
//--------------------------------------------------------------------
{
    //TODO: implement this method in cookie to update score matching
}

//--------------------------------------------------------------------
bool BaseObj::isHpEnded() const
//--------------------------------------------------------------------
{
    return false;
}

//--------------------------------------------------------------------
void BaseObj::clear()
//--------------------------------------------------------------------
{
    mColumn = -1;
    mRow = -1;
    mType = CT::BaseObjType::Unknown;
    mIsMovable = false;
    mIsSwappable = false;
    mIsRemovable = false;
    mIsContainer = false;
    if (getParent()) {
        removeFromParent();
    }
    SpritesFactory->recycle(mSpriteNode, this);
    if (mSpriteNode) {
        mSpriteNode = nullptr;
    }
}

//--------------------------------------------------------------------
void BaseObj::updateDebugLabel()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
void BaseObj::updateZOrder()
//--------------------------------------------------------------------
{
    auto zOrder = (mRow * 10);
    setLocalZOrder(zOrder);
    if (mSpriteNode) {
        mSpriteNode->setLocalZOrder(zOrder);
    }
    updateDebugLabel();
}
