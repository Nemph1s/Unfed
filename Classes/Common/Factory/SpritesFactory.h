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
#include <map>

using cocos2d::Sprite;

class BaseObj;

class _SpritesFactory
{
public:
    static _SpritesFactory& Instance()
    {
        static _SpritesFactory myInstance;
        return myInstance;
    }
    static _SpritesFactory* getInstance();

    Sprite* createWithBaseObject(BaseObj* obj);
    
    bool initCookiesPool(int poolSize);
    bool initFieldObjectsPool(int poolSize);
    bool initTilesPool(int poolSize);

    void recycle(Sprite* spriteNode, CommonTypes::BaseObjType& type);

protected:
    explicit _SpritesFactory() {};
    ~_SpritesFactory();
    _SpritesFactory(_SpritesFactory const&) = delete;
    _SpritesFactory(_SpritesFactory &&) = delete;
    _SpritesFactory & operator=(_SpritesFactory const&) = delete;
    _SpritesFactory & operator=(_SpritesFactory &&) = delete;

private:
    typedef typename std::list<Sprite*> TSpriteList; 
    typedef typename std::list<Sprite*>::iterator TSpriteListItr;

    Sprite* createForTileObj(BaseObj* obj);
    Sprite* createForCookieObj(BaseObj* obj);
    Sprite* createForFieldObj(BaseObj* obj);

    void clearPool(TSpriteList* pool);

    std::map<CommonTypes::CookieType, TSpriteList*> mCookieSpritesPool;
    std::map<CommonTypes::FieldType, TSpriteList*> mFieldSpritesPool;
    TSpriteList* mTileSpritesPool = nullptr;

};

#define SpritesFactory _SpritesFactory::getInstance()
