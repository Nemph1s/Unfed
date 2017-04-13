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
#define RequiredAmountForDudeChainX 5
#define RequiredAmountForDudeAllOfType 5
#define RequiredAmountForDudeSquareBomb 5

using namespace CT;

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
            auto cell = Cell();
            if (mChainCtrl->getCellFromChainAndPrevSwapSet(cell, chain, prevSwapContainers)) {
                int type = Helper::to_underlying(dudeType);
                auto dude = createDudeObject(cell, type);
                set->addObject(dude);
            }            
        }
    }
    return set;
}

//--------------------------------------------------------------------
BaseObj* DudeController::createDudeObjWithoutContainer(int type)
//--------------------------------------------------------------------
{
    //TODO: maybe retain obj?
    BaseObjInfo baseInfo = { BaseObjType::Dude };
    FieldInfo info = { baseInfo, static_cast<FieldType>(type) };
    auto obj = dynamic_cast<DudeObj*>(SmartObjFactory->createDudeObj(info));
    CC_ASSERT(obj);

    auto helper = DudeHelper::createWithDudeObject(obj);
    helper->setDudeController(this);
    mDudeDirections.insert(obj, helper);
    mDudesCount++;

    return obj;
}

//--------------------------------------------------------------------
BaseObj * DudeController::createDudeObject(CT::Cell& cell, int type)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Dude, cell };
    FieldInfo info = { baseInfo, static_cast<FieldType>(type) };
    auto obj = dynamic_cast<DudeObj*>(SmartObjFactory->createDudeObj(info));
    CC_ASSERT(obj);
    auto container = mObjCtrl->getContainer(cell);
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
            
            auto cell = Cell(column, row);
            auto dude = mObjCtrl->dudeAt(cell);
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
    auto cell = obj->getCell();

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
        horizontalSet = verticalSet = xSet = topSet = botSet = leftSet = rightSet = mChainCtrl->createXChainAt(cell, true);
    }
    break;
    case FieldType::DudeSquareBomb:
    {
        horizontalSet = verticalSet = xSet = topSet = botSet = leftSet = rightSet = mChainCtrl->createExplosionChainAt(cell, true);
    }
    break;
    case FieldType::DudeAllOfType:
    {
        topSet = mChainCtrl->createAllOfOneChain(Cell(column, row - 1), true, obj);
        botSet = mChainCtrl->createAllOfOneChain(Cell(column, row + 1), true, obj);
        leftSet = mChainCtrl->createAllOfOneChain(Cell(column - 1, row), true, obj);
        rightSet = mChainCtrl->createAllOfOneChain(Cell(column + 1, row), true, obj);
        xSet = mChainCtrl->createXChainAt(cell, true);
        horizontalSet = verticalSet = xSet;
    }
        break;
    case FieldType::DudeFromAToB:
    case FieldType::DudeFromAToBx3:
    default:
    {
        topSet = mChainCtrl->createChainFromPosToPos(Direction::Up
            , cell
            , Cell(column, 0)
            , true);
        botSet = mChainCtrl->createChainFromPosToPos(Direction::Down
            , cell
            , Cell(column, _GlobalInfo::NumRows - 1)
            , true);
        leftSet = mChainCtrl->createChainFromPosToPos(Direction::Left
            , cell
            , Cell(0, row)
            , true);
        rightSet = mChainCtrl->createChainFromPosToPos(Direction::Right
            , cell
            , Cell(_GlobalInfo::NumColumns - 1, row)
            , true);

        horizontalSet = mChainCtrl->createHorizontalChainAt(cell, true);
        verticalSet = mChainCtrl->createVerticalChainAt(cell, true);
    }
        break;
    }

    if (obj->getFieldType() == FieldType::DudeFromAToBx3) {
        for (int i = -1; i <= 1; i++) {
            if (i == 0) {
                continue;
            }
            auto newTopSet = mChainCtrl->createChainFromPosToPos(Direction::Up
                , Cell(column + i, row)
                , Cell(column + i, 0)
                , true);
            auto newBotSet = mChainCtrl->createChainFromPosToPos(Direction::Down
                , Cell(column + i, row)
                , Cell(column + i, _GlobalInfo::NumRows - 1)
                , true);
            auto newLeftSet = mChainCtrl->createChainFromPosToPos(Direction::Left
                , Cell(column, row + i)
                , Cell(0, row + i)
                , true);
            auto newRightSet = mChainCtrl->createChainFromPosToPos(Direction::Right
                , Cell(column, row + i)
                , Cell(_GlobalInfo::NumColumns - 1, row + i)
                , true);

            auto newHorizontalSet = mChainCtrl->createHorizontalChainAt(Cell(column, row + i), true);
            auto newVerticalSet = mChainCtrl->createVerticalChainAt(Cell(column + i, row), true);
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
bool DudeController::canActivateDudeTo(Cell& fromCell, int direction)
//--------------------------------------------------------------------
{
    cocos2d::log("DudeController::canActivateDudeTo: direction=%d;", direction);

    int horzDelta = 0; int vertDelta = 0;
    Helper::getInstance()->convertDirectionToSwipeDelta(direction, horzDelta, vertDelta);

    auto toCell = Cell(fromCell.column + horzDelta, fromCell.row + vertDelta);

    if (!Helper::isValidCell(toCell)) {
        return false;
    }

    auto fromObj = mObjCtrl->dudeAt(fromCell);
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

                    CT::Set* newChains = nullptr;
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
        if (isEnoughCookiesForDude(cookiesCount, RequiredAmountForDudeAllOfType)) {
            type = FieldType::DudeAllOfType;
        }
        break;
    case ChainType::ChainTypeX:
        if (isEnoughCookiesForDude(cookiesCount, RequiredAmountForDudeSquareBomb)) {
            type = FieldType::DudeSquareBomb;
        }
        break;
    case ChainType::ChainTypeT:
        if (isEnoughCookiesForDude(cookiesCount, RequiredAmountForDudeChainX)) {
            type = FieldType::DudeChainX;
        }        
        break;
    default:
        break;
    }
    return type;
}