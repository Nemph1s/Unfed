/**
* @file Scenes/GameplayScene.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"

//temporary variables
//TODO: move to globalInfo or to json file
static const float TileWidth = 32.0 * 2;
static const float TileHeight = 36.0 * 2;

class LevelObj;
class GameplayScene : public cocos2d::Scene
{
public:

    static GameplayScene* createWithSize(const cocos2d::Size& size);

 CC_CONSTRUCTOR_ACCESS:
    // Nodes should be created using create();
    GameplayScene();
    virtual ~GameplayScene();

    virtual bool initWithSize(const cocos2d::Size& size);

	virtual void onEnter() override;
	virtual void onExit() override;

	void addTiles();
    void addSpritesForCookies(cocos2d::Set* cookies);
    cocos2d::Vec2 pointForColumnAndRow(int column, int row);

	bool convertPointToTilePos(cocos2d::Vec2& point, int& column, int& row);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

protected:

	cocos2d::EventListener* mListener;
    CC_PROPERTY(LevelObj*, mLevel, Level);

	CC_SYNTHESIZE_READONLY(int, mSwipeFromColumn, SwipeFromColumn);
    CC_SYNTHESIZE_READONLY(int, mSwipeFromRow, SwipeFromRow);
	
	CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mGameLayer, GameLayer);
	CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mTilesLayer, TilesLayer);
	CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mCookiesLayer, CookiesLayer);
};

