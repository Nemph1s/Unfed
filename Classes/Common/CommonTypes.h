/**
* @file Common/CommonTypes.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include <stdint.h>
#include <string.h>
#include <vector>

namespace cocos2d { class __Set; }

namespace CommonTypes {

    typedef cocos2d::__Set Set;

    /**
    * @brief GameObjectType enum.
    * Type of the cookie object
    */
    enum class BaseObjType : int {
        Tile = 0 /**< enum value TileObj. */
        , Cookie = 1 /**< enum value CookieObj. */
        , Field = 2 /**< enum value FieldObj. */
        , Dude = 3 /**< enum value DudeObj. */
        , Unknown /**< enum value unknown. */
    };

    /**
    * @brief GameObjectInfo struct.
    */
    struct BaseObjInfo
    {
        BaseObjType type; /**< type of game object */
        int column; /**< vertical series of cells in a table */
        int row; /**< horizontal series of cells in a table */

        BaseObjInfo(BaseObjType _type) 
            : type(_type), column(-1), row(-1) {}
        BaseObjInfo(BaseObjType _type, int _column, int _row) 
            : type(_type), column(_column), row(_row) {}
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
       , DudeFromAToB = 200 /**< enum value DudeLightr. */
       , DudeFromAToBx3 = 201 /**< enum value DudeBulbr. */
       , DudeChainX = 202 /**< enum value DudeOni. */
       , DudeAllOfType = 203 /**< enum value DudePina. */
       , DudeSquareBomb = 204 /**< enum value DudeMBO. */
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
   * @brief FieldInfo struct.
   */
   struct JsonFieldInfo
   {
       BaseObjInfo baseInfo = BaseObjInfo(BaseObjType::Field); /**< type of BaseObjectInfo struct */
       std::vector<int> fieldType; /**< type of Field object */
   };

   /**
   * @brief ChainType enum.
   * Type of the sound object
   */
   enum class SearchEmptyHoles : int {
       ObjFounded = 0 /**< enum value ObjFounded. */
       , ContinueSearch = 1 /**< enum value ContinueSearch. */
       , BreakSearch = 2 /**< enum value BreakSearch. */
       , Unknown /**< enum value unknown. */
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
       , ChainFromAToB = 6 /**< enum value ChainTypeAllOfOne. */
       , ChainFieldObjects = 7 /**< enum value ChainFieldObjects. */
       , Unknown /**< enum value unknown. */
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
   * @brief Direction enum.
   * Type of direction
   */
   enum class Direction : int {
       Down = 0 /**< enum value Down. */
       , Up = 1 /**< enum value Up. */
       , Left = 2 /**< enum value Left. */
       , Right = 3 /**< enum value Right. */
       , Unknown /**< enum value unknown. */
   };

   /**
   * @brief SoundType enum.
   * Type of the sound object
   */
   enum class SoundType : int {
       SwapSound  = 0 /**< enum value Chomp. */
       , AddCookieSound = 1 /**< enum value Drip. */
       , InvalidSwapSound = 2 /**< enum value Error. */
       , MatchSound = 3 /**< enum value KaChing. */
       , FallingCookieSound = 4 /**< enum value Scrape. */
       , SoundsMax = 5 /**< enum value of sounds number. */
       , Unknown /**< enum value unknown. */
   };

   /**
   * @brief LevelInfo struct.
   */
   struct TextLabelInfo
   {
       const char* text; /**< text of label */
       int8_t fontSize; /**< font size */
       float posXPercent; /**< position x in Percent */
       float posYPercent; /**< position Y in Percent */
   };

}