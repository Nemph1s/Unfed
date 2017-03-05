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

#include "Common/CommonTypes.h"
#include "Utils/PlatformMacros.h"

#include "cocos2d.h"

namespace cocos2d { namespace ui { class Text; class Layout; class Button; class Widget; } }

class GameplayScene;
class LevelGoalComponent;

class _GuiManager
{
    CREATE_SINGLETON(_GuiManager);

public:
    bool initWithScene(cocos2d::Scene* scene, LevelGoalComponent* levelGoal);

    void setShuffleButtonCallback(std::function<void()> touchEndedCallback);

    void updateScore(uint32_t value, float percentage);
    void updateMovesLabel(int value);
    void updateLevelGoals(std::vector<CommonTypes::CollectGoalInfo>& levelGoals);

protected:

    void crateInfoLayer();
    void createShuffleButton();
    void createScoreBar();
    void createLevelGoals();

    //TODO: move to sprite factory
    cocos2d::Sprite* createSprite(int baseType, int objType);
    //TODO: move to helper
    cocos2d::Vec2 getPosForGoalSprite(int currGoal, int goalsCount, const cocos2d::Size& spriteSize);

    cocos2d::ui::Text* createLabel(const CommonTypes::TextLabelInfo& info);
    
    CC_SYNTHESIZE(cocos2d::Scene*, mCurrentScene, CurrentScene);
    CC_SYNTHESIZE(LevelGoalComponent*, mLevelGoalComponent, LevelGoalComponent);

    cocos2d::ui::Button* mShuffleButton = nullptr;

    cocos2d::ui::Layout* mInfoLayout = nullptr;
    cocos2d::LayerColor* mTopGuiLayer = nullptr;
    cocos2d::LayerColor* mBottomGuiLayer = nullptr;

    cocos2d::ProgressTimer* mScoreBar = nullptr;

    cocos2d::ui::Text* mMovesLabel = nullptr;
    cocos2d::ui::Text* mScoreLabel = nullptr;

    std::vector<cocos2d::ui::Text*> mGoalsLabels;
};

#define GuiManager _GuiManager::getInstance()