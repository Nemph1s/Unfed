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

using CT::Cell;
 
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
    BaseObj* createRandomCookie(Cell& cell);

    void detectDirectionsForDudes();
    
    ObjContainer* getContainer(Cell& cell);
    ObjContainer* getContainer(int8_t column, int8_t row);

    BaseObj* getObjectForChain(Cell& cell);
    CT::Set* getObjectsForChain(Cell& cell);
    void synchronizeObjectAt(Cell& cell);

    TileObj* tileAt(Cell& cell);
    CookieObj* cookieAt(Cell& cell);
    BaseObj* fieldObjectAt(Cell& cell);
    std::list<FieldObj*>* fieldObjectsAt(Cell& cell);
    DudeObj* dudeAt(Cell& cell);
    
    bool hasChainAt(Cell& cell);
    bool isEmptyTileAt(Cell& cell);
    bool isPossibleToAddCookie(Cell& cell);
    bool isSameTypeOfCookieAt(int8_t column, int8_t row, int type);
    int getRandomCookieType(Cell& cell);

    bool matchObject(BaseObj* obj);

    bool matchFieldObject(BaseObj* obj);
    bool matchCookieObject(BaseObj* obj);
    bool matchDudeObject(BaseObj* obj);

    void updateCookieObjectAt(Cell& cell, BaseObj* cookie);
    void updateObjectAt(Cell& cell, BaseObj* obj);
    void removeObjectAt(Cell& cell, CT::BaseObjType type);

    void removeCookie(Cell& cell);
    void removeAllCookies();

protected:
    // Nodes should be created using create();
    ObjectController();

    BaseObj* createTile(Cell& cell, int type);
    BaseObj* createCookie(Cell& cell, int type);
    BaseObj* createFieldObject(Cell& cell, int type, int priority);

    int getAllowedRandomCookieType();
    
    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(LevelObj*, mLevel, Level);
    CC_SYNTHESIZE(DudeController*, mDudeCtrl, DudeController);

    ObjContainer* mObjects[_GlobalInfo::NumColumns][_GlobalInfo::NumRows] = { nullptr };
};