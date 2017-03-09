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
#include "Common/CommonTypes.h"
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
    static BaseObj * create(const CommonTypes::BaseObjectInfo &info);

    virtual bool init();
    bool init(const CommonTypes::BaseObjectInfo &info);

    virtual cocos2d::String& spriteName() const;
    virtual cocos2d::String& description() const;

    virtual int getTypeAsInt() const;

    virtual void match();
    virtual bool isReadyToRemove() const;

    virtual void clear();

    virtual void updateDebugLabel();

protected:
    // Nodes should be created using create();
    BaseObj();

    //---Class Attributes-------------------------------------------------

    CC_SYNTHESIZE_BOOL_READONLY(bool, mIsMovable, Movable);
    CC_SYNTHESIZE_BOOL_READONLY(bool, mIsRemovable, Removable);
    CC_SYNTHESIZE_BOOL_READONLY(bool, mIsSwappable, Swappable);
    CC_SYNTHESIZE_BOOL_READONLY(bool, mIsContainer, Container);

    CC_SYNTHESIZE_READONLY(uint8_t, mScoreValue, ScoreValue);

    cocos2d::String* mDummyString;

    CC_SYNTHESIZE(int, mColumn, Column);
    CC_SYNTHESIZE(int, mRow, Row);
    CC_SYNTHESIZE(CommonTypes::BaseObjectType, mType, Type);
    CC_SYNTHESIZE(cocos2d::Sprite*, mSpriteNode, SpriteNode);

};