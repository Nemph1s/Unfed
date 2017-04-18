/**
* @file Controller/ObjectController/Enemy/EnemyHelper.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ObjectController/Enemy/EnemyHelper.h"

#include "Controller/ObjectController/ObjectController.h"
#include "Controller/ObjectController/Enemy/EnemyController.h"
#include "Controller/ObjectController/Enemy/EnemyObj.h"

#include "Utils/Helpers/Helper.h"

using namespace CT;

//--------------------------------------------------------------------
EnemyHelper::EnemyHelper()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
EnemyHelper::~EnemyHelper()
//--------------------------------------------------------------------
{
    CC_SAFE_RELEASE_NULL(mAvailableNearbyObjects);
}

//--------------------------------------------------------------------
EnemyHelper* EnemyHelper::createWithEnemyObject(EnemyObj* obj)
//--------------------------------------------------------------------
{
    EnemyHelper * ret = new (std::nothrow) EnemyHelper();
    if (ret && ret->initWithEnemyObject(obj)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool EnemyHelper::initWithEnemyObject(EnemyObj* obj)
//--------------------------------------------------------------------
{
    if (!obj) {
        cocos2d::log("EnemyHelper::initWithEnemyObject: can't init EnemyObj inctance");
        return false;
    }

    mEnemyObj = obj;
    mAvailableNearbyObjects = cocos2d::Array::create();
    CC_SAFE_RETAIN(mAvailableNearbyObjects);

    return true;
}

//--------------------------------------------------------------------
BaseObj* EnemyHelper::getNearbyObjectByDirection(const CT::Direction& direction)
//--------------------------------------------------------------------
{
    BaseObj* obj = nullptr;

    switch (direction)
    {
    case Direction::Up:
        obj = mTopObj;
        break;
    case Direction::Down:
        obj = mBottomObj;
        break;
    case Direction::Left:
        obj = mLeftObj;
        break;
    case Direction::Right:
        obj = mRightObj;
        break;
    default:
        break;
    }
    return obj;
}

//--------------------------------------------------------------------
bool EnemyHelper::updateNearbyObjects(BaseObj* top, BaseObj* bot, BaseObj* left, BaseObj* rigth)
//--------------------------------------------------------------------
{
    mAvailableNearbyObjects->removeAllObjects();

    setTopObj(top);
    setBottomObj(bot);
    setLeftObj(left);
    setRightObj(rigth);

    if (mTopObj) {
        mAvailableNearbyObjects->addObject(mTopObj);
    }
    if (mBottomObj) {
        mAvailableNearbyObjects->addObject(mBottomObj);
    }
    if (mLeftObj) {
        mAvailableNearbyObjects->addObject(mLeftObj);
    }
    if (mRightObj) {
        mAvailableNearbyObjects->addObject(mRightObj);
    }

    return isEnemyReadyToMove();
}

//--------------------------------------------------------------------
bool EnemyHelper::isEnemyReadyToMove()
//--------------------------------------------------------------------
{
    return mAvailableNearbyObjects->count() > 0;
}

//--------------------------------------------------------------------
bool EnemyHelper::isReadyForAction()
//--------------------------------------------------------------------
{
    return isEnemyReadyToMove() && mEnemyObj->isInOparableState();
}

//--------------------------------------------------------------------
void EnemyHelper::runEnemyAction()
//--------------------------------------------------------------------
{
    // TODO: pick random available action
    auto moveCallback = mEnemyCtrl->getMoveActionCallback();
    if (moveCallback) {
        auto secondObj = dynamic_cast<BaseObj*>(mAvailableNearbyObjects->getRandomObject());
        moveCallback(mEnemyObj, secondObj);
    }
}

