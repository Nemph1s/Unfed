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

class CookieObj : public cocos2d::Node
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

   std::string spriteName();
   std::string highlightedSpriteName();
   std::string description();

   void updateDebugTileLabel();
   int getTypeAsInt();

protected:   

   CC_SYNTHESIZE(int, mColumn, Column);
   CC_SYNTHESIZE(int, mRow, Row);
   CC_SYNTHESIZE_READONLY(CommonTypes::CookieType, mType, Type);
   CC_SYNTHESIZE(cocos2d::Sprite*, mSpriteNode, SpriteNode);
   CC_SYNTHESIZE(cocos2d::Label*, mDebugLabel, DebugLabel);
};

