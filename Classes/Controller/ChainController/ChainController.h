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

using CommonTypes::Cell;

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

    CommonTypes::Set* detectHintChainAt(BaseObj* curObj, BaseObj* nextObj);

    bool getCellFromChainAndPrevSwapSet(Cell& cell, ChainObj* chain, CommonTypes::Set* prevSwapObjs);

    /**
    * Add chain to objectContainer and remove duplicate containers from other chains
    * @param chains The set with the chains.
    */
    void activateChains(CommonTypes::Set* chains);
    void deactivateChains(CommonTypes::Set* chains);

    void matchChains(CommonTypes::Set* chains);

    void executeCollectGoalCallback(CommonTypes::Set* chains);

    /**
    * This method will create map with chains, first value is the length between inserted object and main object
    * @param fromCell The start point of cell from where will be created chain.
    * @param length The max size between current object and current obj + length.
    * @return map
    */
    CommonTypes::Set* createExposionChainAtCellForRebound(Cell& fromCell, uint8_t length);

protected:
    // Nodes should be created using create();
    ChainController();

    bool isPossibleToAddObjToChain(Cell& cell, int& prevType, int& nextType);
    void addObjToChain(ChainObj* chain, Cell& cell);
    void addChainToSet(ChainObj* chain, CommonTypes::Set* set);

    CommonTypes::Set* detectHorizontalMatches();
    CommonTypes::Set* detectVerticalMatches();
    CommonTypes::Set* detectDifficultMatches(CommonTypes::Set* horizontal, CommonTypes::Set* vertical);

    ChainObj* detectLChainMatches(ChainObj* horzChain, ChainObj* vertChain);
    ChainObj* detectTChainMatches(ChainObj* horzChain, ChainObj* vertChain);
    ChainObj* detectXChainMatches(ChainObj* horzChain, ChainObj* vertChain);

    bool isNextTwoCookieSuitable(const CommonTypes::ChainType& type, Cell& cell);

    //---Create chain methods-------------------------------------------------

    /**
    * This method will create horizontal chain at cell pos
    * @param cell The cell pos on field at column and startRow.
    * @param isCreatedByDude The flag that must be set if chain created by dude.
    * @return set The cocos2d::Set with chain objects
    */
    CommonTypes::Set* createHorizontalChainAt(Cell& cell, bool isCreatedByDude = false);

    /**
    * This method will create vertical chain at cell pos
    * @param cell The cell pos on field at startColumn and row.
    * @param isCreatedByDude The flag that must be set if chain created by dude.
    * @return set The cocos2d::Set with chain objects
    */

    CommonTypes::Set* createVerticalChainAt(Cell& cell, bool isCreatedByDude = false);

    /**
    * This method will create vertical and horizontal chain at cell pos
    * @param cell The cell pos on field at column and row.
    * @param isCreatedByDude The flag that must be set if chain created by dude.
    * @return set The cocos2d::Set with chain objects
    */
    CommonTypes::Set* createXChainAt(Cell& cell, bool isCreatedByDude = false);

    /**
    * This method will create circle chain at cell pos
    * @param cell The cell pos on field at startColumn and row.
    * @param isCreatedByDude The flag that must be set if chain created by dude.
    * @return set The cocos2d::Set with chain objects
    */
    CommonTypes::Set* createExplosionChainAt(Cell& cell, bool isCreatedByDude = false);

    /**
    * This method will create all of one type chain
    * @param cell The cell pos on field at entryColumn and entryRow.
    * @param isCreatedByDude The flag that must be set if chain created by dude.
    * @return set The cocos2d::Set with chain objects
    */
    CommonTypes::Set* createAllOfOneChain(Cell& entryCell, bool isCreatedByDude = false, BaseObj* dudeObj = nullptr);
    
    /**
    * This method will create chain from cell A to cell B
    * @param direction The direction of cell. Param uses in calculation next direction of sub chains
    * @param fromCell The start point of cell from where will be created chain.
    * @param toCell The end point of cell to where will be created chain.
    * @param isCreatedByDude The flag that must be set if chain created by dude.
    * @return set The cocos2d::Set with chain objects
    */
    CommonTypes::Set* createChainFromPosToPos(const CommonTypes::Direction& direction, Cell& fromCell, Cell& toCell, bool isCreatedByDude = false);

#ifdef COCOS2D_DEBUG
    //void logDebugChains(CommonTypes::Set* horizontal, CommonTypes::Set* vertical, CommonTypes::Set* difficult);
#endif // COCOS2D_DEBUG 

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(BaseObj* obj)>, mUpdateGoalCallback, UpdateGoalCallback);

    CC_SYNTHESIZE(LevelObj*, mLevel, Level);
    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);
};
