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

#include "GameObjects/Base/BaseObj.h"

class CookieObj : public BaseObj
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~CookieObj();

public:
   /**
   * Allocates and initializes a node.
   * @return A initialized node which is marked as "autorelease".
   */
   static CookieObj * create(const CommonTypes::CookieInfo &cookieInfo);

   bool init(const CommonTypes::CookieInfo & cookieInfo);

   cocos2d::String& spriteName() const override;
   cocos2d::String& highlightedSpriteName() const;
   cocos2d::String& description() const;

   int getTypeAsInt() const override;

   virtual bool isMovable() const override;
   virtual bool isRemovable() const override;
   virtual bool isPossibleSwap() const override;

   void updateDebugTileLabel();

protected: 
    // Nodes should be created using create();
    CookieObj();

    //---Class Attributes-------------------------------------------------
   CC_SYNTHESIZE_READONLY(CommonTypes::CookieType, mCookieType, CookieType);

   CC_SYNTHESIZE(cocos2d::Sprite*, mSpriteNode, SpriteNode);
   CC_SYNTHESIZE(cocos2d::Label*, mDebugLabel, DebugLabel);
};

