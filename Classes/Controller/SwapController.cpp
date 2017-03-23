/**
* @file Scenes/SwapController.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/SwapController.h"
#include "Controller/ChainController.h"
#include "Controller/ObjectController/ObjContainer.h"
#include "Controller/ObjectController/ObjectController.h"

#include "GameObjects/Swap/SwapObj.h"
#include "GameObjects/Level/LevelObj.h"
#include "GameObjects/TileObjects/CookieObj.h"

#include "Common/CommonTypes.h"
#include "Utils/Helpers/Helper.h"

#include "Layers/CookiesLayer.h"

//--------------------------------------------------------------------
SwapController::SwapController()
    : mLevel(nullptr)
    , mPossibleSwaps(nullptr)
//--------------------------------------------------------------------
{
}

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
    CommonTypes::Set* set = CommonTypes::Set::create();

    for (int row = 0; row < CommonTypes::NumRows; row++) {
        for (int column = 0; column < CommonTypes::NumColumns; column++) {
            auto objCtrl = mLevel->getObjectController();
            auto container = objCtrl->getObject(column, row);
            if (nullptr == container->getObjectForChain()) {
                continue;
            }
            // Is it possible to swap this cookie with the one on the right?
            if (column < CommonTypes::NumColumns - 1) {
                auto checker = new SwapChecker(set, column, row, column + 1, row);
                detectSwap(checker);
                CC_SAFE_DELETE(checker);
            }
            // This does exactly the same thing, but for the cookie above instead of on the right.
            if (row < CommonTypes::NumRows - 1) {
                auto checker = new SwapChecker(set, column, row, column, row + 1);
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
void SwapController::detectSwap(SwapChecker * checker)
//--------------------------------------------------------------------
{
    CC_ASSERT(checker);
    auto objCtrl = mLevel->getObjectController();

    auto currContainer = objCtrl->getObject(checker->curCol, checker->curRow);
    auto nextContainer = objCtrl->getObject(checker->nextCol, checker->nextRow);

    auto currObject = currContainer->getObjectForChain();
    auto nextObject = nextContainer->getObjectForChain();
    // Have a cookie in this spot? If there is no tile, there is no cookie.
    if (currObject && nextObject) {
        
        // Swap them
        currContainer->updateObjectWith(currObject, nextObject);
        nextContainer->updateObjectWith(nextObject, currObject);

        // Is either cookie now part of a chain?
        bool hasChainFromNextTile = objCtrl->hasChainAt(checker->nextCol, checker->nextRow);
        bool hasChainFromCurrTile = objCtrl->hasChainAt(checker->curCol, checker->curRow);
        if (hasChainFromNextTile || hasChainFromCurrTile) {
            auto chainCtrl = mLevel->getChainController();
            SwapObj* swap = SwapObj::createWithObjects(currObject, nextObject);
            if (hasChainFromNextTile) {
                swap->setObjectsForHint(chainCtrl->detectChainAt(checker->nextCol, checker->nextRow));
            }
            else if (hasChainFromCurrTile) {
                swap->setObjectsForHint(chainCtrl->detectChainAt(checker->curCol, checker->curRow));
            }
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
void SwapController::performSwap(SwapObj * swap)
//--------------------------------------------------------------------
{
    if (!swap)
        return;
    
    cocos2d::log("performSwap::performSwap: %s", swap->description().c_str());
    int columnA = swap->getObjectA()->getColumn();
    int rowA = swap->getObjectA()->getRow();
    int columnB = swap->getObjectB()->getColumn();
    int rowB = swap->getObjectB()->getRow();

    auto objCtrl = mLevel->getObjectController();

    auto currContainer = objCtrl->getObject(columnA, rowA);
    auto nextContainer = objCtrl->getObject(columnB, rowB);

    auto currObject = currContainer->getObjectForChain();
    auto nextObject = nextContainer->getObjectForChain();

    currContainer->updateObjectWith(currObject, nextObject);
    nextContainer->updateObjectWith(nextObject, currObject);
    currContainer->synchronizeTilePos();
    nextContainer->synchronizeTilePos();
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

    if (toColumn < 0 || toColumn >= CommonTypes::NumColumns)
        return false;
    if (toRow < 0 || toRow >= CommonTypes::NumRows)
        return false;

    auto objCtrl = mLevel->getObjectController();
    auto toCookie = objCtrl->getObject(toColumn, toRow)->getObjectForChain();
    if (!toCookie)
        return false;

    auto fromCookie = objCtrl->getObject(fromCol, fromRow)->getObjectForChain();
    if (!fromCookie)
        return false;

    cocos2d::log("GameplayScene::trySwapCookieTo: swap type:%d square:(%d,%d) with type:%d square:(%d,%d),"
        , fromCookie->getTypeAsInt(), fromCookie->getColumn(), fromCookie->getRow(), toCookie->getTypeAsInt()
        , toCookie->getColumn(), toCookie->getRow());

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
    if (!mPossibleSwaps) {
        return;
    }
    mPossibleSwaps->removeAllObjects();
    CC_SAFE_RELEASE_NULL(mPossibleSwaps);
}
