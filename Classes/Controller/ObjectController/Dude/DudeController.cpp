/**
* @file Scenes/ObjectController/Dude/DudeController.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ObjectController/Dude/DudeController.h"
#include "Controller/ObjectController/Dude/DudeHelper.h"
#include "Controller/ObjectController/ObjectController.h"
#include "Controller/ObjectController/ObjContainer.h"

#include "Controller/ChainController/ChainController.h"

#include "Common/Factory/SmartObjFactory.h"
#include "Common/GlobalInfo/GlobalInfo.h"

#include "Utils/Parser/JsonParser.h"
#include "Utils/Helpers/Helper.h"

#include "Controller/ObjectController/Dude/DudeObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "Controller/ChainController/ChainObj.h"

#define RequiredCountForDudeFromAToB 4
#define RequiredCountForDudeFromAToBx3 5
#define RequiredAmountForOni 5
#define RequiredAmountForMBO 5
#define RequiredAmountForPina 5

using namespace CommonTypes;

//--------------------------------------------------------------------
DudeController::DudeController()
    : mObjCtrl(nullptr)
    , mChainCtrl(nullptr)
    , mDudeDirections()
    , mDudesCount(0)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
DudeController::~DudeController()
//--------------------------------------------------------------------
{
    mObjCtrl = nullptr;
    mChainCtrl = nullptr;
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
Set* DudeController::createDudeObectsFromChains(Set* chains, Set* prevSwapContainers)
//--------------------------------------------------------------------
{
    auto set = Set::create();
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);

        auto dudeType = getDudeTypeByChain(chain);
        if (dudeType != FieldType::Unknown) {
            int column = -1;
            int row = -1;
            if (mChainCtrl->getCellFromChainAndPrevSwapSet(column, row, chain, prevSwapContainers)) {
                int type = Helper::to_underlying(dudeType);
                auto dude = createDudeObject(column, row, type);
                set->addObject(dude);
            }            
        }
    }
    return set;
}

//--------------------------------------------------------------------
BaseObj * DudeController::createDudeObject(int column, int row, int type)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Dude, column, row };
    FieldInfo info = { baseInfo, static_cast<FieldType>(type) };
    auto obj = dynamic_cast<DudeObj*>(SmartObjFactory->createDudeObj(info));
    CC_ASSERT(obj);
    auto container = mObjCtrl->getContainer(column, row);
    if (container) {
        container->addObject(obj);
    }
    auto helper = DudeHelper::createWithDudeObject(obj);
    helper->setDudeController(this);
    mDudeDirections.insert(obj, helper);
    mDudesCount++;

    return obj;
}

//--------------------------------------------------------------------
void DudeController::detectDirectionsForDudes()
//--------------------------------------------------------------------
{
    for (int row = 0; row < _GlobalInfo::NumRows; row++) {
        for (int column = 0; column < _GlobalInfo::NumColumns; column++) {
            
            auto dude = mObjCtrl->dudeAt(column, row);
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

    auto topSet = Set::create();
    auto botSet = Set::create();
    auto leftSet = Set::create();
    auto rightSet = Set::create();
    auto horizontalSet = Set::create();
    auto verticalSet = Set::create();
    auto xSet = Set::create();

    switch (obj->getFieldType())
    {
    case FieldType::DudeChainX:
    {
        topSet = botSet = leftSet = rightSet = mChainCtrl->createXChainAt(column, row, true);
        xSet = mChainCtrl->createXChainAt(column, row, true);
        horizontalSet = verticalSet = xSet;
    }
    break;
    case FieldType::DudeSquareBomb:
    case FieldType::DudeAllOfType:
    {
        topSet = mChainCtrl->createAllOfOneChain(column, row - 1, true, obj);
        botSet = mChainCtrl->createAllOfOneChain(column, row + 1, true, obj);
        leftSet = mChainCtrl->createAllOfOneChain(column - 1, row, true, obj);
        rightSet = mChainCtrl->createAllOfOneChain(column + 1, row, true, obj);
        xSet = mChainCtrl->createXChainAt(column, row, true);
        horizontalSet = verticalSet = xSet;
    }
        break;
    case FieldType::DudeFromAToB:
    case FieldType::DudeFromAToBx3:
    default:
    {
        topSet = mChainCtrl->createChainFromPosToPos(Direction::Up, column, row, column, 0, true);
        botSet = mChainCtrl->createChainFromPosToPos(Direction::Down, column, row, column, _GlobalInfo::NumRows - 1, true);
        leftSet = mChainCtrl->createChainFromPosToPos(Direction::Left, column, row, 0, row, true);
        rightSet = mChainCtrl->createChainFromPosToPos(Direction::Right, column, row, _GlobalInfo::NumColumns - 1, row, true);
        horizontalSet = mChainCtrl->createHorizontalChainAt(column, row, true);
        verticalSet = mChainCtrl->createVerticalChainAt(column, row, true);
    }
        break;
    }

    if (obj->getFieldType() == FieldType::DudeFromAToBx3) {
        for (int i = -1; i <= 1; i++) {
            if (i == 0) {
                continue;
            }
            auto newTopSet = mChainCtrl->createChainFromPosToPos(Direction::Up, column + i, row, column + i, 0, true);
            auto newBotSet = mChainCtrl->createChainFromPosToPos(Direction::Down, column + i, row, column + i, _GlobalInfo::NumRows - 1, true);
            auto newLeftSet = mChainCtrl->createChainFromPosToPos(Direction::Left, column, row + i, 0, row + i, true);
            auto newRightSet = mChainCtrl->createChainFromPosToPos(Direction::Right, column, row + i, _GlobalInfo::NumColumns - 1, row + i, true);
            auto newHorizontalSet = mChainCtrl->createHorizontalChainAt(column, row + i, true);
            auto newVerticalSet = mChainCtrl->createVerticalChainAt(column + i, row, true);
            mChainCtrl->addObjectsFromChainToChain(newTopSet, topSet);
            mChainCtrl->addObjectsFromChainToChain(newBotSet, botSet);
            mChainCtrl->addObjectsFromChainToChain(newLeftSet, leftSet);
            mChainCtrl->addObjectsFromChainToChain(newRightSet, rightSet);
            mChainCtrl->addObjectsFromChainToChain(newHorizontalSet, horizontalSet);
            mChainCtrl->addObjectsFromChainToChain(newVerticalSet, verticalSet);
        }
    }

    helper->setTopChain(topSet);
    helper->setBottomChain(botSet);
    helper->setLeftChain(leftSet);
    helper->setRightChain(rightSet);
    helper->setHorizontalChain(horizontalSet);
    helper->setVerticalChain(verticalSet);
    helper->setXChain(xSet);
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

    if (toColumn < 0 || toColumn >= _GlobalInfo::NumColumns || toRow < 0 || toRow >= _GlobalInfo::NumRows)
        return false;

    auto fromObj = mObjCtrl->dudeAt(fromCol, fromRow);
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
Set * DudeController::getChainsForDude(DudeObj* obj, int dir, bool isPreview)
//--------------------------------------------------------------------
{
    Set* set = nullptr;
    if (!obj) {
        cocos2d::log("DudeController::activateDudeAndGetChains: empty ptr DudeObj");
        return set;
    }
    auto direction = static_cast<Direction>(dir);
    if (direction == Direction::Unknown) {
        return set;
    }

    auto helper = mDudeDirections.at(obj);
    if (helper) {

        obj->activate();
        auto chains = helper->getChainByDirection(direction, isPreview);
        if (!chains) {
            cocos2d::log("DudeController::activateDudeAndGetChains: empty chain from helper! direction=%d", dir);
            return set;
        }

        set = Set::create();
        updateChainSetWithDudesInChain(direction, obj, chains, set);
        // create param without dude obj, to skip all dudes from adding except dude at 0 chain pos
        mChainCtrl->addChainsFromSetToSet(chains, set, true); 
        if (isPreview) {
            mChainCtrl->deactivateChains(set);
        }
    }
    
    return set;
}

//--------------------------------------------------------------------
void DudeController::updateChainSetWithDudesInChain(const Direction& direction, DudeObj* activeDude, Set* chains, Set* chainSet)
//--------------------------------------------------------------------
{
    CC_ASSERT(activeDude);
    cocos2d::log("DudeController::updateChainSetWithDudesInChain: direction=%d", Helper::to_underlying(direction));

    mChainCtrl->activateChains(chains);

    for (auto itChain = chains->begin(); itChain != chains->end(); ++itChain) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);

        auto color = Helper::getScoreColorByObj(activeDude);
        chain->setChainColor(color);

        auto objects = chain->getObjects();
        if (!objects) 
            continue;

        uint8_t index = 0;
        for (auto it = objects->begin(); it != objects->end(); it++, index++) {
            auto container = dynamic_cast<ObjContainer*>(*it);
            CC_ASSERT(container);
            
            auto objects = container->getObjectsForChain();
            if (!objects) 
                continue;

            for (auto itObj = objects->begin(); itObj != objects->end(); ++itObj) {
                auto object = dynamic_cast<BaseObj*>(*itObj);
                CC_ASSERT(object);

                if (object->getType() == BaseObjType::Dude) {

                    if (index == 0)  // skip first dude to avoid dead loop
                        continue;

                    CommonTypes::Set* newChains = nullptr;
                    auto dude = container->getDude();
                    auto realDirection = Helper::getDirectionByTileFromAToB(Helper::to_underlying(direction), activeDude, dude);
                    auto invertedDirection = Helper::invertDirection(realDirection);

                    cocos2d::log("DudeController::updateChainSetWithDudesInChain: realDirection=%d", Helper::to_underlying(realDirection));

                    auto helper = mDudeDirections.at(dude);
                    if (helper && !dude->isActivated()) {
                        newChains = helper->getChainByDirection(invertedDirection);
                    }

                    //Beware of recursive
                    if (newChains != nullptr) {
                        dude->activate();

                        updateChainSetWithDudesInChain(invertedDirection, dude, newChains, chainSet);
                        mChainCtrl->addChainsFromSetToSet(newChains, chainSet, true); //TODO: Fix this!!!
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------------
void DudeController::eraseDirectionsForDude(DudeObj * obj)
//--------------------------------------------------------------------
{
    if (!obj) {
        cocos2d::log("DudeController::eraseDirectionsForDude: epmty dude ptr");
        return;
    }
    mDudeDirections.erase(obj);
    mDudesCount--;
}

//--------------------------------------------------------------------
bool DudeController::isEnoughCookiesForDude(int count, int neededCount)
//--------------------------------------------------------------------
{
    return count >= neededCount;
}

//--------------------------------------------------------------------
FieldType DudeController::getDudeTypeByChain(ChainObj * chain)
//--------------------------------------------------------------------
{
    auto type = FieldType::Unknown;
    auto cookiesCount = chain->getCookiesCount();
    switch (chain->getType())
    {
    case ChainType::ChainTypeHorizontal:
    case ChainType::ChainTypeVertical:
        if (isEnoughCookiesForDude(cookiesCount, RequiredCountForDudeFromAToBx3)) {
            type = FieldType::DudeFromAToBx3;
        } else if (isEnoughCookiesForDude(cookiesCount, RequiredCountForDudeFromAToB)) {
            type = FieldType::DudeFromAToB;
        }
        break;
    case ChainType::ChainTypeL:
        if (isEnoughCookiesForDude(cookiesCount, RequiredAmountForOni)) {
            type = FieldType::DudeChainX;
        }
        break;
    case ChainType::ChainTypeX:
        if (isEnoughCookiesForDude(cookiesCount, RequiredAmountForMBO)) {
            type = FieldType::DudeSquareBomb;
        }
        break;
    case ChainType::ChainTypeT:
        if (isEnoughCookiesForDude(cookiesCount, RequiredAmountForPina)) {
            type = FieldType::DudeAllOfType;
        }
        break;
    default:
        break;
    }
    return type;
}