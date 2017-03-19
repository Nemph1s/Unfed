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
#include "Common/CommonTypes.h"

class SwapObj;
class ChainObj;
class BaseObj;
class CookieObj;
class FieldObj;
class DudeObj;

class _AnimationsManager
{
    CREATE_SINGLETON(_AnimationsManager);

public:
    bool initWithScene(cocos2d::Scene* scene);
    
    void animateSwap(SwapObj* swap, cocos2d::CallFunc* completion);
    void animateInvalidSwap(SwapObj* swap, cocos2d::CallFunc* completion);
    void animateMatching(CommonTypes::Set* chains, cocos2d::CallFunc* completion);
    void animateFallingObjects(cocos2d::Array* colums, cocos2d::CallFunc* completion);
    void animateNewCookies(cocos2d::Array* colums, cocos2d::CallFunc* completion);

    void animateNewCookieSprite(cocos2d::Sprite* sprite);

    void animateScoreForChain(ChainObj* chain);
    void animateScoreForFieldObj(BaseObj* obj);

    void animateBouncingObj(BaseObj* obj);
    
protected:
    void animateMatchCookie(CookieObj* obj);
    void animateMatchFieldObj(FieldObj* obj);
    void animateMatchDude(DudeObj* obj);

    cocos2d::Scene* mCurrentScene;
};

#define AnimationsManager _AnimationsManager::getInstance()