/**
* @file GameObjects/Level/LevelObj.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"
#include "GameObjects/Level/LevelTypes.h"

class BaseObj;
class ChainObj;
class ObjectController;
class ChainController;
class SwapController;
class DudeController;

class LevelObj : public cocos2d::Node
{
    friend SwapController;
    friend ChainController;

CC_CONSTRUCTOR_ACCESS:
    virtual ~LevelObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static LevelObj * createWithId(const int16_t& levelId);

    virtual bool initWithId(const int16_t& levelId);

    CT::Set* shuffle();

    CT::Set* detectFieldObjects(CT::Set* chains);

    cocos2d::Array* useGravityToFillHoles();
    cocos2d::Array* fillTopUpHoles();

    void disablePredefinedCookies();

    //TODO: move to callback
    void removeDudeMatches(CT::Set* set);

protected:
    // Nodes should be created using create();
    LevelObj();
    
    CT::SearchEmptyHoles skipFillTopUpHoles(int column, int row, bool& filledTileFouned);

    bool isPossibleToAddCookie(int column, int row);
    bool useGravityOnObject(cocos2d::Array* colArr, cocos2d::Array* rowArr, BaseObj* obj, int destinationRow);

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<bool()>, mDetectPossibleSwapsCallback, DetectPossibleSwapsCallback);

    CC_SYNTHESIZE_READONLY(CT::LevelInfo, mLevelInfo, LevelInfo);

    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);
    CC_SYNTHESIZE(DudeController*, mDudeCtrl, DudeController);
    CC_SYNTHESIZE(ChainController*, mChainCtrl, ChainController);
};
