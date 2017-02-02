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

   /**
   * @brief To Swap or Not to Swap�
   */
   bool isPossibleSwap(SwapObj* swap);
   void performSwap(SwapObj* swap);

   cocos2d::Set* detectHorizontalMatches();
   cocos2d::Set* detectVerticalMatches();

   cocos2d::Set* removeMatches();
   void removeCookies(cocos2d::Set* chains);

protected:

   cocos2d::Set* createInitialCookies();
   CookieObj* createCookie(int column, int row, int type);
   int getRandomCookieType(int column, int row);

   /**
   * @brief A method that checks is the cookie[column][row] type equal to forwarded type  
   * @param column Current cookie column
   * @param row Current cookie row
   * @param type A type of cookie with which must be checked current cookie
   */
   bool isSameTypeOfCookieAt(int column, int row, int type);

   /**
   * @brief A method that checks is the cookie[column][row] type equal to forwarded type 
   * It will step through the rows and columns of the 2 - D grid and simply swap each cookie with the one next to it, one at a time.
   * If swapping these two cookies creates a chain, it will add a new RWTSwap object to the list of possibleSwaps.
   * Then, it will swap these cookies back to restore the original state and continue with the next cookie until it has swapped them all.
   * It will go through the above steps twice : once to check all horizontal swaps and once to check all vertical swaps.
   */
   void detectPossibleSwaps();

   /**
   * @brief A helper method to see if a cookie is part of a chain
   */
   bool hasChainAt(int column, int row);

   CC_SYNTHESIZE_READONLY(cocos2d::Set*, mPossibleSwaps, PossibleSwaps);
   CC_SYNTHESIZE_READONLY(LevelInfo, mLevelInfo, LevelInfo);

   TileObj* mTiles[NumColumns][NumRows] = { nullptr };
   CookieObj* mCookies[NumColumns][NumRows] = { nullptr };
};
