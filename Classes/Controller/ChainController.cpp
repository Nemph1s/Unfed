/**
* @file Scenes/SwapController.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ChainController.h"
#include "Controller/ObjectController/ObjectController.h"

#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/Chain/ChainObj.h"
#include "GameObjects/LevelObj.h"

#include "Utils/Helpers/Helper.h"

#include "Common/CommonTypes.h"

using namespace CommonTypes;

//--------------------------------------------------------------------
ChainController::ChainController()
//--------------------------------------------------------------------
    : mLevel(nullptr)
{
}

//--------------------------------------------------------------------
ChainController::~ChainController()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
ChainController * ChainController::create()
//--------------------------------------------------------------------
{
    ChainController * ret = new (std::nothrow) ChainController();
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
bool ChainController::init()
//--------------------------------------------------------------------
{
    return true;
}

//--------------------------------------------------------------------
cocos2d::Set * ChainController::removeMatches()
//--------------------------------------------------------------------
{
    cocos2d::log("ChainController::removeMatches:");
    auto horizontalChains = detectHorizontalMatches();
    auto verticalChains = detectVerticalMatches();
    auto difficultChains = detectDifficultMatches(horizontalChains, verticalChains);

    auto set = cocos2d::Set::create();

    addChainsFromSetToSet(horizontalChains, set);
    addChainsFromSetToSet(verticalChains, set);
    addChainsFromSetToSet(difficultChains, set);

#ifdef COCOS2D_DEBUG
    logDebugChains(horizontalChains, verticalChains, difficultChains);
#endif //COCOS2D_DEBUG

    mLevel->calculateScore(horizontalChains);
    mLevel->removeCookies(horizontalChains);

    mLevel->calculateScore(verticalChains);
    mLevel->removeCookies(verticalChains);

    mLevel->calculateScore(difficultChains);
    mLevel->removeCookies(difficultChains);

    return set;
}

//--------------------------------------------------------------------
cocos2d::Set * ChainController::removeChainAt(CommonTypes::ChainType & type, cocos2d::Vec2 & pos)
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();

    int column = -1, row = -1;
    if (Helper::convertPointToTilePos(pos, column, row)) {
        cocos2d::Set* chainSet = nullptr;
        switch (type)
        {
        case ChainType::ChainTypeHorizontal:
            chainSet = createHorizontalChainAt(column);
            break;
        case ChainType::ChainTypeVertical:
            chainSet = createVerticalChainAt(row);
            break;
        case ChainType::ChainTypeX:
            chainSet = createXChainAt(column, row);
            break;
        case ChainType::ChainTypeAllOfOne:
            chainSet = createAllOfOneChain(column, row);
            break;
        default:
            break;
        }
        if (chainSet) {
            addChainsFromSetToSet(chainSet, set);
            mLevel->calculateScore(chainSet);
            mLevel->removeCookies(chainSet);
        }
    }
    return set;
}


//--------------------------------------------------------------------
cocos2d::Set * ChainController::detectHorizontalMatches()
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    for (int column = 0; column < NumColumns; column++) {
        for (int row = 0; row < NumRows - 2;) {

            auto cookie = mObjCtrl->cookieAt(column, row);
            // skip over any gaps in the level design.
            if (cookie != nullptr) {
                int matchType = cookie->getTypeAsInt();

                auto other1 = mObjCtrl->cookieAt(column, row + 1);
                auto other2 = mObjCtrl->cookieAt(column, row + 2);
                // check whether the next two columns have the same cookie type.
                if (other1 != nullptr && other2 != nullptr) {

                    if (other1->getTypeAsInt() == matchType
                        && other2->getTypeAsInt() == matchType) {
                        //  There is a chain of at least three cookies but potentially there are more. This steps through all the matching cookies 
                        // until it finds a cookie that breaks the chain or it reaches the end of the grid.
                        auto chain = ChainObj::createWithType(ChainType::ChainTypeHorizontal);
                        int newMatchType = -1;
                        do {
                            cookie = mObjCtrl->cookieAt(column, row);
                            newMatchType = cookie ? cookie->getTypeAsInt() : -1;
                            if (cookie != nullptr && newMatchType == matchType) {
                                chain->addCookie(cookie);
                                row += 1;
                            }
                        } while (row < NumColumns && newMatchType == matchType);

                        set->addObject(chain);
                        continue;
                    }
                }
            }
            //  If the next two cookies don’t match the current one or if there is an empty tile, 
            // then there is no chain, so you skip over the cookie.
            row += 1;
        }
    }
    return set;
}

//--------------------------------------------------------------------
cocos2d::Set * ChainController::detectVerticalMatches()
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns - 2; ) {

            auto cookie = mObjCtrl->cookieAt(column, row);
            // skip over any gaps in the level design.
            if (cookie != nullptr) {
                int matchType = cookie->getTypeAsInt();

                auto other1 = mObjCtrl->cookieAt(column + 1, row);
                auto other2 = mObjCtrl->cookieAt(column + 2, row);
                // check whether the next two columns have the same cookie type.
                if (other1 != nullptr && other2 != nullptr) {

                    if (other1->getTypeAsInt() == matchType
                        && other2->getTypeAsInt() == matchType) {
                        //  There is a chain of at least three cookies but potentially there are more. This steps through all the matching cookies 
                        // until it finds a cookie that breaks the chain or it reaches the end of the grid.
                        auto chain = ChainObj::createWithType(ChainType::ChainTypeVertical);
                        int newMatchType = -1;
                        do {
                            cookie = mObjCtrl->cookieAt(column, row);
                            newMatchType = cookie ? cookie->getTypeAsInt() : -1;
                            if (cookie != nullptr && newMatchType == matchType) {
                                chain->addCookie(cookie);
                                column += 1;
                            }
                        } while (column < NumColumns && newMatchType == matchType);

                        set->addObject(chain);
                        continue;
                    }
                }
            }
            //  If the next two cookies don’t match the current one or if there is an empty tile, 
            // then there is no chain, so you skip over the cookie.
            column += 1;
        }
    }
    return set;
}

//--------------------------------------------------------------------
cocos2d::Set * ChainController::detectDifficultMatches(cocos2d::Set * horizontal, cocos2d::Set * vertical)
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    ChainObj* chainL = nullptr;
    ChainObj* chainT = nullptr;

    for (auto horzIt = horizontal->begin(); horzIt != horizontal->end(); ) {
        auto horzChain = dynamic_cast<ChainObj*>(*horzIt);
        CC_ASSERT(horzChain);

        for (auto vertIt = vertical->begin(); vertIt != vertical->end(); ) {
            auto vertChain = dynamic_cast<ChainObj*>(*vertIt);
            CC_ASSERT(vertChain);

            chainL = detectLChainMatches(horzChain, vertChain);
            chainT = detectTChainMatches(horzChain, vertChain);

            if (chainL) set->addObject(chainL);
            if (chainT) set->addObject(chainT);

            if (chainL || chainT) {
                vertIt++;
                vertical->removeObject(vertChain);
                continue;
            }
            vertIt++;
        }

        if (chainL || chainT) {
            horzIt++;
            horizontal->removeObject(horzChain);
            chainL = nullptr;
            chainT = nullptr;
            continue;
        }
        horzIt++;
    }
    return set;
}

//--------------------------------------------------------------------
ChainObj * ChainController::detectLChainMatches(ChainObj * horzChain, ChainObj * vertChain)
//--------------------------------------------------------------------
{
    ChainObj* chain = nullptr;
    auto horzCookies = horzChain->getCookies();
    CC_ASSERT(horzCookies);
    auto vertCookies = vertChain->getCookies();
    CC_ASSERT(vertCookies);

    auto firstHorzCookie = dynamic_cast<CookieObj*>(horzCookies->getObjectAtIndex(0));
    auto lastHorzCookie = dynamic_cast<CookieObj*>(horzCookies->getLastObject());
    auto firstVertCookie = dynamic_cast<CookieObj*>(vertCookies->getObjectAtIndex(0));
    auto lastVertCookie = dynamic_cast<CookieObj*>(vertCookies->getLastObject());

    if (!firstHorzCookie || !lastHorzCookie || !firstVertCookie || !lastVertCookie) {
        return chain;
    }
    if (firstHorzCookie->getTypeAsInt() != firstVertCookie->getTypeAsInt()) {
        return chain;
    }

    if (firstHorzCookie == firstVertCookie || firstHorzCookie == lastVertCookie ||
        lastHorzCookie == firstVertCookie || lastHorzCookie == lastVertCookie) {
        chain = ChainObj::createWithType(ChainType::ChainTypeL);
        chain->addCookiesFromChain(horzChain);
        chain->addCookiesFromChain(vertChain);
    }
    return chain;
}

//--------------------------------------------------------------------
ChainObj * ChainController::detectTChainMatches(ChainObj * horzChain, ChainObj * vertChain)
//--------------------------------------------------------------------
{
    ChainObj* chain = nullptr;
    auto horzCookies = horzChain->getCookies();
    CC_ASSERT(horzCookies);
    auto vertCookies = vertChain->getCookies();
    CC_ASSERT(vertCookies);

    auto firstHorzCookie = dynamic_cast<CookieObj*>(horzCookies->getObjectAtIndex(0));
    auto lastHorzCookie = dynamic_cast<CookieObj*>(horzCookies->getLastObject());
    auto firstVertCookie = dynamic_cast<CookieObj*>(vertCookies->getObjectAtIndex(0));
    auto lastVertCookie = dynamic_cast<CookieObj*>(vertCookies->getLastObject());

    if (!firstHorzCookie || !lastHorzCookie || !firstVertCookie || !lastVertCookie) {
        return chain;
    }
    if (firstHorzCookie->getTypeAsInt() != firstVertCookie->getTypeAsInt()) {
        return chain;
    }

    int horzMiddlePos = firstHorzCookie->getColumn() + horzCookies->count() / 2;
    int vertMiddlePos = firstVertCookie->getRow() + vertCookies->count() / 2;
    auto middleHorzCookie = mObjCtrl->cookieAt(horzMiddlePos, firstHorzCookie->getRow());
    auto middleVertCookie = mObjCtrl->cookieAt(firstVertCookie->getColumn(), vertMiddlePos);

    if (middleHorzCookie == firstVertCookie || middleHorzCookie == lastVertCookie ||
        middleVertCookie == firstHorzCookie || middleVertCookie == lastHorzCookie) {
        chain = ChainObj::createWithType(ChainType::ChainTypeT);
        chain->addCookiesFromChain(horzChain);
        chain->addCookiesFromChain(vertChain);
    }
    return chain;
}

//--------------------------------------------------------------------
void ChainController::addChainsFromSetToSet(cocos2d::Set * from, cocos2d::Set * to)
//--------------------------------------------------------------------
{
    if (!to) {
        to = cocos2d::Set::create();
    }
    for (auto it = from->begin(); it != from->end(); it++) {
        auto chain = dynamic_cast<ChainObj*>(*it);
        CC_ASSERT(chain);
        to->addObject(chain);
    }
}

//--------------------------------------------------------------------
cocos2d::Set* ChainController::createHorizontalChainAt(int column)
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    auto chain = ChainObj::createWithType(ChainType::ChainTypeHorizontal);
    for (int row = 0; row < NumRows; row++) {
        auto cookie = mObjCtrl->cookieAt(row, column);
        // skip over any gaps in the level design.
        if (cookie != nullptr) {
            chain->addCookie(cookie);
        }
    }
    set->addObject(chain);
    return set;
}

//--------------------------------------------------------------------
cocos2d::Set* ChainController::createVerticalChainAt(int row)
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    auto chain = ChainObj::createWithType(ChainType::ChainTypeVertical);
    for (int column = 0; column < NumColumns; column++) {
        auto cookie = mObjCtrl->cookieAt(row, column);
        // skip over any gaps in the level design.
        if (cookie != nullptr) {
            chain->addCookie(cookie);
        }
    }
    set->addObject(chain);
    return set;
}

//--------------------------------------------------------------------
cocos2d::Set* ChainController::createXChainAt(int column, int row)
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    auto chain = ChainObj::createWithType(ChainType::ChainTypeX);
    for (int i = 0; i < NumColumns; i++) {
        auto cookieA = mObjCtrl->cookieAt(i, row);
        auto cookieB = mObjCtrl->cookieAt(column, i);
        // skip over any gaps in the level design.
        if (cookieA != nullptr) {
            chain->addCookie(cookieA);
        }
        if (cookieB != nullptr) {
            chain->addCookie(cookieB);
        }
    }
    set->addObject(chain);
    return set;
}

//--------------------------------------------------------------------
cocos2d::Set* ChainController::createAllOfOneChain(int entryColumn, int entryRow)
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    auto entryCookie = mObjCtrl->cookieAt(entryColumn, entryRow);
    CC_ASSERT(entryCookie);

    auto chain = ChainObj::createWithType(ChainType::ChainTypeAllOfOne);
    for (int column = 0; column < NumColumns; column++) {
        for (int row = NumRows - 1; row >= 0; row--) {
            auto cookie = mObjCtrl->cookieAt(row, column);
            // skip over any gaps in the level design.
            if (!cookie)
                continue;

            if (cookie->getCookieType() == entryCookie->getCookieType()) {
                chain->addCookie(cookie);
            }
        }
    }
    set->addObject(chain);
    return set;
}

//--------------------------------------------------------------------
cocos2d::Set * ChainController::createChainFromPosToPos(cocos2d::Vec2 from, cocos2d::Vec2 to)
//--------------------------------------------------------------------
{
    int fromCol = -1; int fromRow = -1;
    int toCol = -1; int toRow = -1;
    auto set = cocos2d::Set::create();
    if (!Helper::convertPointToTilePos(from, fromCol, fromRow)) {
        return set;
    }
    if (!Helper::convertPointToTilePos(to, toCol, toRow)) {
        return set;
    }

    return createChainFromPosToPos(fromCol, fromRow, toCol, toRow);
}

//--------------------------------------------------------------------
cocos2d::Set * ChainController::createChainFromPosToPos(int fromCol, int fromRow, int toCol, int toRow)
//--------------------------------------------------------------------
{
    auto set = cocos2d::Set::create();
    auto chain = ChainObj::createWithType(ChainType::ChainFromAToB);
    set->addObject(chain);

    if (!(toCol >= 0 && toCol < NumColumns) || !(toRow >= 0 && toRow < NumColumns)) {
        cocos2d::log("ChainController::createChainFromPosToPos: wrong destinationPos at column=%d, row=%d", toCol, toRow);
        return set;
    }
 
    int i = fromCol; 
    int j = fromRow;
    
    do {
        if (fromCol != toCol) {
            i = fromCol > toCol ? i - 1 : i + 1;
        }
        do {
            if (fromRow != toRow) {
                j = fromRow > toRow ? j - 1 : j + 1;
            }
            if (mObjCtrl->cookieAt(i, j)) {
                chain->addCookie(mObjCtrl->cookieAt(i, j));
            }                  
        } while (j != toRow);
    } while (i != toCol);
    
    return set;
}


#ifdef COCOS2D_DEBUG
//--------------------------------------------------------------------
void ChainController::logDebugChains(cocos2d::Set * horizontal, cocos2d::Set * vertical, cocos2d::Set * difficult)
//--------------------------------------------------------------------
{
    auto strHorizontalChains = cocos2d::String::createWithFormat("Horizontal matches: {\n");
    auto horzIt = horizontal->begin();
    for (horzIt; horzIt != horizontal->end(); horzIt++) {
        auto chain = dynamic_cast<ChainObj*>(*horzIt);
        CC_ASSERT(chain);
        strHorizontalChains->appendWithFormat("%s\n", chain->description().c_str());
    }
    strHorizontalChains->append("}");

    auto strVerticalChains = cocos2d::String::createWithFormat("Vertical matches: {\n");
    auto vertIt = vertical->begin();
    for (vertIt; vertIt != vertical->end(); vertIt++) {
        auto chain = dynamic_cast<ChainObj*>(*vertIt);
        CC_ASSERT(chain);
        strVerticalChains->appendWithFormat("%s\n", chain->description().c_str());
    }
    strVerticalChains->append("}");

    auto strtypeLChains = cocos2d::String::createWithFormat("type L matches: {\n");
    auto strtypeTChains = cocos2d::String::createWithFormat("type T matches: {\n");
    auto lIt = difficult->begin();
    for (lIt; lIt != difficult->end(); lIt++) {
        auto chain = dynamic_cast<ChainObj*>(*lIt);
        CC_ASSERT(chain);
        if (chain->getType() == ChainType::ChainTypeL) {
            strtypeLChains->appendWithFormat("%s\n", chain->description().c_str());
        }
        else if (chain->getType() == ChainType::ChainTypeT) {
            strtypeTChains->appendWithFormat("%s\n", chain->description().c_str());
        }
    }
    strtypeLChains->append("}");
    strtypeTChains->append("}");

    cocos2d::log("ChainController::logDebugChains: %s", strHorizontalChains->getCString());
    cocos2d::log("ChainController::logDebugChains: %s", strVerticalChains->getCString());
    cocos2d::log("ChainController::logDebugChains: %s", strtypeLChains->getCString());
    cocos2d::log("ChainController::logDebugChains: %s", strtypeTChains->getCString());
}
#endif // #ifdef COCOS2D_DEBUG