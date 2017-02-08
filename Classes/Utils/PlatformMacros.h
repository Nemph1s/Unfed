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

/** @def CC_SYNTHESIZE_PTR
* It is used to declare a protected variable.
* We can use getter to read the variable, and use the setter to change the variable.
*
* @param varType     The type of variable.
* @param varName     Variable name.
* @param funName     "get + funName" will be the name of the getter.
*                    "set + funName" will be the name of the setter.
* @warning   The getter and setter are public inline functions.
*            The variables and methods declared after CC_SYNTHESIZE are all public.
*            If you need protected or private, please declare.
*/
#define CC_SYNTHESIZE_PTR(varType, varName, funName)\
protected: varType varName = nullptr;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

/** @def CC_SYNTHESIZE_READONLY_PTR
* It is used to declare a protected variable. We can use getter to read the variable.
*
* @param varType     The type of variable.
* @param varName     Variable name.
* @param funName     "get + funName" will be the name of the getter.
* @warning   The getter is a public inline function.
*            The variables and methods declared after CC_SYNTHESIZE_READONLY are all public.
*            If you need protected or private, please declare.
*/
#define CC_SYNTHESIZE_READONLY_PTR(varType, varName, funName)\
protected: varType varName = nullptr;\
public: virtual varType get##funName(void) const { return varName; }