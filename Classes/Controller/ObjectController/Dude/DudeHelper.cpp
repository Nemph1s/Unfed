/**
* @file GameObjects/TileObjects/Base/BaseObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ObjectController/Dude/DudeHelper.h"

#include "Controller/ObjectController/DudeController.h"
#include "Controller/ChainController.h"

#include "GameObjects/TileObjects/DudeObj.h"

#include "Utils/Helpers/Helper.h"

using CommonTypes::TileType;
using CommonTypes::Direction;

//--------------------------------------------------------------------
DudeHelper::DudeHelper()
    : mDudeObj(nullptr)
    , mDudeCtrl(nullptr)
    , mTopChain(nullptr)
    , mBottomChain(nullptr)
    , mLeftChain(nullptr)
    , mRightChain(nullptr)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
DudeHelper::~DudeHelper()
//--------------------------------------------------------------------
{
    clear();
}

//--------------------------------------------------------------------
DudeHelper * DudeHelper::createWithDudeObject(DudeObj * obj)
//--------------------------------------------------------------------
{
    DudeHelper * ret = new (std::nothrow) DudeHelper();
    if (ret && ret->initWithDudeObject(obj)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool DudeHelper::initWithDudeObject(DudeObj * obj)
//--------------------------------------------------------------------
{
    if (!obj) {
        cocos2d::log("DudeHelper::initWithDudeObject: can't init DudeObj inctance");
        return false;
    }

    mDudeObj = obj;
    return true;
}

//--------------------------------------------------------------------
cocos2d::Set * DudeHelper::getChainByDirection(CommonTypes::Direction & direction)
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    auto chainCtrl = mDudeCtrl->getChainController();
    if (!chainCtrl) {
        cocos2d::log("DudeHelper::getChainByDirection: empty pointer ChainController");
        return set;
    }

    switch (direction)
    {
    case CommonTypes::Direction::Up:
        chainCtrl->addChainsFromSetToSet(mTopChain, set);
        if (!mDudeObj->isActivated()) {
            chainCtrl->addChainsFromSetToSet(mBottomChain, set);
        }
        break;
    case CommonTypes::Direction::Down:
        chainCtrl->addChainsFromSetToSet(mBottomChain, set);
        if (!mDudeObj->isActivated()) {
            chainCtrl->addChainsFromSetToSet(mTopChain, set);
        }
        break;
    case CommonTypes::Direction::Left:
        chainCtrl->addChainsFromSetToSet(mLeftChain, set);
        if (!mDudeObj->isActivated()) {
            chainCtrl->addChainsFromSetToSet(mRightChain, set);
        }
        break;
    case CommonTypes::Direction::Right:
        chainCtrl->addChainsFromSetToSet(mRightChain, set);
        if (!mDudeObj->isActivated()) {
            chainCtrl->addChainsFromSetToSet(mLeftChain, set);
        }
        break;
    default:
        break;
    }
    return set;
}

//--------------------------------------------------------------------
void DudeHelper::clear()
//--------------------------------------------------------------------
{
    CC_SAFE_RELEASE_NULL(mTopChain);
    CC_SAFE_RELEASE_NULL(mBottomChain);
    CC_SAFE_RELEASE_NULL(mLeftChain);
    CC_SAFE_RELEASE_NULL(mRightChain);
    mDudeObj = nullptr;
    mDudeCtrl = nullptr;
}

//--------------------------------------------------------------------
cocos2d::Set* DudeHelper::getTopChain() const
//--------------------------------------------------------------------
{
    return mTopChain;
}

//--------------------------------------------------------------------
cocos2d::Set* DudeHelper::getBottomChain() const
//--------------------------------------------------------------------
{
    return mBottomChain;
}

//--------------------------------------------------------------------
cocos2d::Set* DudeHelper::getLeftChain() const
//--------------------------------------------------------------------
{
    return mLeftChain;
}

//--------------------------------------------------------------------
cocos2d::Set* DudeHelper::getRightChain() const
//--------------------------------------------------------------------
{
    return mRightChain;
}

//--------------------------------------------------------------------
void DudeHelper::setTopChain(cocos2d::Set* var)
//--------------------------------------------------------------------
{
    if (mTopChain) {
        CC_SAFE_RELEASE_NULL(mTopChain);
    }
    mTopChain = var;
    CC_SAFE_RETAIN(mTopChain);
}

//--------------------------------------------------------------------
void DudeHelper::setBottomChain(cocos2d::Set* var)
//--------------------------------------------------------------------
{
    if (mBottomChain) {
        CC_SAFE_RELEASE_NULL(mBottomChain);
    }
    mBottomChain = var;
    CC_SAFE_RETAIN(mBottomChain);
}

//--------------------------------------------------------------------
void DudeHelper::setLeftChain(cocos2d::Set* var)
//--------------------------------------------------------------------
{
    if (mLeftChain) {
        CC_SAFE_RELEASE_NULL(mLeftChain);
    }
    mLeftChain = var;
    CC_SAFE_RETAIN(mLeftChain);
}

//--------------------------------------------------------------------
void DudeHelper::setRightChain(cocos2d::Set* var)
//--------------------------------------------------------------------
{
    if (mRightChain) {
        CC_SAFE_RELEASE_NULL(mRightChain);
    }
    mRightChain = var;
    CC_SAFE_RETAIN(mRightChain);
}
