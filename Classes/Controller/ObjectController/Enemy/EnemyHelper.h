/**
* @file Controller/ObjectController/Enemy/EnemyHelper.hpp
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
#include "Utils/PlatformMacros.h"

class EnemyObj;
class EnemyController;
class BaseObj;
class ObjectController;

class EnemyHelper : public cocos2d::Ref
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~EnemyHelper();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static EnemyHelper* createWithEnemyObject(EnemyObj* obj);

    bool initWithEnemyObject(EnemyObj* obj);

    BaseObj* getNearbyObjectByDirection(const CT::Direction& direction);
    bool updateNearbyObjects(BaseObj* top, BaseObj* bot, BaseObj* left, BaseObj* rigth);

    
    bool isReadyForAction();
    void runEnemyAction();

protected:
    // Nodes should be created using create();
    EnemyHelper();

    bool isEnemyReadyToMove();

    //---Class Attributes-------------------------------------------------

    SYNTHESIZE(EnemyObj*, mEnemyObj, EnemyObj, nullptr)
    SYNTHESIZE(EnemyController*, mEnemyCtrl, EnemyController, nullptr)
    SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController, nullptr)

    SYNTHESIZE(BaseObj*, mTopObj, TopObj, nullptr)
    SYNTHESIZE(BaseObj*, mBottomObj, BottomObj, nullptr)
    SYNTHESIZE(BaseObj*, mLeftObj, LeftObj, nullptr)
    SYNTHESIZE(BaseObj*, mRightObj, RightObj, nullptr)

    //---Helper Attributes-------------------------------------------------
    SYNTHESIZE(cocos2d::Array*, mAvailableNearbyObjects, AvailableNearbyObjects, nullptr)
};