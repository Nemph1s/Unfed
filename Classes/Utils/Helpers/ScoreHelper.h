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

#include "cocos2d.h"
#include "Common/CommonTypes.h"
#include "Utils/PlatformMacros.h"

class BaseObj;
class ChainObj;
class ObjContainer;

class ScoreHelper 
{
    CREATE_SINGLETON(ScoreHelper);

public:
    static void updateChainScore(ChainObj* chain);

    static void calculateScore(CT::Set* chains);

    static uint16_t getScoreByObj(BaseObj* obj, bool isInDudeCain);
    static uint16_t getScoreForContainer(ObjContainer* container);

private:
    
    static uint16_t getScoreByCookieObj(BaseObj* obj, bool isInDudeCain);
    static uint16_t getScoreByFieldObj(BaseObj* obj);
    static uint16_t getScoreByDudeObj(BaseObj* obj);
    static uint16_t getScoreByEnemyObj(BaseObj* obj);
};
