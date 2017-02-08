/**
* @file Controller/ViewController.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"
#include "Utils/PlatformMacros.h"

class LevelObj;
class SwapObj;
class GameplayScene;

class ViewController : public cocos2d::Ref
{
public:

CC_CONSTRUCTOR_ACCESS:
    // Nodes should be created using create();
    ViewController();
    virtual ~ViewController();

    virtual bool init();

    void startGame();

protected:
    void updateInfoLabels();

    void shuffle();
    void handleMatches();

    void beginNextTurn();
    void decrementMoves();

    //---Callbacks-------------------------------------------------------
    void shuffleButtonCallback();
    void swapCallback(SwapObj* swap);
    //--------------------------------------------------------------------

    //---Class Attributes-------------------------------------------------
    std::function<void()> mShuffleButtonCallback;

    CC_SYNTHESIZE_READONLY(int, mMovesLeft, MovesLeft);
    CC_SYNTHESIZE_READONLY(int, mScore, Score);

    CC_SYNTHESIZE_READONLY_PTR(LevelObj*, mLevel, Level);
    CC_SYNTHESIZE_READONLY_PTR(GameplayScene*, mGameplayScene, GameplayScene)
};

