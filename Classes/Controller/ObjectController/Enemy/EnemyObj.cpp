/**
* @file Controller/ObjectController/Enemy/EnemyObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ObjectController/Enemy/EnemyObj.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

#include "Common/GlobalInfo/GlobalInfo.h"

//--------------------------------------------------------------------
EnemyObj::EnemyObj()
    : BaseObj()
    , mEnemyType(CT::EnemyType::Unknown)
    , mDebugLabel(nullptr)
    , mHP(0)
    , mWaitTurnsBeforeAction(1)
    , mWaitedTurns(0)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
EnemyObj::~EnemyObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
EnemyObj * EnemyObj::create(const CT::EnemyInfo & info)
//--------------------------------------------------------------------
{
    EnemyObj * ret = new (std::nothrow) EnemyObj();
    if (ret && ret->init(info)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool EnemyObj::init(const CT::EnemyInfo & info)
//--------------------------------------------------------------------
{
    if (!BaseObj::init(info.baseInfo)) {
        cocos2d::log("DudeObj::init: can't init BaseObj inctance");
        return false;
    }

    mEnemyType = info.enemyType;
    mHP = 1;
    mIsRemovable = true;
    mIsMovable = true;
    mIsSwappable = true;
    mIsContainer = false;


    if (!mDebugLabel) {
#ifdef COCOS2D_DEBUG
        mDebugLabel = cocos2d::Label::create();
        mDebugLabel->setBMFontSize(16);
        mDebugLabel->setDimensions(42, 32);
        mDebugLabel->setHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        mDebugLabel->setVerticalAlignment(cocos2d::TextVAlignment::BOTTOM);
        mDebugLabel->setPosition(cocos2d::Vec2(GlobInfo->getTileWidth() * 0.8f, GlobInfo->getTileHeight() * 0.2f));
        mDebugLabel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        mDebugLabel->setTextColor(cocos2d::Color4B::WHITE);
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
cocos2d::String& EnemyObj::spriteName() const
//--------------------------------------------------------------------
{
    return GameResources::s_enemySpriteNames.at(getTypeAsInt());
}

//--------------------------------------------------------------------
int EnemyObj::getTypeAsInt() const
//--------------------------------------------------------------------
{
    return Helper::getInstance()->to_underlying(mEnemyType);
}

//--------------------------------------------------------------------
void EnemyObj::match()
//--------------------------------------------------------------------
{
    // TODO: implement checking state before match
    if (mIsStunned) {
        mHP--;
    } else {
        mIsStunned = true;
    }
    
    if (mHP > 0) {
        //TODO: change state from idle to stun
    }
    else if (mHP == 0) {
        
    }
}

//--------------------------------------------------------------------
void EnemyObj::clear()
//--------------------------------------------------------------------
{
    BaseObj::clear();
    mEnemyType = CT::EnemyType::Unknown;
    mHP = 0;
    if (mDebugLabel) {
        if (mDebugLabel->getParent()) {
            mDebugLabel->removeFromParent();
        }
        CC_SAFE_RELEASE_NULL(mDebugLabel);
    }
}

//--------------------------------------------------------------------
bool EnemyObj::checkMatchingCondition(CT::Cell& cell)
//--------------------------------------------------------------------
{
    if (!Helper::isValidCell(cell)) {
        return false;
    }
    bool objectOnTop = (mColumn == cell.column && mRow == cell.row - 1);
    bool objectOnBot = (mColumn == cell.column && mRow == cell.row + 1);
    bool objectOnLeft = (mColumn == cell.column - 1 && mRow == cell.row);
    bool objectOnRight = (mColumn == cell.column + 1 && mRow == cell.row);
    return objectOnTop || objectOnBot || objectOnLeft || objectOnRight;
}

//--------------------------------------------------------------------
bool EnemyObj::isHpEnded() const
//--------------------------------------------------------------------
{
    bool result = false;
    if (isRemovable()) {
        result = (mHP <= 0);
    }
    return result;
}

//--------------------------------------------------------------------
void EnemyObj::updateDebugLabel()
//--------------------------------------------------------------------
{
    if (mDebugLabel) {
        if (!mDebugLabel->getParent() && mSpriteNode) {
            mSpriteNode->addChild(mDebugLabel);
        }
        int col = mColumn == -1 ? 0 : mColumn;
        int row = mRow == -1 ? 0 : mRow;

        auto text = cocos2d::StringUtils::format("[%d,%d]z%d", col, row, mSpriteNode->getLocalZOrder());
        mDebugLabel->setString(text);
    }
}

//--------------------------------------------------------------------
void EnemyObj::runAction()
//--------------------------------------------------------------------
{
    if (mRunActionCallback) {
        mRunActionCallback();
    }
    updateState();
}

//--------------------------------------------------------------------
bool EnemyObj::isInOparableState()
//--------------------------------------------------------------------
{
    return !mIsStunned && (mWaitTurnsBeforeAction <= mWaitedTurns);
}

//--------------------------------------------------------------------
void EnemyObj::updateState()
//--------------------------------------------------------------------
{
    if (mWaitTurnsBeforeAction <= mWaitedTurns) {
        mWaitedTurns = 0;
        if (mIsStunned) {
            mIsStunned = false;
        }
    } else {
        mWaitedTurns++;
    }
}
