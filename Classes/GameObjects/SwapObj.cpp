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
SwapObj::SwapObj()
    : mCookieA(nullptr)
    , mCookieB(nullptr)
//--------------------------------------------------------------------
{
}

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
        CCLOGERROR("SwapObj::initWithCookies: can't init Node inctance");
        return false;
    }

    if (!cookieA || !cookieB) {
        CCLOGERROR("SwapObj::initWithCookies: broken cookie pointers: A=%p; B=%p", cookieA, cookieB);
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
    auto str = cocos2d::StringUtils::format("swap [%d,%d] with [%d][%d]"
        , mCookieA->getColumn(), mCookieA->getRow(), mCookieB->getColumn(), mCookieB->getRow());
    CCLOGINFO("GameplayScene::trySwapCookieTo: %s;", str.c_str);
    return str;
}
