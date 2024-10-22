/**
* @file GameObjects/CookieObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/TileObjects/CookieObj.h"

#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

using std::string;
using cocos2d::Node;
using cocos2d::Sprite;
using namespace CT;
using namespace GOT;

//--------------------------------------------------------------------
CookieObj::CookieObj()
    : BaseObj()
    , mCookieType(CookieType::Unknown)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
CookieObj * CookieObj::create(const CookieInfo & cookieInfo)
//--------------------------------------------------------------------
{
    CookieObj * ret = new (std::nothrow) CookieObj();
    if (ret && ret->init(cookieInfo)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
CookieObj::~CookieObj()
//--------------------------------------------------------------------
{
    cocos2d::log("CookieObj::~CookieObj: deallocing CookieObj: %p - tag: %i", this, _tag);
}

//--------------------------------------------------------------------
bool CookieObj::init(const CookieInfo & cookieInfo)
//--------------------------------------------------------------------
{
    if (!BaseObj::init(cookieInfo.baseInfo)) {
        cocos2d::log("CookieObj::init: can't init Node inctance");
        return false;
    }

    mCookieType = cookieInfo.cookieType;
    mIsMovable = true;
    mIsSwappable = true;
    mIsRemovable = true;
    return true;
}

//--------------------------------------------------------------------
cocos2d::String& CookieObj::spriteName() const
//--------------------------------------------------------------------
{
    return GameResources::s_cookieSpriteNames.at(getTypeAsInt());
}

//--------------------------------------------------------------------
cocos2d::String& CookieObj::highlightedSpriteName() const
//--------------------------------------------------------------------
{
    return GameResources::s_cookieHighlightedSpriteNames.at(getTypeAsInt());
}

//--------------------------------------------------------------------
cocos2d::String& CookieObj::description() const
//--------------------------------------------------------------------
{
    return *cocos2d::String::createWithFormat("type:%d square:(%d,%d)", mCookieType, mColumn, mRow);
}

//--------------------------------------------------------------------
void CookieObj::setColumn(int var)
//--------------------------------------------------------------------
{
    BaseObj::setColumn(var);
    updateZOrder();
}

//--------------------------------------------------------------------
void CookieObj::setRow(int var)
//--------------------------------------------------------------------
{
    BaseObj::setRow(var);
    updateZOrder();  
}

//--------------------------------------------------------------------
int CookieObj::getTypeAsInt() const
//--------------------------------------------------------------------
{
    return Helper::getInstance()->to_underlying(mCookieType);
}

//--------------------------------------------------------------------
void CookieObj::clear()
//--------------------------------------------------------------------
{
    BaseObj::clear();
    mCookieType = CookieType::Unknown;
}