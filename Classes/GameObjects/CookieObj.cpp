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
    , mCookieType(CookieType::Unknown)
    , mNormalSpriteNode(nullptr)
    , mHighLightedSpriteNode(nullptr)
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
    CCLOGINFO("CookieObj::~CookieObj: deallocing CookieObj: %p - tag: %i", this, _tag);
}

//--------------------------------------------------------------------
bool CookieObj::init(const CookieInfo & cookieInfo)
//--------------------------------------------------------------------
{
    if (!Node::init()) {
        CCLOGERROR("CookieObj::init: can't init Node inctance");
        return false;
    }

    CCLOGINFO("CookieObj::init: column=%d row=%d cookieType=%d", mColumn, mRow, mCookieType);

    mColumn = cookieInfo.column;
    mRow = cookieInfo.row;
    mCookieType = cookieInfo.cookieType;

    return true;
}

//--------------------------------------------------------------------
string CookieObj::spriteName()
//--------------------------------------------------------------------
{
    auto type = Helper::to_underlying(mCookieType);
    return GameResources::s_cookieSpriteNames.at(type);
}

//--------------------------------------------------------------------
string CookieObj::highlightedSpriteName()
//--------------------------------------------------------------------
{
    auto type = Helper::to_underlying(mCookieType);
    return GameResources::s_cookieHighlightedSpriteNames.at(type);
}

//--------------------------------------------------------------------
string CookieObj::description()
//--------------------------------------------------------------------
{
    CCLOGINFO("CookieObj::description: type:%d square:(%d,%d)", mCookieType, mColumn, mRow);
    return cocos2d::StringUtils::format("type:%d square:(%d,%d)", mCookieType, mColumn, mRow);
}

void CookieObj::updateTilePosLabel()
{
    auto text = cocos2d::StringUtils::format("[%d,%d]", mColumn, mRow);
    if (mNormalSpriteNode) {
        auto ttf1 = cocos2d::LabelTTF::create(text, "fonts/Arial", 16,
            cocos2d::Size(32, 32), cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
        auto size = mNormalSpriteNode->getContentSize();
        ttf1->setPosition(cocos2d::Vec2(size.width / 4, (size.height / 1.25f)));
        ttf1->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        mNormalSpriteNode->addChild(ttf1);
    }
    if (mHighLightedSpriteNode) {
        auto ttf2 = cocos2d::LabelTTF::create(text, "fonts/Arial", 16,
            cocos2d::Size(32, 32), cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
        auto size = mHighLightedSpriteNode->getContentSize();
        ttf2->setPosition(cocos2d::Vec2(size.width / 4, (size.height / 1.25f)));
        ttf2->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        mHighLightedSpriteNode->addChild(ttf2);
    }
}
