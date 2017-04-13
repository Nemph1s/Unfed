/**
* @file Controller/SwapController/SwapController.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/SwapController/SwapController.h"
#include "Controller/ChainController/ChainController.h"
#include "Controller/ObjectController/ObjContainer.h"
#include "Controller/ObjectController/ObjectController.h"

#include "Controller/SwapController/SwapObj.h"
#include "GameObjects/Level/LevelObj.h"
#include "GameObjects/TileObjects/CookieObj.h"

#include "Common/CommonTypes.h"
#include "Utils/Helpers/Helper.h"

#include "Layers/CookiesLayer.h"

using namespace CommonTypes;

//--------------------------------------------------------------------
SwapController::~SwapController()
//--------------------------------------------------------------------
{
    mLevel = nullptr;
    clearPossibleSwaps();
}

//--------------------------------------------------------------------
SwapController * SwapController::create()
//--------------------------------------------------------------------
{
    SwapController * ret = new (std::nothrow) SwapController();
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
bool SwapController::init()
//--------------------------------------------------------------------
{
    return true;
}

//--------------------------------------------------------------------
bool SwapController::detectPossibleSwaps()
//--------------------------------------------------------------------
{
    cocos2d::log("SwapController::detectPossibleSwaps:");

    clearPossibleSwaps();
    Set* set = Set::create();

    for (int row = 0; row < _GlobalInfo::NumRows; row++) {
        for (int column = 0; column < _GlobalInfo::NumColumns; column++) {

            auto objCtrl = mLevel->getObjectController();
            auto curCell = Cell(column, row);

            auto container = objCtrl->getContainer(curCell);
            if (!container) {
                continue;
            }
            if (!container->getObjectForChain()) {
                continue;
            }

            // Is it possible to swap this cookie with the one on the right?
            auto rigthCell = Cell(column + 1, row);
            if (column < _GlobalInfo::NumColumns - 1) {
                auto checker = new SwapChecker(set, curCell, rigthCell);
                detectSwap(checker);
                CC_SAFE_DELETE(checker);
            }
            // This does exactly the same thing, but for the cookie above instead of on the right.
            auto aboveCell = Cell(column, row + 1);
            if (row < _GlobalInfo::NumRows - 1) {
                auto checker = new SwapChecker(set, curCell, aboveCell);
                detectSwap(checker);
                CC_SAFE_DELETE(checker);
            }
        }
    }
    mPossibleSwaps = set;
    CC_SAFE_RETAIN(mPossibleSwaps);
    int count = 0;
    auto strSwaps = cocos2d::String::create("possible swaps: {\n");
    for (auto it = mPossibleSwaps->begin(); it != mPossibleSwaps->end(); ++it, count++) {
        auto swap = static_cast<SwapObj*>(*it);
        strSwaps->appendWithFormat("\t%s\n", swap->description().c_str());
    }
    strSwaps->append("}");
    cocos2d::log("SwapController::detectPossibleSwaps: count = %d\n%s", count, strSwaps->getCString());

    return count != 0 ? true : false;
}

//--------------------------------------------------------------------
void SwapController::detectSwap(SwapChecker* checker)
//--------------------------------------------------------------------
{
    CC_ASSERT(checker);
    auto objCtrl = mLevel->getObjectController();

    auto currContainer = objCtrl->getContainer(checker->curCell);
    auto nextContainer = objCtrl->getContainer(checker->nextCell);
    if (!currContainer || !nextContainer) {
        return;
    }
    auto currObject = currContainer->getObjectForChain();
    auto nextObject = nextContainer->getObjectForChain();
    // Have a cookie in this spot? If there is no tile, there is no cookie.
    if (currObject && nextObject) {
        
        // Swap them
        currContainer->updateObjectWith(currObject, nextObject);
        nextContainer->updateObjectWith(nextObject, currObject);

        // Is either cookie now part of a chain?
        bool hasChainFromNextTile = objCtrl->hasChainAt(checker->nextCell);
        bool hasChainFromCurrTile = false;

        // decrease attempts to looking chains in hasChainAt method
        if (!hasChainFromNextTile) {
            hasChainFromCurrTile = objCtrl->hasChainAt(checker->curCell);
        }

        if (hasChainFromNextTile || hasChainFromCurrTile) {
            auto chainCtrl = mLevel->getChainController();
            SwapObj* swap = SwapObj::createWithObjects(currObject, nextObject);
            
            BaseObj* curr = hasChainFromNextTile ? currObject : nextObject;
            BaseObj* next = hasChainFromNextTile ? nextObject : currObject;

            // need to use callback for determine only necessary chain
            std::function<Set*()> func = [chainCtrl, curr, next]() {
                auto set = chainCtrl->detectHintChainAt(curr, next);
                if (!set) { 
                    CC_ASSERT(set); //TODO: remove in future this check for nullptr. 
                    set = chainCtrl->detectHintChainAt(next, curr);
                }
                return set;
            };
            swap->setDetectHintChainCallback(func);

            checker->set->addObject(swap);
        }
        // Swap them back
        currContainer->updateObjectWith(nextObject, currObject);
        nextContainer->updateObjectWith(currObject, nextObject);
    }
}

//--------------------------------------------------------------------
bool SwapController::isPossibleSwap(SwapObj * swap)
//--------------------------------------------------------------------
{
    auto it = mPossibleSwaps->begin();
    for (; it != mPossibleSwaps->end(); ++it) {
        auto other = static_cast<SwapObj*>(*it);
        if (!other)
            continue;
        if ((other->getObjectA() == swap->getObjectA() && other->getObjectB() == swap->getObjectB()) ||
            (other->getObjectB() == swap->getObjectA() && other->getObjectA() == swap->getObjectB())) {

            if (swap->getObjectA()->isSwappable() && swap->getObjectB()->isSwappable()) {
                return true;
            }
        }            
    }
    return false;
}

//--------------------------------------------------------------------
void SwapController::performSwap(SwapObj* swap)
//--------------------------------------------------------------------
{
    if (!swap)
        return;
    
    cocos2d::log("performSwap::performSwap: %s", swap->description().c_str());

    auto objCtrl = mLevel->getObjectController();

    auto currContainer = objCtrl->getContainer(swap->getObjectA()->getCell());
    auto nextContainer = objCtrl->getContainer(swap->getObjectB()->getCell());

    auto currObject = currContainer->getObjectForChain();
    auto nextObject = nextContainer->getObjectForChain();

    currContainer->updateObjectWith(currObject, nextObject);
    nextContainer->updateObjectWith(nextObject, currObject);
    currContainer->synchronizeTilePos();
    nextContainer->synchronizeTilePos();

    mPreviousSwap = swap;
    CC_SAFE_RETAIN(mPreviousSwap);
}

//--------------------------------------------------------------------
bool SwapController::trySwapCookieTo(int fromCol, int fromRow, int direction)
//--------------------------------------------------------------------
{
    cocos2d::log("SwapController::trySwapCookieTo: direction=%d;", direction);

    int horzDelta = 0; int vertDelta = 0;
    Helper::getInstance()->convertDirectionToSwipeDelta(direction, horzDelta, vertDelta);
    int toColumn = fromCol + horzDelta;
    int toRow = fromRow + vertDelta;

    if (!Helper::isValidColumnAndRow(toColumn, toRow)) {
        return false;
    }
    auto objCtrl = mLevel->getObjectController();
    auto toContainer = objCtrl->getContainer(toColumn, toRow);
    auto fromContainer = objCtrl->getContainer(fromCol, fromRow);
    if (!toContainer || !fromContainer)
        return false;

    auto toCookie = toContainer->getObjectForChain();
    auto fromCookie = fromContainer->getObjectForChain();
    if (!toCookie || !fromCookie)
        return false;

    cocos2d::log("GameplayScene::trySwapCookieTo: swap type:%d square:(%d,%d) with type:%d square:(%d,%d),"
        , fromCookie->getTypeAsInt(), fromCookie->getColumn(), fromCookie->getRow(), toCookie->getTypeAsInt()
        , toCookie->getColumn(), toCookie->getRow());

    if (!fromCookie->isSwappable() || !toCookie->isSwappable()) {
        cocos2d::log("GameplayScene::trySwapCookieTo: cant swap non swapable objecs");
        return false;
    }

    auto cookieType = CommonTypes::BaseObjType::Cookie;
    if (fromCookie->getType() != cookieType && toCookie->getType() != cookieType) {
        cocos2d::log("GameplayScene::trySwapCookieTo: cant swap non cookies obj");
        return false;
    }
    if (!mSwapCallback)
        return false;

    SwapObj* swap = SwapObj::createWithObjects(fromCookie, toCookie);
    if (!swap)
        return false;

    mSwapCallback(swap);

    return true;
}

//--------------------------------------------------------------------
void SwapController::clearPossibleSwaps()
//--------------------------------------------------------------------
{
    if (mPreviousSwap) {
        CC_SAFE_RELEASE_NULL(mPreviousSwap);
    }
    if (mPossibleSwaps) {
        mPossibleSwaps->removeAllObjects();
        CC_SAFE_RELEASE_NULL(mPossibleSwaps);
    }    
}

//--------------------------------------------------------------------
CommonTypes::Set* SwapController::getPreviousSwapContainers()
//--------------------------------------------------------------------
{
    if (!mPreviousSwap) {
        return nullptr;
    }
    CommonTypes::Set* set = CommonTypes::Set::create();
    auto objCtrl = mLevel->getObjectController();
    auto objA = mPreviousSwap->getObjectA();
    if (objA) {
        auto containerA = objCtrl->getContainer(objA->getColumn(), objA->getRow());
        set->addObject(containerA);
    }
    auto objB = mPreviousSwap->getObjectB();
    if (objB) {
        auto containerB = objCtrl->getContainer(objB->getColumn(), objB->getRow());
        set->addObject(containerB);
    }
    CC_SAFE_RELEASE_NULL(mPreviousSwap);
    return set;
}
