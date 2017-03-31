/**
* @file Utils/GameResources.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "stdint.h"
#include <vector>
#include "cocos2d.h"

namespace GameResources {

    //TODO: move to global info or to json file
    static const float TileWidth = 34.0 * 2;
    static const float TileHeight = 38.0 * 2;

   static cocos2d::String s_backgroundImg = "Images/Background1@2x.png";

   static cocos2d::String s_ButtonImg = "Images/Button@2x.png";

   static cocos2d::String s_scoreBarBackgroundImg = "Images/ProgressBarBack@2x.png";
   static cocos2d::String s_scoreBarForegroundImg = "Images/ProgressBarFront@2x.png";

   static cocos2d::String s_TileImg = "Sprites.atlas/TileShadow@2x.png";

   static cocos2d::String s_HintImg = "Sprites.atlas/HintChainPreview@2x.png";

   static cocos2d::String s_RockImg = "Sprites.atlas/Rock@2x.png";

   static cocos2d::String s_DirtImg = "Sprites.atlas/Dirt@2x.png";
   static cocos2d::String s_DirtX2Img = "Sprites.atlas/DirtX2@2x.png";

   static cocos2d::String s_BushCorruptedImg = "Sprites.atlas/Bush1@2x.png";
   static cocos2d::String s_BushNormalImg = "Sprites.atlas/Bush2@2x.png";

   static std::vector<cocos2d::String> s_dudeSpriteNames{
       "Sprites.atlas/MonsterLighthr@2x.png"
       , "Sprites.atlas/MonsterBulbr@2x.png"
       , "Sprites.atlas/MonsterOni@2x.png"
       , "Sprites.atlas/MonsterPina@2x.png"
   };

   static std::vector<cocos2d::String> s_cookieSpriteNames {
      "Sprites.atlas/Croissant@2x.png"
      , "Sprites.atlas/Cupcake@2x.png"
      , "Sprites.atlas/Danish@2x.png" 
      , "Sprites.atlas/Donut@2x.png" 
      , "Sprites.atlas/Macaroon@2x.png" 
      , "Sprites.atlas/SugarCookie@2x.png" 
   };

   static std::vector<cocos2d::String> s_cookieHighlightedSpriteNames {
      "Sprites.atlas/Croissant-Highlighted@2x.png"
      , "Sprites.atlas/Cupcake-Highlighted@2x.png"
      , "Sprites.atlas/Danish-Highlighted@2x.png"
      , "Sprites.atlas/Donut-Highlighted@2x.png"
      , "Sprites.atlas/Macaroon-Highlighted@2x.png"
      , "Sprites.atlas/SugarCookie-Highlighted@2x.png"
   };
   
   static cocos2d::String s_templateLevelName = "Levels/Level_%d.json";
   //TODO: move to config file
   static int16_t s_levelsCount = 4;

   static std::vector<cocos2d::String> s_gameplaySounds{
       "Sounds/Chomp.wav"
       , "Sounds/Drip.wav"
       , "Sounds/Error.wav"
       , "Sounds/Ka-Ching.wav"
       , "Sounds/Scrape.wav"
   };

   static cocos2d::String s_gameplaySoundBG= "Sounds/erokia_elementary-wave.mp3";

   static cocos2d::String s_fontYellow = "fonts/YELLOW.TTF";
   static cocos2d::String s_fontYellowExpanded = "fonts/Yellowe.ttf";
   static cocos2d::String s_fontYellowCondensed = "fonts/Yellowc.ttf";
   static cocos2d::String s_fontYellowItalic = "fonts/Yellowi.ttf";
   static cocos2d::String s_fontYellowShadow = "fonts/Yellows.ttf";

}