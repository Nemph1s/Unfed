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
#include "Managers/AnimationsManager.h"
#include "Managers/AudioManager.h"

#include "GameObjects/LevelObj.h"
#include "Scenes/GameplayScene.h"

using cocos2d::Director;
using cocos2d::CallFunc;
using namespace CommonTypes;

#define COCOS2D_DEBUG 0

//--------------------------------------------------------------------
ViewController::ViewController()
   : mLevel(nullptr)
   , mGameplayScene(nullptr)
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
}

//--------------------------------------------------------------------
bool ViewController::init()
//--------------------------------------------------------------------
{
   cocos2d::log("ViewController::init");

   auto director = Director::getInstance();
   auto glview = director->getOpenGLView();

   auto animationMgr = AnimationsManager::getInstance();
   auto audioMgr = AudioManager::getInstance();
      
   // Create and configure the scene.
   mGameplayScene = GameplayScene::createWithSize(glview->getFrameSize());
   //self.scene.scaleMode = SKSceneScaleModeAspectFill;

   animationMgr->initWithScene(mGameplayScene);

   // Load the level.
   int levelId = 0;
   mLevel = LevelObj::createWithId(levelId);

   mScore = mLevel->getLevelInfo().targetScore;
   mMovesLeft = mLevel->getLevelInfo().moves;

   //TODO: create tags instead of name
   mLevel->setName("Level");

   mGameplayScene->setLevel(mLevel);
   mGameplayScene->addTiles();

   auto callback = [&](SwapObj* swap) {
       // disable touches on layer.
       mGameplayScene->userInteractionDisabled();

       auto swapCallback = CallFunc::create([=]() {
           handleMatches();
       });
       auto invalidSwapCallback = CallFunc::create([=]() {
           // enable touches on layer.
           mGameplayScene->userInteractionEnabled();
       });

       if (mLevel->isPossibleSwap(swap)) {

           mLevel->performSwap(swap);
           animationMgr->animateSwap(swap, swapCallback);
           audioMgr->playSound(SoundType::SwapSound);

       } else {

           animationMgr->animateInvalidSwap(swap, invalidSwapCallback);
           audioMgr->playSound(SoundType::InvalidSwapSound);

       }
   };

   mGameplayScene->setSwapCallback(callback);

   // Present the scene.
   director->runWithScene(mGameplayScene);

   startGame();

   return true;
}

//--------------------------------------------------------------------
void ViewController::startGame()
//--------------------------------------------------------------------
{
   cocos2d::log("ViewController::startGame");
   shuffle();
}

//--------------------------------------------------------------------
void ViewController::shuffle()
//--------------------------------------------------------------------
{
   cocos2d::log("ViewController::shuffle");
   auto newCookies = mLevel->shuffle();
   mGameplayScene->addSpritesForCookies(newCookies);
}

//--------------------------------------------------------------------
void ViewController::handleMatches()
//--------------------------------------------------------------------
{
    cocos2d::log("ViewController::handleMatches");
    auto chains = mLevel->removeMatches();

    if (chains->count() == 0) {
        beginNextTurn();
        return;
    }

    auto animationMgr = AnimationsManager::getInstance();
    auto audioMgr = AudioManager::getInstance();

    auto completion = CallFunc::create([=]() {

        auto columns = mLevel->useGravityToFillHoles();
        auto addNewCookies = CallFunc::create([=]() {

            auto newColumns = mLevel->fillTopUpHoles();
            auto enableTouches = CallFunc::create([=]() {
                handleMatches();                
            });

            animationMgr->animateNewCookies(newColumns, enableTouches);
        });

        animationMgr->animateFallingCookies(columns, addNewCookies);
    });
    
    animationMgr->animateMatching(chains, completion);
    audioMgr->playSound(SoundType::MatchSound);
}

//--------------------------------------------------------------------
void ViewController::beginNextTurn()
//--------------------------------------------------------------------
{
    mLevel->detectPossibleSwaps();
    mGameplayScene->userInteractionEnabled();
}
