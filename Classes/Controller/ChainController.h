/**
* @file Controller/ChainController.hpp
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
class LevelObj;
class GameplayScene;
class DudeController;
class ObjectController;

class ChainController : public cocos2d::Ref
{
    friend DudeController;

CC_CONSTRUCTOR_ACCESS:
    virtual ~ChainController();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease" and "retain".
    */
    static ChainController* create();

    bool init();

    void addChainsFromSetToSet(cocos2d::Set* from, cocos2d::Set* to);
    void addCookiesFromChainToChain(cocos2d::Set* from, cocos2d::Set* to);

    cocos2d::Set* removeMatches();
    cocos2d::Set* removeChainAt(CommonTypes::ChainType& type, cocos2d::Vec2& pos);

    void calculateChainScore(ChainObj* chain);

protected:
    // Nodes should be created using create();
    ChainController();

    void addChainToSet(ChainObj* chain, cocos2d::Set* set);
    cocos2d::Set* detectHorizontalMatches();
    cocos2d::Set* detectVerticalMatches();
    cocos2d::Set* detectDifficultMatches(cocos2d::Set* horizontal, cocos2d::Set* vertical);

    ChainObj* detectLChainMatches(ChainObj* horzChain, ChainObj* vertChain);
    ChainObj* detectTChainMatches(ChainObj* horzChain, ChainObj* vertChain);

    cocos2d::Set* createHorizontalChainAt(int column);
    cocos2d::Set* createVerticalChainAt(int row);
    cocos2d::Set* createXChainAt(int column, int row, bool isCreatedByDude = false);
    cocos2d::Set* createAllOfOneChain(int entryColumn, int entryRow, bool isCreatedByDude = false);
    cocos2d::Set* createChainFromPosToPos(cocos2d::Vec2 from, cocos2d::Vec2 to, bool isCreatedByDude = false);
    cocos2d::Set* createChainFromPosToPos(int fromCol, int fromRow, int toCol, int toRow, bool isCreatedByDude = false);

#ifdef COCOS2D_DEBUG
    void logDebugChains(cocos2d::Set* horizontal, cocos2d::Set* vertical, cocos2d::Set* difficult);
#endif // COCOS2D_DEBUG 

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(BaseObj* obj)>, mUpdateGoalCallback, UpdateGoalCallback);

    CC_SYNTHESIZE(LevelObj*, mLevel, Level);
    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);
};
