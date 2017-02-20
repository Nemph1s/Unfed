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

class _GuiManager
{
    CREATE_SINGLETON(_GuiManager);

public:
    bool initWithScene(cocos2d::Scene* scene);

    void setShuffleButtonCallback(std::function<void()> touchEndedCallback);

    void updateScoreLabel(uint32_t value);
    void updateTargetScoreLabel(uint32_t value);
    void updateMovesLabel(int value);

protected:

    void crateInfoLayer();
    void createShuffleButton();
    cocos2d::ui::Text* createLabel(const CommonTypes::TextLabelInfo& info);
    
    CC_SYNTHESIZE(cocos2d::Scene*, mCurrentScene, CurrentScene);

    cocos2d::ui::Button* mShuffleButton = nullptr;

    cocos2d::ui::Layout* mInfoLayout = nullptr;
    cocos2d::LayerColor* mTopGuiLayer = nullptr;
    cocos2d::LayerColor* mBottomGuiLayer = nullptr;

    cocos2d::ui::Text* mTargetLabel = nullptr;
    cocos2d::ui::Text* mMovesLabel = nullptr;
    cocos2d::ui::Text* mScoreLabel = nullptr;
};

#define GuiManager _GuiManager::getInstance()