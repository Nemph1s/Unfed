/**
* @file GameObjects/TileObjects/FieldObjects/DirtObject.h
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"

class DirtObject : public FieldObj
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~DirtObject();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static DirtObject * create(const GOT::FieldInfo &info);

    bool init(const GOT::FieldInfo &info);

    cocos2d::String& spriteName() const override;

    virtual bool checkMatchingCondition(CT::Cell& cell) override;

protected:
    // Nodes should be created using create();
    DirtObject();

    //---Class Attributes-------------------------------------------------
};
