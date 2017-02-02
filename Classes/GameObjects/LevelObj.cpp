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
#include "GameObjects/SwapObj.h"
#include "GameObjects/CookieObj.h"
#include "GameObjects/ChainObj.h"

#include "Utils/Helpers/Helper.h"
#include "Utils/JsonParser.h"

//--------------------------------------------------------------------
LevelObj::LevelObj()
    : mPossibleSwaps(nullptr)
//--------------------------------------------------------------------
{
   cocos2d::log("LevelObj::LevelObj");
}

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

    JsonParser::Instance().parseLevelInfo(levelId);
    if (!JsonParser::Instance().checkStatus()) {
        cocos2d::log("LevelObj::initWithId: can't parse json file");
        return false;
    }

    mLevelInfo = JsonParser::Instance().getLevelInfo();

    for (int i = 0; i < NumColumns; i++) {
        for (int j = 0; j < NumRows; j++) {

            if (mLevelInfo.tiles[i][j] == 1) {
                mTiles[i][j] = new TileObj();
            }
        }
    }

    return true;
}

//--------------------------------------------------------------------
cocos2d::Set* LevelObj::shuffle()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::shuffle:");
    cocos2d::Set* set = createInitialCookies();
    detectPossibleSwaps();

    while (mPossibleSwaps->count() == 0) {
        set = createInitialCookies();
        detectPossibleSwaps();
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
    return mTiles[column][row];
}

//--------------------------------------------------------------------
CookieObj* LevelObj::cookieAt(int column, int row)
//--------------------------------------------------------------------
{
    bool invalidColumn = column >= 0 && column < NumColumns;
    bool invalidRow = row >= 0 && row < NumColumns;
    if (!invalidColumn) {
        cocos2d::log("LevelObj::cookieAt: Invalid column : %d", column);
        CC_ASSERT(invalidColumn);
    }
    if (!invalidRow) {
        cocos2d::log("LevelObj::cookieAt: Invalid row: %d", row);
        CC_ASSERT(invalidRow);
    }
    return mCookies[column][row];
}

//--------------------------------------------------------------------
void LevelObj::performSwap(SwapObj * swap)
//--------------------------------------------------------------------
{
    if (!swap) {
        return;
    }
    cocos2d::log("LevelObj::performSwap: %s", swap->description().c_str());
    int columnA = swap->getCookieA()->getColumn();
    int rowA = swap->getCookieA()->getRow();
    int columnB = swap->getCookieB()->getColumn();
    int rowB = swap->getCookieB()->getRow();

    mCookies[columnA][rowA] = swap->getCookieB();
    swap->getCookieB()->setColumn(columnA);
    swap->getCookieB()->setRow(rowA);

    mCookies[columnB][rowB] = swap->getCookieA();
    swap->getCookieA()->setColumn(columnB);
    swap->getCookieA()->setRow(rowB);
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
cocos2d::Set * LevelObj::removeMatches()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::removeMatches:");
    auto horizontalChains = detectHorizontalMatches();
    auto verticalChains = detectVerticalMatches();
    auto set = cocos2d::Set::create();

    auto strHorizontalChains = cocos2d::String::createWithFormat("Horizontal matches: {\n");
    auto horzIt = horizontalChains->begin();
    for (horzIt; horzIt != horizontalChains->end(); horzIt++) {
        auto chain = dynamic_cast<ChainObj*>(*horzIt);
        if (!chain) {
            cocos2d::log("LevelObj::removeMatches: can't cast Ref* to ChainObj*");
            CC_ASSERT(chain);
            continue;
        }
        strHorizontalChains->appendWithFormat("%s\n", chain->description().c_str());
        set->addObject(chain);
    }
    strHorizontalChains->append("}");

    auto strVerticalChains = cocos2d::String::createWithFormat("Vertical matches: {\n");
    auto vertIt = verticalChains->begin();
    for (vertIt; vertIt != verticalChains->end(); vertIt++) {
        auto chain = dynamic_cast<ChainObj*>(*vertIt);
        if (!chain) {
            cocos2d::log("LevelObj::removeMatches: can't cast Ref* to ChainObj*");
            CC_ASSERT(chain);
            continue;
        }
        strVerticalChains->appendWithFormat("%s\n", chain->description().c_str());
        set->addObject(chain);
    }
    strVerticalChains->append("}");

    cocos2d::log("LevelObj::removeMatches: %s", strHorizontalChains->getCString());
    cocos2d::log("LevelObj::removeMatches: %s", strVerticalChains->getCString());

    return set;
}

