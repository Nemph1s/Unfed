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

#include "GameObjects/TileObjects/Base/BaseObj.h"
#include "GameObjects/TileObjects/CookieObj.h"

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
    cocos2d::log("LevelGoalComponent::~LevelGoalComponent: %p - tag: %i", this, _tag);
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

    if (!JsonParser->checkLevelInfoStatus()) {
        cocos2d::log("LevelGoalComponent::init: can't parse json file");
        return false;
    }
    mLevelGoals = JsonParser->getJsonLevelGoals();

    return true;
}

//--------------------------------------------------------------------
void LevelGoalComponent::updateGoalByObject(BaseObj * obj)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);

    auto baseType = Helper::to_underlying(obj->getType());
    auto objType = obj->getTypeAsInt();

    for (CommonTypes::CollectGoalInfo& info : mLevelGoals.collectGoals) {
        if (baseType == info.baseObjectType && objType == info.objectType) {
            info.currentCount++;
            break;
        }
    }
}
