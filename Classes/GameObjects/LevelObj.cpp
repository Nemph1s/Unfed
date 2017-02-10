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
#include "GameObjects/TileObj.h"
#include "GameObjects/Base/BaseObj.h"
#include "GameObjects/CookieObj.h"
#include "GameObjects/ChainObj.h"
#include "GameObjects/Swap/SwapObj.h"

#include "Utils/Helpers/Helper.h"
#include "Utils/Parser/JsonParser.h"

#include "Common/Factory/SmartFactory.h"

using namespace CommonTypes;

//--------------------------------------------------------------------
LevelObj::~LevelObj()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::~LevelObj: deallocing CookieObj: %p - tag: %i", this, _tag);
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

    createInitialTiles();

    return true;
}

//--------------------------------------------------------------------
cocos2d::Set* LevelObj::shuffle()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::shuffle:");
    cocos2d::Set* set = createInitialCookies();
    bool isSwapsDetected = mDetectPossibleSwapsCallback();

    while (!isSwapsDetected) {
        set = createInitialCookies();
        isSwapsDetected = mDetectPossibleSwapsCallback();
    }

    return set;
}

//--------------------------------------------------------------------
TileObj* LevelObj::tileAt(int column, int row)
//--------------------------------------------------------------------
{
    bool invalidColumn = column >= 0 && column < NumColumns;
    bool invalidRow = row >= 0 && row < NumColumns;
    if (!invalidColumn) {
        cocos2d::log("LevelObj::tileAt: Invalid column : %d", column);
        CC_ASSERT(invalidColumn);
    }
    if (!invalidRow) {
        cocos2d::log("LevelObj::tileAt: Invalid row: %d", row);
        CC_ASSERT(invalidRow);
    }
    return dynamic_cast<TileObj*>(mTiles[column][row]);
}

//--------------------------------------------------------------------
CookieObj* LevelObj::cookieAt(int column, int row)
//--------------------------------------------------------------------
{
    bool invalidColumn = column >= 0 && column < NumColumns;
    bool invalidRow = row >= 0 && row < NumColumns;
    if (!invalidColumn || !invalidRow) {
        cocos2d::log("LevelObj::cookieAt: Invalid cookie at column = %d, row = %d", column, row);
        return nullptr;
    }
    return dynamic_cast<CookieObj*>(mCookies[column][row]);
}

