/**
* @file Utils/Parser/JsonParser.hpp
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

    CT::SGlobalInfo getJsonGlobalInfo();

    void parseScoreValues();
    bool checkScoreValuesStatus();

    void updateScoreValues();

    //---Level Info--------------------------------------------------

    void parseLevelInfo(const int16_t& level);
    bool checkLevelInfoStatus();

    CT::LevelInfo getJsonLevelInfo();
    CT::LevelGoals getJsonLevelGoals();
    //---------------------------------------------------------------

protected:

    //---Global Info--------------------------------------------------
    const char* getSwVersion() const;
    const char* getImagePackType() const;
    const char* getImagePackVersion() const;
    const double& getTileWidth() const;   
    const double& getTileHeight() const;
    const uint16_t& getLevelsCount() const;

    const uint16_t& getCookieMinimalScore() const;
    const uint16_t& getCookieDefaultScore() const;
    const uint16_t& getFieldObjScore() const;
    const uint16_t& getEnemyScore() const;
    void updateDudeScoreMap();

    const Json::Value& getScoreDudes();
    uint16_t getScoreDudeType(const Json::Value & node);
    uint16_t getScoreDudeValue(const Json::Value & node);

    Json::Value mScoreValuesRootNode;
    Json::Value mGlobalInfoRootNode;

    //---Level Info--------------------------------------------------
    void updateTiles(CT::LevelInfo& levelInfo);
    void updatePredefinedCookies(CT::LevelInfo& levelInfo);
    void updateAllowedCookieTypes(CT::LevelInfo& levelInfo);
    void updateFieldObjects(CT::LevelInfo& levelInfo);

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