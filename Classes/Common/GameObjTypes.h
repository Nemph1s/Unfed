/**
* @file Common/GameObjTypes.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "Common/CommonTypes.h"

namespace GOT {

    /**
    * @brief BaseObjType enum.
    * Type of the cookie object
    */
    enum class BaseObjType : int {
        Tile = 0 /**< enum value TileObj. */
        , Cookie = 1 /**< enum value CookieObj. */
        , Field = 2 /**< enum value FieldObj. */
        , Dude = 3 /**< enum value DudeObj. */
        , Enemy = 4 /**< enum value EnemyObj. */
        , Unknown /**< enum value unknown. */
    };

    /**
    * @brief BaseObjInfo struct.
    */
    struct BaseObjInfo
    {
        BaseObjType type; /**< type of game object */
        int8_t column; /**< vertical series of cells in a table */
        int8_t row; /**< horizontal series of cells in a table */

        BaseObjInfo(BaseObjType _type)
            : type(_type), column(-1), row(-1) {}
        BaseObjInfo(BaseObjType _type, CT::Cell _cell)
            : type(_type), column(_cell.column), row(_cell.row) {}
        BaseObjInfo(BaseObjType _type, int8_t _column, int8_t _row)
            : type(_type), column(_column), row(_row) {}
    };

    /**
    * @brief TileType enum.
    * Type of the sound object
    */
    enum class TileType : int {
        Empty = 0 /**< enum value Empty. */
        , Normal = 1 /**< enum value Normal. */
        , Water = 2 /**< enum value Water. */
        , Unknown /**< enum value unknown. */
    };

    /**
    * @brief TileInfo struct.
    */
    struct TileInfo
    {
        BaseObjInfo baseInfo; /**< type of GaneInfoObject struct */
        TileType tileType; /**< type of Tile object */
    };

    /**
    * @brief CookieType enum.
    * Type of the cookie object
    */
    enum class CookieType : int {
        Croissant = 0 /**< enum value Croissant. */
        , Cupcake = 1 /**< enum value Cupcake. */
        , Danish = 2 /**< enum value Danish. */
        , Donut = 3 /**< enum value Donut. */
        , Macaron = 4 /**< enum value Macaron. */
        , SugarCookie = 5 /**< enum value SugarCookie. */
        , CookieMax = 6 /**< enum value of number cookie types. */
        , Unknown /**< enum value unknown. */
    };

    /**
    * @brief CookieInfo struct.
    */
    struct CookieInfo
    {
        BaseObjInfo baseInfo; /**< type of GaneInfoObject struct */
        CookieType cookieType; /**< type of Cookie object */
    };

    enum class FieldType : int {
        Dirt = 10 /**< enum value Dirt. */
        , Dirt_HP2 = 11 /**< enum value DirtX2. */
        , Dirt_HP3 = 12 /**< enum value DirtX3. */
        , Bush = 20 /**< enum value DirtX3. */
        , Bush_HP2 = 21 /**< enum value DirtX3. */
        , RockWall = 30 /**< enum value RockWall. */
        , DudeFromAToB = 200 /**< enum value DudeFromAToB. */
        , DudeFromAToBx3 = 201 /**< enum value DudeFromAToBx3. */
        , DudeChainX = 202 /**< enum value DudeChainX. */
        , DudeAllOfType = 203 /**< enum value DudeAllOfType. */
        , DudeSquareBomb = 204 /**< enum value DudeSquareBomb. */
        , Unknown /**< enum value unknown. */
    };

    /**
    * @brief FieldInfo struct.
    */
    struct FieldInfo
    {
        BaseObjInfo baseInfo; /**< type of BaseObjectInfo struct */
        FieldType fieldType; /**< type of Field object */
        int priority; /**< priority for z level of Field object. 0 - low priority; 3 - high priority */
    };

    /**
    * @brief EnemyType enum.
    * Type of the enemy object
    */
    enum class EnemyType : int {
        Simple = 0 /**< Simple enemy. It can be removed by match nearest cookie 2 times, or by Dude */
        , Shielded = 1 /**< Shielded enemy. It can be removed only by Dude */
        , Unknown /**< enum value unknown. */
    };

    /**
    * @brief CookieInfo struct.
    */
    struct EnemyInfo
    {
        BaseObjInfo baseInfo; /**< type of GaneInfoObject struct */
        EnemyType enemyType; /**< type of Cookie object */
    };

    /**
    * @brief JsonFieldInfo struct.
    */
    struct JsonFieldInfo
    {
        BaseObjInfo baseInfo = BaseObjInfo(BaseObjType::Field); /**< type of BaseObjectInfo struct */
        std::vector<int> fieldType; /**< type of Field object */
    };

    /**
    * @brief JsonEnemyInfo struct.
    */
    struct JsonEnemyInfo
    {
        BaseObjInfo baseInfo = BaseObjInfo(BaseObjType::Enemy); /**< type of BaseObjectInfo struct */
        int16_t enemyType; /**< type of Field object */
    };

    /**
    * @brief ChainType enum.
    * Type of the sound object
    */
    enum class ChainType : int {
        ChainTypeHorizontal = 0 /**< enum value ChainTypeHorizontal. */
        , ChainTypeVertical = 1 /**< enum value ChainTypeVertical. */
        , ChainTypeL = 2 /**< enum value ChainTypeL. */
        , ChainTypeT = 3 /**< enum value ChainTypeT. */
        , ChainTypeX = 4 /**< enum value ChainTypeX. */
        , ChainTypeAllOfOne = 5 /**< enum value ChainTypeAllOfOne. */
        , ChainExplosion = 7 /**< enum value ChainExplosion. */
        , ChainFromAToB = 6 /**< enum value ChainTypeAllOfOne. */
        , ChainMathcedObjects = 7 /**< enum value ChainFieldObjects. */
        , ChainCircle = 8 /**< enum value ChainCircle. */
        , Unknown /**< enum value unknown. */
    };
}