/**
* @file GameObjects/CookieObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/CookieObj.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"



//--------------------------------------------------------------------
CookieObj::CookieObj()
   : mColumn(0)
   , mRow(0)
   , mCookieType(CommonTypes::CookieType::Unknown)
   , mSpriteNode(nullptr)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
CookieObj * CookieObj::create(const CommonTypes::CookieInfo & cookieInfo)
//--------------------------------------------------------------------
{
   CookieObj * ret = new (std::nothrow) CookieObj();
   if (ret && ret->init(cookieInfo)) {
      ret->autorelease();
   }
   else {
      CC_SAFE_DELETE(ret);
   }
   return ret;
}

//--------------------------------------------------------------------
CookieObj::~CookieObj()
//--------------------------------------------------------------------
{
   CCLOGINFO("CookieObj::~CookieObj: deallocing CookieObj: %p - tag: %i", this, _tag);
}

//--------------------------------------------------------------------
bool CookieObj::init(const CommonTypes::CookieInfo & cookieInfo)
//--------------------------------------------------------------------
{
   if (!Node::init()) {
      CCLOGERROR("CookieObj::init: can't init Node inctance");
      return false;
   }

   CCLOGINFO("CookieObj::init: column=%d row=%d cookieType=%d", mColumn, mRow, mCookieType);

   mColumn = cookieInfo.column;
   mRow = cookieInfo.row;
   mCookieType = cookieInfo.cookieType;

   return true;
}

//--------------------------------------------------------------------
string CookieObj::spriteName()
//--------------------------------------------------------------------
{
   auto type = Helper::to_underlying(mCookieType);
   return GameResources::s_cookieSpriteNames.at(type);
}

//--------------------------------------------------------------------
string CookieObj::highlightedSpriteName()
//--------------------------------------------------------------------
{
   auto type = Helper::to_underlying(mCookieType);
   return GameResources::s_cookieHighlightedSpriteNames.at(type);
}

//--------------------------------------------------------------------
string CookieObj::description()
//--------------------------------------------------------------------
{
   CCLOGINFO("CookieObj::description: type:%d square:(%d,%d)", mCookieType, mColumn, mRow);
   return cocos2d::StringUtils::format("type:%d square:(%d,%d)", mCookieType, mColumn, mRow);
}
