#/**
* @file GameObjects/ChainObj.hpp
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



class CookieObj;
class ChainObj : public cocos2d::Node
{
public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static ChainObj * createWithType(const CommonTypes::ChainType &type);

CC_CONSTRUCTOR_ACCESS:
    // Nodes should be created using create();
    ChainObj();
    virtual ~ChainObj();

    virtual bool initWithType(const CommonTypes::ChainType &type);

    std::string description();
    std::string typeAsString();
    int getTypeAsInt();

    void addCookie(CookieObj* cookie);

protected:

    CC_SYNTHESIZE_READONLY(cocos2d::Array*, mCookies, Cookies);
    CC_SYNTHESIZE_READONLY(CommonTypes::ChainType, mType, Type);
    CC_SYNTHESIZE(int, mScore, Score);

};

