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
      int column; /**< vertical series of cells in a table */
      int row; /**< horizontal series of cells in a table */
      CookieType cookieType; /**< type of Cookie object */
   };

   //temporary variables
   //TODO: move to globalInfo or to json file
   static const int NumColumns = 9;
   static const int NumRows = 9;

   typedef int TilesArray[CommonTypes::NumColumns][CommonTypes::NumRows];

   /**
   * @brief LevelInfo struct.
   */
   struct LevelInfo
   {
      int16_t id; /**< current level ID */
      int16_t targetScore; /**< Goal score */
      int moves; /**< available moves */
	  TilesArray tiles; /**< array of tiles */
   };

   /**
   * @brief ChainType enum.
   * Type of the sound object
   */
   enum class ChainType : int {
       ChainTypeHorizontal = 0 /**< enum value ChainTypeHorizontal. */
       , ChainTypeVertical = 1 /**< enum value ChainTypeVertical. */
       , ChainTypeL = 2 /**< enum value ChainTypeL. */
       , ChainTypeT = 3 /**< enum value ChainTypeL. */
       , Unknown /**< enum value unknown. */
   };

   /**
   * @brief TileType enum.
   * Type of the sound object
   */
   enum class TileType : int {
       Empty = 0 /**< enum value Empty. */
       , Normal = 1 /**< enum value Normal. */
       , RemovableX1 = 2 /**< enum value RemovableX1. */
       , RemovableX2 = 3 /**< enum value ChainTypeL. */
       , Unknown /**< enum value unknown. */
   };

   /**
   * @brief TileInfo struct.
   */
   struct TileInfo
   {
       int column; /**< vertical series of cells in a table */
       int row; /**< horizontal series of cells in a table */
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
   struct LabelInfo
   {
       const char* text; /**< text of label */
       int8_t fontSize; /**< font size */
       float posXPercent; /**< position x in Percent */
       float posYPercent; /**< position Y in Percent */
   };

}