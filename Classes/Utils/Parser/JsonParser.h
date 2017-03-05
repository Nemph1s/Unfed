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
   const Json::Value& getPredefinedCookies();
   const Json::Value& getAllowedCookieTypes();
   const Json::Value& getFieldObjects();
   uint32_t getTargetScore();
   uint8_t getMoves();   
   uint8_t getTypesCount();
   bool isPredefinedCookies();
   bool getSkipEmptyHoles();

   CommonTypes::LevelGoals getLevelGoals();

   uint8_t getGoalsCount(const Json::Value & node);
   const Json::Value& getGoalTypeCollect(const Json::Value & node);
   uint8_t getTargetBaseObjType(const Json::Value & node);
   uint8_t getTargetObjType(const Json::Value & node);
   uint8_t getTargetObjectCount(const Json::Value & node);

protected:

   Json::Value mRootNode;
   int16_t mLoadedLevel = -1;
};

#define JsonParser _JsonParser::getInstance()