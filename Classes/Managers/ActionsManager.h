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
#include "Common/GameObjTypes.h"

class BaseObj;

class _ActionsManager
{
    CREATE_SINGLETON(_ActionsManager);

public:
    bool init();
    
    cocos2d::ActionInstant* actionBounceInNormal(BaseObj* obj, float duration = 0.2f);
    cocos2d::ActionInstant* actionBounceInHeavy(BaseObj* obj, float duration = 0.3f);
    cocos2d::ActionInstant* actionBounceOut(BaseObj* obj, float duration = 0.2f);

    cocos2d::ActionInstant* actionSwapObj(BaseObj* objA, BaseObj* objB, bool isFirstSwapObject = false);

    cocos2d::ActionInterval* actionThrowDownAnObj(BaseObj* obj, CT::Cell& desinationCell);
    cocos2d::ActionInterval* actionFallObject(BaseObj* obj, cocos2d::CallFunc* moveCallback, float duration = 0.2f);

protected:

    cocos2d::ActionInstant* reverseScaleXCallback(BaseObj* obj, float duration);

    //TODO: create queue for actions with actionEntitiy struct (param 1: base obj, to get a sprite, action (if it move action start pos should be current pos))
};

#define ActionsManager _ActionsManager::getInstance()