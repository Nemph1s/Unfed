/**
* @file GameObjects/CookieObj.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"
#include <string.h>
#include "Common/CommonTypes.h"

using std::string;
using cocos2d::Node;
using cocos2d::Sprite;

class CookieObj : public Node
{
public:

   /**
   * Allocates and initializes a node.
   * @return A initialized node which is marked as "autorelease".
   */
   static CookieObj * create(const CommonTypes::CookieInfo &cookieInfo);

CC_CONSTRUCTOR_ACCESS:
   // Nodes should be created using create();
   CookieObj();
   virtual ~CookieObj();

   virtual bool init(const CommonTypes::CookieInfo & cookieInfo);

   string spriteName();
   string highlightedSpriteName();
   string description();

protected:   
   CC_SYNTHESIZE_READONLY(int8_t, mColumn, Column);
   CC_SYNTHESIZE_READONLY(int8_t, mRow, Row);
   CC_SYNTHESIZE_READONLY(CommonTypes::CookieType, mCookieType, CookieType);
   CC_SYNTHESIZE(Sprite*, mSpriteNode, SpriteNode);
};

