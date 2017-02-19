/**
* @file GameObjects/TileObjects/TileObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/TileObjects/TileObj.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

//--------------------------------------------------------------------
TileObj::TileObj()
    : BaseObj()
    , mTileType(CommonTypes::TileType::Unknown)
    , mDebugLabel(nullptr)
    , mHP(0)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
TileObj::~TileObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
TileObj * TileObj::create(const CommonTypes::TileInfo & info)
//--------------------------------------------------------------------
{
    TileObj * ret = new (std::nothrow) TileObj();
    if (ret && ret->init(info)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool TileObj::init(const CommonTypes::TileInfo & info)
//--------------------------------------------------------------------
{
    if (!BaseObj::init(info.baseInfo)) {
        cocos2d::log("TileObj::init: can't init Node inctance");
        return false;
    }
    mTileType = info.tileType;

    if (!mDebugLabel && mType != CommonTypes::BaseObjectType::FieldObj) {
#ifdef COCOS2D_DEBUG
        mDebugLabel = cocos2d::Label::create();
        mDebugLabel->setBMFontSize(16);
        mDebugLabel->setDimensions(32, 32);
        mDebugLabel->setHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        mDebugLabel->setVerticalAlignment(cocos2d::TextVAlignment::BOTTOM);
        mDebugLabel->setPosition(cocos2d::Vec2(GameResources::TileWidth * 0.8f, GameResources::TileHeight * 0.2f));
        mDebugLabel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        mDebugLabel->setTextColor(cocos2d::Color4B::BLACK);
        mDebugLabel->setGlobalZOrder(1000);
        CC_SAFE_RETAIN(mDebugLabel);
        //mSpriteNode->addChild(mDebugLabel, 10);

        int col = mColumn == -1 ? 0 : mColumn;
        int row = mRow == -1 ? 0 : mRow;

        auto text = cocos2d::StringUtils::format("[%d,%d]", col, row);
        mDebugLabel->setString(text);
#endif //UNFED_ENABLE_DEBUG
    }

    return true;
}

//--------------------------------------------------------------------
cocos2d::String& TileObj::spriteName() const
//--------------------------------------------------------------------
{
    return GameResources::s_TileImg;
}

//--------------------------------------------------------------------
void TileObj::setSpriteNode(cocos2d::Sprite * var)
//--------------------------------------------------------------------
{
    mSpriteNode = var;
    if (mSpriteNode && mDebugLabel) {
        if (!mDebugLabel->getParent()) {
            mSpriteNode->addChild(mDebugLabel, 10);
        }
    }
}

//--------------------------------------------------------------------
int TileObj::getTypeAsInt() const
//--------------------------------------------------------------------
{
    return Helper::getInstance()->to_underlying(mTileType);
}

//--------------------------------------------------------------------
void TileObj::match()
//--------------------------------------------------------------------
{
    mHP--;
    if (mHP > 0) {
        mTileType = static_cast<CommonTypes::TileType>(getTypeAsInt() - 1);
    }
    else {
        mTileType = CommonTypes::TileType::Unknown;
    }
}

//--------------------------------------------------------------------
void TileObj::clear()
//--------------------------------------------------------------------
{
    BaseObj::clear();
    mTileType = CommonTypes::TileType::Unknown;
    mHP = 0;
}

//--------------------------------------------------------------------
bool TileObj::isReadyToRemove() const
//--------------------------------------------------------------------
{
    bool result = false;
    if (getIsRemovable()) {
        result = (mHP <= 0);
    }
    return result;
}

//--------------------------------------------------------------------
bool TileObj::isEmptyTile()
//--------------------------------------------------------------------
{
    if (mTileType == CommonTypes::TileType::Empty) {
        return true;
    } else {
        return false;
    }    
}

//--------------------------------------------------------------------
void TileObj::updateDebugTileLabel()
//--------------------------------------------------------------------
{
    if (mDebugLabel) {
        int col = mColumn == -1 ? 0 : mColumn;
        int row = mRow == -1 ? 0 : mRow;

        auto text = cocos2d::StringUtils::format("[%d,%d]", col, row);
        mDebugLabel->setString(text);
    }
}
