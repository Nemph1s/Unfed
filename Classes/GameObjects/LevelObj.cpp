/**
* @file GameObjects/LevelObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/LevelObj.h"
#include "GameObjects/TileObjects/TileObj.h"
#include "GameObjects/TileObjects/Base/BaseObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/Chain/ChainObj.h"
#include "GameObjects/Swap/SwapObj.h"

#include "Utils/Helpers/Helper.h"
#include "Utils/Parser/JsonParser.h"

#include "Common/Factory/SmartFactory.h"
#include "Controller/ObjectController.h"

using namespace CommonTypes;

//--------------------------------------------------------------------
LevelObj::LevelObj()
    : mObjCtrl(nullptr)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
LevelObj::~LevelObj()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::~LevelObj: deallocing CookieObj: %p - tag: %i", this, _tag);
    CC_SAFE_RELEASE_NULL(mObjCtrl);
}

//--------------------------------------------------------------------
LevelObj* LevelObj::createWithId(const int16_t& levelId)
//--------------------------------------------------------------------
{
    LevelObj* ret = new (std::nothrow) LevelObj();
    if (ret && ret->initWithId(levelId)) {
        CC_SAFE_RETAIN(ret);
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool LevelObj::initWithId(const int16_t& levelId)
//--------------------------------------------------------------------
{
    if (!Node::init()) {
        cocos2d::log("LevelObj::initWithId: can't init Node inctance");
        return false;
    }

    JsonParser->parseLevelInfo(levelId);
    if (!JsonParser->checkStatus()) {
        cocos2d::log("LevelObj::initWithId: can't parse json file");
        return false;
    }
    mLevelInfo = JsonParser->getLevelInfo();

    initObjectController();

    return true;
}

//--------------------------------------------------------------------
void LevelObj::initObjectController()
//--------------------------------------------------------------------
{
    mObjCtrl = ObjectController::create();
    mObjCtrl->setLevel(this);

    mObjCtrl->createInitialTiles();
}

//--------------------------------------------------------------------
cocos2d::Set* LevelObj::shuffle()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::shuffle:");
    auto set = mObjCtrl->createInitialCookies();
    bool isSwapsDetected = mDetectPossibleSwapsCallback();

    while (!isSwapsDetected) {
        set = mObjCtrl->createInitialCookies();
        isSwapsDetected = mDetectPossibleSwapsCallback();
    }

    return set;
}

//--------------------------------------------------------------------
cocos2d::Set * LevelObj::removeMatches()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::removeMatches:");
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

    calculateScore(horizontalChains);
    removeCookies(horizontalChains);

    calculateScore(verticalChains);
    removeCookies(verticalChains);

    calculateScore(difficultChains);
    removeCookies(difficultChains);

    return set;
}

//--------------------------------------------------------------------
cocos2d::Set * LevelObj::detectHorizontalMatches()
//--------------------------------------------------------------------
{
    auto set = new cocos2d::Set();
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
cocos2d::Set * LevelObj::detectVerticalMatches()
//--------------------------------------------------------------------
{
    auto set = new cocos2d::Set();
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
cocos2d::Set * LevelObj::detectDifficultMatches(cocos2d::Set * horizontal, cocos2d::Set * vertical)
//--------------------------------------------------------------------
{
    auto set = new cocos2d::Set();
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
ChainObj * LevelObj::detectLChainMatches(ChainObj * horzChain, ChainObj * vertChain)
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
ChainObj * LevelObj::detectTChainMatches(ChainObj * horzChain, ChainObj * vertChain)
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
void LevelObj::removeCookies(cocos2d::Set * chains)
//--------------------------------------------------------------------
{
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);

        auto cookies = chain->getCookies();
        for (auto it = cookies->begin(); it != cookies->end(); it++) {
            auto cookie = dynamic_cast<CookieObj*>(*it);
            CC_ASSERT(cookie);

            mObjCtrl->removeCookie(cookie->getColumn(), cookie->getRow());
        }
    }
}

//--------------------------------------------------------------------
cocos2d::Array* LevelObj::useGravityToFillHoles()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::useGravityToFillHoles:");
    auto columns = cocos2d::Array::createWithCapacity(NumColumns);
    // loop through the rows, from bottom to top
    for (int column = 0; column < NumColumns; column++) {

        cocos2d::Array* array = nullptr;
        for (int row = NumRows - 1; row >= 0; row--) {

            // If there’s a tile at a position but no cookie, then there’s a hole.
            auto isEmptyTile = mObjCtrl->isEmptyTileAt(column, row);
            auto isCookieAt = mObjCtrl->cookieAt(column, row);
            if (!isEmptyTile && isCookieAt == nullptr) {
            
                // Scan upward to find the cookie that sits directly above the hole
                for (int lookup = row - 1; lookup >= 0; lookup--) {
                    auto cookie = mObjCtrl->cookieAt(column, lookup);
                    if (cookie == nullptr)
                        continue;
                    
                    if (!cookie->getIsMovable())
                        continue;

                    // If find another cookie, move that cookie to the hole. This effectively moves the cookie down.
                    mObjCtrl->updateCookieObjectAt(column, lookup, nullptr);
                    mObjCtrl->updateCookieObjectAt(column, row, cookie);
                    cookie->setRow(row);

                    // Lazy creation of array
                    if (array == nullptr) {
                        array = cocos2d::Array::createWithCapacity(NumRows);
                        columns->addObject(array);
                    }
                    array->addObject(cookie);

                    // Once you’ve found a cookie, you don’t need to scan up any farther so you break out of the inner loop.
                    break;
                }
            }
        }
    }
    return columns;
}

//--------------------------------------------------------------------
cocos2d::Array * LevelObj::fillTopUpHoles()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::fillTopUpHoles:");
    auto columns = cocos2d::Array::createWithCapacity(NumColumns);
    auto createdString = cocos2d::String("");
    int cookieType = -1;
    // loop through the rows, from top to bottom
    for (int row = 0; row < NumRows; row++) {

        cocos2d::Array* array = nullptr;
        for (int column = 0; column < NumColumns; column++) {

            // If there’s a tile at a position but no cookie, then there’s a hole.
            auto isEmptyTile = mObjCtrl->isEmptyTileAt(column, row);
            auto isCookieAt = mObjCtrl->cookieAt(column, row);
            if (!isEmptyTile && isCookieAt == nullptr) {

                BaseObj* cookie = mObjCtrl->createRandomCookie(column, row);

                if (array == nullptr) {
                    array = cocos2d::Array::createWithCapacity(NumRows);
                    columns->addObject(array);
                }
                array->addObject(cookie);
                createdString.appendWithFormat("\t%s\n", cookie->description());
            }
        }
        createdString.append("}\n");
    }
    cocos2d::log("LevelObj::fillTopUpHoles:  new created cookies: {\n%s", createdString.getCString());
    return columns;    
}

//--------------------------------------------------------------------
void LevelObj::calculateScore(cocos2d::Set * chains)
//--------------------------------------------------------------------
{
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);
        auto cookies = chain->getCookies();

        //TODO: move to other location
        int chainValue = 60;
        switch (chain->getType())
        {
        case ChainType::ChainTypeL:
            chainValue = 70;
            break;
        case ChainType::ChainTypeT:
            chainValue = 80;
        default:
            break;
        }

        //TODO: make more intelligent way to get score value
        chain->setScore(chainValue * (cookies->count() - 2) * mComboMultiplier);
        mComboMultiplier++;
    }
}

//--------------------------------------------------------------------
void LevelObj::resetComboMultiplier()
//--------------------------------------------------------------------
{
    mComboMultiplier = 1;
}

//--------------------------------------------------------------------
void LevelObj::addChainsFromSetToSet(cocos2d::Set * from, cocos2d::Set * to)
//--------------------------------------------------------------------
{
    for (auto it = from->begin(); it != from->end(); it++) {
        auto chain = dynamic_cast<ChainObj*>(*it);
        CC_ASSERT(chain);
        to->addObject(chain);
    }
}

#ifdef COCOS2D_DEBUG
//--------------------------------------------------------------------
void LevelObj::logDebugChains(cocos2d::Set * horizontal, cocos2d::Set * vertical, cocos2d::Set * difficult)
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
        } else if (chain->getType() == ChainType::ChainTypeT) {
            strtypeTChains->appendWithFormat("%s\n", chain->description().c_str());
        }
    }
    strtypeLChains->append("}");
    strtypeTChains->append("}");
    
    cocos2d::log("LevelObj::logDebugChains: %s", strHorizontalChains->getCString());
    cocos2d::log("LevelObj::logDebugChains: %s", strVerticalChains->getCString());
    cocos2d::log("LevelObj::logDebugChains: %s", strtypeLChains->getCString());
    cocos2d::log("LevelObj::logDebugChains: %s", strtypeTChains->getCString());
}
#endif // #ifdef COCOS2D_DEBUG