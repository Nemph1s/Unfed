/**
* @file GameObjects/Level/LevelTypes.h
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include <stdint.h>
#include <vector>
#include "Common/CommonTypes.h"
#include "Common/GlobalInfo/GlobalInfo.h"

namespace CT {

    typedef int TilesArray[_GlobalInfo::NumColumns][_GlobalInfo::NumRows];

    /**
    * @brief LevelInfo struct.
    */
    struct LevelInfo
    {
        uint32_t id = 0; /**< current level ID */
        uint32_t targetScore = 0; /**< Goal score */
        int moves = 0; /**< available moves */
        TilesArray tiles = { 0 }; /**< array of tiles */
        TilesArray cookies = { 0 }; /**< array of cookies */
        std::vector<CT::JsonFieldInfo> fieldObjects; /**< array of fieldObjects */
        std::vector<CT::JsonEnemyInfo> enemyObjects; /**< array of enemyObjects */
        int typesCount = 0; /**< count of types */
        bool isPredefinedCookies = false; /**< availability to load predefined cookies */
        bool skipEmptyHoles = false; /**< availability to skip empty tiles when obj falling */
        std::vector<int> allowedCookieTypes;
    };

    /**
    * @brief CollectGoalInfo struct.
    */
    struct CollectGoalInfo
    {
        int8_t baseObjectType = 0; /**< baseObjectType type of base field object(tile, cookie, field, dude) */
        int8_t objectType = 0; /**< objectType type of child field obj */
        int8_t targetCount = 0; /**< targetCount object count goal*/
        int8_t currentCount = 0; /**< currentCount object count goal*/
    };

    /**
    * @brief LevelInfo struct.
    */
    struct LevelGoals
    {
        int8_t goalsCount = 0; /**< goalsCount count current goals*/
        std::vector<CollectGoalInfo> collectGoals; /**< collectGoal vector of CollectGoalInfo struct*/
    };
}