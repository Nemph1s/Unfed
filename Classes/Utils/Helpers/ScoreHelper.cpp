/**
* @file Utils/Helpers/ScoreHelper.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Utils/Helpers/ScoreHelper.h"
#include "Common/GlobalInfo/GlobalInfo.h"

#include "GameObjects/TileObjects/Base/BaseObj.h"

#include "Controller/ChainController/ChainObj.h"
#include "Controller/ObjectController/ObjContainer.h"

using namespace CommonTypes;

static uint16_t EmptyScoreValue = 0;

//--------------------------------------------------------------------
void ScoreHelper::updateChainScore(ChainObj* chain)
//--------------------------------------------------------------------
{
    CC_ASSERT(chain);
    int score = 0;
    auto objs = chain->getObjects();
    if (objs) {
        for (auto it = objs->begin(); it != objs->end(); it++) {
            auto container = dynamic_cast<ObjContainer*>(*it);
            auto obj = container->getObjectForChain();
            CC_ASSERT(obj);
            score += getScoreForContainer(container);
        }
        chain->setScore(score);
    }    
}

//--------------------------------------------------------------------
void ScoreHelper::calculateScore(CommonTypes::Set* chains)
//--------------------------------------------------------------------
{
    CC_ASSERT(chains);
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);

        if (chain->getChainObjects()) {
            updateChainScore(chain);
        }
    }
}

//--------------------------------------------------------------------
uint16_t ScoreHelper::getScoreByObj(BaseObj * obj, bool isInDudeCain)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);
    uint16_t score = 0;
    auto type = obj->getType();
    if (type == BaseObjType::Cookie) {
        score = getScoreByCookieObj(obj, isInDudeCain) * GlobInfo->getComboMultiplier();
    }
    else if (type == BaseObjType::Field) {
        score = getScoreByFieldObj(obj);
    }
    else if (type == BaseObjType::Dude) {
        score = getScoreByDudeObj(obj);
    }
    // TODO: add enemy type
    return score;
}

//--------------------------------------------------------------------
uint16_t ScoreHelper::getScoreForContainer(ObjContainer* container)
//--------------------------------------------------------------------
{
    CC_ASSERT(container);
    uint16_t score = 0;
    auto baseObj = container->getObjectForChain();
    if (baseObj) {
        auto type = baseObj->getType();
        if (type == BaseObjType::Cookie) {
            bool isInDudeCain = false;
            auto chain = container->isObjectInChain();
            if (chain) {
                isInDudeCain = chain->getIsCreatedByDude();
            }
            score = getScoreByCookieObj(baseObj, isInDudeCain) * GlobInfo->getComboMultiplier();
        }
        else if (type == BaseObjType::Field) {
            score = getScoreByFieldObj(baseObj);
        }
        else if (type == BaseObjType::Dude) {
            score = getScoreByDudeObj(baseObj);
        }
        // TODO: add enemy type
    }
    return score;
}

//--------------------------------------------------------------------
uint16_t ScoreHelper::getScoreByCookieObj(BaseObj * obj, bool isInDudeCain)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);
    uint16_t score = GlobInfo->getCookieDefaultScore();
    if (isInDudeCain) {
        score = GlobInfo->getCookieMinScore() * GlobInfo->getCookieScoreMultiplier();
    }
    return score;
}

//--------------------------------------------------------------------
uint16_t ScoreHelper::getScoreByFieldObj(BaseObj* obj)
//--------------------------------------------------------------------
{
    // TODO: improve when will be available more strong field objects
    CC_ASSERT(obj);
    uint16_t score = GlobInfo->getFieldObjScore();
    return score;
}

//--------------------------------------------------------------------
uint16_t ScoreHelper::getScoreByDudeObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);
    auto dudeScoreMap = GlobInfo->getDudeScoreMap();

    uint16_t score = 0;
    auto dudeScore = dudeScoreMap.find(obj->getTypeAsInt());
    if (dudeScore != dudeScoreMap.end()) {
        score = dudeScore->second;
    }
    return score;
}

//--------------------------------------------------------------------
uint16_t ScoreHelper::getScoreByEnemyObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    // TODO: improve when will be available more strong enemies
    CC_ASSERT(obj);
    uint16_t score = GlobInfo->getEnemyScore();
    return score;
}
