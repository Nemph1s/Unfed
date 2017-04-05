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

class BaseObj;

class _ActionsManager
{
    CREATE_SINGLETON(_ActionsManager);

public:
    bool init();
    
    cocos2d::ActionInstant* actionBounceIn(BaseObj* obj);
    cocos2d::ActionInstant* actionBounceOut(BaseObj* obj);

    cocos2d::ActionInstant* actionSwapObj(BaseObj* objA, BaseObj* objB, bool isFirstSwapObject = false);
    
protected:

};

#define ActionsManager _ActionsManager::getInstance()