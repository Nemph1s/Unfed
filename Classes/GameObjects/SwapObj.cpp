/**
* @file GameObjects/SwapObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/SwapObj.h"
#include "GameObjects/CookieObj.h"

//--------------------------------------------------------------------
SwapObj::~SwapObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
SwapObj* SwapObj::createWithCookies(CookieObj * cookieA, CookieObj * cookieB)
//--------------------------------------------------------------------
{
    SwapObj* ret = new (std::nothrow) SwapObj();
    if (ret && ret->initWithCookies(cookieA, cookieB)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool SwapObj::initWithCookies(CookieObj * cookieA, CookieObj * cookieB)
//--------------------------------------------------------------------
{
    if (!Node::init()) {
        cocos2d::log("SwapObj::initWithCookies: can't init Node inctance");
        return false;
    }

    if (!cookieA || !cookieB) {
        cocos2d::log("SwapObj::initWithCookies: broken cookie pointers: A=%p; B=%p", cookieA, cookieB);
        return false;
    }
    mCookieA = cookieA;
    mCookieB = cookieB;

    return true;
}

//--------------------------------------------------------------------
std::string SwapObj::description()
//--------------------------------------------------------------------
{
    auto str = cocos2d::StringUtils::format("swap type:%d square:(%d,%d) with type:%d square:(%d,%d),"
        , mCookieA->getTypeAsInt(), mCookieA->getColumn(), mCookieA->getRow(), mCookieB->getTypeAsInt()
        , mCookieB->getColumn(), mCookieB->getRow());
    //cocos2d::log("GameplayScene::trySwapCookieTo: %s;", str.c_str());
    return str;
}
