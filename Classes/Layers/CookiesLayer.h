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
    void addSpritesForObjects(CommonTypes::Set* set);

    void createSpriteWithObj(BaseObj* obj, int column, int row);
    void createSpriteWithFieldObj(FieldObj* fieldObj, int column, int row);
    void createChainPreviewSprites(CommonTypes::Set* set);

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
    int getSwipeDirection(int newCol, int newRow);

    bool updateChainPreviewHint(int column, int row);


    //---Class Attributes-------------------------------------------------
    cocos2d::EventListener* mListener;

    CommonTypes::Direction mHintPreviewDirection = CommonTypes::Direction::Unknown;
    CC_SYNTHESIZE(std::function<CommonTypes::Set*(BaseObj* dudeObj, int direction)>, mUpdateDirectionCallback, UpdateDirectionCallback);

    CC_SYNTHESIZE(std::function<bool(int fromCol, int fromRow, int direction)>, mTrySwapCookieCallback, TrySwapCookieCallback);
    CC_SYNTHESIZE(std::function<bool(int fromCol, int fromRow, int direction)>, mCanActivateDudeCallback, CanActivateDudeCallback);

    CC_SYNTHESIZE(LevelObj*, mLevel, Level);

    CC_SYNTHESIZE(BaseObj*, mTouchedObj, TouchedObj);
    CC_SYNTHESIZE_READONLY(int, mSwipeFromColumn, SwipeFromColumn);
    CC_SYNTHESIZE_READONLY(int, mSwipeFromRow, SwipeFromRow);

    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mDudesLayer, DudesLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite*, mSelectionSprite, SelectionSprite);
};
