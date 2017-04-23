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
#include <map>

class BaseObj;
class LevelObj;

class _SpritesFactory
{
public:
    static _SpritesFactory& Instance()
    {
        static _SpritesFactory myInstance;
        return myInstance;
    }
    static _SpritesFactory* getInstance();

    cocos2d::Sprite* createWithBaseObject(BaseObj* obj);
    cocos2d::Sprite* createHintSprite(const cocos2d::Color4B& color);
    cocos2d::Sprite* createGoalSprite(int baseType, int objType);
    
    bool initTilesPool(int poolSize);
    bool initCookiesPool(int poolSize);
    bool initFieldObjectsPool(int poolSize);
    bool initDudesPool(int poolSize); 
    bool initEnemyPool(int poolSize);

    bool initHintPool(int poolSize);

    void recycleHintSprite(cocos2d::Sprite* spriteNode);
    void recycle(cocos2d::Sprite* spriteNode, BaseObj* obj);

protected:
    explicit _SpritesFactory() {};
    ~_SpritesFactory();
    _SpritesFactory(_SpritesFactory const&) = delete;
    _SpritesFactory(_SpritesFactory &&) = delete;
    _SpritesFactory & operator=(_SpritesFactory const&) = delete;
    _SpritesFactory & operator=(_SpritesFactory &&) = delete;

private:
    typedef typename std::list<cocos2d::Sprite*> TSpriteList;
    typedef typename std::list<cocos2d::Sprite*>::iterator TSpriteListItr;

    cocos2d::Sprite* createForTileObj(BaseObj* obj);
    cocos2d::Sprite* createForCookieObj(BaseObj* obj);
    cocos2d::Sprite* createForFieldObj(BaseObj* obj);
    cocos2d::Sprite* createForDudeObj(BaseObj* obj);
    cocos2d::Sprite* createForEnemyObj(BaseObj* obj);

    cocos2d::Sprite* createSpriteForObj(BaseObj* obj);
    cocos2d::Sprite* createNewHintSprite(const cocos2d::Color4B& color, bool isVisible = false);

    
    void clearPool(TSpriteList* pool);

    std::map<GOT::CookieType, TSpriteList*> mCookieSpritesPool;
    std::map<GOT::FieldType, TSpriteList*> mFieldSpritesPool;
    std::map<GOT::FieldType, TSpriteList*> mDudeSpritesPool;
    std::map<GOT::EnemyType, TSpriteList*> mEnemySpritesPool;
    TSpriteList* mTileSpritesPool = nullptr;

    TSpriteList* mHintSpritesPool = nullptr;

    CC_SYNTHESIZE(LevelObj*, mLevel, Level);

};

#define SpritesFactory _SpritesFactory::getInstance()
