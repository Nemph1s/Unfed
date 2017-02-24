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

class BaseObj;
class DudeObj;
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

    BaseObj* createDudeObject(int column, int row, int type);

    DudeObj* dudeObjectAt(int column, int row);

    //TODO: add to each update field method!!!
    bool detectDirectionsForDudes();

    //TODO: use as callback in DudesLayer!
    bool canActivateDudeTo(int horzDelta, int vertDelta);
    cocos2d::Set* activateDude(DudeObj* obj, CommonTypes::Direction direction);
    void activateAllDudes();

    void removeDude(int column, int row);
    void removeAllDudes();

protected:
    // Nodes should be created using create();
    DudeController();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(DudeObj*, CommonTypes::Direction direction)>, mActivateDudeCallback, ActivateDudeCallback);

    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);
    CC_SYNTHESIZE(ChainController*, mChainCtrl, ChainController);
    
    cocos2d::Map<DudeObj*, DudeHelper*> mDudeDirections;

    // Dude object array 
    BaseObj* mDudeObjects[CommonTypes::NumColumns][CommonTypes::NumRows] = { nullptr };
};