/**
* @file GameObjects/CookieObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/CookieObj.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

using std::string;
using cocos2d::Node;
using cocos2d::Sprite;
using namespace CommonTypes;

//--------------------------------------------------------------------
CookieObj::CookieObj()
    : mColumn(0)
    , mRow(0)
    , mType(CookieType::Unknown)
    , mSpriteNode(nullptr)
    , mDebugLabel(nullptr)
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
    if (!Node::init()) {
        cocos2d::log("CookieObj::init: can't init Node inctance");
        return false;
    }

    //cocos2d::log("CookieObj::init: column=%d row=%d cookieType=%d", mColumn, mRow, mType);

    mColumn = cookieInfo.column;
    mRow = cookieInfo.row;
    mType = cookieInfo.cookieType;

    return true;
}

//--------------------------------------------------------------------
string CookieObj::spriteName()
//--------------------------------------------------------------------
{
    return GameResources::s_cookieSpriteNames.at(getTypeAsInt());
}

//--------------------------------------------------------------------
string CookieObj::highlightedSpriteName()
//--------------------------------------------------------------------
{
    return GameResources::s_cookieHighlightedSpriteNames.at(getTypeAsInt());
}

//--------------------------------------------------------------------
string CookieObj::description()
//--------------------------------------------------------------------
{
    return cocos2d::StringUtils::format("type:%d square:(%d,%d)", mType, mColumn, mRow);
}

//--------------------------------------------------------------------
int CookieObj::getTypeAsInt()
//--------------------------------------------------------------------
{
    return Helper::getInstance()->to_underlying(mType);
}

//--------------------------------------------------------------------
void CookieObj::updateDebugTileLabel()
//--------------------------------------------------------------------
{
    if (mDebugLabel) {
        auto text = cocos2d::StringUtils::format("[%d,%d]", mColumn, mRow);
        mDebugLabel->setString(text);
    }
}
