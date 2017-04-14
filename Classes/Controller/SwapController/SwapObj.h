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

namespace CT {
    typedef cocos2d::__Set Set;
}

class BaseObj;

class SwapObj : public cocos2d::Node
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~SwapObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static SwapObj* createWithObjects(BaseObj* objectA, BaseObj* objectB);

    virtual bool initWithObjects(BaseObj* objectA, BaseObj* objectB);

    std::string description();

protected:
    // Nodes should be created using create();
    SwapObj();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE_READONLY(BaseObj*, mObjectA, ObjectA);
    CC_SYNTHESIZE_READONLY(BaseObj*, mObjectB, ObjectB);

    CC_SYNTHESIZE(std::function<CT::Set*()>, mDetectHintChainCallback, DetectHintChainCallback);
};

