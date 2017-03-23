/**
* @file GameObjects/SwapObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/SwapController/SwapObj.h"
#include "GameObjects/TileObjects/Base/BaseObj.h"

//--------------------------------------------------------------------
SwapObj::SwapObj()
    : mObjectA(nullptr)
    , mObjectB(nullptr)
    , mObjectsForHint(nullptr)
    //--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
SwapObj::~SwapObj()
//--------------------------------------------------------------------
{
    cocos2d::log("SwapObj::~SwapObj: deallocing %p - objA: %s; objB: %s;"
        , this
        , mObjectA ? mObjectA->description().getCString() : ""
        , mObjectB ? mObjectB->description().getCString() : "");
    mObjectA = nullptr;
    mObjectB = nullptr;
    if (mObjectsForHint) {
        mObjectsForHint->removeAllObjects();
    }
    CC_SAFE_RELEASE_NULL(mObjectsForHint);
}

//--------------------------------------------------------------------
SwapObj* SwapObj::createWithObjects(BaseObj * objectA, BaseObj * objectB)
//--------------------------------------------------------------------
{
    SwapObj* ret = new (std::nothrow) SwapObj();
    if (ret && ret->initWithObjects(objectA, objectB)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool SwapObj::initWithObjects(BaseObj * objectA, BaseObj * objectB)
//--------------------------------------------------------------------
{
    if (!Node::init()) {
        cocos2d::log("SwapObj::initWithCookies: can't init Node inctance");
        return false;
    }

    if (!objectA || !objectB) {
        cocos2d::log("SwapObj::initWithCookies: broken cookie pointers: A=%p; B=%p", objectA, objectB);
        return false;
    }
    mObjectA = objectA;
    mObjectB = objectB;

    return true;
}

//--------------------------------------------------------------------
std::string SwapObj::description()
//--------------------------------------------------------------------
{
    auto str = cocos2d::StringUtils::format("swap type:%d square:(%d,%d) with type:%d square:(%d,%d),"
        , mObjectA->getTypeAsInt(), mObjectA->getColumn(), mObjectA->getRow(), mObjectB->getTypeAsInt()
        , mObjectB->getColumn(), mObjectB->getRow());
    //cocos2d::log("GameplayScene::trySwapCookieTo: %s;", str.c_str());
    return str;
}

//--------------------------------------------------------------------
CommonTypes::Set* SwapObj::getObjectsForHint() const
//--------------------------------------------------------------------
{
    return mObjectsForHint;
}

//--------------------------------------------------------------------
void SwapObj::setObjectsForHint(CommonTypes::Set* var)
//--------------------------------------------------------------------
{
    if (mObjectsForHint) {
        CC_SAFE_RELEASE_NULL(mObjectsForHint);
    }
    mObjectsForHint = var;
    CC_SAFE_RETAIN(mObjectsForHint);
}