/**
* @file Scenes/ObjectController/DudeController.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ObjectController/DudeController.h"
#include "Controller/ObjectController/ObjectController.h"

#include "Common/Factory/SmartFactory.h"

#include "Utils/Parser/JsonParser.h"
#include "Utils/Helpers/Helper.h"

#include "GameObjects/TileObjects/DudeObj.h"

using namespace CommonTypes;

//--------------------------------------------------------------------
DudeController::DudeController()
    : mObjCtrl(nullptr)
    , mDudeDirections()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
DudeController::~DudeController()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
DudeController * DudeController::create()
//--------------------------------------------------------------------
{
    DudeController * ret = new (std::nothrow) DudeController();
    if (ret && ret->init()) {
        ret->autorelease();
        CC_SAFE_RETAIN(ret);
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool DudeController::init()
//--------------------------------------------------------------------
{
    return true;
}

//--------------------------------------------------------------------
BaseObj * DudeController::createDudeObject(int column, int row, int type)
//--------------------------------------------------------------------
{
    BaseObjectInfo baseInfo = { BaseObjectType::DudeObj, column, row };
    TileInfo info = { baseInfo, static_cast<TileType>(type) };
    auto obj = dynamic_cast<DudeObj*>(SmartFactory->createDudeObj(info));
    CC_ASSERT(obj);
    mDudeObjects[column][row] = obj;

    auto helper = DudeHelper::createWithDudeObject(obj);
    helper->setDudeController(this);
    mDudeDirections.insert(obj, helper);

    return obj;
}

//--------------------------------------------------------------------
DudeObj * DudeController::dudeObjectAt(int column, int row)
//--------------------------------------------------------------------
{
    if (!(column >= 0 && column < NumColumns) || !(row >= 0 && row < NumColumns)) {
        cocos2d::log("ChainController::createChainFromPosToPos: wrong destinationPos at column=%d, row=%d", column, row);
        return nullptr;
    }
    return dynamic_cast<DudeObj*>(mDudeObjects[column][row]);
}

//--------------------------------------------------------------------
bool DudeController::detectDirectionsForDudes()
//--------------------------------------------------------------------
{
    bool isDetected = false;
    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns; column++) {
            
            auto dude = dudeObjectAt(column, row);
            if (dude) {
                auto helper = mDudeDirections->at(dude);
                if (helper) {
                    auto topSet = mChainCtrl->createChainFromPosToPos(column, row, column, row - 1);
                    auto botSet = mChainCtrl->createChainFromPosToPos(column, row, column, row + 1);
                    auto leftSet = mChainCtrl->createChainFromPosToPos(column, row, column - 1, row);
                    auto rightSet = mChainCtrl->createChainFromPosToPos(column, row, column + 1, row);
                    helper->setTopChain(topSet);
                    helper->setTopChain(botSet);
                    helper->setTopChain(leftSet);
                    helper->setTopChain(rightSet);
                    isDetected = true;
                }
            }            
        }
    }
    return isDetected;
}

bool DudeController::canActivateDudeTo(int horzDelta, int vertDelta)
{
    return false;
}

//--------------------------------------------------------------------
cocos2d::Set * DudeController::activateDude(DudeObj* obj, CommonTypes::Direction direction)
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    if (!obj) {
        cocos2d::log("DudeController::activateDude: empty ptr DudeObj");
        return set;
    }

    auto helper = mDudeDirections.at(obj);
    if (helper) {
        auto newSet = helper->getChainByDirection(direction);
        mChainCtrl->addChainsFromSetToSet(newSet, set);
    }
    
    return set;
}

void DudeController::activateAllDudes()
{
}


//--------------------------------------------------------------------
void DudeController::removeDude(int column, int row)
//--------------------------------------------------------------------
{
    auto dude = dudeObjectAt(column, row);

    if (!dude) {
        cocos2d::log("DudeController::removeDude: dude at (%d,%d) already removed", column, row);
        return;
    }
    cocos2d::log("DudeController::removeDude: remove %s", dude->description().getCString());
    SmartFactory->recycle(dude);
    mDudeObjects[column][row] = nullptr;
}

//--------------------------------------------------------------------
void DudeController::removeAllDudes()
//--------------------------------------------------------------------
{
    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns; column++) {
            removeDude(column, row);
        }
    }
}
