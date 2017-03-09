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

class BaseObj;
class TileObj;
class CookieObj;
class FieldObj;
class DudeObj;

class ObjContainer : public cocos2d::Ref
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~ObjContainer();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static ObjContainer* createWithObject(BaseObj* obj);

    virtual bool initWithObject(BaseObj* obj);

    bool addObject(BaseObj* obj);
    BaseObj* getObject(CommonTypes::BaseObjectType& type) const;
    bool removeObject(BaseObj* obj);

protected:
    // Nodes should be created using create();
    ObjContainer();

    bool addDudeObject(BaseObj* obj);
    bool addTileObject(BaseObj* obj);
    bool addFieldObject(BaseObj* obj);
    bool addCookieObject(BaseObj* obj);

    bool removeFieldObject(bool removeWithCleanup = true);

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE_READONLY(DudeObj*, mDudeObj, DudeObj);
    CC_SYNTHESIZE_READONLY(TileObj*, mTileObj, TileObj);
    CC_SYNTHESIZE_READONLY(std::list<FieldObj*>, mFieldObjects, FieldObjects);
    CC_SYNTHESIZE_READONLY(CookieObj*, mCookieObj, CookieObj);
};