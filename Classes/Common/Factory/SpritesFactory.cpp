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

#include "GameObjects/Level/LevelObj.h"
#include "GameObjects/TileObjects/TileObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "Controller/ObjectController/Dude/DudeObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"

using namespace CommonTypes;

const int kSpriteTag = 3;
const int kGlowingSpriteTag = 4;

const std::map<FieldType, cocos2d::String*> FieldTypeNames = {
    { FieldType::Bush, &GameResources::s_BushCorruptedImg }
    , { FieldType::Bush_HP2, &GameResources::s_BushNormalImg }
    , { FieldType::Dirt, &GameResources::s_DirtImg }
    , { FieldType::Dirt_HP2, &GameResources::s_DirtX2Img }
    , { FieldType::RockWall, &GameResources::s_RockImg } };

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
        sprite = createForDudeObj(obj);
        break;
    case BaseObjType::Unknown:
    default:
        break;
    }
    return sprite;
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
bool _SpritesFactory::initCookiesPool(int poolSize)
//--------------------------------------------------------------------
{
    auto cookieTypeInt = Helper::to_underlying(CookieType::Croissant);
    auto cookieTypeMax = Helper::to_underlying(CommonTypes::CookieType::CookieMax);
    while(cookieTypeInt != cookieTypeMax) {
        auto cookieList = new TSpriteList;
        CCASSERT(cookieList, "error while creating cookieLis");

        auto cookiePoolSize = poolSize;
        while (cookiePoolSize--) {
            auto sprite = Sprite::create(GameResources::s_cookieSpriteNames.at(cookieTypeInt).getCString());
            sprite->setTag(cookieTypeInt);
            sprite->setVisible(false);
            CC_SAFE_RETAIN(sprite);
            CCASSERT(sprite, "error while creating sprite for CookieObj");

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
    for (auto fieldName : FieldTypeNames) {
        
        auto fieldTypeInt = Helper::to_underlying(fieldName.first);
        auto fieldList = new TSpriteList;
        CCASSERT(fieldList, "error while creating fieldList");

        auto fieldObjsPoolSize = poolSize;
        while (fieldObjsPoolSize--) {
            auto sprite = Sprite::create(fieldName.second->getCString());
            sprite->setTag(fieldTypeInt);
            sprite->setVisible(false);
            CC_SAFE_RETAIN(sprite);
            CCASSERT(sprite, "error while creating sprite for FieldObj");

            fieldList->push_back(sprite);
        }
        mFieldSpritesPool[fieldName.first] = fieldList;
        cocos2d::log("SpritesFactory::initFieldObjectsPool: actual fieldList type=%d size=%d", fieldTypeInt, fieldList->size());
    }
    cocos2d::log("SpritesFactory::initFieldObjectsPool: actual mFieldSpritesPool size=%d", mFieldSpritesPool.size());
    return true;
}

//--------------------------------------------------------------------
bool _SpritesFactory::initDudesPool(int poolSize)
//--------------------------------------------------------------------
{
    auto dudeTypeInt = Helper::to_underlying(FieldType::DudeFromAToB);
    auto dudeTypeMax = Helper::to_underlying(FieldType::DudeAllOfType) + 1;
    while (dudeTypeInt != dudeTypeMax) {
        auto dudeList = new TSpriteList;
        CCASSERT(dudeList, "error while creating dudeList");

        auto dudePoolSize = poolSize;
        while (dudePoolSize--) {
            auto spriteName = GameResources::s_dudeSpriteNames.at(dudeTypeInt - Helper::to_underlying(FieldType::DudeFromAToB)).getCString();
            auto sprite = Sprite::create(spriteName);
            sprite->setTag(dudeTypeInt);
            sprite->setVisible(false);
            CC_SAFE_RETAIN(sprite);
            CCASSERT(sprite, "error while creating sprite for DudeObj");

            dudeList->push_back(sprite);
        }
        auto type = static_cast<FieldType>(dudeTypeInt);
        mDudeSpritesPool[type] = dudeList;
        cocos2d::log("SpritesFactory::initDudesPool: actual dudeList type=%d size=%d", dudeTypeInt, dudeList->size());
        dudeTypeInt++;
    };

    cocos2d::log("SpritesFactory::initDudesPool: actual mDudeSpritesPool size=%d", mDudeSpritesPool.size());
    return true;
}

//--------------------------------------------------------------------
bool _SpritesFactory::initHintPool(int poolSize)
//--------------------------------------------------------------------
{
    bool result = false;

    do {
        mHintSpritesPool = new TSpriteList;
        CCASSERT(mHintSpritesPool, "error while creating mHintSpritesPool");

        while (poolSize--) {
            auto sprite = createNewHintSprite(cocos2d::Color4B::WHITE);
            CCASSERT(sprite, "error while creating sprite for Hint");

            mHintSpritesPool->push_back(sprite);
        }
        result = true;
    } while (0);

    cocos2d::log("SpritesFactory::initHintPool: actual mHintSpritesPool size=%d", mHintSpritesPool->size());
    return result;
}

//--------------------------------------------------------------------
void _SpritesFactory::recycleHintSprite(Sprite* spriteNode)
//--------------------------------------------------------------------
{
    if (!spriteNode) {
        return;
    }
    spriteNode->setScale(1);
    spriteNode->setVisible(false);
    spriteNode->stopAllActions();
    if (spriteNode->getParent()) {
        spriteNode->removeFromParent();
    }
    mHintSpritesPool->push_back(spriteNode);
}

//--------------------------------------------------------------------
void _SpritesFactory::recycle(Sprite* spriteNode, BaseObj* obj)
//--------------------------------------------------------------------
{
    if (!spriteNode) {
        return;
    }
    spriteNode->setScale(1);
    spriteNode->setVisible(false);
    if (spriteNode->getParent()) {
        spriteNode->removeFromParent();
    }
    if (!obj) {
        return;
    }
    switch (obj->getType())
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
    {
        auto fieldType = static_cast<FieldType>(spriteNode->getTag());
        mFieldSpritesPool.at(fieldType)->push_back(spriteNode);
    }
    break;
    case BaseObjType::Dude:
    {
        auto fieldType = static_cast<FieldType>(spriteNode->getTag());
        mDudeSpritesPool.at(fieldType)->push_back(spriteNode);
    }
        break;
    case BaseObjType::Unknown:
    default:
        break;
    }
    spriteNode->setUserObject(nullptr);
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
    Sprite* sprite = nullptr;
    auto tile = dynamic_cast<TileObj*>(obj);
    if (!tile) {
        return sprite;
    }
    if (mTileSpritesPool->size() > 0) {
        sprite = mTileSpritesPool->front();
        mTileSpritesPool->pop_front();
        sprite->setVisible(true);
    }
    else {
        sprite = createSpriteForObj(obj);
    }
    return sprite;
}

//--------------------------------------------------------------------
Sprite* _SpritesFactory::createForCookieObj(BaseObj* obj)
//--------------------------------------------------------------------
{
    Sprite* sprite = nullptr;
    auto cookie = dynamic_cast<CookieObj*>(obj);
    if (!cookie) {
        return sprite;
    }
    auto type = cookie->getCookieType();
    if (mCookieSpritesPool.size() > 0) {
        if (mCookieSpritesPool.at(type)->size() > 0) {
            sprite = mCookieSpritesPool.at(type)->front();
            mCookieSpritesPool.at(type)->pop_front();
            sprite->setVisible(true);
        }
    }
    if (!sprite) {
        sprite = createSpriteForObj(obj);
    }
    return sprite;
}

//--------------------------------------------------------------------
Sprite * _SpritesFactory::createForFieldObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    Sprite* sprite = nullptr;
    auto fieldObj = dynamic_cast<FieldObj*>(obj);
    if (!fieldObj) {
        return sprite;
    }
    auto type = fieldObj->getFieldType();
    if (mFieldSpritesPool.size() > 0) {
        if (mFieldSpritesPool.at(type)->size() > 0) {
            sprite = mFieldSpritesPool.at(type)->front();
            mFieldSpritesPool.at(type)->pop_front();
            sprite->setVisible(true);
        }
    }
    if (!sprite) {
        sprite = createSpriteForObj(obj);
    }
    return sprite;
}

