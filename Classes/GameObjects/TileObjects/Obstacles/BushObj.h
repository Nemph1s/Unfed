/**
* @file GameObjects/Obstacles/BushObj.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "GameObjects/TileObjects/TileObj.h"

class BushObj : public TileObj
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~BushObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static BushObj * create(const CommonTypes::TileInfo &info);

    bool init(const CommonTypes::TileInfo &info);

    cocos2d::String& spriteName() const override;
    cocos2d::String& description() const override;

protected:
    // Nodes should be created using create();
    BushObj();

    //---Class Attributes-------------------------------------------------
};

