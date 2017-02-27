/**
* @file GameObjects/TileObjects/Obstacles/RockObj.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "GameObjects/TileObjects/TileObj.h"

class RockObj : public TileObj
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~RockObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static RockObj * create(const CommonTypes::TileInfo &info);

    bool init(const CommonTypes::TileInfo &info);

    cocos2d::String& spriteName() const override;

    virtual bool checkMatchingCondition(int column, int row) override;

protected:
    // Nodes should be created using create();
    RockObj();

    //---Class Attributes-------------------------------------------------
};
