/**
* @file GameObjects/ChainObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/


#include "GameObjects/ChainObj.h"
#include "GameObjects/CookieObj.h"
#include "Utils/Helpers/Helper.h"

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
ChainObj::~ChainObj()
//--------------------------------------------------------------------
{
    cocos2d::log("ChainObj::~ChainObj: deallocing ChainObj: %p - tag: %i", this, _tag);
    CC_SAFE_RELEASE_NULL(mCookies);
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
        mCookies = new cocos2d::Array();
        mCookies->init();
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

//--------------------------------------------------------------------
ChainObj::ChainObj()
    : mCookies(nullptr)
//--------------------------------------------------------------------
{
}
