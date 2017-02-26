/**
* @file Controller/ObjectController/DudeController.hpp
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

    cocos2d::Set* createDudeObectsFromChains(cocos2d::Set* chains);

    BaseObj* createDudeObject(int column, int row, int type);

    BaseObj* objectAt(int column, int row);
    DudeObj* dudeObjectAt(int column, int row);

    //TODO: add to each update field method!!!
    void detectDirectionsForDudes();

    //TODO: use as callback in DudesLayer!
    bool canActivateDudeTo(int fromCol, int fromRow, int direction);
    cocos2d::Set* activateDude(DudeObj* obj, int direction);
    void activateAllDudes();

    void removeDude(int column, int row);
    void removeAllDudes();

protected:
    // Nodes should be created using create();
    DudeController();

    void updateDirectionsForDude(DudeObj* obj, DudeHelper* helper);

    bool isEnoughCookiesForDude(int count, int neededCount);

    CommonTypes::TileType getDudeTypeByMatchedCount(int count);
    CommonTypes::TileType getDudeTypeByChain(ChainObj* chain); //TODO: implement

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(DudeObj*, int direction)>, mActivateDudeCallback, ActivateDudeCallback);

    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);
    CC_SYNTHESIZE(ChainController*, mChainCtrl, ChainController);
    
    cocos2d::Map<DudeObj*, DudeHelper*> mDudeDirections;
    std::map<int, CommonTypes::TileType> mDudeTypes;

    // Dude object array 
    BaseObj* mDudeObjects[CommonTypes::NumColumns][CommonTypes::NumRows] = { nullptr };
};