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

    CommonTypes::Set* createDudeObectsFromChains(CommonTypes::Set* chains, CommonTypes::Set* prevSwapContainers);

    BaseObj* createDudeObjWithoutContainer(int type);
    BaseObj* createDudeObject(CommonTypes::Cell& cell, int type);

    void detectDirectionsForDudes();

    bool canActivateDudeTo(CommonTypes::Cell& fromCell, int direction);
    CommonTypes::Set* getChainsForDude(DudeObj* obj, int dir, bool isPreview = false);

    void updateChainSetWithDudesInChain(const CommonTypes::Direction& direction, DudeObj* activeDude, CommonTypes::Set* chains, CommonTypes::Set* chainSet);
    void eraseDirectionsForDude(DudeObj* obj);

protected:
    // Nodes should be created using create();
    DudeController();

    void updateDirectionsForDude(DudeObj* obj, DudeHelper* helper);

    bool isEnoughCookiesForDude(int count, int neededCount);

    CommonTypes::FieldType getDudeTypeByChain(ChainObj* chain);

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(DudeObj*, int direction)>, mActivateDudeCallback, ActivateDudeCallback);

    CC_SYNTHESIZE(uint8_t, mDudesCount, DudesCount);

    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);
    CC_SYNTHESIZE(ChainController*, mChainCtrl, ChainController);
    
    cocos2d::Map<DudeObj*, DudeHelper*> mDudeDirections;
};