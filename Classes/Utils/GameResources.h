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
#include <string.h>
#include <vector>

namespace GameResources {

    //TODO: move to global info or to json file
    static const float TileWidth = 32.0 * 2;
    static const float TileHeight = 36.0 * 2;

   static std::string s_backgroundImg = "Images/Background2@2x.jpg";

   static std::string s_TileImg = "Sprites.atlas/Tile@2x.png";

   static std::vector<std::string> s_cookieSpriteNames {
      "Sprites.atlas/Croissant@2x.png"
      , "Sprites.atlas/Cupcake@2x.png"
      , "Sprites.atlas/Danish@2x.png" 
      , "Sprites.atlas/Donut@2x.png" 
      , "Sprites.atlas/Macaroon@2x.png" 
      , "Sprites.atlas/SugarCookie@2x.png" 
   };

   static std::vector<std::string> s_cookieHighlightedSpriteNames {
      "Sprites.atlas/Croissant-Highlighted@2x.png"
      , "Sprites.atlas/Cupcake-Highlighted@2x.png"
      , "Sprites.atlas/Danish-Highlighted@2x.png"
      , "Sprites.atlas/Donut-Highlighted@2x.png"
      , "Sprites.atlas/Macaroon-Highlighted@2x.png"
      , "Sprites.atlas/SugarCookie-Highlighted@2x.png"
   };
   
   static std::string s_templateLevelName = "Levels/Level_%d.json";
   //TODO: move to config file
   static int16_t s_levelsCount = 4;

   static std::vector<std::string> s_gameplaySounds{
       "Sounds/Chomp.wav"
       , "Sounds/Drip.wav"
       , "Sounds/Error.wav"
       , "Sounds/Ka-Ching.wav"
       , "Sounds/Scrape.wav"
   };

   static std::string s_gameplaySoundBG= "Sounds/erokia_elementary-wave.mp3";

   static std::string s_fontYellow = "fonts/YELLOW.TTF";
   static std::string s_fontYellowExpanded = "fonts/Yellowe.ttf";
   static std::string s_fontYellowCondensed = "fonts/Yellowc.ttf";
   static std::string s_fontYellowItalic = "fonts/Yellowi.ttf";
   static std::string s_fontYellowShadow = "fonts/Yellows.ttf";

}