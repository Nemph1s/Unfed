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
class BaseObj;

class _AnimationsManager
{
    CREATE_SINGLETON(_AnimationsManager);

public:
    bool initWithScene(cocos2d::Scene* scene);
    
    void animateSwap(SwapObj* swap, cocos2d::CallFunc* completion);
    void animateInvalidSwap(SwapObj* swap, cocos2d::CallFunc* completion);
    void animateMatching(cocos2d::Set* chains, cocos2d::CallFunc* completion);
    void animateFallingObjects(cocos2d::Array* colums, cocos2d::CallFunc* completion);
    void animateNewCookies(cocos2d::Array* colums, cocos2d::CallFunc* completion);

    void animateNewCookieSprite(cocos2d::Sprite* sprite);

    void animateScoreForChain(ChainObj* chain);
    void animateScoreForFieldObj(BaseObj* obj);

    void animateBouncingObj(BaseObj* obj);

    void animateRemovingFieldObjects(cocos2d::Set* fieldObjects);


    void animateRemoveDude(BaseObj* obj, cocos2d::CallFunc* completion);

protected:

    cocos2d::Scene* mCurrentScene;
};

#define AnimationsManager _AnimationsManager::getInstance()