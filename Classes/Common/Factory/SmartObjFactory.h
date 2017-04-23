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
#include "Common/GameObjTypes.h"
 
#include <list>

class BaseObj;

class _SmartObjFactory
{
public:
    static _SmartObjFactory& Instance();
    static _SmartObjFactory* getInstance();

    BaseObj* create(const GOT::BaseObjInfo &info);

    BaseObj* createBaseObj(const GOT::BaseObjInfo &info);
    BaseObj* createTileObj(const GOT::TileInfo &info);
    BaseObj* createCookieObj(const GOT::CookieInfo &info);
    BaseObj* createFieldObj(const GOT::FieldInfo &info);
    BaseObj* createDudeObj(const GOT::FieldInfo &info);

    BaseObj* createEnemyObj(const GOT::EnemyInfo &info);

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
