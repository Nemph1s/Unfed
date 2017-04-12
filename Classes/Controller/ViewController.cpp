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
#include "Controller/SwapController/SwapController.h"
#include "Controller/ChainController/ChainController.h"
#include "Controller/ObjectController/Dude/DudeController.h"
#include "Controller/ObjectController/ObjectController.h"

#include "Managers/AnimationsManager.h"
#include "Managers/AudioManager.h"
#include "Managers/GuiManager.h"

#include "Common/Factory/SmartObjFactory.h"
#include "Common/Factory/SpritesFactory.h"

#include "Controller/SwapController/SwapObj.h"
#include "GameObjects/Level/LevelObj.h"
#include "GameObjects/Level/LevelGoalComponent.h"
#include "Controller/ObjectController/Dude/DudeObj.h"
#include "Controller/ChainController/ChainObj.h"

#include "Scenes/GameplayScene.h"

using cocos2d::Director;
using cocos2d::CallFunc;
using namespace CommonTypes;
using namespace std::placeholders;
 
#define COCOS2D_DEBUG 1
#define UNFED_ENABLE_DEBUG 1

const float showHintInterval = 10.0f;

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
    initSpritesFactory();
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

    SmartObjFactory->initTilesPool(_GlobalInfo::NumColumns * _GlobalInfo::NumRows);
    SmartObjFactory->initCookiesPool((_GlobalInfo::NumColumns * _GlobalInfo::NumRows) * 2);

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
bool ViewController::initSpritesFactory()
//--------------------------------------------------------------------
{
    SpritesFactory->setLevel(mLevel);
    SpritesFactory->initTilesPool(_GlobalInfo::NumColumns * _GlobalInfo::NumRows);
    auto cookiesPoolSize = (_GlobalInfo::NumColumns * _GlobalInfo::NumRows);
    SpritesFactory->initCookiesPool(cookiesPoolSize / 2);
    SpritesFactory->initDudesPool(_GlobalInfo::NumRows / 2);
    auto fieldObjsPoolSize = mLevel->getLevelInfo().fieldObjects.size();
    SpritesFactory->initFieldObjectsPool(fieldObjsPoolSize);
    SpritesFactory->initHintPool(_GlobalInfo::NumColumns * _GlobalInfo::NumRows);

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

    auto updateDirectionCallback = [dudeCtrl](BaseObj* obj, int direction) {
        auto dude = dynamic_cast<DudeObj*>(obj);
        auto set = dudeCtrl->getChainsForDude(dude, direction, true);
        CC_SAFE_RETAIN(set);
        return set;
    };
    mGameplayScene->setUpdateDirectionCallback(updateDirectionCallback);

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
   GlobInfo->resetComboMultiplier();

   shuffle();

   startHintTimer();
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
    auto prevSwapContainers = mSwapController->getPreviousSwapContainers();
    auto dudes = mDudeController->createDudeObectsFromChains(chains, prevSwapContainers);
    mGameplayScene->addSpritesForObjects(dudes);

    if (chains->count() == 0) {
        mDudeController->detectDirectionsForDudes();
        beginNextTurn();
        return;
    }

    updateScore(chains);
    animateHandleMatches(chains);
    GlobInfo->increaseComboMultiplier();
}


//--------------------------------------------------------------------
void ViewController::animateHandleMatches(CommonTypes::Set* chains)
//--------------------------------------------------------------------
{
    CC_ASSERT(chains);

    stopHintTimer();
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
    if (!mSwapController->detectPossibleSwaps() && mDudeController->getDudesCount() == 0) {
        delay = 0.5f;
        shuffle();
    }

    auto callback = cocos2d::CallFunc::create([=]() {
        mGameplayScene->userInteractionEnabled();

        startHintTimer();
    });

    GlobInfo->resetComboMultiplier();
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
    auto set = mDudeController->getChainsForDude(obj, direction);
    if (set) {
        auto chains = dynamic_cast<ChainObj*>(set->anyObject());
        if (chains) {
            if (chains->getChainObjects()) {
                stopHintTimer();
                mGameplayScene->userInteractionDisabled();

                mLevel->removeDudeMatches(set);

                updateScore(set);
                animateHandleMatches(set);
            }
        }
    }    
}

//--------------------------------------------------------------------
void ViewController::throwDownAnObject(BaseObj* obj, CommonTypes::CellPos destPos, bool isHeavyObject)
//--------------------------------------------------------------------
{
    auto objsToRemove = mObjectController->getObjectsForChain(destPos);
    if (!objsToRemove) {
        return;
    }
    CC_SAFE_RETAIN(objsToRemove);
    //TODO: refactor createDudeObject to set cellPos -1,-1 and dont add to container!
    auto onCompleteCallback = CallFunc::create([&]() {
        
        for (auto itObj = objsToRemove->begin(); itObj != objsToRemove->end(); ++itObj) {
            auto object = dynamic_cast<BaseObj*>(*itObj);
            CC_ASSERT(object);

            if (mObjectController->matchObject(object)) {
                AnimationsManager->animateMatchObj(object);
            }
            //TODO: for other special abilities add score animation
        }
        CC_SAFE_RELEASE_NULL(objsToRemove);

        mObjectController->updateObjectAt(destPos.column, destPos.row, obj);
        mObjectController->synchronizeObjectAt(destPos);
    });

    AnimationsManager->animateThrowDownAnObj(obj, destPos, onCompleteCallback, isHeavyObject);
}

//--------------------------------------------------------------------
void ViewController::startHintTimer()
//--------------------------------------------------------------------
{
    auto selector = schedule_selector(ViewController::showSwapHint);
    Director::getInstance()->getScheduler()->schedule(selector, this, showHintInterval, 0);
}

//--------------------------------------------------------------------
void ViewController::stopHintTimer()
//--------------------------------------------------------------------
{
    auto selector = schedule_selector(ViewController::showSwapHint);
    Director::getInstance()->getScheduler()->unschedule(selector, this);
}

//--------------------------------------------------------------------
void ViewController::showSwapHint(float dt)
//--------------------------------------------------------------------
{
    CommonTypes::Set* set = nullptr;
    if (mGameplayScene->isObjTouched()) {
        return;
    }
    cocos2d::log("ViewController::showSwapHint:");
    auto swapObj = dynamic_cast<SwapObj*>(mSwapController->getPossibleSwaps()->anyObject());
    if (swapObj) {
        auto detectChain = swapObj->getDetectHintChainCallback();
        if (detectChain) {

            mGameplayScene->userInteractionDisabled();
            set = detectChain();

            auto callback = CallFunc::create([=]() {
                // enable touches on layer.
                mGameplayScene->userInteractionEnabled();
            });

            if (set) {
                AnimationsManager->animateHintSwap(set, callback);
            } else {
                cocos2d::log("ViewController::showSwapHint: ERROR! cant detect chain set");
                mGameplayScene->userInteractionEnabled();
            }

        }               
    }
}
