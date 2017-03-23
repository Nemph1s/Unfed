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

#include "Controller/ObjectController/Dude/DudeController.h"
#include "Controller/ChainController/ChainController.h"

#include "Controller/ObjectController/Dude/DudeObj.h"

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
    , mHorizontalChain(nullptr)
    , mVerticalChain(nullptr)
    , mXChain(nullptr)
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
CommonTypes::Set * DudeHelper::getChainByDirection(const CommonTypes::Direction & direction)
//--------------------------------------------------------------------
{
    CommonTypes::Set* set = nullptr;
    auto chainCtrl = mDudeCtrl->getChainController();
    if (!chainCtrl) {
        cocos2d::log("DudeHelper::getChainByDirection: empty pointer ChainController");
        return set;
    }

    if (mDudeObj->getFieldType() == FieldType::DudeChainX 
        || mDudeObj->getFieldType() == FieldType::DudeAllOfType) {
        set = mXChain;
    }

    if (!set) {
        switch (direction)
        {
        case CommonTypes::Direction::Up:
            set = mDudeObj->isActivated() ? mTopChain : mVerticalChain;
            break;
        case CommonTypes::Direction::Down:
            set = mDudeObj->isActivated() ? mBottomChain : mVerticalChain;
            break;
        case CommonTypes::Direction::Left:
            set = mDudeObj->isActivated() ? mLeftChain : mHorizontalChain;
            break;
        case CommonTypes::Direction::Right:
            set = mDudeObj->isActivated() ? mRightChain : mHorizontalChain;
            break;
        default:
            break;
        }
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
    CC_SAFE_RELEASE_NULL(mHorizontalChain);
    CC_SAFE_RELEASE_NULL(mVerticalChain);
    CC_SAFE_RELEASE_NULL(mXChain);
    mDudeObj = nullptr;
    mDudeCtrl = nullptr;
}

//--------------------------------------------------------------------
CommonTypes::Set* DudeHelper::getTopChain() const
//--------------------------------------------------------------------
{
    return mTopChain;
}

//--------------------------------------------------------------------
CommonTypes::Set* DudeHelper::getBottomChain() const
//--------------------------------------------------------------------
{
    return mBottomChain;
}

//--------------------------------------------------------------------
CommonTypes::Set* DudeHelper::getLeftChain() const
//--------------------------------------------------------------------
{
    return mLeftChain;
}

//--------------------------------------------------------------------
CommonTypes::Set* DudeHelper::getRightChain() const
//--------------------------------------------------------------------
{
    return mRightChain;
}

//--------------------------------------------------------------------
CommonTypes::Set* DudeHelper::getHorizontalChain() const
//--------------------------------------------------------------------
{
    return mHorizontalChain;
}

//--------------------------------------------------------------------
CommonTypes::Set* DudeHelper::getVerticalChain() const
//--------------------------------------------------------------------
{
    return mVerticalChain;
}

//--------------------------------------------------------------------
CommonTypes::Set* DudeHelper::getXChain() const
//--------------------------------------------------------------------
{
    return mXChain;
}

//--------------------------------------------------------------------
void DudeHelper::setTopChain(CommonTypes::Set* var)
//--------------------------------------------------------------------
{
    if (mTopChain) {
        CC_SAFE_RELEASE_NULL(mTopChain);
    }
    mTopChain = var;
    CC_SAFE_RETAIN(mTopChain);
}

//--------------------------------------------------------------------
void DudeHelper::setBottomChain(CommonTypes::Set* var)
//--------------------------------------------------------------------
{
    if (mBottomChain) {
        CC_SAFE_RELEASE_NULL(mBottomChain);
    }
    mBottomChain = var;
    CC_SAFE_RETAIN(mBottomChain);
}

//--------------------------------------------------------------------
void DudeHelper::setLeftChain(CommonTypes::Set* var)
//--------------------------------------------------------------------
{
    if (mLeftChain) {
        CC_SAFE_RELEASE_NULL(mLeftChain);
    }
    mLeftChain = var;
    CC_SAFE_RETAIN(mLeftChain);
}

//--------------------------------------------------------------------
void DudeHelper::setRightChain(CommonTypes::Set* var)
//--------------------------------------------------------------------
{
    if (mRightChain) {
        CC_SAFE_RELEASE_NULL(mRightChain);
    }
    mRightChain = var;
    CC_SAFE_RETAIN(mRightChain);
}

//--------------------------------------------------------------------
void DudeHelper::setHorizontalChain(CommonTypes::Set* var)
//--------------------------------------------------------------------
{
    if (mHorizontalChain) {
        CC_SAFE_RELEASE_NULL(mHorizontalChain);
    }
    mHorizontalChain = var;
    CC_SAFE_RETAIN(mHorizontalChain);
}

//--------------------------------------------------------------------
void DudeHelper::setVerticalChain(CommonTypes::Set* var)
//--------------------------------------------------------------------
{
    if (mVerticalChain) {
        CC_SAFE_RELEASE_NULL(mVerticalChain);
    }
    mVerticalChain = var;
    CC_SAFE_RETAIN(mVerticalChain);
}

//--------------------------------------------------------------------
void DudeHelper::setXChain(CommonTypes::Set* var)
//--------------------------------------------------------------------
{
    if (mXChain) {
        CC_SAFE_RELEASE_NULL(mXChain);
    }
    mXChain = var;
    CC_SAFE_RETAIN(mXChain);
}
