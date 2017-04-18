/**
* @file Controller/ObjectController/Enemy/EnemyObj.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"
#include "Utils/PlatformMacros.h"

class EnemyObj : public BaseObj
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~EnemyObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static EnemyObj * create(const CT::EnemyInfo &info);

    bool init(const CT::EnemyInfo &info);

    cocos2d::String& spriteName() const override;

    virtual int getTypeAsInt() const override;

    virtual void match() override;
    virtual void clear() override;
    virtual bool checkMatchingCondition(CT::Cell& cell);
    virtual bool isHpEnded() const override;

    virtual void updateDebugLabel() override;

    virtual void runAction();

protected:
    // Nodes should be created using create();
    EnemyObj();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE_READONLY(CT::EnemyType, mEnemyType, EnemyType)
    
    CC_SYNTHESIZE(int, mHP, HP)
    CC_SYNTHESIZE(int, mWaitTurnsBeforeAction, WaitTurnsBeforeAction)
    CC_SYNTHESIZE(cocos2d::Label*, mDebugLabel, DebugLabel)

    SYNTHESIZE_IS_READONLY(bool, mIsRemovableByMatch, RemovableByMatch, false)

    CC_SYNTHESIZE(std::function<void(BaseObj*)>, mRemoveEnemyCallback, RemoveEnemyCallback)
};
