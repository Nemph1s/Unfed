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

class SwapObj;
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

   TileObj* tileAt(int column, int row);
   CookieObj* cookieAt(int column, int row);

   void performSwap(SwapObj* swap);

protected:

   cocos2d::Set* createInitialCookies();
   CookieObj* createCookie(int column, int row, int type);
   int getRandomCookieType(int column, int row);

   CC_SYNTHESIZE_READONLY(LevelInfo, mLevelInfo, LevelInfo);

   TileObj* mTiles[NumColumns][NumRows] = { nullptr };
   CookieObj* mCookies[NumColumns][NumRows] = { nullptr };
};
