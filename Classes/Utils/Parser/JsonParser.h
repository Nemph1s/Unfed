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
#include "Utils/PlatformMacros.h"

#include "libs/jsoncpp/include/json/json.h"

class _JsonParser
{
    CREATE_SINGLETON(_JsonParser);

public:
   void parseLevelInfo(const int16_t& level);

   bool checkStatus();
   CommonTypes::LevelInfo getLevelInfo();

   const Json::Value& getTiles();
   int16_t getTargetScore();
   int16_t getMoves();   
   int16_t getTypesCount();

protected:

   Json::Value mRootNode;
   int16_t mLoadedLevel = -1;
};

#define JsonParser _JsonParser::getInstance()