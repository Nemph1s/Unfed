/**
* @file GameObjects/Level/LevelGoalComponent.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/Level/LevelGoalComponent.h"

#include "Utils/Helpers/Helper.h"
#include "Utils/Parser/JsonParser.h"

using namespace CommonTypes;

//--------------------------------------------------------------------
LevelGoalComponent::LevelGoalComponent()
    : cocos2d::Node()
    , mLevelGoals()
    //--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
LevelGoalComponent::~LevelGoalComponent()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelGoalComponent::~LevelGoalComponent: deallocing CookieObj: %p - tag: %i", this, _tag);
}

//--------------------------------------------------------------------
LevelGoalComponent* LevelGoalComponent::create()
//--------------------------------------------------------------------
{
    LevelGoalComponent* ret = new (std::nothrow) LevelGoalComponent();
    if (ret && ret->init()) {
        CC_SAFE_RETAIN(ret);
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool LevelGoalComponent::init()
//--------------------------------------------------------------------
{
    if (!Node::init()) {
        cocos2d::log("LevelGoalComponent::init: can't init Node inctance");
        return false;
    }

    if (!JsonParser->checkStatus()) {
        cocos2d::log("LevelObj::initWithId: can't parse json file");
        return false;
    }
    auto goals = JsonParser->getLevelGoals();

    return true;
}
