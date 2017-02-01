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

class AudioManager
{
    CREATE_SINGLETON(AudioManager);

public:
    bool init();

    void playBGMusic();
    void playSound(CommonTypes::SoundType type);
    
protected:
    bool preloadSounds();

};
