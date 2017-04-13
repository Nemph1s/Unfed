/**
* @file Common/Factory/SmartObjFactory.hpp
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
 
#include <list>

class BaseObj;

class _SmartObjFactory
{
public:
    static _SmartObjFactory& Instance();
    static _SmartObjFactory* getInstance();

    BaseObj* create(const CT::BaseObjInfo &info);

    BaseObj* createBaseObj(const CT::BaseObjInfo &info);
    BaseObj* createTileObj(const CT::TileInfo &info);
    BaseObj* createCookieObj(const CT::CookieInfo &info);
    BaseObj* createFieldObj(const CT::FieldInfo &info);
    BaseObj* createDudeObj(const CT::FieldInfo &info);

    bool init(int poolSize);
    bool initCookiesPool(int poolSize);
    bool initTilesPool(int poolSize);

    void recycle(BaseObj* obj);

protected:
    explicit _SmartObjFactory() {};
    ~_SmartObjFactory();
    _SmartObjFactory(_SmartObjFactory const&) = delete;
    _SmartObjFactory(_SmartObjFactory &&) = delete;
    _SmartObjFactory & operator=(_SmartObjFactory const&) = delete;
    _SmartObjFactory & operator=(_SmartObjFactory &&) = delete;

private:
    typedef typename std::list<BaseObj*> TBaseObjList; 
    typedef typename std::list<BaseObj*>::iterator TBaseObjListItr;

    void clearPool(TBaseObjList* pool);
    void clearEntity(BaseObj* obj);

    TBaseObjList* mBaseObjPool = nullptr;
    TBaseObjList* mCookieObjPool = nullptr;
    TBaseObjList* mTileObjPool = nullptr;

};

#define SmartObjFactory _SmartObjFactory::getInstance()
