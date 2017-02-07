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
