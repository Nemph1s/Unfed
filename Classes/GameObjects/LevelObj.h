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

class ChainObj;
class ObjectController;
class SwapController;

class LevelObj : public cocos2d::Node
{
    friend SwapController;

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

    cocos2d::Set* shuffle();

    cocos2d::Set* removeMatches();
    cocos2d::Set* removeChainAt(CommonTypes::ChainType& type, cocos2d::Vec2& pos);

//     cocos2d::Set* detectMatches();
//     void removeMatches(cocos2d::Set* matches);
    cocos2d::Set* removeFieldObjects(cocos2d::Set* chains);


    cocos2d::Array* useGravityToFillHoles();
    cocos2d::Array* fillTopUpHoles();

    void resetComboMultiplier();

    cocos2d::Set* createHorizontalChainAt(int column);
    cocos2d::Set* createVerticalChainAt(int row);
    cocos2d::Set* createXChainAt(int column, int row);

protected:
    // Nodes should be created using create();
    LevelObj();

    void addChainsFromSetToSet(cocos2d::Set* from, cocos2d::Set* to);



    cocos2d::Set* detectHorizontalMatches();
    cocos2d::Set* detectVerticalMatches();
    cocos2d::Set* detectDifficultMatches(cocos2d::Set* horizontal, cocos2d::Set* vertical);

    ChainObj* detectLChainMatches(ChainObj* horzChain, ChainObj* vertChain);
    ChainObj* detectTChainMatches(ChainObj* horzChain, ChainObj* vertChain);

    void removeCookies(cocos2d::Set* chains);

    void calculateScore(cocos2d::Set* chains);

#ifdef COCOS2D_DEBUG
    void logDebugChains(cocos2d::Set* horizontal, cocos2d::Set* vertical, cocos2d::Set* difficult);
#endif // COCOS2D_DEBUG 

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<bool()>, mDetectPossibleSwapsCallback, DetectPossibleSwapsCallback);

    CC_SYNTHESIZE_READONLY(int, mComboMultiplier, ComboMultiplier);
    CC_SYNTHESIZE_READONLY(CommonTypes::LevelInfo, mLevelInfo, LevelInfo);

    CC_SYNTHESIZE_READONLY(ObjectController*, mObjCtrl, ObjectController);
};