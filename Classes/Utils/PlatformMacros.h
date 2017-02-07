/**
* @file Utils/PlatformMacros.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

/** @def CREATE_SINGLETON(__TYPE__)
* Define a singleton functions for a specific type
*
* @param __TYPE__  class type
*/
#define CREATE_SINGLETON(__TYPE__) \
    public: \
    static __TYPE__& Instance() \
    { \
        static __TYPE__ myInstance; \
        return myInstance; \
    } \
    static __TYPE__* getInstance() \
    { \
        return &__TYPE__::Instance(); \
    } \
    protected: \
    __TYPE__ () {}; \
    __TYPE__ (__TYPE__ const&) = delete; \
    __TYPE__ ( __TYPE__ &&) = delete; \
    __TYPE__ & operator=( __TYPE__ const&) = delete; \
    __TYPE__ & operator=( __TYPE__ &&) = delete; \
    ~ __TYPE__ () {}; 