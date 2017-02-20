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
      TilesArray fieldObjects; /**< array of tileObjects */
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
       , SandWall = 10 /**< enum value SandWall. */
       , Dirt = 11 /**< enum value Dirt. */
       , Dirt_HP2 = 12 /**< enum value DirtX2. */
       , Dirt_HP3 = 13 /**< enum value DirtX3. */
       , Bush = 21 /**< enum value DirtX3. */
       , Bush_HP2 = 22 /**< enum value DirtX3. */
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