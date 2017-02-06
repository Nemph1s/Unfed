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

class LevelObj;
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

   void shuffle();
   void handleMatches();

   void beginNextTurn();

   CC_SYNTHESIZE_READONLY(int, mMovesLeft, MovesLeft);
   CC_SYNTHESIZE_READONLY(int, mScore, Score);

   CC_SYNTHESIZE_READONLY(LevelObj*, mLevel, Level);
   CC_SYNTHESIZE_READONLY(GameplayScene*, mGameplayScene, GameplayScene);
};

