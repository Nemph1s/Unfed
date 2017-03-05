/**
* @file Utils/Parser/JsonHelper.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once



namespace JsonNames {

    //standard level parameters
    static const char* loadResult = "res";
    static const char* tilesArray = "tiles";
    static const char* movesLeft = "moves";
    static const char* objTypesCount = "typesCount";
    static const char* targetScore = "targetScore";

    //extended level parameters
    static const char* skipEmptyHoles = "skipEmptyHoles";
    static const char* predefinedCookies = "predefinedCookies";
    static const char* cookiesArray = "cookies";
    static const char* fieldObjectsArray = "fieldObjects";
    static const char* allowedCookieTypes = "allowedCookieTypes";

    //level goals
    static const char* levelGoals = "levelGoals";
    static const char* goalsCount = "goalsCount";
    static const char* collectGoal = "collect";
    static const char* targetBaseObjType = "baseObjType";
    static const char* targetObjType = "objType";
    static const char* targetObjectCount = "targetCount";
}