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

class LevelObj;
class CookieObj;
class BaseObj;
class CookiesLayer;

class GameplayScene : public cocos2d::Scene
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~GameplayScene();

public:
    static GameplayScene* createWithSize(const cocos2d::Size& size);

    virtual bool initWithSize(const cocos2d::Size& size);

	void addTiles();
    void addSpritesForCookies(cocos2d::Set* cookies);

    void createSpriteWithCookie(CookieObj* cookie, int column, int row);
    void createSpriteWithFieldObj(BaseObj* fieldObj);

    void userInteractionEnabled();
    void userInteractionDisabled();

    void setSwapCookieCallback(std::function<bool(int horzDelta, int vertDelta)> func);
    void removeAllCookieSprites();

protected:
    // Nodes should be created using create();
    GameplayScene();

    //---Class Attributes-------------------------------------------------

    CC_PROPERTY(LevelObj*, mLevel, Level);

    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mGameLayer, GameLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mTilesLayer, TilesLayer);
    CC_SYNTHESIZE_READONLY(CookiesLayer*, mCookiesLayer, CookiesLayer);
};

