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
#include "Utils/PlatformMacros.h"

class LevelObj;
class SwapObj;
class CookieObj;

class GameplayScene : public cocos2d::Scene
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~GameplayScene();

public:
    static GameplayScene* createWithSize(const cocos2d::Size& size);

    virtual bool initWithSize(const cocos2d::Size& size);

	virtual void onEnter() override;
	virtual void onExit() override;

	void addTiles();
    void addSpritesForCookies(cocos2d::Set* cookies);

    void createSpriteWithCookie(CookieObj* cookie, int column, int row);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);

    void userInteractionEnabled();
    void userInteractionDisabled();

    void showSelectionIndicatorForCookie(CookieObj* cookie);
    void hideSelectionIndicator();

    void removeAllCookieSprites();

protected:
    // Nodes should be created using create();
    GameplayScene();

    bool isCookieTouched();
    void clearTouchedCookie();
    void updateSwipeDelta(int column, int row, int& horzDelta, int& vertDelta);

    //---Class Attributes-------------------------------------------------
	cocos2d::EventListener* mListener;

    CC_PROPERTY(LevelObj*, mLevel, Level);

    CC_SYNTHESIZE(std::function<bool(int horzDelta, int vertDelta)>, mTrySwapCookieCallback, TrySwapCookieCallback);

	CC_SYNTHESIZE_READONLY(int, mSwipeFromColumn, SwipeFromColumn);
    CC_SYNTHESIZE_READONLY(int, mSwipeFromRow, SwipeFromRow);

    CC_SYNTHESIZE_READONLY_PTR(cocos2d::Sprite*, mSelectionSprite, SelectionSprite);
    CC_SYNTHESIZE_READONLY_PTR(cocos2d::Layer*, mGameLayer, GameLayer);
    CC_SYNTHESIZE_READONLY_PTR(cocos2d::Layer*, mTilesLayer, TilesLayer);
    CC_SYNTHESIZE_READONLY_PTR(cocos2d::Layer*, mCookiesLayer, CookiesLayer);
};

