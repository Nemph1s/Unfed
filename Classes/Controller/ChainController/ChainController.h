/**
* @file Controller/ChainController/ChainController.hpp
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

    /**
    * Moves chains from Set to Set
    * @param from The set from which the chains are moved.
    * @param to The set to which the chains are moved.
    * @param skipDudes The flag that allows to add first dude and skip the other ones(if true). If false doesnt skip any dudes.
    */
    void addChainsFromSetToSet(CommonTypes::Set* from, CommonTypes::Set* to, bool skipDudes = false);

    void addObjectsFromChainToChain(CommonTypes::Set* from, CommonTypes::Set* to);

    void addFieldOjbectsToChainSet(CommonTypes::Set* fieldObjects, CommonTypes::Set* chainSet);
    bool checkMathicngFieldObjWithChain(CommonTypes::Set* chains, BaseObj* obj);

    CommonTypes::Set* removeMatches();
    CommonTypes::Set* removeChainAt(CommonTypes::ChainType& type, cocos2d::Vec2& pos);

    CommonTypes::Set* detectChainAt(int column, int row);

    bool getCellFromChainAndPrevSwapSet(int& column, int& row, ChainObj* chain, CommonTypes::Set* prevSwapObjs);

    /**
    * Add chain to objectContainer and remove duplicate containers from other chains
    * @param chains The set with the chains.
    */
    void activateChains(CommonTypes::Set* chains);
    void deactivateChains(CommonTypes::Set* chains);

    void matchChains(CommonTypes::Set* chains);

    void executeCollectGoalCallback(CommonTypes::Set* chains);

protected:
    // Nodes should be created using create();
    ChainController();

    bool isPossibleToAddObjToChain(int col, int row, int& prevType, int& nextType);
    void addObjToChain(ChainObj* chain, int col, int row);
    void addChainToSet(ChainObj* chain, CommonTypes::Set* set);

    CommonTypes::Set* detectHorizontalMatches();
    CommonTypes::Set* detectVerticalMatches();
    CommonTypes::Set* detectDifficultMatches(CommonTypes::Set* horizontal, CommonTypes::Set* vertical);

    ChainObj* detectLChainMatches(ChainObj* horzChain, ChainObj* vertChain);
    ChainObj* detectTChainMatches(ChainObj* horzChain, ChainObj* vertChain);
    ChainObj* detectXChainMatches(ChainObj* horzChain, ChainObj* vertChain);

    bool isNextTwoCookieSuitable(const CommonTypes::ChainType& type, int col, int row);

    CommonTypes::Set* createHorizontalChainAt(int column, int startRow, bool isCreatedByDude = false);
    CommonTypes::Set* createVerticalChainAt(int startColumn, int row, bool isCreatedByDude = false);
    CommonTypes::Set* createXChainAt(int column, int row, bool isCreatedByDude = false);

    CommonTypes::Set* createAllOfOneChain(int entryColumn, int entryRow, bool isCreatedByDude = false, BaseObj* dudeObj = nullptr);
    CommonTypes::Set* createChainFromPosToPos(cocos2d::Vec2 from, cocos2d::Vec2 to, bool isCreatedByDude = false);
    CommonTypes::Set* createChainFromPosToPos(const CommonTypes::Direction& direction, int fromCol, int fromRow, int toCol, int toRow, bool isCreatedByDude = false);

#ifdef COCOS2D_DEBUG
    //void logDebugChains(CommonTypes::Set* horizontal, CommonTypes::Set* vertical, CommonTypes::Set* difficult);
#endif // COCOS2D_DEBUG 

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(BaseObj* obj)>, mUpdateGoalCallback, UpdateGoalCallback);

    CC_SYNTHESIZE(LevelObj*, mLevel, Level);
    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);
};
