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

#define RequiredCountForDudeFromAToB 4
#define RequiredCountForDudeFromAToBx3 5
#define RequiredAmountForOni 5
#define RequiredAmountForPina 5

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
    mDudeTypes[RequiredCountForDudeFromAToB] = TileType::DudeFromAToBx3;
    mDudeTypes[RequiredCountForDudeFromAToBx3] = TileType::DudeFromAToB;
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

        auto dudeType = getDudeTypeByChain(chain);
        if (dudeType != TileType::Unknown) {
            auto cookies = chain->getCookies();
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
void DudeController::detectDirectionsForDudes()
//--------------------------------------------------------------------
{
    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns; column++) {
            
            auto dude = dudeObjectAt(column, row);
            if (dude) {
                auto helper = mDudeDirections.at(dude);
                if (helper) {
                    updateDirectionsForDude(dude, helper);
                }
            }            
        }
    }
}

//--------------------------------------------------------------------
void DudeController::updateDirectionsForDude(DudeObj* obj, DudeHelper* helper)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);
    CC_ASSERT(helper);

    int column = obj->getColumn();
    int row = obj->getRow();

    auto topSet = cocos2d::Set::create();
    auto botSet = cocos2d::Set::create();
    auto leftSet = cocos2d::Set::create();
    auto rightSet = cocos2d::Set::create();

    switch (obj->getTileType())
    {
    case TileType::DudeFromAToBx3:
    {
        for (int i = -1; i <= 1; i++) {
            auto newTopSet = mChainCtrl->createChainFromPosToPos(column + i, row, column + i, 0);
            auto newBotSet = mChainCtrl->createChainFromPosToPos(column + i, row, column + i, NumRows - 1);
            auto newLeftSet = mChainCtrl->createChainFromPosToPos(column, row + i, 0, row + i);
            auto newRightSet = mChainCtrl->createChainFromPosToPos(column, row + i, NumColumns - 1, row + i);
            mChainCtrl->addChainsFromSetToSet(newTopSet, topSet);
            mChainCtrl->addChainsFromSetToSet(newBotSet, botSet);
            mChainCtrl->addChainsFromSetToSet(newLeftSet, leftSet);
            mChainCtrl->addChainsFromSetToSet(newRightSet, rightSet);
        }
    }
        break;
    case TileType::DudeChainX:
    {
        auto set = mChainCtrl->createXChainAt(column, row);
        mChainCtrl->addChainsFromSetToSet(set, topSet);
        mChainCtrl->addChainsFromSetToSet(set, botSet);
        mChainCtrl->addChainsFromSetToSet(set, leftSet);
        mChainCtrl->addChainsFromSetToSet(set, rightSet);
    }
    break;
    case TileType::DudeAllOfType:
    {
        auto newTopSet = mChainCtrl->createAllOfOneChain(column, row - 1);
        auto newBotSet = mChainCtrl->createAllOfOneChain(column, row + 1);
        auto newLeftSet = mChainCtrl->createAllOfOneChain(column - 1, row);
        auto newRightSet = mChainCtrl->createAllOfOneChain(column + 1, row);
        mChainCtrl->addChainsFromSetToSet(newTopSet, topSet);
        mChainCtrl->addChainsFromSetToSet(newBotSet, botSet);
        mChainCtrl->addChainsFromSetToSet(newLeftSet, leftSet);
        mChainCtrl->addChainsFromSetToSet(newRightSet, rightSet);
    }
        break;
    case TileType::DudeFromAToB:
    default:
    {
        auto newTopSet = mChainCtrl->createChainFromPosToPos(column, row, column, 0);
        auto newBotSet = mChainCtrl->createChainFromPosToPos(column, row, column, NumRows - 1);
        auto newLeftSet = mChainCtrl->createChainFromPosToPos(column, row, 0, row);
        auto newRightSet = mChainCtrl->createChainFromPosToPos(column, row, NumColumns - 1, row);
        mChainCtrl->addChainsFromSetToSet(newTopSet, topSet);
        mChainCtrl->addChainsFromSetToSet(newBotSet, botSet);
        mChainCtrl->addChainsFromSetToSet(newLeftSet, leftSet);
        mChainCtrl->addChainsFromSetToSet(newRightSet, rightSet);
    }
        break;
    }

    helper->setTopChain(topSet);
    helper->setBottomChain(botSet);
    helper->setLeftChain(leftSet);
    helper->setRightChain(rightSet);
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
cocos2d::Set * DudeController::activateDude(DudeObj* obj, int dir)
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    if (!obj) {
        cocos2d::log("DudeController::activateDude: empty ptr DudeObj");
        return set;
    }
    auto helper = mDudeDirections.at(obj);
    if (helper) {
        obj->activate();

        auto direction = static_cast<Direction>(dir);
        auto chains = helper->getChainByDirection(direction);

        mChainCtrl->addChainsFromSetToSet(chains, set);
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
bool DudeController::isEnoughCookiesForDude(int count, int neededCount)
//--------------------------------------------------------------------
{
    return count >= neededCount;
}

//--------------------------------------------------------------------
CommonTypes::TileType DudeController::getDudeTypeByChain(ChainObj * chain)
//--------------------------------------------------------------------
{
    auto type = TileType::Unknown;
    auto cookiesCount = chain->getCookies() ? chain->getCookies()->count() : 0;
    switch (chain->getType())
    {
    case ChainType::ChainTypeHorizontal:
    case ChainType::ChainTypeVertical:
        if (isEnoughCookiesForDude(cookiesCount, RequiredCountForDudeFromAToBx3)) {
            type = TileType::DudeFromAToBx3;
        } else if (isEnoughCookiesForDude(cookiesCount, RequiredCountForDudeFromAToB)) {
            type = TileType::DudeFromAToB;
        }
        break;
    case ChainType::ChainTypeL:
        if (isEnoughCookiesForDude(cookiesCount, RequiredAmountForOni)) {
            type = TileType::DudeChainX;
        }
        break;
    case ChainType::ChainTypeT:
        if (isEnoughCookiesForDude(cookiesCount, RequiredAmountForPina)) {
            type = TileType::DudeAllOfType;
        }
        break;
    default:
        break;
    }
    return type;
}