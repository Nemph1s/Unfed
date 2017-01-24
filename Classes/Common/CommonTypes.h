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

using std::int8_t;

namespace CommonTypes {

   /**
   * @brief CookieType enum.
   * Type of the cookie object
   */
   enum class CookieType : int8_t {
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
      int8_t column; /**< vertical series of cells in a table */
      int8_t row; /**< horizontal series of cells in a table */
      CookieType cookieType; /**< type of Cookie object */
   };

}