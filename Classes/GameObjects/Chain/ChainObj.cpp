/**
* @file GameObjects/ChainObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/


#include "GameObjects/Chain/ChainObj.h"
#include "Controller/ObjectController/ObjContainer.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "Utils/Helpers/Helper.h"

//--------------------------------------------------------------------
ChainObj::ChainObj()
    : mObjects(nullptr)
    , mIsCreatedByDude(false)
    , mCookiesScore(0)
    , mScore(0)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
ChainObj::~ChainObj()
//--------------------------------------------------------------------
{
    cocos2d::log("ChainObj::~ChainObj: deallocing ChainObj: %p - tag: %i", this, _tag);
    if (getParent()) {
        removeFromParent();
    }
    CC_SAFE_RELEASE_NULL(mObjects);
}

//--------------------------------------------------------------------
ChainObj * ChainObj::createWithType(const CommonTypes::ChainType &type)
//--------------------------------------------------------------------
{
    ChainObj * ret = new (std::nothrow) ChainObj();
    if (ret && ret->initWithType(type)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool ChainObj::initWithType(const CommonTypes::ChainType &type)
//--------------------------------------------------------------------
{
    if (!Node::init()) {
        cocos2d::log("ChainObj::init: can't init Node inctance");
        return false;
    }

    mType = type;

    return true;
}

//--------------------------------------------------------------------
std::string ChainObj::description()
//--------------------------------------------------------------------
{
    int count = 0;
    auto objects = getChainObjects();
    auto strChains = cocos2d::String::createWithFormat("\ttype:%d cookies:{\n", getTypeAsInt());
    for (auto it = objects->begin(); it != objects->end(); ++it, count++) {
        
        auto cookie = static_cast<BaseObj*>(*it);
        strChains->appendWithFormat("\t\t%s\n", cookie->description().getCString());
    }
    strChains->append("\t}");

    return std::string(strChains->getCString());
}

//--------------------------------------------------------------------
std::string ChainObj::typeAsString()
//--------------------------------------------------------------------
{
    std::string type;
    switch (mType)
    {
    case CommonTypes::ChainType::ChainTypeHorizontal:
        type = "Horizontal";
        break;
    case CommonTypes::ChainType::ChainTypeVertical:
        type = "Vertical";
        break;
    case CommonTypes::ChainType::ChainTypeL:
        type = "Type L";
        break;
    case CommonTypes::ChainType::ChainTypeT:
        type = "Type T";
        break;
    case CommonTypes::ChainType::Unknown:
    default:
        type = "Unknown";
        break;
    }
    return type;
}

//--------------------------------------------------------------------
int ChainObj::getTypeAsInt()
//--------------------------------------------------------------------
{
    return Helper::getInstance()->to_underlying(mType);
}

//--------------------------------------------------------------------
void ChainObj::addObjectToChain(ObjContainer * obj)
//--------------------------------------------------------------------
{
    if (mObjects == nullptr) {
        mObjects = cocos2d::Array::createWithCapacity(CommonTypes::NumColumns * CommonTypes::NumRows);
        CC_SAFE_RETAIN(mObjects);
    }
    mObjects->addObject(obj);
}

//--------------------------------------------------------------------
void ChainObj::addCookiesFromChain(ChainObj* chain)
//--------------------------------------------------------------------
{
    cocos2d::log("ChainObj::addCookiesFromChain:");
    CC_ASSERT(chain);
    auto objects = chain->getObjects();
    CC_ASSERT(objects);

    for (auto it = objects->begin(); it != objects->end(); it++) {
        auto obj = dynamic_cast<ObjContainer*>(*it);
        CC_ASSERT(obj);
        addObjectToChain(obj);
    }
}

//--------------------------------------------------------------------
void ChainObj::executeCollectGoalCallback()
//--------------------------------------------------------------------
{
    auto objects = getChainObjects();
    for (auto it = objects->begin(); it != objects->end(); it++) {
        auto obj = dynamic_cast<BaseObj*>(*it);
        CC_ASSERT(obj);
        if (mUpdateGoalCallback) {
            mUpdateGoalCallback(obj);
        }
    }
}

//--------------------------------------------------------------------
cocos2d::Array* ChainObj::getChainObjects()
//--------------------------------------------------------------------
{
    auto arr = cocos2d::Array::create();
    for (auto it = mObjects->begin(); it != mObjects->end(); it++) {
        auto container = dynamic_cast<ObjContainer*>(*it);
        CC_ASSERT(container);

        auto obj = container->getObjectForChain();
        if (obj) {
            arr->addObject(obj);
        }
    }
    return arr;
}

//--------------------------------------------------------------------
int ChainObj::getCookiesCount()
//--------------------------------------------------------------------
{
    int count = 0;
    auto objs = getChainObjects();
    for (auto it = objs->begin(); it != objs->end(); it++) {
        auto obj = dynamic_cast<BaseObj*>(*it);
        CC_ASSERT(obj);
        if (obj->getType() == CommonTypes::BaseObjType::Cookie) {
            count++;
        }
    }
    return count;
}