//--------------------------------------------------------------------
cocos2d::Set * LevelObj::detectHorizontalMatches()
//--------------------------------------------------------------------
{
    cocos2d::Set* set = new cocos2d::Set();

    for (int column = 0; column < NumColumns; column++) {
        for (int row = 0; row < NumRows - 2;) {

            auto cookie = cookieAt(column, row);
            // skip over any gaps in the level design.
            if (cookie != nullptr) {
                int matchType = cookie->getTypeAsInt();

                auto other1 = cookieAt(column, row + 1);
                auto other2 = cookieAt(column, row + 2);
                // check whether the next two columns have the same cookie type.
                if (other1 != nullptr && other2 != nullptr) {

                    if (other1->getTypeAsInt() == matchType
                        && other2->getTypeAsInt() == matchType) {
                        //  There is a chain of at least three cookies but potentially there are more. This steps through all the matching cookies 
                        // until it finds a cookie that breaks the chain or it reaches the end of the grid.
                        ChainObj* chain = ChainObj::createWithType(CommonTypes::ChainType::ChainTypeHorizontal);
                        int newMatchType = -1;
                        do {
                            cookie = cookieAt(column, row);
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
    cocos2d::Set* set = new cocos2d::Set();

    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns - 2; ) {

            auto cookie = cookieAt(column, row);
            // skip over any gaps in the level design.
            if (cookie != nullptr) {
                int matchType = cookie->getTypeAsInt();

                auto other1 = cookieAt(column + 1, row);
                auto other2 = cookieAt(column + 2, row);
                // check whether the next two columns have the same cookie type.
                if (other1 != nullptr && other2 != nullptr) {

                    if (other1->getTypeAsInt() == matchType 
                        && other2->getTypeAsInt() == matchType) {
                        //  There is a chain of at least three cookies but potentially there are more. This steps through all the matching cookies 
                        // until it finds a cookie that breaks the chain or it reaches the end of the grid.
                        ChainObj* chain = ChainObj::createWithType(CommonTypes::ChainType::ChainTypeVertical);
                        int newMatchType = -1;
                        do {
                            cookie = cookieAt(column, row);
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
    cocos2d::Set* set = new cocos2d::Set();
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
        chain = ChainObj::createWithType(CommonTypes::ChainType::ChainTypeL);
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
    auto middleHorzCookie = cookieAt(horzMiddlePos, firstHorzCookie->getRow());
    auto middleVertCookie = cookieAt(firstVertCookie->getColumn(), vertMiddlePos);

    if (middleHorzCookie == firstVertCookie || middleHorzCookie == lastVertCookie ||
        middleVertCookie == firstHorzCookie || middleVertCookie == lastHorzCookie) {
        chain = ChainObj::createWithType(CommonTypes::ChainType::ChainTypeT);
        chain->addCookiesFromChain(horzChain);
        chain->addCookiesFromChain(vertChain);
    }
    return chain;
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
void LevelObj::removeCookies(cocos2d::Set * chains)
//--------------------------------------------------------------------
{
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        if (!chain) {
            cocos2d::log("LevelObj::removeCookies: can't cast Ref* to ChainObj*");
            CC_ASSERT(chain);
            continue;
        }
        auto cookies = chain->getCookies();
        for (auto it = cookies->begin(); it != cookies->end(); it++) {
            auto cookie = dynamic_cast<CookieObj*>(*it);
            if (!cookie) {
                cocos2d::log("LevelObj::removeCookies: can't cast Ref* to CookieObj*");
                CC_ASSERT(cookie);
                continue;
            }
            cocos2d::log("LevelObj::removeCookies: remove %s", cookie->description());
            cookie->removeFromParent();
            SmartFactory->recycle(cookie);
            mCookies[cookie->getColumn()][cookie->getRow()] = nullptr;
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
            if (!isEmptyTileAt(column, row) && cookieAt(column, row) == nullptr) {
            
                // Scan upward to find the cookie that sits directly above the hole
                for (int lookup = row - 1; lookup >= 0; lookup--) {
                    auto cookie = cookieAt(column, lookup);
                    if (cookie == nullptr) {
                        continue;
                    }
                    if (!cookie->getIsMovable()) {
                        continue;
                    }

                    // If find another cookie, move that cookie to the hole. This effectively moves the cookie down.
                    mCookies[column][lookup] = nullptr;
                    mCookies[column][row] = cookie;
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
            if (!isEmptyTileAt(column, row) && (cookieAt(column, row) == nullptr)) {

                int cookieType = getRandomCookieType(column, row);
                BaseObj* cookie = createCookie(column, row, cookieType);

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
void LevelObj::createInitialTiles()
//--------------------------------------------------------------------
{
    mLevelInfo = JsonParser->getLevelInfo();

    for (int column = 0; column < NumColumns; column++) {
        for (int row = 0; row < NumRows; row++) {

            int tileType = mLevelInfo.tiles[column][row];
            auto tile = createTile(column, row, tileType);
            this->addChild(tile);
        }
    }
}
   
//--------------------------------------------------------------------
BaseObj * LevelObj::createTile(int column, int row, int type)
//--------------------------------------------------------------------
{
    BaseObjectInfo baseInfo = { BaseObjectType::TileObj, column, row  };
    TileInfo info = { baseInfo, static_cast<TileType>(type) };
    BaseObj* tile = SmartFactory->createTileObj(info);
    mTiles[column][row] = tile;
    return tile;
}

//--------------------------------------------------------------------
bool LevelObj::isEmptyTileAt(int column, int row)
//--------------------------------------------------------------------
{
    return tileAt(column, row) ? tileAt(column, row)->isEmptyTile() : false;
}

//--------------------------------------------------------------------
cocos2d::Set* LevelObj::createInitialCookies()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::createInitialCookies:");
    cocos2d::Set* set = new cocos2d::Set();
    auto createdString = cocos2d::String("");
    
    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns; column++) {
            if (!isEmptyTileAt(column, row)) {
                int cookieType = getRandomCookieType(column, row);
                BaseObj* cookie = createCookie(column, row, cookieType);
                set->addObject(cookie);
                createdString.appendWithFormat("%d ", cookieType);
            }
        }
        createdString.append("\n");
    }
    cocos2d::log("LevelObj::createInitialCookies: created array=%s", createdString.getCString());
    return set;
}

//--------------------------------------------------------------------
BaseObj * LevelObj::createCookie(int column, int row, int type)
//--------------------------------------------------------------------
{
    BaseObjectInfo baseInfo = { BaseObjectType::CookieObj, column, row };
    CookieInfo info = { baseInfo, static_cast<CookieType>(type) };
    BaseObj* cookie = SmartFactory->createCookieObj(info);
    mCookies[column][row] = cookie;
    return cookie;
}

//--------------------------------------------------------------------
int LevelObj::getRandomCookieType(int column, int row)
//--------------------------------------------------------------------
{
    int cookieMax = Helper::getInstance()->to_underlying(CommonTypes::CookieType::CookieMax);
    if (mLevelInfo.typesCount < cookieMax) {
        cookieMax = mLevelInfo.typesCount;
    }
    int type = 0;
    bool findNextType = false;
    do {
        type = Helper::getInstance()->random(0, cookieMax - 1);
        auto isCookiesToTheLeft = (column >= 2 && // there are already two cookies of this type to the left
            isSameTypeOfCookieAt(column - 1, row, type) &&
            isSameTypeOfCookieAt(column - 2, row, type));
        auto isCookiesBelow = (row >= 2 && // or there are already two cookies of this type below
            isSameTypeOfCookieAt(column, row - 1, type) &&
            isSameTypeOfCookieAt(column, row - 2, type));
        findNextType = (isCookiesToTheLeft || isCookiesBelow);
    } while (findNextType);
    
   return type;
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

//--------------------------------------------------------------------
bool LevelObj::isSameTypeOfCookieAt(int column, int row, int type)
//--------------------------------------------------------------------
{
    auto cookie = cookieAt(column, row);
    if (!cookie)
        return false;
    
    if (cookie->getTypeAsInt() != type)
        return false;

    return true;    
}

//--------------------------------------------------------------------
bool LevelObj::hasChainAt(int column, int row)
//--------------------------------------------------------------------
{
    if (!cookieAt(column, row))
        return false;

    int type = cookieAt(column, row)->getTypeAsInt();
    int fieldSize = CommonTypes::NumColumns;

    int horzLength = 1;

    for (int i = column - 1; i >= 0 && isSameTypeOfCookieAt(i, row, type); i--, horzLength++);
    for (int i = column + 1; i < fieldSize && isSameTypeOfCookieAt(i, row, type); i++, horzLength++);
    if (horzLength >= 3) 
        return true;

    int vertLength = 1;

    for (int i = row - 1; i >= 0 && isSameTypeOfCookieAt(column, i, type); i--, vertLength++);
    for (int i = row + 1; i < fieldSize && isSameTypeOfCookieAt(column, i, type); i++, vertLength++);

    return (vertLength >= 3);
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