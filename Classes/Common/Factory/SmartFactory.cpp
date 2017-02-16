/**
* @file Common/Factory/SmartFactory.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Common/Factory/SmartFactory.h"

#include "GameObjects/TileObjects/Base/BaseObj.h"
#include "GameObjects/TileObjects/TileObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/TileObjects/DirtObject.h"


//--------------------------------------------------------------------
_SmartFactory * _SmartFactory::getInstance()
//--------------------------------------------------------------------
{
    return &_SmartFactory::Instance();
}

//--------------------------------------------------------------------
bool _SmartFactory::init(int poolSize)
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

    cocos2d::log("SmartFactory::init: actual mBaseObjPool size=%d", mBaseObjPool->size());
    return result;
}

//--------------------------------------------------------------------
bool _SmartFactory::initCookiesPool(int poolSize)
//--------------------------------------------------------------------
{
    bool result = false;

    do {
        mCookieObjPool = new TBaseObjList;
        CCASSERT(mCookieObjPool, "error while creating mCookieObjPool");

        while (poolSize--) {
            auto info = CommonTypes::BaseObjectInfo(CommonTypes::BaseObjectType::CookieObj);
            CommonTypes::CookieInfo cookieInfo = { info, CommonTypes::CookieType::Unknown };
            auto baseObject = CookieObj::create(cookieInfo);
            CC_SAFE_RETAIN(baseObject);
            CCASSERT(baseObject, "error while creating CookieObj");

            mCookieObjPool->push_back(baseObject);
        }
        result = true;
    } while (0);

    cocos2d::log("SmartFactory::initCookiesPool: actual mCookieObjPool size=%d", mCookieObjPool->size());
    return result;
}

//--------------------------------------------------------------------
bool _SmartFactory::initTilesPool(int poolSize)
//--------------------------------------------------------------------
{
    bool result = false;

    do {
        mTileObjPool = new TBaseObjList;
        CCASSERT(mTileObjPool, "error while creating mTileObjPool");

        while (poolSize--) {
            auto info = CommonTypes::BaseObjectInfo(CommonTypes::BaseObjectType::TileObj);
            CommonTypes::TileInfo tileInfo = { info, CommonTypes::TileType::Unknown };
            auto baseObject = TileObj::create(tileInfo);
            CC_SAFE_RETAIN(baseObject);
            CCASSERT(baseObject, "error while creating TileObj");

            mTileObjPool->push_back(baseObject);
        }
        result = true;
    } while (0);

    cocos2d::log("SmartFactory::initTilesPool: actual mCookieObjPool size=%d", mTileObjPool->size());
    return result;
}

//--------------------------------------------------------------------
BaseObj * _SmartFactory::create(const CommonTypes::BaseObjectInfo & info)
//--------------------------------------------------------------------
{
    BaseObj* baseObject = nullptr;
    CommonTypes::CookieInfo cookieInfo = { info, CommonTypes::CookieType::Unknown };
    CommonTypes::TileInfo tileInfo = { info, CommonTypes::TileType::Unknown };

    switch (info.type)
    {
    case CommonTypes::BaseObjectType::CookieObj:
        baseObject = createCookieObj(cookieInfo);
        break;
    case CommonTypes::BaseObjectType::TileObj:
        baseObject = createTileObj(tileInfo);
        break;
    case CommonTypes::BaseObjectType::FieldObj:
        baseObject = createFieldObj(tileInfo);
        break;
    case CommonTypes::BaseObjectType::Unknown:
    default:
        baseObject = createBaseObj(info);
        break;
    }
    cocos2d::log("SmartFactory::create: created with type=%d", baseObject->getTypeAsInt());

    return baseObject;
}

//--------------------------------------------------------------------
void _SmartFactory::recycle(BaseObj * obj)
//--------------------------------------------------------------------
{
    switch (obj->getType())
    {
    case CommonTypes::BaseObjectType::CookieObj:
        mCookieObjPool->push_back(obj);
        break;
    case CommonTypes::BaseObjectType::TileObj:
        mTileObjPool->push_back(obj);
        break;
    case CommonTypes::BaseObjectType::FieldObj:
        CC_SAFE_RELEASE(obj);
        break;
    case CommonTypes::BaseObjectType::Unknown:
    default:
        mBaseObjPool->push_back(obj);
        break;
    }
    cocos2d::log("SmartFactory::recycle: type=%d", obj->getTypeAsInt());
}

//--------------------------------------------------------------------
BaseObj * _SmartFactory::createBaseObj(const CommonTypes::BaseObjectInfo & info)
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
BaseObj * _SmartFactory::createTileObj(const CommonTypes::TileInfo & info)
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
BaseObj * _SmartFactory::createCookieObj(const CommonTypes::CookieInfo & info)
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

BaseObj * _SmartFactory::createFieldObj(const CommonTypes::TileInfo & info)
{
    BaseObj* baseObject = nullptr;

    switch (info.tileType)
    {
    case CommonTypes::TileType::Dirt:
    case CommonTypes::TileType::DirtX2:
    case CommonTypes::TileType::DirtX3:
        baseObject = DirtObject::create(info);
        CC_SAFE_RETAIN(baseObject);
        CCASSERT(baseObject, "error while creating DirtObject");
        break;
    default:
        CC_ASSERT(baseObject);
        break;
    }
    cocos2d::log("SmartFactory::create: created with type=%d", baseObject->getTypeAsInt());

    return baseObject;
}

//--------------------------------------------------------------------
_SmartFactory::~_SmartFactory()
//--------------------------------------------------------------------
{
    if (mBaseObjPool) {
        for (TBaseObjListItr itr = mBaseObjPool->begin(); itr != mBaseObjPool->end(); itr++) {
            BaseObj* entity = *itr;
            CC_SAFE_RELEASE(entity);
        }
        mBaseObjPool->clear();
        CC_SAFE_DELETE(mBaseObjPool);
    }
    if (mTileObjPool) {
        for (TBaseObjListItr itr = mTileObjPool->begin(); itr != mTileObjPool->end(); itr++) {
            BaseObj* entity = *itr;
            CC_SAFE_RELEASE(entity);
        }
        mTileObjPool->clear();
        CC_SAFE_DELETE(mTileObjPool);
    }
    if (mCookieObjPool) {
        for (TBaseObjListItr itr = mCookieObjPool->begin(); itr != mCookieObjPool->end(); itr++) {
            BaseObj* entity = *itr;
            CC_SAFE_RELEASE(entity);
        }
        mCookieObjPool->clear();
        CC_SAFE_DELETE(mCookieObjPool);
    }
}
