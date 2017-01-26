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
#include "GameObjects/CookieObj.h"
#include "Utils/Helpers/Helper.h"

#include <random>

using namespace CommonTypes;

//--------------------------------------------------------------------
LevelObj::LevelObj()
   : mNumColumns(0)
   , mNumRows(0)
//--------------------------------------------------------------------
{
   CCLOGINFO("LevelObj::LevelObj");
}

//--------------------------------------------------------------------
LevelObj* LevelObj::create()
//--------------------------------------------------------------------
{
   LevelObj* ret = new (std::nothrow) LevelObj();
   if (ret && ret->init()) {
      ret->autorelease();
   }
   else {
      CC_SAFE_DELETE(ret);
   }
   return ret;
}

//--------------------------------------------------------------------
LevelObj::~LevelObj()
//--------------------------------------------------------------------
{
   CCLOGINFO("LevelObj::~LevelObj: deallocing CookieObj: %p - tag: %i", this, _tag);
}

//--------------------------------------------------------------------
bool LevelObj::init()
//--------------------------------------------------------------------
{
   if (!Node::init()) {
      CCLOGERROR("LevelObj::init: can't init Node inctance");
      return false;
   }

   mNumRows = NumRows;
   mNumColumns = NumColumns;

   CCLOGINFO("LevelObj::init: mNumColumns=%d mNumRows=%d", mNumColumns, mNumRows);

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
CookieObj* LevelObj::cookieAt(int8_t column, int8_t row)
//--------------------------------------------------------------------
{
   bool invalidColumn = column >= 0 && column < mNumColumns;
   bool invalidRow = row >= 0 && row < mNumRows;
   if (invalidColumn) {
      CCLOGERROR("LevelObj::cookieAt: Invalid column : %d", column);
      CC_ASSERT(invalidColumn);
   }
   if (invalidRow) {
      CCLOGERROR("LevelObj::cookieAt: Invalid row: %d", row);
      CC_ASSERT(invalidRow);
   }
    return mCookies[column][row];
}

//--------------------------------------------------------------------
cocos2d::Set* LevelObj::createInitialCookies()
//--------------------------------------------------------------------
{
   CCLOGINFO("LevelObj::createInitialCookies:");
   cocos2d::Set* set = new cocos2d::Set();

   for (int8_t row = 0; row < mNumRows; row++) {
      for (int8_t column = 0; column < mNumColumns; column++) {

         CookieObj* cookie = createCookie(column, row, getRandomCookieType());
         set->addObject(cookie);
      }
   }
   CCLOGINFO("LevelObj::createInitialCookies: set.size=", set->count);
   return set;
}

//--------------------------------------------------------------------
CookieObj * LevelObj::createCookie(int8_t column, int8_t row, int8_t type)
//--------------------------------------------------------------------
{
   CookieInfo info = { column, row, static_cast<CookieType>(type) };
   CookieObj* cookie = CookieObj::create(info);
   mCookies[column][row] = cookie;
   return cookie;
}

//--------------------------------------------------------------------
int8_t LevelObj::getRandomCookieType()
//--------------------------------------------------------------------
{
   //TODO: move to helper
   auto cookieMax = Helper::to_underlying(CookieType::CookieMax);

   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_int_distribution<> cookieType(0, cookieMax - 1);

   return cookieType(gen);
}