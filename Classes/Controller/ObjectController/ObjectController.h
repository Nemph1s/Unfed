/**
* @file Controller/ObjectController/ObjectController.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include <queue>
#include "cocos2d.h"
#include "Common/CommonTypes.h"
#include "Common/GlobalInfo/GlobalInfo.h"

class DudeObj;
class BaseObj;
class TileObj;
class CookieObj;
class FieldObj;
class LevelObj;
class DudeController;
class ObjContainer;

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

    void createObjects();
    CT::Set* createInitialFieldObjects();
    CT::Set* createInitialCookies();
    BaseObj* createRandomCookie(CT::Cell& cell);

    void detectDirectionsForDudes();
    
    ObjContainer* getContainer(CT::Cell& cell);

    BaseObj* getObjectForChain(CT::Cell& cell);
    CT::Set* getObjectsForChain(CT::Cell& cell);
    void synchronizeObjectAt(CT::Cell& cell);

    TileObj* tileAt(CT::Cell& cell);
    CookieObj* cookieAt(CT::Cell& cell);
    BaseObj* fieldObjectAt(CT::Cell& cell);
    std::list<FieldObj*>* fieldObjectsAt(CT::Cell& cell);
    DudeObj* dudeAt(CT::Cell& cell);
    
    bool hasChainAt(CT::Cell& cell);
    bool isEmptyTileAt(CT::Cell& cell);
    bool isPossibleToAddCookie(CT::Cell& cell);
    bool isSameTypeOfCookieAt(int8_t column, int8_t row, int type);
    int getRandomCookieType(CT::Cell& cell);

    bool matchObject(BaseObj* obj);

    bool matchFieldObject(BaseObj* obj);
    bool matchCookieObject(BaseObj* obj);
    bool matchDudeObject(BaseObj* obj);

    void updateCookieObjectAt(CT::Cell& cell, BaseObj* cookie);
    void updateObjectAt(CT::Cell& cell, BaseObj* obj);
    void removeObjectAt(CT::Cell& cell, CT::BaseObjType type);

    void removeCookie(CT::Cell& cell);
    void removeAllCookies();

protected:
    // Nodes should be created using create();
    ObjectController();

    BaseObj* createTile(CT::Cell& cell, int type);
    BaseObj* createCookie(CT::Cell& cell, int type);
    BaseObj* createFieldObject(CT::Cell& cell, int type, int priority);

    int getAllowedRandomCookieType();
    
    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(LevelObj*, mLevel, Level);
    CC_SYNTHESIZE(DudeController*, mDudeCtrl, DudeController);

    ObjContainer* mObjects[_GlobalInfo::NumColumns][_GlobalInfo::NumRows] = { nullptr };
};