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
#include "Common/CommonTypes.h"

class LevelObj;
class SwapObj;
class DudeObj;
class GameplayScene;
class ObjectController;
class ChainController;
class SwapController;
class DudeController;

class ViewController : public cocos2d::Ref
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~ViewController();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease" and "retain".
    */
    static ViewController* create();

    bool init();
    void startGame();

protected:
    // Nodes should be created using create();
    ViewController();

    bool initGameScene();
    bool initObjectController();
    bool initSwapController();
    bool initChainController();
    bool initDudeController();

    void updateScore(cocos2d::Set* chains);
    void updateInfoLabels();

    void shuffle();
    void handleMatches();
    void animateHandleMatches(cocos2d::Set* chains);

    void beginNextTurn();
    void decrementMoves();

    //---Callbacks-------------------------------------------------------
    void shuffleButtonCallback();
    void swapCallback(SwapObj* swap);
    //???? something wrong with
    void activateDudeCallback(DudeObj* obj, int direction);
    void activateChainCallback(CommonTypes::ChainType& type, cocos2d::Vec2& pos);
    //--------------------------------------------------------------------

    //---Class Attributes-------------------------------------------------
    std::function<void()> mShuffleButtonCallback;

    CC_SYNTHESIZE_READONLY(uint8_t, mMovesLeft, MovesLeft)
    CC_SYNTHESIZE_READONLY(uint32_t, mScore, Score)

    CC_SYNTHESIZE_READONLY(LevelObj*, mLevel, Level)
    
    CC_SYNTHESIZE_READONLY(GameplayScene*, mGameplayScene, GameplayScene)
    
    CC_SYNTHESIZE_READONLY(ObjectController*, mObjectController, ObjectController)
    CC_SYNTHESIZE_READONLY(ChainController*, mChainController, ChainController)
    CC_SYNTHESIZE_READONLY(SwapController*, mSwapController, SwapController)
    CC_SYNTHESIZE_READONLY(DudeController*, mDudeController, DudeController)
};

