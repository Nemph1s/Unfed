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
#include "Common/GameObjTypes.h"

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
    void addFieldObjectsAt(CT::Cell& cell);
    void addSpritesForObjects(CT::Set* set);

    void createChainPreviewSprites(CT::Set* set);

    void createSpriteWithCookie(CookieObj* cookie, CT::Cell& cell);
    void createSpriteWithObj(BaseObj* obj, CT::Cell& cell);
    void createSpriteForDude(BaseObj* dudeObj);
    void createSpriteWithFieldObj(FieldObj* fieldObj);

    bool isObjTouched();

    void userInteractionEnabled();
    void userInteractionDisabled();

    void setUpdateDirectionCallback(std::function<CT::Set*(BaseObj* obj, int direction)> func);

    void setSwapCookieCallback(std::function<bool(CT::Cell& fromCell, int direction)> func);
    void setDudeActivationCallback(std::function<bool(CT::Cell& fromCell, int direction)> func);
    void removeAllCookieSprites();

    void removeAllChainPreviewSprites();

protected:
    // Nodes should be created using create();
    GameplayScene();

    //---Class Attributes-------------------------------------------------

    CC_PROPERTY(LevelObj*, mLevel, Level);

    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mGameLayer, GameLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mTilesLayer, TilesLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mChainPreviewLayer, ChainPreviewLayer);
    CC_SYNTHESIZE_READONLY(CookiesLayer*, mCookiesLayer, CookiesLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mFieldObjectsLayer, FieldObjectsLayer);

    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, mInfoLayer, InfoLayer);
};

