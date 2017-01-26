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

#include <string.h>

using std::string;
using std::vector;

namespace GameResources {

   static string s_backgroundImg = "Images/Background0@2x.png";

   static vector<string> s_cookieSpriteNames {
      "Sprites.atlas/Croissant@2x.png"
      , "Sprites.atlas/Cupcake@2x.png"
      , "Sprites.atlas/Danish@2x.png" 
      , "Sprites.atlas/Donut@2x.png" 
      , "Sprites.atlas/Macaroon@2x.png" 
      , "Sprites.atlas/SugarCookie@2x.png" 
   };

   static vector<string> s_cookieHighlightedSpriteNames {
      "Sprites.atlas/Croissant-Highlighted@2x.png"
      , "Sprites.atlas/Cupcake-Highlighted@2x.png"
      , "Sprites.atlas/Danish-Highlighted@2x.png"
      , "Sprites.atlas/Donut-Highlighted@2x.png"
      , "Sprites.atlas/Macaroon-Highlighted@2x.png"
      , "Sprites.atlas/SugarCookie-Highlighted@2x.png"
   };
}