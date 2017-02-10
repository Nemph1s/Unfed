/**
* @file Controller/ObjectController.hpp
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

class BaseObj;
class TileObj;
class CookieObj;
class LevelObj;

class ObjectController : public cocos2d::Ref
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~ObjectController();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease" and "retain".
    */
    static ObjectController* create();

    bool init();

    void createInitialTiles();
    cocos2d::Set* createInitialCookies();

    BaseObj* createRandomCookie(int column, int row);
    int getRandomCookieType(int column, int row);

    TileObj* tileAt(int column, int row);
    CookieObj* cookieAt(int column, int row);
    bool hasChainAt(int column, int row);

    bool isEmptyTileAt(int column, int row);
    bool isSameTypeOfCookieAt(int column, int row, int type);

    void updateCookieObjectAt(int column, int row, BaseObj* cookie);
    void removeCookie(int column, int row);

protected:
    // Nodes should be created using create();
    ObjectController();

    BaseObj* createTile(int column, int row, int type);
    BaseObj* createCookie(int column, int row, int type);

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(LevelObj*, mLevel, Level);

    BaseObj* mTiles[CommonTypes::NumColumns][CommonTypes::NumRows] = { nullptr };
    BaseObj* mCookies[CommonTypes::NumColumns][CommonTypes::NumRows] = { nullptr };
};