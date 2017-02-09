/**
* @file GameObjects/Base/BaseObj.hpp
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
    static BaseObj * create(const CommonTypes::GameObjectInfo &info);

    virtual bool init();
    bool init(const CommonTypes::GameObjectInfo &info);

    virtual cocos2d::String& spriteName() const;

    virtual int getTypeAsInt() const;

    virtual bool isMovable() const;
    virtual bool isRemovable() const;
    virtual bool isPossibleSwap() const;

protected:
    // Nodes should be created using create();
    BaseObj();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(int, mColumn, Column);
    CC_SYNTHESIZE(int, mRow, Row);
    CC_SYNTHESIZE(CommonTypes::GameObjectType, mType, Type);

};