#/**
* @file Controller/ChainController/ChainObj.hpp
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
class CookieObj;
class ObjContainer;

class ChainObj : public cocos2d::Node
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~ChainObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static ChainObj* createWithType(const GOT::ChainType& type);

    virtual bool initWithType(const GOT::ChainType& type);

    std::string description();
    std::string typeAsString();
    int getTypeAsInt();

    void addObjectToChain(ObjContainer* obj);

    cocos2d::Array* getChainObjects();
    int getCookiesCount();

    void addObjectsFromChain(ChainObj* chain);

    void activateObjects();
    void removeDuplicateObjects(cocos2d::Array* objsToRemove);
    void deactivateObjects();

    void removeDudeObjectsFromChain(bool skipFirst = true);

    //---Callbacks-------------------------------------------------
    void executeCollectGoalCallback();

protected:
    // Nodes should be created using create();
    ChainObj();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(BaseObj* obj)>, mUpdateGoalCallback, UpdateGoalCallback);

    CC_SYNTHESIZE_PASS_BY_REF(CT::Direction, mDirection, Direction);
    
    CC_SYNTHESIZE(int, mScore, Score);
    CC_SYNTHESIZE(bool, mIsCreatedByDude, IsCreatedByDude);
    CC_SYNTHESIZE(cocos2d::Color4B, mChainColor, ChainColor);

    CC_SYNTHESIZE_READONLY(GOT::ChainType, mType, Type);
    CC_SYNTHESIZE_READONLY(cocos2d::Array*, mObjsToRemove, ObjsToRemove);
    CC_SYNTHESIZE_READONLY(cocos2d::Array*, mObjects, Objects);
};

