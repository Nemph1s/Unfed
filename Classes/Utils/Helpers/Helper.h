/**
* @file Utils/Helpers/Helper.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "Common/CommonTypes.h"
#include <random>

class Helper 
{
public:
    static Helper & Instance() {
        static Helper myInstance;
        return myInstance;
    }
    // delete copy and move constructors and assign operators
    Helper(Helper const&) = delete;             // Copy construct
    Helper(Helper&&) = delete;                  // Move construct
    Helper& operator=(Helper const&) = delete;  // Copy assign
    Helper& operator=(Helper &&) = delete;      // Move assign

   template <typename E>
   static constexpr int to_underlying(E e) noexcept {

      return static_cast<std::underlying_type_t<E>>(e);
   }

   static int random(int fromRange, int toRange) {

       std::random_device rd;
       std::mt19937 gen(rd());    
       std::uniform_int_distribution<> uniform_dist(fromRange, toRange);
       return uniform_dist(gen);
   }

   static CommonTypes::CookieType randomCookieType(int fromRange, int toRange) {

       int randValue = random(fromRange, toRange);
       return static_cast<CommonTypes::CookieType>(randValue);
   }

protected:
    Helper() {};
    ~Helper() {};
};
