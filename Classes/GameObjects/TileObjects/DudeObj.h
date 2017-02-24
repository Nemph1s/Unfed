/**
* @file GameObjects/TileObjects/DudeObj.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "GameObjects/TileObjects/TileObj.h"

class DudeObj : public TileObj
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~DudeObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static DudeObj * create(const CommonTypes::TileInfo &info);

    bool init(const CommonTypes::TileInfo &info);

    cocos2d::String& spriteName() const override;

    CommonTypes::DudePathType getPathType();


protected:
    // Nodes should be created using create();
    DudeObj();

    //---Class Attributes-------------------------------------------------
};

