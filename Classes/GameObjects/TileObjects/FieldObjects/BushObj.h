/**
* @file GameObjects/TileObjects/FieldObjects/BushObj.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"

class BushObj : public FieldObj
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~BushObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static BushObj * create(const CT::FieldInfo &info);

    bool init(const CT::FieldInfo &info);

    cocos2d::String& spriteName() const override;

    virtual bool checkMatchingCondition(int column, int row) override;

protected:
    // Nodes should be created using create();
    BushObj();

    //---Class Attributes-------------------------------------------------
};

