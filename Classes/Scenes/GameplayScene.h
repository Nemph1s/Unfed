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

class BaseObj;
class CookieObj;
class FieldObj;

class LevelObj;
class CookiesLayer;

class GameplayScene : public cocos2d::Scene
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~GameplayScene();

public:
    static GameplayScene* createWithSize(const cocos2d::Size& size);

    virtual bool initWithSize(const cocos2d::Size& size);

	void addTiles();
    void addFieldObjectsAt(int column, int row);
    void addSpritesForCookies(cocos2d::Set* cookies);
    void addSpritesForObjects(cocos2d::Set* set);

    void createSpriteWithCookie(CookieObj* cookie, int column, int row);
    void createSpriteWithDude(BaseObj* dudeObj);
    void createSpriteWithFieldObj(FieldObj* fieldObj);

    void userInteractionEnabled();
    void userInteractionDisabled();

    void setSwapCookieCallback(std::function<bool(int fromCol, int fromRow, int direction)> func);
    void setDudeActivationCallback(std::function<bool(int fromCol, int fromRow, int direction)> func);
    void removeAllCookieSprites();

protected:
    // Nodes should be created using create();
    GameplayScene();

    //---Class Attributes-------------------------------------------------

    CC_PROPERTY(LevelObj*, mLevel, Level);

    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mGameLayer, GameLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mTilesLayer, TilesLayer);
    CC_SYNTHESIZE_READONLY(CookiesLayer*, mCookiesLayer, CookiesLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mFieldObjectsLayer, FieldObjectsLayer);
};

