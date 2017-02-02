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
#include "Utils/PlatformMacros.h"

class Helper 
{
    CREATE_SINGLETON(Helper);

public:

    template <typename E>
    static constexpr int to_underlying(E e) noexcept {

        return static_cast<std::underlying_type_t<E>>(e);
    }

    static int random(int fromRange, int toRange);
    static CommonTypes::CookieType randomCookieType(int fromRange, int toRange);
};
