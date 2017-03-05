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
#include "Controller/ObjectController/ObjectController.h"

#include "GameObjects/Swap/SwapObj.h"
#include "GameObjects/Level/LevelObj.h"
#include "GameObjects/TileObjects/CookieObj.h"

#include "Common/CommonTypes.h"
#include "Utils/Helpers/Helper.h"

#include "Layers/CookiesLayer.h"

//--------------------------------------------------------------------
SwapController::SwapController()
//--------------------------------------------------------------------
    : mLevel(nullptr)
    , mPossibleSwaps(nullptr)
{
}

//--------------------------------------------------------------------
SwapController::~SwapController()
//--------------------------------------------------------------------
{
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
    cocos2d::Set* set = cocos2d::Set::create();

    for (int row = 0; row < CommonTypes::NumRows; row++) {
        for (int column = 0; column < CommonTypes::NumColumns; column++) {
            auto objCtrl = mLevel->getObjectController();
            if (nullptr == objCtrl->cookieAt(column, row)) {
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
    auto cookie = objCtrl->cookieAt(checker->curCol, checker->curRow);
    // Have a cookie in this spot? If there is no tile, there is no cookie.
    auto other = objCtrl->cookieAt(checker->nextCol, checker->nextRow);
    if (cookie && other) {
        // Swap them
        objCtrl->updateCookieObjectAt(checker->curCol, checker->curRow, other);
        objCtrl->updateCookieObjectAt(checker->nextCol, checker->nextRow, cookie);

        // Is either cookie now part of a chain?
        if (objCtrl->hasChainAt(checker->nextCol, checker->nextRow) || objCtrl->hasChainAt(checker->curCol, checker->curRow)) {

            SwapObj *swap = SwapObj::createWithCookies(cookie, other);
            checker->set->addObject(swap);
        }
        // Swap them back
        objCtrl->updateCookieObjectAt(checker->curCol, checker->curRow, cookie);
        objCtrl->updateCookieObjectAt(checker->nextCol, checker->nextRow, other);
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
        if ((other->getCookieA() == swap->getCookieA() && other->getCookieB() == swap->getCookieB()) ||
            (other->getCookieB() == swap->getCookieA() && other->getCookieA() == swap->getCookieB())) {

            if (swap->getCookieA()->isSwappable() && swap->getCookieB()->isSwappable()) {
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
    int columnA = swap->getCookieA()->getColumn();
    int rowA = swap->getCookieA()->getRow();
    int columnB = swap->getCookieB()->getColumn();
    int rowB = swap->getCookieB()->getRow();

    auto objCtrl = mLevel->getObjectController();
    objCtrl->updateCookieObjectAt(columnA, rowA, swap->getCookieB());
    swap->getCookieB()->setColumn(columnA);
    swap->getCookieB()->setRow(rowA);

    objCtrl->updateCookieObjectAt(columnB, rowB, swap->getCookieA());
    swap->getCookieA()->setColumn(columnB);
    swap->getCookieA()->setRow(rowB);
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
    CookieObj* toCookie = objCtrl->cookieAt(toColumn, toRow);
    if (!toCookie)
        return false;

    CookieObj* fromCookie = objCtrl->cookieAt(fromCol, fromRow);
    if (!fromCookie)
        return false;

    cocos2d::log("GameplayScene::trySwapCookieTo: fromCookie=[%d,%d]; toCookie=[%d][%d];"
        , fromCookie->getColumn(), fromCookie->getRow(), toCookie->getColumn(), toCookie->getRow());

    if (!mSwapCallback)
        return false;

    SwapObj* swap = SwapObj::createWithCookies(fromCookie, toCookie);
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
