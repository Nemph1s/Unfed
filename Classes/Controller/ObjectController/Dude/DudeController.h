/**
* @file Controller/ObjectController/Dude/DudeController.hpp
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
 
#include <map>

using CommonTypes::FieldType;
using CommonTypes::Set;
using CommonTypes::Direction;

class BaseObj;
class DudeObj;
class ChainObj;
class DudeHelper;

class ObjectController;
class ChainController;

class DudeController : public cocos2d::Ref
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~DudeController();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease" and "retain".
    */
    static DudeController* create();

    bool init();

    Set* createDudeObectsFromChains(Set* chains, Set* prevSwapContainers);

    BaseObj* createDudeObject(int column, int row, int type);

    void detectDirectionsForDudes();

    bool canActivateDudeTo(int fromCol, int fromRow, int direction);
    Set* getChainsForDude(DudeObj* obj, int dir, bool isPreview = false);

    void updateChainSetWithDudesInChain(const Direction& direction, DudeObj* activeDude, Set* chains, Set* chainSet);
    void eraseDirectionsForDude(DudeObj* obj);

protected:
    // Nodes should be created using create();
    DudeController();

    void updateDirectionsForDude(DudeObj* obj, DudeHelper* helper);

    bool isEnoughCookiesForDude(int count, int neededCount);

    FieldType getDudeTypeByChain(ChainObj* chain);

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(DudeObj*, int direction)>, mActivateDudeCallback, ActivateDudeCallback);

    CC_SYNTHESIZE(uint8_t, mDudesCount, DudesCount);

    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);
    CC_SYNTHESIZE(ChainController*, mChainCtrl, ChainController);
    
    cocos2d::Map<DudeObj*, DudeHelper*> mDudeDirections;
};