//--------------------------------------------------------------------
Sprite* _SpritesFactory::createForDudeObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    Sprite* sprite = nullptr;
    auto fieldObj = dynamic_cast<DudeObj*>(obj);
    if (!fieldObj) {
        return sprite;
    }
    auto type = fieldObj->getFieldType();
    if (mDudeSpritesPool.size() > 0) {
        if (mDudeSpritesPool.at(type)->size() > 0) {
            sprite = mDudeSpritesPool.at(type)->front();
            mDudeSpritesPool.at(type)->pop_front();
            sprite->setVisible(true);
        }
    }
    if (!sprite) {
        sprite = createSpriteForObj(obj);
    }
    return sprite;
}

//--------------------------------------------------------------------
Sprite* _SpritesFactory::createSpriteForObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    Sprite* sprite = nullptr;
    if (!obj) {
        return sprite;
    }
    auto spriteName = obj->spriteName().getCString();
    auto baseType = Helper::to_underlying(obj->getType());

    cocos2d::log("SpritesFactory::createSpriteForObj: spriteName=%s; baseType=%d; typeAsInt=%d;"
        , spriteName, baseType, obj->getTypeAsInt());

    sprite = Sprite::create(spriteName);
    sprite->setTag(obj->getTypeAsInt());
    sprite->setVisible(false);
    sprite->setUserObject(obj);
    CC_SAFE_RETAIN(sprite);
    return sprite;
}

//--------------------------------------------------------------------
Sprite* _SpritesFactory::createNewHintSprite(const cocos2d::Color4B& color, bool isVisible)
//--------------------------------------------------------------------
{
    auto color3b = cocos2d::Color3B(color);
    auto sprite = Sprite::create(GameResources::s_HintImg.getCString());
    sprite->setVisible(isVisible);
    sprite->setOpacity(255);
    sprite->setColor(color3b);

    cocos2d::BlendFunc f = { GL_ONE, GL_ONE };
    auto pos = cocos2d::Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    auto glowSprite = Sprite::create(GameResources::s_HintImg.getCString());

    glowSprite->setColor(color3b);
    glowSprite->setPosition(pos);
    glowSprite->setOpacity(200);
    glowSprite->setBlendFunc(f);
    sprite->addChild(glowSprite, -1, kGlowingSpriteTag);

    CC_SAFE_RETAIN(sprite);
    return sprite;
}

//--------------------------------------------------------------------
Sprite* _SpritesFactory::createHintSprite(const cocos2d::Color4B& color)
//--------------------------------------------------------------------
{
    Sprite* sprite = nullptr;
    if (mHintSpritesPool->size() > 0) {
        auto color3b = cocos2d::Color3B(color);
        sprite = mHintSpritesPool->front();
        mHintSpritesPool->pop_front();
        sprite->setVisible(true);
        sprite->setColor(color3b);
        sprite->getChildByTag(kGlowingSpriteTag)->setColor(color3b);
    }
    else {
        sprite = createNewHintSprite(color, true);
    }
    return sprite;
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