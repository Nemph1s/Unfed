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

    void addChainsFromSetToSet(CommonTypes::Set* from, CommonTypes::Set* to);
    void addCookiesFromChainToChain(CommonTypes::Set* from, CommonTypes::Set* to);

    CommonTypes::Set* removeMatches();
    CommonTypes::Set* removeChainAt(CommonTypes::ChainType& type, cocos2d::Vec2& pos);

    void calculateChainScore(ChainObj* chain);
    void executeCollectGoalCallback(CommonTypes::Set* chains);

protected:
    // Nodes should be created using create();
    ChainController();

    void addChainToSet(ChainObj* chain, CommonTypes::Set* set);
    CommonTypes::Set* detectHorizontalMatches();
    CommonTypes::Set* detectVerticalMatches();
    CommonTypes::Set* detectDifficultMatches(CommonTypes::Set* horizontal, CommonTypes::Set* vertical);

    ChainObj* detectLChainMatches(ChainObj* horzChain, ChainObj* vertChain);
    ChainObj* detectTChainMatches(ChainObj* horzChain, ChainObj* vertChain);

    CommonTypes::Set* createHorizontalChainAt(int column);
    CommonTypes::Set* createVerticalChainAt(int row);
    CommonTypes::Set* createXChainAt(int column, int row, bool isCreatedByDude = false);
    CommonTypes::Set* createAllOfOneChain(int entryColumn, int entryRow, bool isCreatedByDude = false);
    CommonTypes::Set* createChainFromPosToPos(cocos2d::Vec2 from, cocos2d::Vec2 to, bool isCreatedByDude = false);
    CommonTypes::Set* createChainFromPosToPos(int fromCol, int fromRow, int toCol, int toRow, bool isCreatedByDude = false);

#ifdef COCOS2D_DEBUG
    void logDebugChains(CommonTypes::Set* horizontal, CommonTypes::Set* vertical, CommonTypes::Set* difficult);
#endif // COCOS2D_DEBUG 

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(BaseObj* obj)>, mUpdateGoalCallback, UpdateGoalCallback);

    CC_SYNTHESIZE(LevelObj*, mLevel, Level);
    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);
};
