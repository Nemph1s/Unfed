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

#include "GameObjects/TileObjects/Base/BaseObj.h"

class CookieObj : public BaseObj
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~CookieObj();

/*    friend SmartObjFactory;*/

public:
   /**
   * Allocates and initializes a node.
   * @return A initialized node which is marked as "autorelease".
   */
   static CookieObj * create(const GOT::CookieInfo &cookieInfo);

   bool init(const GOT::CookieInfo & cookieInfo);

   cocos2d::String& spriteName() const override;
   cocos2d::String& highlightedSpriteName() const;
   cocos2d::String& description() const;

   virtual void setColumn(int var) override;
   virtual void setRow(int var) override;
   virtual int getTypeAsInt() const override;

   virtual void clear() override;

protected: 
    // Nodes should be created using create();
    CookieObj();

    //---Class Attributes-------------------------------------------------
   CC_SYNTHESIZE_READONLY(GOT::CookieType, mCookieType, CookieType)
};

