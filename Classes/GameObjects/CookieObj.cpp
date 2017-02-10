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
    : BaseObj()
    , mCookieType(CookieType::Unknown)
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
    if (!BaseObj::init(cookieInfo.baseInfo)) {
        cocos2d::log("CookieObj::init: can't init Node inctance");
        return false;
    }

    mCookieType = cookieInfo.cookieType;
    mIsMovable = true;
    mIsPossibleSwap = true;
    mIsRemovable = true;

#ifdef UNFED_ENABLE_DEBUG
    mDebugLabel = cocos2d::Label::create();
    mDebugLabel->setBMFontSize(16);
    mDebugLabel->setDimensions(32, 32);
    mDebugLabel->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    mDebugLabel->setVerticalAlignment(cocos2d::TextVAlignment::TOP);
    mDebugLabel->setPosition(cocos2d::Vec2(GameResources::TileWidth / 4, (GameResources::TileHeight / 1.25f)));
    mDebugLabel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    mSpriteNode->addChild(mDebugLabel, 10);
#endif //UNFED_ENABLE_DEBUG

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
int CookieObj::getTypeAsInt() const
//--------------------------------------------------------------------
{
    return Helper::getInstance()->to_underlying(mCookieType);
}

//--------------------------------------------------------------------
void CookieObj::clear()
//--------------------------------------------------------------------
{
    mCookieType = CommonTypes::CookieType::Unknown;
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