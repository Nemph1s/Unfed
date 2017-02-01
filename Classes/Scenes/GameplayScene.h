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
#include <functional>

//temporary variables
//TODO: move to globalInfo or to json file
static const float TileWidth = 32.0 * 2;
static const float TileHeight = 36.0 * 2;

class LevelObj;
class SwapObj;
class CookieObj;
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
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);

    void userInteractionEnabled();
    void userInteractionDisabled();

    void animateSwap(SwapObj* swap, cocos2d::CallFunc* func);
    void animateInvalidSwap(SwapObj* swap, cocos2d::CallFunc* func);

    void showSelectionIndicatorForCookie(CookieObj* cookie);
    void hideSelectionIndicator();

protected:

    bool isCookieTouched();
    void clearTouchedCookie();
    void updateSwipeDelta(int column, int row, int& horzDelta, int& vertDelta);
    bool trySwapCookieTo(int horzDelta, int vertDelta);  

	cocos2d::EventListener* mListener;

    CC_PROPERTY(LevelObj*, mLevel, Level);

    CC_SYNTHESIZE(std::function<void(SwapObj* swap)>, mSwapCallback, SwapCallback);

	CC_SYNTHESIZE_READONLY(int, mSwipeFromColumn, SwipeFromColumn);
    CC_SYNTHESIZE_READONLY(int, mSwipeFromRow, SwipeFromRow);

    CC_SYNTHESIZE_READONLY(cocos2d::Sprite*, mSelectionSprite, SelectionSprite);

	CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mGameLayer, GameLayer);
	CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mTilesLayer, TilesLayer);
	CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mCookiesLayer, CookiesLayer);
};

