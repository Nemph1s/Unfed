/**
* @file Managers/AnimationsManager.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"
#include "Utils/PlatformMacros.h"

class SwapObj;
class ChainObj;
class AnimationsManager
{
    CREATE_SINGLETON(AnimationsManager);

public:

    bool initWithScene(cocos2d::Scene* scene);
    
    void animateSwap(SwapObj* swap, cocos2d::CallFunc* completion);
    void animateInvalidSwap(SwapObj* swap, cocos2d::CallFunc* completion);
    void animateMatching(cocos2d::Set* chains, cocos2d::CallFunc* completion);
    void animateFallingCookies(cocos2d::Array* colums, cocos2d::CallFunc* completion);
    void animateNewCookies(cocos2d::Array* colums, cocos2d::CallFunc* completion);

    void animateScoreForChain(ChainObj* chain);

protected:

    cocos2d::Scene* mCurrentScene;

};