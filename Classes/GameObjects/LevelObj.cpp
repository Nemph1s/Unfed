/**
* @file GameObjects/LevelObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/LevelObj.h"

//--------------------------------------------------------------------
LevelObj::LevelObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
LevelObj * LevelObj::create()
//--------------------------------------------------------------------
{
   LevelObj * ret = new (std::nothrow) LevelObj();
   if (ret && ret->init()) {
      ret->autorelease();
   }
   else {
      CC_SAFE_DELETE(ret);
   }
   return ret;
}

//--------------------------------------------------------------------
LevelObj::~LevelObj()
//--------------------------------------------------------------------
{
   CCLOGINFO("LevelObj::~LevelObj: deallocing CookieObj: %p - tag: %i", this, _tag);
}

//--------------------------------------------------------------------
bool LevelObj::init()
//--------------------------------------------------------------------
{
   if (!Node::init()) {
      CCLOGERROR("LevelObj::init: can't init Node inctance");
      return false;
   }
// 
//    CCLOGINFO("CookieObj::init: column=%d row=%d cookieType=%d", mColumn, mRow, mCookieType);
// 
//    mColumn = cookieInfo.column;
//    mRow = cookieInfo.row;
//    mCookieType = cookieInfo.cookieType;

   return true;
}

//--------------------------------------------------------------------
cocos2d::Set * LevelObj::shuffle()
//--------------------------------------------------------------------
{
   return nullptr;
}

//--------------------------------------------------------------------
CookieObj * LevelObj::cookieAt(int8_t columt, int8_t row)
//--------------------------------------------------------------------
{
   return nullptr;
}
