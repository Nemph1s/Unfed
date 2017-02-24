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

namespace CommonTypes {

    //temporary variables
    //TODO: move to globalInfo or to json file
    static const int NumColumns = 9;
    static const int NumRows = 9;

    typedef int TilesArray[CommonTypes::NumColumns][CommonTypes::NumRows];

    /**
    * @brief GameObjectType enum.
    * Type of the cookie object
    */
    enum class BaseObjectType : int {
        TileObj = 0 /**< enum value TileObj. */
        , CookieObj = 1 /**< enum value CookieObj. */
        , FieldObj = 2 /**< enum value FieldObj. */
        , DudeObj = 3 /**< enum value DudeObj. */
        , Unknown /**< enum value unknown. */
    };

    /**
    * @brief GameObjectInfo struct.
    */
    struct BaseObjectInfo
    {
        BaseObjectType type; /**< type of game object */
        int column; /**< vertical series of cells in a table */
        int row; /**< horizontal series of cells in a table */

        BaseObjectInfo(BaseObjectType _type) 
            : type(_type), column(-1), row(-1) {}
        BaseObjectInfo(BaseObjectType _type, int _column, int _row) 
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
       BaseObjectInfo baseInfo; /**< type of GaneInfoObject struct */
       CookieType cookieType; /**< type of Cookie object */
   };

   /**
   * @brief LevelInfo struct.
   */
   struct LevelInfo
   {
      uint32_t id; /**< current level ID */
      uint32_t targetScore; /**< Goal score */
      int moves; /**< available moves */
	  TilesArray tiles; /**< array of tiles */
      int typesCount; /**< count of types */
      bool skipEmptyHoles; /**< availability to skip empty tiles when obj falling */
      TilesArray fieldObjects; /**< array of tileObjects */
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
       , Dirt = 10 /**< enum value Dirt. */
       , Dirt_HP2 = 11 /**< enum value DirtX2. */
       , Dirt_HP3 = 12 /**< enum value DirtX3. */
       , Bush = 20 /**< enum value DirtX3. */
       , Bush_HP2 = 21 /**< enum value DirtX3. */
       , RockWall = 30 /**< enum value RockWall. */
       , DudeLightr = 200 /**< enum value DudeLightr. */
       , DudeBulbr = 201 /**< enum value DudeLightr. */
       , Unknown /**< enum value unknown. */
   };

   /**
   * @brief TileInfo struct.
   */
   struct TileInfo
   {
       BaseObjectInfo baseInfo; /**< type of GaneInfoObject struct */
       TileType tileType; /**< type of Tile object */
   };

   /**
   * @brief Direction enum.
   * Type of direction
   */
   enum Direction : int {
       Down = 0 /**< enum value Down. */
       , Up = 1 /**< enum value Up. */
       , Left = 2 /**< enum value Left. */
       , Right = 3 /**< enum value Right. */
       , MaxDirections = 4
   };

   /**
   * @brief DudePathType enum.
   * Type of chain path for dude
   */
   enum class DudePathType : int {
       DudePathFromAToB = 0 /**< enum value DudePathFromAToB. */
       , DudePathHorizontal = 1 /**< enum value DudePathHorizontal. */
       , DudePathVertical = 2 /**< enum value DudePathVertical. */
       , DudePathTypeX = 3 /**< enum value DudePathTypeX. */
       , DudePathAllOfOne = 4 /**< enum value DudePathAllOfOne. */
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