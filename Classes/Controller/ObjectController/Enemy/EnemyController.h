/**
* @file Controller/ObjectController/Enemy/EnemyController.hpp
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
#include "Common/GlobalInfo/GlobalInfo.h"

class BaseObj;
class EnemyObj;
class EnemyHelper;
class ObjectController;

class EnemyController : public cocos2d::Ref
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~EnemyController();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease" and "retain".
    */
    static EnemyController* create();

    bool init();

    CT::Set* createInitialEnemies();

    void detectDirectionsForEnemies();
    void eraseEnemyHelper(EnemyObj* obj);

    bool beginEnemiesTurn(bool updateDirections = true);

    bool matchEnemyObject(BaseObj* obj);

protected:
    // Nodes should be created using create();
    EnemyController();

    BaseObj* createEnemy(CT::Cell& cell, int type, int priority = 0);
    bool runAction(EnemyObj* obj);

    void updateDirectionsForEnemy(EnemyObj* obj, EnemyHelper* helper);
    void updateNotAllowedDirectionToObj(BaseObj* obj);
    
    //---Class Attributes-------------------------------------------------
    SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController, nullptr)

    SYNTHESIZE_IS(bool, mIsEnemiesTurn, EnemiesTurn, false)
    cocos2d::Map<EnemyObj*, EnemyHelper*> mEnemyDirections;

    // Enemies Actions
    CC_SYNTHESIZE(std::function<void(BaseObj*, BaseObj*)>, mMoveActionCallback, MoveActionCallback)
};