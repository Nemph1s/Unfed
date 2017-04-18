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

    void beginEnemiesTurn();
    
    bool matchEnemyObject(BaseObj* obj);

protected:
    // Nodes should be created using create();
    EnemyController();

    BaseObj* createEnemy(CT::Cell& cell, int type, int priority = 0);
    
    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(ObjectController*, mObjCtrl, ObjectController);

    //TODO: create a list of enemies
};