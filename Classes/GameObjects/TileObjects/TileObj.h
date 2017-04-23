/**
* @file GameObjects/TileObjects/TileObj.h
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "GameObjects/TileObjects/Base/BaseObj.h"

class TileObj : public BaseObj
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~TileObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static TileObj * create(const GOT::TileInfo &info);

    bool init(const GOT::TileInfo &info);

    virtual cocos2d::String& spriteName() const override;
    virtual cocos2d::String& description() const override;

    virtual int getTypeAsInt() const override;

    bool isEmptyTile();

protected:
    // Nodes should be created using create();
    TileObj();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE_READONLY(GOT::TileType, mTileType, TileType);

    CC_SYNTHESIZE(cocos2d::Sprite*, mChainPreviewSpriteNode, ChainPreviewSpriteNode);
};

