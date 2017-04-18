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

using CT::Cell;

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
    void addChainsFromSetToSet(CT::Set* from, CT::Set* to, bool skipDudes = false);

    void addObjectsFromChainToChain(CT::Set* from, CT::Set* to);

    void addMatchedOjbectsToChainSet(CT::Set* fieldObjects, CT::Set* chainSet);
    bool checkMathingObjWithChain(CT::Set* chains, BaseObj* obj);

    CT::Set* removeMatches();
    CT::Set* removeChainAt(CT::ChainType& type, cocos2d::Vec2& pos);
    void removeDudeMatches(CT::Set* set);

    CT::Set* detectHintChainAt(BaseObj* curObj, BaseObj* nextObj);

    bool getCellFromChainAndPrevSwapSet(Cell& cell, ChainObj* chain, CT::Set* prevSwapObjs);

    /**
    * Add chain to objectContainer and remove duplicate containers from other chains
    * @param chains The set with the chains.
    */
    void activateChains(CT::Set* chains);
    void deactivateChains(CT::Set* chains);

    void matchChains(CT::Set* chains);

    void executeCollectGoalCallback(CT::Set* chains);

    /**
    * This method will create map with chains, first value is the length between inserted object and main object
    * @param fromCell The start point of cell from where will be created chain.
    * @param length The max size between current object and current obj + length.
    * @return map
    */
    CT::Set* createExposionChainAtCellForRebound(Cell& fromCell, uint8_t length);

protected:
    // Nodes should be created using create();
    ChainController();

    bool isPossibleToAddObjToChain(Cell& cell, int& prevType, int& nextType);
    void addObjToChain(ChainObj* chain, Cell& cell);
    void addChainToSet(ChainObj* chain, CT::Set* set);

    CT::Set* detectHorizontalMatches();
    CT::Set* detectVerticalMatches();
    CT::Set* detectDifficultMatches(CT::Set* horizontal, CT::Set* vertical);

    ChainObj* detectLChainMatches(ChainObj* horzChain, ChainObj* vertChain);
    ChainObj* detectTChainMatches(ChainObj* horzChain, ChainObj* vertChain);
    ChainObj* detectXChainMatches(ChainObj* horzChain, ChainObj* vertChain);

    bool isNextTwoCookieSuitable(const CT::ChainType& type, Cell& cell);

    //---Create chain methods-------------------------------------------------

    /**
    * This method will create horizontal chain at cell pos
    * @param cell The cell pos on field at column and startRow.
    * @param isCreatedByDude The flag that must be set if chain created by dude.
    * @return set The cocos2d::Set with chain objects
    */
    CT::Set* createHorizontalChainAt(Cell& cell, bool isCreatedByDude = false);

    /**
    * This method will create vertical chain at cell pos
    * @param cell The cell pos on field at startColumn and row.
    * @param isCreatedByDude The flag that must be set if chain created by dude.
    * @return set The cocos2d::Set with chain objects
    */

    CT::Set* createVerticalChainAt(Cell& cell, bool isCreatedByDude = false);

    /**
    * This method will create vertical and horizontal chain at cell pos
    * @param cell The cell pos on field at column and row.
    * @param isCreatedByDude The flag that must be set if chain created by dude.
    * @return set The cocos2d::Set with chain objects
    */
    CT::Set* createXChainAt(Cell& cell, bool isCreatedByDude = false);

    /**
    * This method will create circle chain at cell pos
    * @param cell The cell pos on field at startColumn and row.
    * @param isCreatedByDude The flag that must be set if chain created by dude.
    * @return set The cocos2d::Set with chain objects
    */
    CT::Set* createExplosionChainAt(Cell& cell, bool isCreatedByDude = false);

    /**
    * This method will create all of one type chain
    * @param cell The cell pos on field at entryColumn and entryRow.
    * @param isCreatedByDude The flag that must be set if chain created by dude.
    * @return set The cocos2d::Set with chain objects
    */
    CT::Set* createAllOfOneChain(Cell& entryCell, bool isCreatedByDude = false, BaseObj* dudeObj = nullptr);
    
    /**
    * This method will create chain from cell A to cell B
    * @param direction The direction of cell. Param uses in calculation next direction of sub chains
    * @param fromCell The start point of cell from where will be created chain.
    * @param toCell The end point of cell to where will be created chain.
    * @param isCreatedByDude The flag that must be set if chain created by dude.
    * @return set The cocos2d::Set with chain objects
    */
    CT::Set* createChainFromPosToPos(const CT::Direction& direction, Cell& fromCell, Cell& toCell, bool isCreatedByDude = false);

#ifdef COCOS2D_DEBUG
    //void logDebugChains(CommonTypes::Set* horizontal, CommonTypes::Set* vertical, CommonTypes::Set* difficult);
#endif // COCOS2D_DEBUG 

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(BaseObj* obj)>, mUpdateGoalCallback, UpdateGoalCallback);

    CC_SYNTHESIZE(LevelObj*, mLevel, Level);
    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);
};
