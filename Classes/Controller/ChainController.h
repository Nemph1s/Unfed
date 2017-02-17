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

class ChainObj;
class LevelObj;
class GameplayScene;
class ObjectController;

class ChainController : public cocos2d::Ref
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~ChainController();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease" and "retain".
    */
    static ChainController* create();

    bool init();

    cocos2d::Set* removeMatches();
    cocos2d::Set* removeChainAt(CommonTypes::ChainType& type, cocos2d::Vec2& pos);

protected:
    // Nodes should be created using create();
    ChainController();

    cocos2d::Set* detectHorizontalMatches();
    cocos2d::Set* detectVerticalMatches();
    cocos2d::Set* detectDifficultMatches(cocos2d::Set* horizontal, cocos2d::Set* vertical);

    ChainObj* detectLChainMatches(ChainObj* horzChain, ChainObj* vertChain);
    ChainObj* detectTChainMatches(ChainObj* horzChain, ChainObj* vertChain);

    void addChainsFromSetToSet(cocos2d::Set* from, cocos2d::Set* to);

    cocos2d::Set* createHorizontalChainAt(int column);
    cocos2d::Set* createVerticalChainAt(int row);
    cocos2d::Set* createXChainAt(int column, int row);
    cocos2d::Set* createAllOfOneChain(int entryColumn, int entryRow);

#ifdef COCOS2D_DEBUG
    void logDebugChains(cocos2d::Set* horizontal, cocos2d::Set* vertical, cocos2d::Set* difficult);
#endif // COCOS2D_DEBUG 

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(LevelObj*, mLevel, Level);
    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);
};