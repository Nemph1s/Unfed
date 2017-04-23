/**
* @file Common/Factory/SmartObjFactory.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Common/Factory/SmartObjFactory.h"

#include "GameObjects/TileObjects/Base/BaseObj.h"
#include "GameObjects/TileObjects/TileObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "Controller/ObjectController/Dude/DudeObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"
#include "GameObjects/TileObjects/FieldObjects/DirtObject.h"
#include "GameObjects/TileObjects/FieldObjects/BushObj.h"
#include "GameObjects/TileObjects/FieldObjects/RockObj.h"
#include "Controller/ObjectController/Enemy/EnemyObj.h"

using namespace CT;
using namespace GOT;

//--------------------------------------------------------------------
_SmartObjFactory & _SmartObjFactory::Instance()
//--------------------------------------------------------------------
{
    static _SmartObjFactory myInstance;
    return myInstance;
}

//--------------------------------------------------------------------
_SmartObjFactory * _SmartObjFactory::getInstance()
//--------------------------------------------------------------------
{
    return &_SmartObjFactory::Instance();
}

//--------------------------------------------------------------------
bool _SmartObjFactory::init(int poolSize)
//--------------------------------------------------------------------
{
    bool result = false;

    do {
        mBaseObjPool = new TBaseObjList;
        CCASSERT(mBaseObjPool, "error while creating mBaseObjPool");

        while (poolSize--) {
            auto baseObject = BaseObj::create();
            CC_SAFE_RETAIN(baseObject);
            CCASSERT(baseObject, "error while creating BaseObj");

            mBaseObjPool->push_back(baseObject);
        }
        result = true;
    } while (0);

    cocos2d::log("SmartObjFactory::init: actual mBaseObjPool size=%d", mBaseObjPool->size());
    return result;
}

//--------------------------------------------------------------------
bool _SmartObjFactory::initCookiesPool(int poolSize)
//--------------------------------------------------------------------
{
    bool result = false;

    do {
        mCookieObjPool = new TBaseObjList;
        CCASSERT(mCookieObjPool, "error while creating mCookieObjPool");

        while (poolSize--) {
            auto info = BaseObjInfo(BaseObjType::Cookie);
            CookieInfo cookieInfo = { info, CookieType::Unknown };
            auto baseObject = CookieObj::create(cookieInfo);
            CC_SAFE_RETAIN(baseObject);
            CCASSERT(baseObject, "error while creating CookieObj");

            mCookieObjPool->push_back(baseObject);
        }
        result = true;
    } while (0);

    cocos2d::log("SmartObjFactory::initCookiesPool: actual mCookieObjPool size=%d", mCookieObjPool->size());
    return result;
}

//--------------------------------------------------------------------
bool _SmartObjFactory::initTilesPool(int poolSize)
//--------------------------------------------------------------------
{
    bool result = false;

    do {
        mTileObjPool = new TBaseObjList;
        CCASSERT(mTileObjPool, "error while creating mTileObjPool");

        while (poolSize--) {
            auto info = BaseObjInfo(BaseObjType::Tile);
            TileInfo tileInfo = { info, TileType::Unknown };
            auto baseObject = TileObj::create(tileInfo);
            CC_SAFE_RETAIN(baseObject);
            CCASSERT(baseObject, "error while creating TileObj");

            mTileObjPool->push_back(baseObject);
        }
        result = true;
    } while (0);

    cocos2d::log("SmartObjFactory::initTilesPool: actual mCookieObjPool size=%d", mTileObjPool->size());
    return result;
}

//--------------------------------------------------------------------
BaseObj * _SmartObjFactory::create(const BaseObjInfo & info)
//--------------------------------------------------------------------
{
    BaseObj* baseObject = nullptr;
    CookieInfo cookieInfo = { info, CookieType::Unknown };
    TileInfo tileInfo = { info, TileType::Unknown };
    FieldInfo fieldInfo = { info, FieldType::Unknown };

    switch (info.type)
    {
    case BaseObjType::Cookie:
        baseObject = createCookieObj(cookieInfo);
        break;
    case BaseObjType::Tile:
        baseObject = createTileObj(tileInfo);
        break;
    case BaseObjType::Field:
        baseObject = createFieldObj(fieldInfo);
        break;
    case BaseObjType::Dude:
        baseObject = createDudeObj(fieldInfo);
        break;
    case BaseObjType::Unknown:
    default:
        baseObject = createBaseObj(info);
        break;
    }
    return baseObject;
}

//--------------------------------------------------------------------
void _SmartObjFactory::recycle(BaseObj * obj)
//--------------------------------------------------------------------
{
    //cocos2d::log("SmartObjFactory::recycle: type=%d", obj->getTypeAsInt());
    switch (obj->getType())
    {
    case BaseObjType::Cookie:
        mCookieObjPool->push_back(obj);
        break;
    case BaseObjType::Tile:
        mTileObjPool->push_back(obj);
        break;
    case BaseObjType::Field:
    case BaseObjType::Dude:
    case BaseObjType::Enemy:
    case BaseObjType::Unknown:
    default:
        obj->clear();
        CC_SAFE_RELEASE(obj);
        break;
    }
}

//--------------------------------------------------------------------
BaseObj * _SmartObjFactory::createBaseObj(const BaseObjInfo & info)
//--------------------------------------------------------------------
{
    BaseObj* baseObject = nullptr;
    // grab a pre-made entity from the pool or allocate a new one
    if (mBaseObjPool->size() > 0)
    {
        baseObject = mBaseObjPool->front();
        mBaseObjPool->pop_front();
        baseObject->clear();
    }
    else
    {
        baseObject = BaseObj::create(info);
        CCASSERT(baseObject, "error while creating BaseObj");
    }
    return baseObject;
}

//--------------------------------------------------------------------
BaseObj * _SmartObjFactory::createTileObj(const TileInfo & info)
//--------------------------------------------------------------------
{
    TileObj* baseObject = nullptr;

    if (mTileObjPool->size() > 0)
    {
        baseObject = dynamic_cast<TileObj*>(mTileObjPool->front());
        mTileObjPool->pop_front();
        baseObject->clear();
        baseObject->init(info);
    }
    else
    {
        baseObject = TileObj::create(info);
        CC_SAFE_RETAIN(baseObject);
        CCASSERT(baseObject, "error while creating TileObj");
    }
    return baseObject;
}

//--------------------------------------------------------------------
BaseObj * _SmartObjFactory::createCookieObj(const CookieInfo & info)
//--------------------------------------------------------------------
{
    CookieObj* baseObject = nullptr;

    if (mCookieObjPool->size() > 0)
    {
        baseObject = dynamic_cast<CookieObj*>(mCookieObjPool->front());
        mCookieObjPool->pop_front();
        baseObject->clear();
        baseObject->init(info);
    }
    else
    {
        baseObject = CookieObj::create(info);
        CC_SAFE_RETAIN(baseObject);
        CCASSERT(baseObject, "error while creating CookieObj");
    }
    return baseObject;
}

//--------------------------------------------------------------------
BaseObj* _SmartObjFactory::createFieldObj(const FieldInfo & info)
//--------------------------------------------------------------------
{
    BaseObj* baseObject = nullptr;

    switch (info.fieldType)
    {
    case FieldType::Dirt:
    case FieldType::Dirt_HP2:
    case FieldType::Dirt_HP3:
        baseObject = DirtObject::create(info);
        CC_SAFE_RETAIN(baseObject);
        CCASSERT(baseObject, "error while creating DirtObject");
        break;
    case FieldType::Bush:
    case FieldType::Bush_HP2:
        baseObject = BushObj::create(info);
        CC_SAFE_RETAIN(baseObject);
        CCASSERT(baseObject, "error while creating BushObj");
        break;
    case FieldType::RockWall:
        baseObject = RockObj::create(info);
        CC_SAFE_RETAIN(baseObject);
        CCASSERT(baseObject, "error while creating RockObj");
        break;
    default:
        CC_ASSERT(baseObject);
        break;
    }
    return baseObject;
}

//--------------------------------------------------------------------
BaseObj * _SmartObjFactory::createDudeObj(const FieldInfo & info)
//--------------------------------------------------------------------
{
    BaseObj* baseObject = nullptr;

    switch (info.fieldType)
    {
    case FieldType::DudeFromAToB:
    case FieldType::DudeFromAToBx3:
    case FieldType::DudeChainX:
    case FieldType::DudeAllOfType:
    case FieldType::DudeSquareBomb:
        baseObject = DudeObj::create(info);
        CC_SAFE_RETAIN(baseObject);
        CCASSERT(baseObject, "error while creating DudeObj");
        break;
    default:
        CC_ASSERT(baseObject);
        break;
    }
    return baseObject;
}

//--------------------------------------------------------------------
BaseObj* _SmartObjFactory::createEnemyObj(const GOT::EnemyInfo & info)
//--------------------------------------------------------------------
{
    BaseObj* baseObject = EnemyObj::create(info);
    CC_SAFE_RETAIN(baseObject);
    CCASSERT(baseObject, "error while creating EnemyObj");
    return baseObject;
}

//--------------------------------------------------------------------
_SmartObjFactory::~_SmartObjFactory()
//--------------------------------------------------------------------
{
    clearPool(mBaseObjPool);
    clearPool(mTileObjPool);
    clearPool(mCookieObjPool);
}

//--------------------------------------------------------------------
void _SmartObjFactory::clearPool(TBaseObjList * pool)
//--------------------------------------------------------------------
{
    if (pool) {
        for (TBaseObjListItr itr = pool->begin(); itr != pool->end(); itr++) {
            clearEntity(*itr);
        }
        pool->clear();
        CC_SAFE_DELETE(pool);
    }
}

//--------------------------------------------------------------------
void _SmartObjFactory::clearEntity(BaseObj * obj)
//--------------------------------------------------------------------
{
    if (obj) {
        obj->clear();
        CC_SAFE_RELEASE_NULL(obj);
    }    
}

