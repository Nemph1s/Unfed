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
#include "Utils/PlatformMacros.h"

class CookieObj : public cocos2d::Node
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~CookieObj();

public:
   /**
   * Allocates and initializes a node.
   * @return A initialized node which is marked as "autorelease".
   */
   static CookieObj * create(const CommonTypes::CookieInfo &cookieInfo);

   virtual bool init(const CommonTypes::CookieInfo & cookieInfo);

   std::string spriteName();
   std::string highlightedSpriteName();
   std::string description();

   void updateDebugTileLabel();
   int getTypeAsInt();

protected: 
    // Nodes should be created using create();
    CookieObj();

    //---Class Attributes-------------------------------------------------
   CC_SYNTHESIZE(int, mColumn, Column);
   CC_SYNTHESIZE(int, mRow, Row);

   CC_SYNTHESIZE_READONLY(CommonTypes::CookieType, mType, Type);

   CC_SYNTHESIZE_PTR(cocos2d::Sprite*, mSpriteNode, SpriteNode);
   CC_SYNTHESIZE_PTR(cocos2d::Label*, mDebugLabel, DebugLabel);
};

