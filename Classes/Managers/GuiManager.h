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
#include "cocos2d/cocos/ui/UIText.h"
#include "cocos2d/cocos/ui/UILayout.h"

class GameplayScene;
class GuiManager
{
    CREATE_SINGLETON(GuiManager);

public:
    bool initWithScene(cocos2d::Scene* scene);

    void crateInfoLayer();

    void updateScoreLabel(int value);
    void updateTargetScoreLabel(int value);
    void updateMovesLabel(int value);

    cocos2d::ui::Text* createLabel(const CommonTypes::LabelInfo& info);

protected:
    
    CC_SYNTHESIZE(cocos2d::Scene*, mCurrentScene, CurrentScene);

    cocos2d::ui::Layout* mInfoLayout = nullptr;
    cocos2d::LayerColor* mLeftGuiLayer = nullptr;
    cocos2d::LayerColor* mRightGuiLayer = nullptr;

    cocos2d::ui::Text* mTargetLabel = nullptr;
    cocos2d::ui::Text* mMovesLabel = nullptr;
    cocos2d::ui::Text* mScoreLabel = nullptr;
};
