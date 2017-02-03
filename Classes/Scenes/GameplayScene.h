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

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);

    void userInteractionEnabled();
    void userInteractionDisabled();

    void animateSwap(SwapObj* swap, cocos2d::CallFunc* func);
    void animateInvalidSwap(SwapObj* swap, cocos2d::CallFunc* func);
    void animateMatching(cocos2d::Set* chains, cocos2d::CallFunc* func);
    void animateFallingCookies(cocos2d::Array* colums, cocos2d::CallFunc* func);
    void animateNewCookies(cocos2d::Array* colums, cocos2d::CallFunc* func);

    void showSelectionIndicatorForCookie(CookieObj* cookie);
    void hideSelectionIndicator();

protected:

    bool isCookieTouched();
    void clearTouchedCookie();
    void updateSwipeDelta(int column, int row, int& horzDelta, int& vertDelta);
    bool trySwapCookieTo(int horzDelta, int vertDelta);  
    
    void createSpriteWithCookie(CookieObj* cookie, int column, int row);

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

