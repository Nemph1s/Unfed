/**
* @file Utils/JsonParser.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"
#include "Common/CommonTypes.h"
#include "cocos2d/external/json/document.h"

class JsonParser
{
public:
   static JsonParser & Instance() {
      static JsonParser myInstance;
      return myInstance;
   }

   // delete copy and move constructors and assign operators
   JsonParser(JsonParser const&) = delete;             // Copy construct
   JsonParser(JsonParser&&) = delete;                  // Move construct
   JsonParser& operator=(JsonParser const&) = delete;  // Copy assign
   JsonParser& operator=(JsonParser &&) = delete;      // Move assign

   void parseLevelInfo(const int16_t& level);

   bool checkStatus();
   CommonTypes::LevelInfo getLevelInfo();

   const rapidjson::Value& getTiles();
   int16_t getTargetScore();
   int16_t getMoves();   

protected:
   JsonParser() {};
   ~JsonParser() {};

   rapidjson::Document mDoc;
   int16_t mLoadedLevel = -1;
};