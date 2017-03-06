/**
* @file Common/Factory/SmartFactory.hpp
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
#include <list>

class BaseObj;

class _SmartFactory
{
public:
    static _SmartFactory& Instance()
    {
        static _SmartFactory myInstance;
        return myInstance;
    }
    static _SmartFactory* getInstance();

    BaseObj* create(const CommonTypes::BaseObjectInfo &info);

    BaseObj* createBaseObj(const CommonTypes::BaseObjectInfo &info);
    BaseObj* createTileObj(const CommonTypes::TileInfo &info);
    BaseObj* createCookieObj(const CommonTypes::CookieInfo &info);
    BaseObj* createFieldObj(const CommonTypes::FieldInfo &info);
    BaseObj* createDudeObj(const CommonTypes::FieldInfo &info);

    bool init(int poolSize);
    bool initCookiesPool(int poolSize);
    bool initTilesPool(int poolSize);

    void recycle(BaseObj* obj);

protected:
    explicit _SmartFactory() {};
    ~_SmartFactory();
    _SmartFactory(_SmartFactory const&) = delete;
    _SmartFactory(_SmartFactory &&) = delete;
    _SmartFactory & operator=(_SmartFactory const&) = delete;
    _SmartFactory & operator=(_SmartFactory &&) = delete;

private:
    typedef typename std::list<BaseObj*> TBaseObjList; 
    typedef typename std::list<BaseObj*>::iterator TBaseObjListItr;

    TBaseObjList* mBaseObjPool = nullptr;
    TBaseObjList* mCookieObjPool = nullptr;
    TBaseObjList* mTileObjPool = nullptr;

};

#define SmartFactory _SmartFactory::getInstance()
