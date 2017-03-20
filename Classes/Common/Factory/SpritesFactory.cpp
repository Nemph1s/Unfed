/**
* @file Common/Factory/SmartObjFactory.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Common/Factory/SpritesFactory.h"

#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

#include "GameObjects/TileObjects/Base/BaseObj.h"
#include "GameObjects/TileObjects/TileObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/TileObjects/DudeObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"
#include "GameObjects/TileObjects/FieldObjects/DirtObject.h"
#include "GameObjects/TileObjects/FieldObjects/BushObj.h"
#include "GameObjects/TileObjects/FieldObjects/RockObj.h"

using namespace CommonTypes;

//--------------------------------------------------------------------
_SpritesFactory * _SpritesFactory::getInstance()
//--------------------------------------------------------------------
{
    return &_SpritesFactory::Instance();
}

//--------------------------------------------------------------------
Sprite* _SpritesFactory::createWithBaseObject(BaseObj* obj)
//--------------------------------------------------------------------
{
    Sprite* sprite = nullptr;
    if (!obj) {
        return sprite;
    }
    switch (obj->getType())
    {
    case BaseObjType::Cookie:
        sprite = createForCookieObj(obj);
        break;
    case BaseObjType::Tile:
        sprite = createForTileObj(obj);
        break;
    case BaseObjType::Field:
        sprite = createForFieldObj(obj);
        break;
    case BaseObjType::Dude:
        sprite = createForFieldObj(obj);
        break;
    case BaseObjType::Unknown:
    default:
        break;
    }
    return sprite;
}

//--------------------------------------------------------------------
bool _SpritesFactory::initCookiesPool(int poolSize)
//--------------------------------------------------------------------
{
    auto cookieTypeInt = Helper::to_underlying(CookieType::Croissant);
    auto cookieTypeMax = Helper::to_underlying(CommonTypes::CookieType::CookieMax) - 1;
    while(cookieTypeInt != cookieTypeMax) {
        auto cookieList = new TSpriteList;
        CCASSERT(cookieList, "error while creating cookieList with type:%d", cookieTypeInt);

        auto cookiePoolSize = poolSize;
        while (cookiePoolSize--) {
            auto sprite = Sprite::create(GameResources::s_cookieSpriteNames.at(cookieTypeInt).getCString());
            sprite->setTag(cookieTypeInt);
            sprite->setVisible(false);
            CC_SAFE_RETAIN(sprite);
            CCASSERT(sprite, "error while creating sprite for CookieObj with type:%d", cookieTypeInt);

            cookieList->push_back(sprite);
        }
        auto type = static_cast<CookieType>(cookieTypeInt);
        mCookieSpritesPool[type] = cookieList;
        cocos2d::log("SpritesFactory::initCookiesPool: actual cookieList type=%d size=%d", cookieTypeInt, cookieList->size());
        cookieTypeInt++;
    };

    cocos2d::log("SpritesFactory::initCookiesPool: actual mCookieSpritesPool size=%d", mCookieSpritesPool.size());
    return true;
}

//--------------------------------------------------------------------
bool _SpritesFactory::initFieldObjectsPool(int poolSize)
//--------------------------------------------------------------------
{
    return false;
}

//--------------------------------------------------------------------
bool _SpritesFactory::initTilesPool(int poolSize)
//--------------------------------------------------------------------
{
    bool result = false;

    do {
        mTileSpritesPool = new TSpriteList;
        CCASSERT(mTileSpritesPool, "error while creating mTilesObjPool");

        while (poolSize--) {
            auto sprite = Sprite::create(GameResources::s_TileImg.getCString());
            sprite->setVisible(false);
            CC_SAFE_RETAIN(sprite);
            CCASSERT(sprite, "error while creating sprite for TileObj");

            mTileSpritesPool->push_back(sprite);
        }
        result = true;
    } while (0);

    cocos2d::log("SpritesFactory::initTilesPool: actual mCookieObjPool size=%d", mTileSpritesPool->size());
    return result;
}

//--------------------------------------------------------------------
void _SpritesFactory::recycle(Sprite* spriteNode, CommonTypes::BaseObjType & type)
//--------------------------------------------------------------------
{
    if (!spriteNode) {
        return;
    }
    
    switch (type)
    {
    case BaseObjType::Cookie:
    {
        auto cookieType = static_cast<CookieType>(spriteNode->getTag());
        mCookieSpritesPool.at(cookieType)->push_back(spriteNode);
    }
        break;
    case BaseObjType::Tile:
        mTileSpritesPool->push_back(spriteNode);
        break;
    case BaseObjType::Field:
    case BaseObjType::Dude:
    {
        auto fieldType = static_cast<FieldType>(spriteNode->getTag());
        mFieldSpritesPool.at(fieldType)->push_back(spriteNode);
    }
        break;
    case BaseObjType::Unknown:
    default:
        break;
    }
}

//--------------------------------------------------------------------
_SpritesFactory::~_SpritesFactory()
//--------------------------------------------------------------------
{
//     clearPool(mBaseObjPool);
//     clearPool(mTileObjPool);
//     clearPool(mCookieObjPool);
}

//--------------------------------------------------------------------
Sprite * _SpritesFactory::createForTileObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    return nullptr;
}

//--------------------------------------------------------------------
Sprite * _SpritesFactory::createForCookieObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    return nullptr;
}

//--------------------------------------------------------------------
Sprite * _SpritesFactory::createForFieldObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    return nullptr;
}

//--------------------------------------------------------------------
void _SpritesFactory::clearPool(TSpriteList * pool)
//--------------------------------------------------------------------
{
    if (pool) {
        for (TSpriteListItr itr = pool->begin(); itr != pool->end(); itr++) {
            CC_SAFE_RELEASE_NULL(*itr);
        }
        pool->clear();
        CC_SAFE_DELETE(pool);
    }
}