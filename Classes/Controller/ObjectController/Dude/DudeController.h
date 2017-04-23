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
#include "Common/GameObjTypes.h"
 
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

    CT::Set* createDudeObectsFromChains(CT::Set* chains, CT::Set* prevSwapContainers);

    BaseObj* createDudeObjWithoutContainer(int type);
    BaseObj* createDudeObject(CT::Cell& cell, int type);

    void detectDirectionsForDudes();
    void eraseDirectionsForDude(DudeObj* obj);

    bool matchDudeObject(BaseObj* obj);

    bool canActivateDudeTo(CT::Cell& fromCell, int direction);
    CT::Set* getChainsForDude(DudeObj* obj, int dir, bool isPreview = false);

    void updateChainSetWithDudesInChain(const CT::Direction& direction, DudeObj* activeDude, CT::Set* chains, CT::Set* chainSet);
 
protected:
    // Nodes should be created using create();
    DudeController();

    void updateDirectionsForDude(DudeObj* obj, DudeHelper* helper);

    bool isEnoughCookiesForDude(int count, int neededCount);

    GOT::FieldType getDudeTypeByChain(ChainObj* chain);

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(DudeObj*, int direction)>, mActivateDudeCallback, ActivateDudeCallback);

    CC_SYNTHESIZE(uint8_t, mDudesCount, DudesCount);

    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);
    CC_SYNTHESIZE(ChainController*, mChainCtrl, ChainController);
    
    cocos2d::Map<DudeObj*, DudeHelper*> mDudeDirections;
};