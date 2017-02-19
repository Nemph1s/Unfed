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

/*    friend SmartFactory;*/

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static TileObj * create(const CommonTypes::TileInfo &info);

    bool init(const CommonTypes::TileInfo &info);

    virtual cocos2d::String& spriteName() const override;

    virtual void setSpriteNode(cocos2d::Sprite* var) override;
    virtual int getTypeAsInt() const override;

    //TODO: move to field component
    virtual void match() override;
    virtual void clear() override;
    virtual bool checkMatchingCondition(int column, int row);
    virtual bool isReadyToRemove() const override;

    bool isEmptyTile();

    void updateDebugTileLabel();

protected:
    // Nodes should be created using create();
    TileObj();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE_READONLY(CommonTypes::TileType, mTileType, TileType);  
    CC_SYNTHESIZE(int, mHP, HP);
    CC_SYNTHESIZE(cocos2d::Label*, mDebugLabel, DebugLabel);
};

