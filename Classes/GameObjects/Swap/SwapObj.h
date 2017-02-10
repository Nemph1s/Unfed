/**
* @file GameObjects/SwapObj.hpp
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

class CookieObj;

class SwapObj : public cocos2d::Node
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~SwapObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static SwapObj* createWithCookies(CookieObj* cookieA, CookieObj* cookieB);

    virtual bool initWithCookies(CookieObj* cookieA, CookieObj* cookieB);

    std::string description();

protected:
    // Nodes should be created using create();
    SwapObj();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE_READONLY(CookieObj*, mCookieA, CookieA);
    CC_SYNTHESIZE_READONLY(CookieObj*, mCookieB, CookieB);
   
};

