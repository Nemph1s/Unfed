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
#include "Common/CommonTypes.h"

class LevelObj;
class BaseObj;
class CookieObj;
class FieldObj;

class CookiesLayer : public cocos2d::Layer
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~CookiesLayer();

public:
    static CookiesLayer* create();

    virtual bool init();

    virtual void onEnter() override;
    virtual void onExit() override;
    void addSpritesForObjects(CT::Set* set);

    void createSpriteWithObj(BaseObj* obj, CT::Cell& cell);
    void createSpriteWithFieldObj(FieldObj* fieldObj, CT::Cell& cell);
    void createChainPreviewSprites(CT::Set* set);

    bool isObjTouched();

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);

    void showSelectionIndicatorForCookie(CookieObj* cookie);
    void hideSelectionIndicator();

    void removeAllCookieSprites();
    void removeChainPreviewSprites();

protected:
    // Nodes should be created using create();
    CookiesLayer();
        
    void clearTouchedObj();
    int getSwipeDirection(CT::Cell& cell);
    bool isSameDirection(int direction);

    bool updateChainPreviewHint(CT::Cell& cell, int direction);


    //---Class Attributes-------------------------------------------------
    cocos2d::EventListener* mListener;

    CT::Direction mPreviousDirection = CT::Direction::Unknown;
    CC_SYNTHESIZE(std::function<CT::Set*(BaseObj* dudeObj, int direction)>, mUpdateDirectionCallback, UpdateDirectionCallback);

    CC_SYNTHESIZE(std::function<bool(CT::Cell& fromCell, int direction)>, mTrySwapCookieCallback, TrySwapCookieCallback);
    CC_SYNTHESIZE(std::function<bool(CT::Cell& fromCell, int direction)>, mCanActivateDudeCallback, CanActivateDudeCallback);

    CC_SYNTHESIZE(LevelObj*, mLevel, Level);

    CC_SYNTHESIZE(BaseObj*, mTouchedObj, TouchedObj);
    CC_SYNTHESIZE_READONLY(CT::Cell, mSwipeFromCell, SwipeFromCell);

    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mDudesLayer, DudesLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite*, mSelectionSprite, SelectionSprite);
};
