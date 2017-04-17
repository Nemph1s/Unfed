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
class BaseObj;
class DudeObj;
class GameplayScene;
class ObjectController;
class ChainController;
class SwapController;
class DudeController;
class EnemyController;
class LevelGoalComponent;

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

    //---Initialization-------------------------------------------------------
    bool initGameScene();
    bool initLevel();
    bool initSpritesFactory();
    bool initObjectController();
    bool initSwapController();
    bool initChainController();
    bool initDudeController();
    bool initEnemyController();

    //---Gui-------------------------------------------------------
    void updateScore(CT::Set* chains);
    void updateInfoLabels();

    //---Operable sequence-------------------------------------------------------
    void shuffle();
    void handleMatches();
    void animateHandleMatches(CT::Set* chains);
    void beginNextTurn();

    //---Callbacks-------------------------------------------------------
    void shuffleButtonCallback();
    void swapCallback(SwapObj* swap);
    void activateDudeCallback(DudeObj* obj, int direction);

    //---Special-------------------------------------------------------
    void throwDownAnObject(BaseObj* obj, CT::Cell& destPos, bool isHeavyObject);

    //---Hints-------------------------------------------------------
    void startHintTimer();
    void stopHintTimer();
    void showSwapHint(float dt);

    //---Other-------------------------------------------------------
    void decrementMoves();
    //--------------------------------------------------------------------

    //---Class Attributes-------------------------------------------------
    std::function<void()> mShuffleButtonCallback;

    CC_SYNTHESIZE_READONLY(uint8_t, mMovesLeft, MovesLeft)
    CC_SYNTHESIZE_READONLY(uint32_t, mScore, Score)

    CC_SYNTHESIZE_READONLY(LevelObj*, mLevel, Level)
    
    CC_SYNTHESIZE_READONLY(GameplayScene*, mGameplayScene, GameplayScene)

    CC_SYNTHESIZE_READONLY(LevelGoalComponent*, mLevelGoals, LevelGoalComponent)
    
    CC_SYNTHESIZE_READONLY(ObjectController*, mObjectController, ObjectController)
    CC_SYNTHESIZE_READONLY(ChainController*, mChainController, ChainController)
    CC_SYNTHESIZE_READONLY(SwapController*, mSwapController, SwapController)
    CC_SYNTHESIZE_READONLY(DudeController*, mDudeController, DudeController)
    CC_SYNTHESIZE_READONLY(EnemyController*, mEnemyController, EnemyController)
};

