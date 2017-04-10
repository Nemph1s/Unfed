/**
* @file Utils/Parser/JsonNames.hpp
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
    static const char* CommonTypesCount = "typesCount";
    static const char* targetScore = "targetScore";

    //extended level parameters
    static const char* skipEmptyHoles = "skipEmptyHoles";
    static const char* predefinedCookies = "predefinedCookies";
    static const char* cookiesArray = "cookies";
    static const char* allowedCookieTypes = "allowedCookieTypes";

    //field objects
    static const char* fieldObjectsArray = "fieldObjects";
    static const char* fieldObject_objects = "objects";
    static const char* fieldObject_col = "col";
    static const char* fieldObject_row = "row";
    static const char* fieldObject_types = "types";

    //level goals
    static const char* levelGoals = "levelGoals";
    static const char* goalsCount = "goalsCount";
    static const char* collectGoal = "collect";
    static const char* targetBaseObjType = "baseObjType";
    static const char* targetObjType = "objType";
    static const char* targetObjectCount = "targetCount";

    //GlobalInfo parameters
    namespace GlobalInfo {
        static const char* loadResult = "res";
        static const char* swVersion = "swVersion";
        static const char* imgPackType = "imgPackType";
        static const char* imgPackVersion = "imgPackVersion";
        static const char* tileWidth = "tileWidth";
        static const char* tileHeigth = "tileHeigth";
        static const char* levelsCount = "levelsCount";

        static const char* scoreValues = "scoreValues";

        //ScoreValues parameters
        namespace ScoreValues {
            static const char* cookieMinimal = "cookieMinimal";
            static const char* cookieDefault = "cookieDefault";
            static const char* fieldObject = "fieldObject";
            static const char* enemy = "enemy";

            static const char* dudes = "dudes";
            static const char* dudeType = "dudeType";
            static const char* dudeValue = "dudeValue";


        }
    }
}