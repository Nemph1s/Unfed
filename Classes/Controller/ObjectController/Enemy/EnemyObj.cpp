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

static const uint8_t defaultWaitTurnsBeforeAction = 1;

//--------------------------------------------------------------------
EnemyObj::EnemyObj()
    : BaseObj()
    , mEnemyType(GOT::EnemyType::Unknown)
    , mHP(0)
    , mWaitTurnsBeforeAction(defaultWaitTurnsBeforeAction)
    , mWaitedTurns(defaultWaitTurnsBeforeAction)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
EnemyObj::~EnemyObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
EnemyObj * EnemyObj::create(const GOT::EnemyInfo& info)
//--------------------------------------------------------------------
{
    EnemyObj* ret = new (std::nothrow) EnemyObj();
    if (ret && ret->init(info)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool EnemyObj::init(const GOT::EnemyInfo& info)
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
    mEnemyType = GOT::EnemyType::Unknown;
    mHP = 0;
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
