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

#define COCOS2D_DEBUG 1
//#define UNFED_ENABLE_DEBUG 1

namespace cocos2d { class __Set; }

namespace CT {

    typedef cocos2d::__Set Set;

    /**
    * @brief Cell struct.
    */
    struct Cell
    {
        int8_t column; /**< horizontal cell in a table */
        int8_t row; /**< vertical cell in a table */

        Cell()
            : column(-1), row(-1) {}
        Cell(int8_t _column, int8_t _row)
            : column(_column), row(_row) {}
        Cell(Cell& _cell, int8_t _row)
            : column(_cell.column), row(_row) {}
        Cell(int8_t _column, Cell& _cell)
            : column(_column), row(_cell.row) {}
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