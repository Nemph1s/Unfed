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
#include "Controller/ObjectController/Dude/DudeHelper.h"
#include "Controller/ObjectController/ObjectController.h"
#include "Controller/ChainController.h"

#include "Common/Factory/SmartFactory.h"

#include "Utils/Parser/JsonParser.h"
#include "Utils/Helpers/Helper.h"

#include "GameObjects/TileObjects/DudeObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/Chain/ChainObj.h"

#define RequiredAmountForLightr 4
#define RequiredAmountForBulbr 5

using namespace CommonTypes;

//--------------------------------------------------------------------
DudeController::DudeController()
    : mObjCtrl(nullptr)
    , mChainCtrl(nullptr)
    , mDudeDirections()
    , mDudeTypes()
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
    mDudeTypes[RequiredAmountForLightr] = TileType::DudeLightr;
    mDudeTypes[RequiredAmountForBulbr] = TileType::DudeBulbr;
    return true;
}

//--------------------------------------------------------------------
cocos2d::Set* DudeController::createDudeObectsFromChains(cocos2d::Set * chains)
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);

        auto cookies = chain->getCookies();
        auto dudeType = getDudeTypeByMatchedCount(cookies->count());

        if (dudeType != TileType::Unknown) {
            auto cookie = dynamic_cast<CookieObj*>(cookies->getRandomObject());
            if (!cookie) {
                continue;
            }
            int type = Helper::to_underlying(dudeType);
            auto dude = createDudeObject(cookie->getColumn(), cookie->getRow(), type);
            set->addObject(dude);
        }
    }
    return set;
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
BaseObj* DudeController::objectAt(int column, int row)
//--------------------------------------------------------------------
{
    if (!(column >= 0 && column < NumColumns) || !(row >= 0 && row < NumColumns)) {
        cocos2d::log("ChainController::createChainFromPosToPos: wrong destinationPos at column=%d, row=%d", column, row);
        return nullptr;
    }
    return mDudeObjects[column][row];
}

//--------------------------------------------------------------------
DudeObj* DudeController::dudeObjectAt(int column, int row)
//--------------------------------------------------------------------
{
    return dynamic_cast<DudeObj*>(objectAt(column,row));
}

//--------------------------------------------------------------------
bool DudeController::detectDirectionsForDudes()
//--------------------------------------------------------------------
{
    bool isDetected = false;
    if (mDudeDirections.size() == 0) {
        return isDetected;
    }
    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns; column++) {
            
            auto dude = dudeObjectAt(column, row);
            if (dude) {
                auto helper = mDudeDirections.at(dude);
                if (helper) {
                    auto topSet = mChainCtrl->createChainFromPosToPos(column, row, column, 0);
                    auto botSet = mChainCtrl->createChainFromPosToPos(column, row, column, NumRows - 1);
                    auto leftSet = mChainCtrl->createChainFromPosToPos(column, row, 0, row);
                    auto rightSet = mChainCtrl->createChainFromPosToPos(column, row, NumColumns - 1, row);
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

//--------------------------------------------------------------------
bool DudeController::canActivateDudeTo(int fromCol, int fromRow, int direction)
//--------------------------------------------------------------------
{
    cocos2d::log("DudeController::canActivateDudeTo: direction=%d;", direction);

    int horzDelta = 0; int vertDelta = 0;
    Helper::getInstance()->convertDirectionToSwipeDelta(direction, horzDelta, vertDelta);
    int toColumn = fromCol + horzDelta;
    int toRow = fromRow + vertDelta;

    if (toColumn < 0 || toColumn >=NumColumns)
        return false;
    if (toRow < 0 || toRow >= NumRows)
        return false;

    auto fromObj = dudeObjectAt(fromCol, fromRow);
    if (!fromObj)
        return false;

    cocos2d::log("DudeController::canActivateDudeTo: objAt=[%d,%d];"
        , fromObj->getColumn(), fromObj->getRow());

    if (!mActivateDudeCallback)
        return false;


    mActivateDudeCallback(fromObj, direction);

    return true;
}

//--------------------------------------------------------------------
cocos2d::Set * DudeController::activateDude(DudeObj* obj, int direction)
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    if (!obj) {
        cocos2d::log("DudeController::activateDude: empty ptr DudeObj");
        return set;
    }

    auto helper = mDudeDirections.at(obj);
    if (helper) {
        auto dir = static_cast<Direction>(direction);
        auto newSet = helper->getChainByDirection(dir);
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
    mDudeDirections.erase(dude);
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

//--------------------------------------------------------------------
CommonTypes::TileType DudeController::getDudeTypeByMatchedCount(int count)
//--------------------------------------------------------------------
{
    auto type = TileType::Unknown;
    if (count < RequiredAmountForLightr || count > RequiredAmountForBulbr) {
        return type;
    }
    return mDudeTypes.at(count);
}