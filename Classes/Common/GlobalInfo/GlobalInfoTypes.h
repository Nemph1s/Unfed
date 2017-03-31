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
    * @brief JsonGlobalInfo struct.
    */
    struct JsonGlobalInfo
    {
        const char* swVersion = nullptr; /**< Software Version */
        const char* imgPackType = nullptr; /**< Image Pack Type (SD/HD/HDR) */
        const char* imgPackVersion = nullptr; /**< Image Pack Version */
        float tileWidth = 0.0f; /**< width of the Tile */
        float tileHeigth = 0.0f; /**< height of the Tile */
        uint16_t levelsCount = 0; /**< actual level count */
    };
}