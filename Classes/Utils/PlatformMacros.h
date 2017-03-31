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

/** @def SYNTHESIZE_BOOL_READONLY
* It is used to declare a protected variable. We can use getter to read the variable.
*
* @param varType     The type of variable.
* @param varName     Variable name.
* @param funName     "get + funName" will be the name of the getter.
* @warning   The getter is a public inline function.
*            The variables and methods declared after CC_SYNTHESIZE_READONLY are all public.
*            If you need protected or private, please declare.
*/
#define SYNTHESIZE_IS_READONLY(varType, varName, funName, defaultVal)\
protected: varType varName = defaultVal;\
public: virtual varType is##funName(void) const { return varName; }

/** @def CC_SYNTHESIZE
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
#define SYNTHESIZE_IS(varType, varName, funName, defaultVal)\
protected: varType varName = defaultVal;\
public: virtual varType is##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

/** @def SYNTHESIZE_READONLY
* It is used to declare a protected variable. We can use getter to read the variable.
*
* @param varType     The type of variable.
* @param varName     Variable name.
* @param funName     "get + funName" will be the name of the getter.
* @warning   The getter is a public inline function.
*            The variables and methods declared after CC_SYNTHESIZE_READONLY are all public.
*            If you need protected or private, please declare.
*/
#define SYNTHESIZE_READONLY(varType, varName, funName, defaultVal)\
protected: varType varName = defaultVal;\
public: virtual varType get##funName(void) const { return varName; }

#define SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName, defaultVal)\
protected: varType varName = defaultVal;\
public: virtual const varType& get##funName(void) const { return varName; }

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
