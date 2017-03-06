/**
* @file GameObjects/TileObjects/FieldObjects/Base/FieldTypes.h
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "Common/CommonTypes.h"

namespace ObjTypes {

    /**
    * @brief FieldType enum.
    * Type of the field object
    */
    enum class FieldType : int {
        Empty = 0 /**< enum value Empty. */
        , Dirt = 10 /**< enum value Dirt. */
        , Dirt_HP2 = 11 /**< enum value DirtX2. */
        , Dirt_HP3 = 12 /**< enum value DirtX3. */
        , Bush = 20 /**< enum value DirtX3. */
        , Bush_HP2 = 21 /**< enum value DirtX3. */
        , RockWall = 30 /**< enum value RockWall. */
        , DudeFromAToB = 200 /**< enum value DudeLightr. */
        , DudeFromAToBx3 = 201 /**< enum value DudeBulbr. */
        , DudeChainX = 202 /**< enum value DudeOni. */
        , DudeAllOfType = 203 /**< enum value DudePina. */
        , Unknown /**< enum value unknown. */
    };

    /**
    * @brief FieldInfo struct.
    */
    struct FieldInfo
    {
        CommonTypes::BaseObjectInfo baseInfo; /**< type of BaseObjectInfo struct */
        FieldType fieldType; /**< type of Field object */
    };

    /**
    * @brief FieldInfo struct.
    */
    struct FieldJsonInfo
    {
        CommonTypes::BaseObjectInfo baseInfo = CommonTypes::BaseObjectInfo(BaseObjectType::FieldObj); /**< type of BaseObjectInfo struct */
        std::vector<int> fieldType; /**< type of Field object */
    };
}