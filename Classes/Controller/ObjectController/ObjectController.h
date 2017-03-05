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
#include "GameObjects/TileObjects/FieldObjects/Base/FieldTypes.h"

class BaseObj;
class TileObj;
class CookieObj;
class LevelObj;
class DudeController;

class ObjectController : public cocos2d::Ref
{

    friend DudeController;

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
    void createInitialFieldObjects();
    cocos2d::Set* createInitialCookies();
    
    BaseObj* createRandomCookie(int column, int row);
    int getRandomCookieType(int column, int row);

    TileObj* tileAt(int column, int row);
    CookieObj* cookieAt(int column, int row);
    bool hasChainAt(int column, int row);

    BaseObj* fieldObjectAt(int column, int row);
    BaseObj* dudeObjectAt(int column, int row);

    bool isEmptyTileAt(int column, int row);
    bool isPossibleToAddCookie(int column, int row);
    bool isSameTypeOfCookieAt(int column, int row, int type);

    bool matchFieldObject(BaseObj* obj);

    void updateCookieObjectAt(int column, int row, BaseObj* cookie);
    void updateObjectAt(int column, int row, BaseObj* obj, CommonTypes::BaseObjectType type);
    void removeCookie(int column, int row);

    void removeAllCookies();

protected:
    // Nodes should be created using create();
    ObjectController();

    BaseObj* createTile(int column, int row, int type);
    BaseObj* createCookie(int column, int row, int type);
    
    BaseObj* createFieldObject(int column, int row, int type);

    int getAllowedRandomCookieType();
    
    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(LevelObj*, mLevel, Level);
    CC_SYNTHESIZE(DudeController*, mDudeCtrl, DudeController);

    BaseObj* mTiles[CommonTypes::NumColumns][CommonTypes::NumRows] = { nullptr };
    BaseObj* mCookies[CommonTypes::NumColumns][CommonTypes::NumRows] = { nullptr };
    // Tile object array (Dirt etc)
    BaseObj* mFieldObjects[CommonTypes::NumColumns][CommonTypes::NumRows] = { nullptr };
};