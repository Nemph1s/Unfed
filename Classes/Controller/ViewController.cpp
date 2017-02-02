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

#include "GameObjects/LevelObj.h"
#include "Scenes/GameplayScene.h"

using cocos2d::Director;
using cocos2d::CallFunc;

#define COCOS2D_DEBUG 1

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
      
   // Create and configure the scene.
   mGameplayScene = GameplayScene::createWithSize(glview->getFrameSize());
   //self.scene.scaleMode = SKSceneScaleModeAspectFill;

   // Load the level.
   int levelId = 1;
   mLevel = LevelObj::createWithId(levelId);

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
           mGameplayScene->animateSwap(swap, swapCallback);
       } else {
           mGameplayScene->animateInvalidSwap(swap, invalidSwapCallback);
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

    auto completionCallback = CallFunc::create([=]() {
        mGameplayScene->userInteractionEnabled();
    });
    
    mGameplayScene->animateMatching(chains, completionCallback);
}
