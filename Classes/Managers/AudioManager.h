/**
* @file Managers/AudioManager.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "Utils/PlatformMacros.h"
#include "Common/CommonTypes.h"

class _AudioManager
{
    CREATE_SINGLETON(_AudioManager);

public:
    bool init();

    void playBGMusic();
    void playSound(CommonTypes::SoundType type);
    
protected:
    bool preloadSounds();

};

#define AudioManager _AudioManager::getInstance()