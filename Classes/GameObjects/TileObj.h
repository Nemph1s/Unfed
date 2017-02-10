/**
* @file GameObjects/TileObj.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"
#include "GameObjects/Base/BaseObj.h"


class TileObj : public BaseObj
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~TileObj();

/*    friend SmartFactory;*/

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static TileObj * create(const CommonTypes::TileInfo &info);

    bool init(const CommonTypes::TileInfo &info);

    virtual cocos2d::String& spriteName() const override;

    virtual int getTypeAsInt() const override;

    virtual void clear() override;

    bool isEmptyTile();

protected:
    // Nodes should be created using create();
    TileObj();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE_READONLY(CommonTypes::TileType, mTileType, TileType);  
};

