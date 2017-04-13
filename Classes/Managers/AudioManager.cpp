/**
* @file Managers/AudioManager.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Managers/AudioManager.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"  

using namespace CT;

const auto AudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();

//--------------------------------------------------------------------
bool _AudioManager::init()
//--------------------------------------------------------------------
{
    cocos2d::log("AudioManager::init:");
    if (!preloadSounds()) {
        return false;
    }
    return true;
}

//--------------------------------------------------------------------
void _AudioManager::playBGMusic()
//--------------------------------------------------------------------
{
    AudioEngine->playBackgroundMusic(GameResources::s_gameplaySoundBG.getCString(), true);
}

//--------------------------------------------------------------------
void _AudioManager::playSound(CT::SoundType type)
//--------------------------------------------------------------------
{
    auto soundType = Helper::getInstance()->to_underlying(type);
    //cocos2d::log("AudioManager::playSound: type=%d", soundType);

    if (type < SoundType::SwapSound || type >= SoundType::SoundsMax) {
        cocos2d::log("AudioManager::playSound: wrong sound type!");
        return;
    }
    AudioEngine->playEffect(GameResources::s_gameplaySounds.at(soundType).getCString());
}

//--------------------------------------------------------------------
bool _AudioManager::preloadSounds()
//--------------------------------------------------------------------
{
    cocos2d::log("AudioManager::preloadSounds:");
    AudioEngine->preloadBackgroundMusic(GameResources::s_gameplaySoundBG.getCString());
    auto soundMax = Helper::getInstance()->to_underlying(SoundType::SoundsMax);

    for (int i = 0; i < soundMax; i++) {
        AudioEngine->preloadEffect(GameResources::s_gameplaySounds.at(i).getCString());
    }
    return true;
}