//--------------------------------------------------------------------
cocos2d::Set* LevelObj::createInitialCookies()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::createInitialCookies:");
    cocos2d::Set* set = new cocos2d::Set();
    auto createdString = cocos2d::String("");
    
    for (int column = 0; column < NumColumns; column++) {
        for (int row = 0; row < NumRows; row++) {
            if (mTiles[column][row] != nullptr) {
                int cookieType = getRandomCookieType(column, row);
                CookieObj* cookie = createCookie(column, row, cookieType);
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
CookieObj * LevelObj::createCookie(int column, int row, int type)
//--------------------------------------------------------------------
{
   CookieInfo info = { column, row, static_cast<CookieType>(type) };
   CookieObj* cookie = CookieObj::create(info);
   mCookies[column][row] = cookie;
   return cookie;
}

//--------------------------------------------------------------------
int LevelObj::getRandomCookieType(int column, int row)
//--------------------------------------------------------------------
{
    int cookieMax = Helper::getInstance()->to_underlying(CommonTypes::CookieType::CookieMax);

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
bool LevelObj::isSameTypeOfCookieAt(int column, int row, int type)
//--------------------------------------------------------------------
{
    auto cookie = cookieAt(column, row);
    if (!cookie)
        return false;
    
    if (cookie->getTypeAsInt() != type)
        return false;
    
    cocos2d::log("LevelObj::isSameTypeOfCookieAt: current = %s; randomType = %d;"
        , cookie->description().c_str(), type);
    return true;    
}

//--------------------------------------------------------------------
void LevelObj::detectPossibleSwaps()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::detectPossibleSwaps:");
    cocos2d::Set* set = new cocos2d::Set();

    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns; column++) {
            auto cookie = cookieAt(column, row);
            if (cookie != nullptr) {
                // Is it possible to swap this cookie with the one on the right?
                if (column < NumColumns - 1) {
                    // Have a cookie in this spot? If there is no tile, there is no cookie.
                    auto other = cookieAt(column + 1, row);
                    if (other != nullptr) {
                        // Swap them
                        mCookies[column][row] = other;
                        mCookies[column + 1][row] = cookie;

                        // Is either cookie now part of a chain?
                        if (hasChainAt(column + 1, row) || hasChainAt(column, row)) {

                            SwapObj *swap = SwapObj::createWithCookies(cookie, other);
                            set->addObject(swap);
                        }
                        // Swap them back
                        mCookies[column][row] = cookie;
                        mCookies[column + 1][row] = other;
                    }
                }
                // This does exactly the same thing, but for the cookie above instead of on the right.
                if (row < NumRows - 1) {

                    auto other = cookieAt(column, row + 1);
                    if (other != nullptr) {
                        // Swap them
                        mCookies[column][row] = other;
                        mCookies[column][row + 1] = cookie;

                        if (hasChainAt(column, row + 1) || hasChainAt(column, row)) {

                            SwapObj *swap = SwapObj::createWithCookies(cookie, other);
                            set->addObject(swap);
                        }

                        mCookies[column][row] = cookie;
                        mCookies[column][row + 1] = other;
                    }
                }
            }
        }
    }
    mPossibleSwaps = set;
    int count = 0;
    auto strSwaps = cocos2d::String::create("possible swaps: {\n");
    for (auto it = mPossibleSwaps->begin(); it != mPossibleSwaps->end(); ++it, count++) {
        auto swap = static_cast<SwapObj*>(*it);
        strSwaps->appendWithFormat("\t%s\n", swap->description().c_str());
    }
    strSwaps->append("}");
    cocos2d::log("LevelObj::detectPossibleSwaps: count = %d\n%s", count, strSwaps->getCString());
}

//--------------------------------------------------------------------
bool LevelObj::isPossibleSwap(SwapObj* swap)
//--------------------------------------------------------------------
{
    auto it = mPossibleSwaps->begin();
    for (; it != mPossibleSwaps->end(); ++it) {
        auto other = static_cast<SwapObj*>(*it);
        if (!other)
            continue;
        if ((other->getCookieA() == swap->getCookieA() && other->getCookieB() == swap->getCookieB()) ||
            (other->getCookieB() == swap->getCookieA() && other->getCookieA() == swap->getCookieB()))
            return true;
    }
    return false;
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
