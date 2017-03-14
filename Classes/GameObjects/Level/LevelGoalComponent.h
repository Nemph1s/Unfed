#/**
* @file GameObjects/Level/LevelGoalComponent.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"
#include "GameObjects/Level/LevelTypes.h"

class BaseObj;

class LevelGoalComponent : public cocos2d::Node
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~LevelGoalComponent();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static LevelGoalComponent * create();

    virtual bool init();

    void updateGoalByObject(BaseObj* obj);

protected:
    // Nodes should be created using create();
    LevelGoalComponent();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE_READONLY(CommonTypes::LevelGoals, mLevelGoals, LevelGoals);

};
