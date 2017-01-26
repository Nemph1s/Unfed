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

//temporary variables
//TODO: move to globalInfo or to json file
static const int8_t NumColumns = 9;
static const int8_t NumRows = 9;

class CookieObj;
class LevelObj : public cocos2d::Node
{
public:
   /**
   * Allocates and initializes a node.
   * @return A initialized node which is marked as "autorelease".
   */
   static LevelObj * create();

CC_CONSTRUCTOR_ACCESS:
   // Nodes should be created using create();
   LevelObj();
   virtual ~LevelObj();

   virtual bool init();

   cocos2d::Set* shuffle();
   CookieObj* cookieAt(int8_t column, int8_t row);

protected:

   cocos2d::Set* createInitialCookies();
   CookieObj* createCookie(int8_t column, int8_t row, int8_t type);
   int8_t getRandomCookieType();

   CC_SYNTHESIZE_READONLY(int8_t, mNumColumns, NumColumns);
   CC_SYNTHESIZE_READONLY(int8_t, mNumRows, NumRows);

   CookieObj* mCookies[NumColumns][NumRows];
};
