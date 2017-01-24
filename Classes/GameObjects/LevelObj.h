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

#include "GameObjects/CookieObj.h"

class LevelObj : public Node
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
   CookieObj* cookieAt(int8_t columt, int8_t row);

protected:

   CC_SYNTHESIZE_READONLY(int8_t, mNumColumns, NumColumns);
   CC_SYNTHESIZE_READONLY(int8_t, mNumRows, NumRows);
};
