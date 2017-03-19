/**
* @file Scenes/ViewController.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ViewController.h"
#include "Controller/SwapController.h"
#include "Controller/ChainController.h"
#include "Controller/ObjectController/Dude/DudeController.h"
#include "Controller/ObjectController/ObjectController.h"

#include "Managers/AnimationsManager.h"
#include "Managers/AudioManager.h"
#include "Managers/GuiManager.h"

#include "Common/Factory/SmartFactory.h"

#include "GameObjects/Swap/SwapObj.h"
#include "GameObjects/Level/LevelObj.h"
#include "GameObjects/Level/LevelGoalComponent.h"
#include "GameObjects/TileObjects/DudeObj.h"
#include "GameObjects/Chain/ChainObj.h"

#include "Scenes/GameplayScene.h"

using cocos2d::Director;
using cocos2d::CallFunc;
using namespace CommonTypes;
using namespace std::placeholders;

#define COCOS2D_DEBUG 1
#define UNFED_ENABLE_DEBUG 1

#define CURRENT_LEVEL 667

//--------------------------------------------------------------------
ViewController::ViewController()
    : mMovesLeft(0)
    , mScore(0)
    , mLevel(nullptr)
    , mGameplayScene(nullptr)
    , mLevelGoals(nullptr)
    , mObjectController(nullptr)
    , mChainController(nullptr)
    , mSwapController(nullptr)
    , mDudeController(nullptr)
//--------------------------------------------------------------------
{
   cocos2d::log("ViewController::ViewController");
}

//--------------------------------------------------------------------
ViewController::~ViewController()
//--------------------------------------------------------------------
{
    cocos2d::log("ViewController::~ViewController");
    
    CC_SAFE_RELEASE_NULL(mLevel);
    CC_SAFE_RELEASE_NULL(mGameplayScene);
    CC_SAFE_RELEASE_NULL(mLevelGoals);
    CC_SAFE_RELEASE_NULL(mObjectController);
    CC_SAFE_RELEASE_NULL(mChainController);
    CC_SAFE_RELEASE_NULL(mSwapController);
    CC_SAFE_RELEASE_NULL(mDudeController);
}

//--------------------------------------------------------------------
ViewController * ViewController::create()
//--------------------------------------------------------------------
{
    ViewController * ret = new (std::nothrow) ViewController();
    if (ret && ret->init()) {
        ret->autorelease();
        CC_SAFE_RETAIN(ret);
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool ViewController::init()
//--------------------------------------------------------------------
{
    cocos2d::log("ViewController::init");

    initGameScene();
    initLevel();
    initObjectController();
    initChainController();
    initSwapController();
    initDudeController();

    mShuffleButtonCallback = std::bind(&ViewController::shuffleButtonCallback, this);
    GuiManager->setShuffleButtonCallback(mShuffleButtonCallback);

    // Present the scene.
    Director::getInstance()->runWithScene(mGameplayScene);

    startGame();

    return true;
}

//--------------------------------------------------------------------
bool ViewController::initGameScene()
//--------------------------------------------------------------------
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    // Create and configure the scene.
    mGameplayScene = GameplayScene::createWithSize(glview->getFrameSize());
    //self.scene.scaleMode = SKSceneScaleModeAspectFill;

    AudioManager->init();
    AnimationsManager->initWithScene(mGameplayScene);

    SmartFactory->initTilesPool(NumColumns * NumRows);
    SmartFactory->initCookiesPool((NumColumns * NumRows) * 2);

    return true;
}

//--------------------------------------------------------------------
bool ViewController::initLevel()
//--------------------------------------------------------------------
{
    // Load the level.
    int levelId = CURRENT_LEVEL;
    mLevel = LevelObj::createWithId(levelId);
    mScore = mLevel->getLevelInfo().targetScore;
    mMovesLeft = mLevel->getLevelInfo().moves;

    //TODO: create tags instead of name
    mLevel->setName("Level");

    mGameplayScene->setLevel(mLevel);
    mLevelGoals = LevelGoalComponent::create();

    GuiManager->initWithScene(mGameplayScene, mLevelGoals);

    return true;
}

//--------------------------------------------------------------------
bool ViewController::initObjectController()
//--------------------------------------------------------------------
{
    mObjectController = ObjectController::create();
    mObjectController->setLevel(mLevel);
    mLevel->setObjectController(mObjectController);

    mObjectController->createObjects();
    auto fieldObjecs = mObjectController->createInitialFieldObjects();
    mGameplayScene->addTiles();
    mGameplayScene->addSpritesForObjects(fieldObjecs);

    return true;
}

//--------------------------------------------------------------------
bool ViewController::initChainController()
//--------------------------------------------------------------------
{
    mChainController = ChainController::create();
    mChainController->setLevel(mLevel);
    mChainController->setObjectController(mObjectController);
    mLevel->setChainController(mChainController);

    auto updateGoalCallback = [=](BaseObj* obj) {
        mLevelGoals->updateGoalByObject(obj);
        GuiManager->updateLevelGoals(mLevelGoals->getLevelGoals().collectGoals);
    };
    mChainController->setUpdateGoalCallback(updateGoalCallback);

    return true;
}

//--------------------------------------------------------------------
bool ViewController::initDudeController()
//--------------------------------------------------------------------
{
    mDudeController = DudeController::create();
    mDudeController->setObjectController(mObjectController);
    mDudeController->setChainController(mChainController);
    mObjectController->setDudeController(mDudeController);
    mLevel->setDudeController(mDudeController);

    auto activateDudeCallback = std::bind(&ViewController::activateDudeCallback, this, _1, _2);
    mDudeController->setActivateDudeCallback(activateDudeCallback);

    auto dudeCtrl = mDudeController;
    auto dudeActivationCallback = [dudeCtrl](int fromCol, int fromRow, int direction) {
        return dudeCtrl->canActivateDudeTo(fromCol, fromRow, direction);
    };
    mGameplayScene->setDudeActivationCallback(dudeActivationCallback);

    return true;
}

//--------------------------------------------------------------------
bool ViewController::initSwapController()
//--------------------------------------------------------------------
{
    mSwapController = SwapController::create();

    mSwapController->setLevel(mLevel);

    auto swapCallback = std::bind(&ViewController::swapCallback, this, _1);
    mSwapController->setSwapCallback(swapCallback);

    auto swapCtrl = mSwapController;
    auto tryToSwapCallback = [swapCtrl](int fromCol, int fromRow, int direction) {
        return swapCtrl->trySwapCookieTo(fromCol, fromRow, direction);
    };
    mGameplayScene->setSwapCookieCallback(tryToSwapCallback);

    auto detectPossibleSwapsCallback = [swapCtrl]() {
        return swapCtrl->detectPossibleSwaps();
    };
    mLevel->setDetectPossibleSwapsCallback(detectPossibleSwapsCallback);

    return true;
}

//--------------------------------------------------------------------
void ViewController::startGame()
//--------------------------------------------------------------------
{
   cocos2d::log("ViewController::startGame");

   mMovesLeft = mLevel->getLevelInfo().moves;
   mScore = 0;
   updateInfoLabels();
   mLevel->resetComboMultiplier();

   shuffle();
}

//--------------------------------------------------------------------
void ViewController::updateScore(CommonTypes::Set * chains)
//--------------------------------------------------------------------
{
    CC_ASSERT(chains);
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        if (!chain)
            continue;
        mScore += chain->getScore();
    }
}

//--------------------------------------------------------------------
void ViewController::updateInfoLabels()
//--------------------------------------------------------------------
{
    cocos2d::log("ViewController::updateInfoLabels");
    CC_ASSERT(mLevel);

    float targetScore = mLevel->getLevelInfo().targetScore;
    float percentage = static_cast<float>(mScore / targetScore);
    GuiManager->updateScore(mScore, percentage);
    GuiManager->updateMovesLabel(mMovesLeft);
}

//--------------------------------------------------------------------
void ViewController::shuffle()
//--------------------------------------------------------------------
{
   cocos2d::log("ViewController::shuffle");
   mLevel->getObjectController()->removeAllCookies();
   mGameplayScene->removeAllCookieSprites();
   mGameplayScene->addSpritesForObjects(mLevel->shuffle());
   mDudeController->detectDirectionsForDudes();
}

//--------------------------------------------------------------------
void ViewController::handleMatches()
//--------------------------------------------------------------------
{
    cocos2d::log("ViewController::handleMatches");
    auto chains = mChainController->removeMatches();
    auto dudes = mDudeController->createDudeObectsFromChains(chains);
    mGameplayScene->addSpritesForObjects(dudes);

    if (chains->count() == 0) {
        mDudeController->detectDirectionsForDudes();
        beginNextTurn();
        return;
    }

    updateScore(chains);
    animateHandleMatches(chains);
}


//--------------------------------------------------------------------
void ViewController::animateHandleMatches(CommonTypes::Set* chains)
//--------------------------------------------------------------------
{
    CC_ASSERT(chains);

    mChainController->executeCollectGoalCallback(chains);

    auto completion = CallFunc::create([&]() {

        auto columns = mLevel->useGravityToFillHoles();
        auto newColumns = mLevel->fillTopUpHoles();
        auto enableTouches = CallFunc::create([&]() {
            handleMatches();
        });

        auto addNewCookies = CallFunc::create([&]() {
            updateInfoLabels();
        });
        AnimationsManager->animateNewCookies(newColumns, enableTouches);
        AnimationsManager->animateFallingObjects(columns, addNewCookies);
    });

    auto fieldObjects = mLevel->detectFieldObjects(chains);
    if (fieldObjects->count() > 0) {
        mChainController->addFieldOjbectsToChainSet(fieldObjects, chains);
    }
    AnimationsManager->animateMatching(chains, completion);
    AudioManager->playSound(SoundType::MatchSound);
}

//--------------------------------------------------------------------
void ViewController::beginNextTurn()
//--------------------------------------------------------------------
{
    cocos2d::log("ViewController::beginNextTurn");
    
    float delay = 0.01f;
    if (!mSwapController->detectPossibleSwaps()) {
        delay = 0.5f;
        shuffle();
    }

    auto callback = cocos2d::CallFunc::create([=]() {
        mGameplayScene->userInteractionEnabled();
    });

    mLevel->resetComboMultiplier();
    decrementMoves();
  
    mGameplayScene->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), callback, nullptr));
}

//--------------------------------------------------------------------
void ViewController::decrementMoves()
//--------------------------------------------------------------------
{
    cocos2d::log("ViewController::decrementMoves");
    mMovesLeft--;
    updateInfoLabels();
}

//--------------------------------------------------------------------
void ViewController::shuffleButtonCallback()
//--------------------------------------------------------------------
{
    cocos2d::log("ViewController::onShuffleButtonPressed");
    shuffle();
    decrementMoves();
}

//--------------------------------------------------------------------
void ViewController::swapCallback(SwapObj * swap)
//--------------------------------------------------------------------
{
    // disable touches on layer.
    mGameplayScene->userInteractionDisabled();

    auto swapCallback = CallFunc::create([=]() {
        handleMatches();
    });
    auto invalidSwapCallback = CallFunc::create([=]() {
        // enable touches on layer.
        mGameplayScene->userInteractionEnabled();
    });

    if (mSwapController->isPossibleSwap(swap)) {

        mSwapController->performSwap(swap);
        AnimationsManager->animateSwap(swap, swapCallback);
        AudioManager->playSound(SoundType::SwapSound);
    }
    else {
        AnimationsManager->animateInvalidSwap(swap, invalidSwapCallback);
        AudioManager->playSound(SoundType::InvalidSwapSound);
    }
}

//--------------------------------------------------------------------
void ViewController::activateDudeCallback(DudeObj * obj, int direction)
//--------------------------------------------------------------------
{
    auto set = mDudeController->activateDudeAndGetChains(obj, direction);
    auto chains = dynamic_cast<ChainObj*>(set->anyObject());
    if (chains) {
        if (chains->getChainObjects()) {
            mGameplayScene->userInteractionDisabled();

            mLevel->removeDudeMatches(set);
 
            updateScore(set);
            animateHandleMatches(set);
        }
    }
}

//--------------------------------------------------------------------
void ViewController::activateChainCallback(CommonTypes::ChainType & type, cocos2d::Vec2 & pos)
//--------------------------------------------------------------------
{
    cocos2d::log("ViewController::activateChainCallback");
    auto chains = mChainController->removeChainAt(type, pos);

    if (chains->count() > 0) {
        mGameplayScene->userInteractionDisabled();

        updateScore(chains);
        animateHandleMatches(chains);
    }    
}
