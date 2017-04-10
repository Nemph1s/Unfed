/**
* @file Common/GlobalInfo/GlobalInfoTypes.h
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include <stdint.h>

namespace CommonTypes {

    /**
    * @brief SGlobalInfo struct.
    */
    struct SGlobalInfo
    {
        const char* swVersion = nullptr; /**< Software Version */
        const char* imgPackType = nullptr; /**< Image Pack Type (SD/HD/HDR) */
        const char* imgPackVersion = nullptr; /**< Image Pack Version */
        double tileWidth = 0.0f; /**< width of the Tile */
        double tileHeigth = 0.0f; /**< height of the Tile */
        uint16_t levelsCount = 0; /**< actual level count */
    };
    /**
    * @brief SScoreValues struct.
    */
    struct SScoreValues
    {
        uint16_t cookieMinimal = 0; /**< cookieMinimal score (using for dude chains) */
        uint16_t cookieDefault = 0; /**< cookieDefault score */
        uint16_t fieldObject = 0; /**< fieldObject score */
        uint16_t enemyObject = 0; /**< enemyObject score */
        std::map<uint16_t, uint16_t> dudeMap; /**< dude Score Map */
    };
}