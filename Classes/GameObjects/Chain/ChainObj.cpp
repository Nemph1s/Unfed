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
#include "GameObjects/TileObjects/CookieObj.h"
#include "Utils/Helpers/Helper.h"

//--------------------------------------------------------------------
ChainObj::ChainObj()
    : mCookies(nullptr)
    , mIsCreatedByDude(false)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
void ChainObj::addObjectToGoalMap(BaseObj * obj)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);

    auto baseType = obj->getType();
    iterator baseObjInMap = mCollectGoalMap.find(baseType);
    if (baseObjInMap != mCollectGoalMap->end()) {
        auto map = static_cast<TFieldObjGoalMap>(*baseObjInMap);
        auto fieldObjInMap = map.find(obj->getTypeAsInt());
        if (fieldObjInMap != map->end()) {

        }
    }
    else {
        //TODO: create map with this kind of base object
    }

}

//--------------------------------------------------------------------
ChainObj::~ChainObj()
//--------------------------------------------------------------------
{
    cocos2d::log("ChainObj::~ChainObj: deallocing ChainObj: %p - tag: %i", this, _tag);
    if (getParent()) {
        removeFromParent();
    }
    CC_SAFE_RELEASE_NULL(mCookies);
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
    auto strChains = cocos2d::String::createWithFormat("\ttype:%d cookies:{\n", getTypeAsInt());
    for (auto it = mCookies->begin(); it != mCookies->end(); ++it, count++) {
        
        auto cookie = static_cast<CookieObj*>(*it);
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
void ChainObj::addCookie(CookieObj * cookie)
//--------------------------------------------------------------------
{
    if (mCookies == nullptr) {
        mCookies = cocos2d::Array::createWithCapacity(CommonTypes::NumColumns * CommonTypes::NumRows);
        CC_SAFE_RETAIN(mCookies);
    }
    mCookies->addObject(cookie);
}

//--------------------------------------------------------------------
void ChainObj::addCookiesFromChain(ChainObj * chain)
//--------------------------------------------------------------------
{
    cocos2d::log("ChainObj::addCookiesFromChain:");
    CC_ASSERT(chain);
    auto cookies = chain->getCookies();
    CC_ASSERT(cookies);

    for (auto it = cookies->begin(); it != cookies->end(); it++) {
        auto cookie = dynamic_cast<CookieObj*>(*it);
        CC_ASSERT(cookie);
        addCookie(cookie);
    }
}
