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
    static BaseObj * create(const CommonTypes::BaseObjInfo &info);

    virtual bool init();
    bool init(const CommonTypes::BaseObjInfo &info);

    CommonTypes::Cell getCell() const;

    virtual cocos2d::String& spriteName() const;
    virtual cocos2d::String& description() const;

    virtual int getTypeAsInt() const;

    virtual void match();
    virtual bool isHpEnded() const;

    virtual void clear();

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

    cocos2d::String* mDummyString;

    CC_SYNTHESIZE(int, mColumn, Column);
    CC_SYNTHESIZE(int, mRow, Row);
    CC_SYNTHESIZE(CommonTypes::BaseObjType, mType, Type);
    CC_SYNTHESIZE(cocos2d::Sprite*, mSpriteNode, SpriteNode);

};