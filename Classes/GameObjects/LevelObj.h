/**
* @file GameObjects/LevelObj.hpp
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

class BaseObj;
class ChainObj;
class ObjectController;
class ChainController;
class SwapController;

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

    void initObjectController();
    void initChainController();

    cocos2d::Set* shuffle();

    cocos2d::Set* removeMatches();
    cocos2d::Set* removeChainAt(CommonTypes::ChainType& type, cocos2d::Vec2& pos);
    cocos2d::Set* removeFieldObjects(cocos2d::Set* chains);

    cocos2d::Array* useGravityToFillHoles();
    cocos2d::Array* fillTopUpHoles();

    void resetComboMultiplier();

protected:
    // Nodes should be created using create();
    LevelObj();

    void calculateScore(cocos2d::Set* chains);
    void removeCookies(cocos2d::Set* chains);   

    bool isPossibleToAddCookie(int column, int row);
    bool useGravityOnObject(cocos2d::Array* colArr, cocos2d::Array* rowArr, BaseObj* obj, int destinationRow);

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<bool()>, mDetectPossibleSwapsCallback, DetectPossibleSwapsCallback);

    CC_SYNTHESIZE_READONLY(int, mComboMultiplier, ComboMultiplier);
    CC_SYNTHESIZE_READONLY(CommonTypes::LevelInfo, mLevelInfo, LevelInfo);

    CC_SYNTHESIZE_READONLY(ObjectController*, mObjCtrl, ObjectController);
    CC_SYNTHESIZE_READONLY(ChainController*, mChainCtrl, ChainController);
};
