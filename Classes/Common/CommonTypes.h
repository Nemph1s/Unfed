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

}