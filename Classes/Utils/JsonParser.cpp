/**
* @file Utils/JsonParser.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/


#include "JsonParser.h"
#include "Utils/GameResources.h"

#include "cocos2d/external/json/document.h"
#include "cocos2d/external/json/rapidjson.h"

using namespace rapidjson;

//--------------------------------------------------------------------
CommonTypes::LevelInfo JsonParser::getLevelInfo(const int16_t& level)
//--------------------------------------------------------------------
{
   CCLOGINFO("JsonParser::getLevelInfo: loading level=%d", level);
   auto fullPath = cocos2d::StringUtils::format(GameResources::s_templateLevelName.c_str(), level);
   auto json = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath.c_str());
   CCLOGINFO("JsonParser::getLevelInfo: loading json=%s", json.c_str());

   auto levelInfo = CommonTypes::LevelInfo();
   Document jsonDict;
   jsonDict.Parse(json.c_str());

   if (jsonDict.HasMember("tiles")) {
      const Value& tiles = jsonDict["tiles"].GetArray();
      CC_ASSERT(tiles.IsArray());

      for (SizeType i = 0; i < tiles.Size(); i++) {
         CC_ASSERT(tiles[i].IsArray());

         for (SizeType j = 0; j < tiles[i].Size(); j++) {
            const Value& tile = tiles[i][j];
            levelInfo.tiles[i][j] = tile.GetInt();
         }
      }
   }

   if (jsonDict.HasMember("targetScore")) {
      CC_ASSERT(jsonDict["targetScore"].IsInt());
      levelInfo.targetScore = jsonDict["targetScore"].GetInt();
   }

   if (jsonDict.HasMember("moves")) {
      CC_ASSERT(jsonDict["moves"].IsInt());
      levelInfo.moves = jsonDict["moves"].GetInt();
   }

   return levelInfo;
}
