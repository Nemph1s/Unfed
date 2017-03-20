#/**
* @file Controller/ObjectController/ObjContainer.hpp
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
#include "Utils/PlatformMacros.h"

class BaseObj;
class TileObj;
class CookieObj;
class FieldObj;
class DudeObj;
class ChainObj;
class GameplayScene;

class ObjContainer : public cocos2d::Node
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~ObjContainer();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static ObjContainer* create();

    virtual bool init();

    bool addObject(BaseObj* obj);
    bool removeObject(const CommonTypes::BaseObjType& type);

    void updateObjectWith(BaseObj* currObj, BaseObj* newObj);
    void synchronizeTilePos();

    BaseObj* getObject(const CommonTypes::BaseObjType& type) const;
    FieldObj* getFieldObject() const;
    std::list<FieldObj*>& getFieldObjects();

    bool isContainObjForChain(); //TODO: rename it
    BaseObj* getObjectForChain();

    bool isEmptyTileAt();
    bool isPossibleToAddCookie();
    bool isSameTypeOfCookieAt(int type);

    //---Callbacks-------------------------------------------------
    void onRemoveCookie(BaseObj* obj);
    void onRemoveDude(BaseObj* obj);
    void onFieldObjChangeState(BaseObj* obj, std::function<void(FieldObj*)> createSpriteFunc);

protected:
    // Nodes should be created using create();
    ObjContainer();

    bool addDudeObject(BaseObj* obj);
    bool addTileObject(BaseObj* obj);
    bool addFieldObject(BaseObj* obj);
    bool addCookieObject(BaseObj* obj);

    //---Class Attributes-------------------------------------------------
    std::list<FieldObj*> mFieldObjects;

    CC_SYNTHESIZE_BOOL(ChainObj*, mObjectInChain, ObjectInChain);

    CC_SYNTHESIZE_READONLY(DudeObj*, mDudeObj, Dude);
    CC_SYNTHESIZE_READONLY(TileObj*, mTileObj, Tile);
    CC_SYNTHESIZE_READONLY(CookieObj*, mCookieObj, Cookie);
};