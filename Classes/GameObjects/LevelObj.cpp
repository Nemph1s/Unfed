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

#include "Utils/Helpers/Helper.h"
#include "Utils/JsonParser.h"

//--------------------------------------------------------------------
LevelObj::LevelObj()
    : mPossibleSwaps(nullptr)
//--------------------------------------------------------------------
{
   CCLOGINFO("LevelObj::LevelObj");
}

//--------------------------------------------------------------------
LevelObj::~LevelObj()
//--------------------------------------------------------------------
{
    CCLOGINFO("LevelObj::~LevelObj: deallocing CookieObj: %p - tag: %i", this, _tag);
}

//--------------------------------------------------------------------
LevelObj* LevelObj::createWithId(const int16_t& levelId)
//--------------------------------------------------------------------
{
   LevelObj* ret = new (std::nothrow) LevelObj();
   if (ret && ret->initWithId(levelId)) {
      ret->retain();
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
      CCLOGERROR("LevelObj::initWithId: can't init Node inctance");
      return false;
   }

   JsonParser::Instance().parseLevelInfo(levelId);
   if (!JsonParser::Instance().checkStatus()) {
	   CCLOGERROR("LevelObj::initWithId: can't parse json file");
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
   CCLOGINFO("LevelObj::shuffle:");
   cocos2d::Set* set = nullptr;
   do {
       set = createInitialCookies();
       detectPossibleSwaps();

       CCLOGINFO("LevelObj::shuffle: possible swaps - %d", mPossibleSwaps->count());
   } while (mPossibleSwaps->count() == 0);
   return set;
}

//--------------------------------------------------------------------
TileObj* LevelObj::tileAt(int column, int row)
//--------------------------------------------------------------------
{
   bool invalidColumn = column >= 0 && column < NumColumns;
   bool invalidRow = row >= 0 && row < NumColumns;
   if (!invalidColumn) {
      CCLOGERROR("LevelObj::tileAt: Invalid column : %d", column);
      CC_ASSERT(invalidColumn);
   }
   if (!invalidRow) {
      CCLOGERROR("LevelObj::tileAt: Invalid row: %d", row);
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
      CCLOGERROR("LevelObj::cookieAt: Invalid column : %d", column);
      CC_ASSERT(invalidColumn);
   }
   if (!invalidRow) {
      CCLOGERROR("LevelObj::cookieAt: Invalid row: %d", row);
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
    CCLOGINFO("LevelObj::performSwap: %s", swap->description().c_str());
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
cocos2d::Set* LevelObj::createInitialCookies()
//--------------------------------------------------------------------
{
    CCLOGINFO("LevelObj::createInitialCookies:");
    cocos2d::Set* set = new cocos2d::Set();

    for (int column = 0; column < NumColumns; column++) {
        for (int row = 0; row < NumRows; row++) {
            if (mTiles[column][row] != nullptr) {
                int cookieType = getRandomCookieType(column, row);
                CookieObj* cookie = createCookie(column, row, cookieType);
                set->addObject(cookie);
            }
        }
    }
    CCLOGINFO("LevelObj::createInitialCookies: set.size=", set->count);
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
    int cookieMax = Helper::Instance().to_underlying(CommonTypes::CookieType::CookieMax);

    int type = 0;
    bool findNextType = false;
    do {
        type = Helper::Instance().random(0, cookieMax - 1);

        findNextType = ((column >= 2 && // there are already two cookies of this type to the left
            isSameTypeOfCookieAt(column - 1, row, type) &&
            isSameTypeOfCookieAt(column - 2, row, type)) ||
            (row >= 2 && // or there are already two cookies of this type below
                isSameTypeOfCookieAt(column, row - 1 , type) &&
                isSameTypeOfCookieAt(column, row - 2, type)));
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

    return true;    
}

//--------------------------------------------------------------------
void LevelObj::detectPossibleSwaps()
//--------------------------------------------------------------------
{
    CCLOGINFO("LevelObj::detectPossibleSwaps:");
    cocos2d::Set* set = new cocos2d::Set();

    for (int column = 0; column < NumColumns; column++) {
        for (int row = 0; row < NumRows; row++) {
            auto cookie = cookieAt(row, column);
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
    CCLOGINFO("LevelObj::createInitialCookies: set.size=", set->count);
    mPossibleSwaps = set;
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
    if (cookieAt(column, row))
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
