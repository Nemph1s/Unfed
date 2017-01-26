/**
* @file Scenes/GameplayScene.cpp
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
   mLevel = LevelObj::create();
   mGameplayScene->setLevel(mLevel);

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
