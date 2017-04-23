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
#include "Common/GlobalInfo/GlobalInfo.h"
#include "Utils/Helpers/Helper.h"

//--------------------------------------------------------------------
BaseObj::BaseObj()
    : mColumn(-1)
    , mRow(-1)
    , mType(GOT::BaseObjType::Unknown)
    , mSpriteNode(nullptr)
    , mDummyString(nullptr)
    , mPriority(0)
    , mDebugLabel(nullptr)
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
BaseObj * BaseObj::create(const GOT::BaseObjInfo & info)
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
bool BaseObj::init(const GOT::BaseObjInfo & info)
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

#ifdef UNFED_ENABLE_DEBUG
    if (!mDebugLabel && mType != GOT::BaseObjType::Tile) {
        mDebugLabel = cocos2d::Label::create();
        mDebugLabel->setBMFontSize(16);
        mDebugLabel->setDimensions(42, 32);
        mDebugLabel->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        mDebugLabel->setVerticalAlignment(cocos2d::TextVAlignment::TOP);
        mDebugLabel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        mDebugLabel->setPosition(cocos2d::Vec2(GlobInfo->getTileWidth() / 4, (GlobInfo->getTileHeight() / 1.25f)));
        mDebugLabel->setColor(cocos2d::Color3B::BLACK);
        CC_SAFE_RETAIN(mDebugLabel);

        int col = mColumn == -1 ? 0 : mColumn;
        int row = mRow == -1 ? 0 : mRow;

        auto text = cocos2d::StringUtils::format("[%d,%d]", col, row);
        mDebugLabel->setString(text);
}
#endif //UNFED_ENABLE_DEBUG

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

//--------------------------------------------------------------------
cocos2d::String & BaseObj::description() const
//--------------------------------------------------------------------
{
    return *mDummyString;
}

//--------------------------------------------------------------------
cocos2d::Sprite* BaseObj::getSpriteNode() const
//--------------------------------------------------------------------
{
    return mSpriteNode;
}

//--------------------------------------------------------------------
void BaseObj::setSpriteNode(cocos2d::Sprite* var)
//--------------------------------------------------------------------
{
    mSpriteNode = var;
#ifdef UNFED_ENABLE_DEBUG
    if (mSpriteNode && mDebugLabel) {
        if (!mDebugLabel->getParent()) {
            mSpriteNode->addChild(mDebugLabel, 10);
        }
    }
#endif
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
    mPriority = 0;
    mType = GOT::BaseObjType::Unknown;
    mIsMovable = false;
    mIsSwappable = false;
    mIsRemovable = false;
    mIsContainer = false;
    if (mDebugLabel) {
        mDebugLabel->removeFromParent();
        CC_SAFE_RELEASE_NULL(mDebugLabel);
    }
    if (getParent()) {
        removeFromParent();
    }
    SpritesFactory->recycle(mSpriteNode, this);
    if (mSpriteNode) {
        mSpriteNode = nullptr;
    }
}

//--------------------------------------------------------------------
bool BaseObj::checkMatchingCondition(CT::Cell & cell)
//--------------------------------------------------------------------
{
    return false;
}

//--------------------------------------------------------------------
void BaseObj::updateDebugLabel()
//--------------------------------------------------------------------
{
    if (mDebugLabel) {
        int col = mColumn == -1 ? 0 : mColumn;
        int row = mRow == -1 ? 0 : mRow;

        auto text = cocos2d::StringUtils::format("[%d,%d]z%d", col, row, mSpriteNode->getLocalZOrder());
        mDebugLabel->setString(text);
    }
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
