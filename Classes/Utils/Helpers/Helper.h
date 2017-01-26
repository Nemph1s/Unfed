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

#include <stdint.h>
#include <random>

namespace Helper {

   template <typename E>
   constexpr int8_t to_underlying(E e) noexcept {

      return static_cast<std::underlying_type_t<E>>(e);
   }
}
