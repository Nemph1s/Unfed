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

#include <random>

//--------------------------------------------------------------------
LevelObj::LevelObj()
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
   return createInitialCookies();
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

    for (int i = 0; i < NumRows; i++) {
        for (int j = 0; j < NumColumns; j++) {
            if (mTiles[i][j] != nullptr) {
                CookieObj* cookie = createCookie(i, j, getRandomCookieType());
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
int LevelObj::getRandomCookieType()
//--------------------------------------------------------------------
{
   //TODO: move to helper
   auto cookieMax = Helper::to_underlying(CookieType::CookieMax);

   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_int_distribution<> cookieType(0, cookieMax - 1);
   return cookieType(gen);
}
