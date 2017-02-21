/**
* @file Layers/GameplayScene.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"

class LevelObj;
class CookieObj;

class CookiesLayer : public cocos2d::Layer
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~CookiesLayer();

public:
    static CookiesLayer* create();

    virtual bool init();

    virtual void onEnter() override;
    virtual void onExit() override;
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
    CookiesLayer();

    bool isCookieTouched();
    void clearTouchedCookie();
    void updateSwipeDelta(int column, int row, int& horzDelta, int& vertDelta);

    //---Class Attributes-------------------------------------------------
    cocos2d::EventListener* mListener;

    CC_SYNTHESIZE(std::function<bool(int horzDelta, int vertDelta)>, mTrySwapCookieCallback, TrySwapCookieCallback);

    CC_SYNTHESIZE(LevelObj*, mLevel, Level);

    CC_SYNTHESIZE_READONLY(int, mSwipeFromColumn, SwipeFromColumn);
    CC_SYNTHESIZE_READONLY(int, mSwipeFromRow, SwipeFromRow);

    CC_SYNTHESIZE_READONLY(cocos2d::Sprite*, mSelectionSprite, SelectionSprite);
};
