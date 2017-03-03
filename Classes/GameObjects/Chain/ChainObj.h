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
#include "Common/CommonTypes.h"

class BaseObj;
class CookieObj;

typedef cocos2d::Map<int8_t, int16_t> TFieldObjGoalMap;

class ChainObj : public cocos2d::Node
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~ChainObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static ChainObj * createWithType(const CommonTypes::ChainType &type);

    virtual bool initWithType(const CommonTypes::ChainType &type);

    std::string description();
    std::string typeAsString();
    int getTypeAsInt();

    void addObject(BaseObj* obj);

    void addCookie(CookieObj* cookie);
    void addCookiesFromChain(ChainObj* chain);

protected:
    // Nodes should be created using create();
    ChainObj();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(BaseObj* obj)>, mUpdateGoalCallback, UpdateGoalCallback);

    CC_SYNTHESIZE(int, mScore, Score);
    CC_SYNTHESIZE(bool, mIsCreatedByDude, IsCreatedByDude);

    CC_SYNTHESIZE_READONLY(CommonTypes::ChainType, mType, Type);
    CC_SYNTHESIZE_READONLY(cocos2d::Array*, mCookies, Cookies);
};

