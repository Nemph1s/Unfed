/**
* @file GameObjects/LevelObj.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"
#include "Common/CommonTypes.h"

using namespace CommonTypes;

class TileObj;
class CookieObj;

class LevelObj : public cocos2d::Node
{
public:
   /**
   * Allocates and initializes a node.
   * @return A initialized node which is marked as "autorelease".
   */
   static LevelObj * createWithId(const int16_t& levelId);

CC_CONSTRUCTOR_ACCESS:
   // Nodes should be created using create();
   LevelObj();
   virtual ~LevelObj();

   virtual bool initWithId(const int16_t& levelId);

   cocos2d::Set* shuffle();
   TileObj* tileAt(int8_t column, int8_t row);
   CookieObj* cookieAt(int8_t column, int8_t row);
protected:

   cocos2d::Set* createInitialCookies();
   CookieObj* createCookie(int8_t column, int8_t row, int8_t type);
   int8_t getRandomCookieType();

   CC_SYNTHESIZE_READONLY(LevelInfo, mLevelInfo, LevelInfo);

   TileObj* mTiles[NumColumns][NumRows] = { nullptr };
   CookieObj* mCookies[NumColumns][NumRows] = { nullptr };
};
