/**
* @file Managers/GuiManager.hpp
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

class GuiManager
{
    CREATE_SINGLETON(GuiManager);

public:
    bool init();

protected:
    
    cocos2d::Scene* getCurrentScene() const;


};
