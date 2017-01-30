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

//--------------------------------------------------------------------
ViewController::ViewController()
   : mLevel(nullptr)
   , mGameplayScene(nullptr)
   //--------------------------------------------------------------------
{
   CCLOGINFO("ViewController::ViewController");
}

//--------------------------------------------------------------------
ViewController::~ViewController()
//--------------------------------------------------------------------
{
   CCLOGINFO("ViewController::~ViewController");
   mLevel->release();
}

//--------------------------------------------------------------------
bool ViewController::init()
//--------------------------------------------------------------------
{
   CCLOGINFO("ViewController::init");

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
       mGameplayScene->userInteractionDisabled();
       auto funcCallAction = cocos2d::CallFunc::create([=]() {
           mGameplayScene->userInteractionEnabled();
       });
       mLevel->performSwap(swap);
       mGameplayScene->animateSwap(swap, funcCallAction);
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
   CCLOGINFO("ViewController::startGame");
   shuffle();
}

//--------------------------------------------------------------------
void ViewController::shuffle()
//--------------------------------------------------------------------
{
   CCLOGINFO("ViewController::shuffle");
   auto newCookies = mLevel->shuffle();
   mGameplayScene->addSpritesForCookies(newCookies);
}
