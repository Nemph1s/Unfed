/**
* @file Managers/GuiManager.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Managers/GuiManager.h"

using cocos2d::Director;

//--------------------------------------------------------------------
bool GuiManager::init()
//--------------------------------------------------------------------
{
    cocos2d::log("GuiManager::init:");
    if (false) {
        return false;
    }
    return true;
}

//--------------------------------------------------------------------
cocos2d::Scene * GuiManager::getCurrentScene() const
//--------------------------------------------------------------------
{
    return Director::getInstance()->getRunningScene();
}
