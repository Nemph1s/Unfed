/**
* @file GameObjects/TileObjects/Base/BaseObj.hpp
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

class BaseObj : public cocos2d::Node
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~BaseObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static BaseObj * create();
    static BaseObj * create(const GOT::BaseObjInfo &info);

    virtual bool init();
    bool init(const GOT::BaseObjInfo &info);

    CT::Cell getCell() const;

    virtual cocos2d::String& spriteName() const;
    virtual cocos2d::String& description() const;

    virtual int getTypeAsInt() const;

    virtual void match();
    virtual void clear();
    virtual bool checkMatchingCondition(CT::Cell& cell);
    virtual bool isHpEnded() const;   

    virtual void updateDebugLabel();
    virtual void updateZOrder();

protected:
    // Nodes should be created using create();
    BaseObj();

    //---Class Attributes-------------------------------------------------

    SYNTHESIZE_IS_READONLY(bool, mIsMovable, Movable, false);
    SYNTHESIZE_IS_READONLY(bool, mIsRemovable, Removable, false);
    SYNTHESIZE_IS_READONLY(bool, mIsSwappable, Swappable, false);
    SYNTHESIZE_IS_READONLY(bool, mIsContainer, Container, false);

    CC_SYNTHESIZE(int, mPriority, Priority);
    cocos2d::String* mDummyString;

    CC_SYNTHESIZE(int, mColumn, Column);
    CC_SYNTHESIZE(int, mRow, Row);
    CC_SYNTHESIZE(GOT::BaseObjType, mType, Type);
    CC_SYNTHESIZE(cocos2d::Label*, mDebugLabel, DebugLabel);
    CC_PROPERTY(cocos2d::Sprite*, mSpriteNode, SpriteNode);

    CC_SYNTHESIZE(std::function<void(BaseObj*)>, mRemoveObjectCallback, RemoveObjectCallback)

};