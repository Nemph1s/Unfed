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
#include "GameObjects/Level/LevelTypes.h"
#include "Common/GlobalInfo/GlobalInfoTypes.h"

#include "Utils/PlatformMacros.h"

#include "libs/jsoncpp/include/json/json.h"

class _JsonParser
{
    CREATE_SINGLETON(_JsonParser);

public:
    //---Global Info--------------------------------------------------

    void parseGlobalInfo();
    bool checkGlobalInfoStatus();

    CommonTypes::JsonGlobalInfo getJsonGlobalInfo();

    //---Level Info--------------------------------------------------

    void parseLevelInfo(const int16_t& level);
    bool checkLevelInfoStatus();

    CommonTypes::LevelInfo getJsonLevelInfo();
    CommonTypes::LevelGoals getJsonLevelGoals();
    //---------------------------------------------------------------

protected:

    //---Global Info--------------------------------------------------

    Json::Value mGlobalInfoRootNode;

    //---Level Info--------------------------------------------------
    void updateTiles(CommonTypes::LevelInfo& levelInfo);
    void updatePredefinedCookies(CommonTypes::LevelInfo& levelInfo);
    void updateAllowedCookieTypes(CommonTypes::LevelInfo& levelInfo);
    void updateFieldObjects(CommonTypes::LevelInfo& levelInfo);

    const Json::Value& getTiles();
    const Json::Value& getPredefinedCookies();
    const Json::Value& getAllowedCookieTypes();
    uint32_t getTargetScore();
    uint8_t getMoves();
    uint8_t getTypesCount();
    bool isPredefinedCookies();
    bool getSkipEmptyHoles();

    uint8_t getGoalsCount(const Json::Value & node);
    const Json::Value& getGoalTypeCollect(const Json::Value & node);
    uint8_t getTargetBaseObjType(const Json::Value & node);
    uint8_t getTargetObjType(const Json::Value & node);
    uint8_t getTargetObjectCount(const Json::Value & node);

    const Json::Value& getFieldObjects(const Json::Value & node);
    uint8_t getFieldObjectCol(const Json::Value & node);
    uint8_t getFieldObjectRow(const Json::Value & node);
    const Json::Value& getFieldObjectTypes(const Json::Value & node);

    Json::Value mLevelInfoRootNode;
    int16_t mLoadedLevel = -1;
    //---------------------------------------------------------------

};

#define JsonParser _JsonParser::getInstance()