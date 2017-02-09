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
#include "Common/CommonTypes.h"
#include "Utils/PlatformMacros.h"

class TileObj : public cocos2d::Node
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~TileObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static TileObj * create(const CommonTypes::TileInfo &info);

    virtual bool init(const CommonTypes::TileInfo &info);

    std::string spriteName() const;

    int getTypeAsInt() const;

    bool isVisibleTile();

    //Move to subclass 

protected:
    // Nodes should be created using create();
    TileObj();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(int, mColumn, Column);
    CC_SYNTHESIZE(int, mRow, Row);

    CC_SYNTHESIZE_READONLY(CommonTypes::TileType, mType, Type);

    CC_SYNTHESIZE_PTR(cocos2d::Sprite*, mSpriteNode, SpriteNode);
   
};

