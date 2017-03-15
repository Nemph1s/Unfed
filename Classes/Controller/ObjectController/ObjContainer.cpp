/**
* @file Controller/ObjectController/ObjContainer.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ObjectController/ObjContainer.h"

#include "GameObjects/TileObjects/TileObj.h"
#include "GameObjects/TileObjects/DudeObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"

#include "Common/Factory/SmartFactory.h"

// #include "Controller/ObjectController/Dude/DudeController.h"
// 
// #include "Common/Factory/SmartFactory.h"
// #include "GameObjects/Level/LevelObj.h"
// 
// #include "Utils/Parser/JsonParser.h"
// #include "Utils/Helpers/Helper.h"


using namespace CommonTypes;


//--------------------------------------------------------------------
ObjContainer::ObjContainer()
    : cocos2d::Node()
    , mTileObj(nullptr)
    , mCookieObj(nullptr)
    , mDudeObj(nullptr)
    , mFieldObjects()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
ObjContainer::~ObjContainer()
//--------------------------------------------------------------------
{
    mTileObj = nullptr;
    mCookieObj = nullptr;
    mDudeObj = nullptr;
    mFieldObjects.clear();
}

//--------------------------------------------------------------------
ObjContainer* ObjContainer::create()
//--------------------------------------------------------------------
{
    ObjContainer * ret = new (std::nothrow) ObjContainer();
    if (ret && ret->init()) {
        ret->autorelease();
        CC_SAFE_RETAIN(ret);
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool ObjContainer::init()
//--------------------------------------------------------------------
{
    if (!cocos2d::Node::init()) {
        return false;
    }
    return true;
}

//--------------------------------------------------------------------
bool ObjContainer::addObject(BaseObj* obj)
//--------------------------------------------------------------------
{
    bool result = false;
    if (obj) {
        switch (obj->getType())
        {
        case BaseObjType::Tile:
            result = addTileObject(obj);
            break;
        case BaseObjType::Field:
            result = addFieldObject(obj);
            break;
        case BaseObjType::Cookie:
            result = addCookieObject(obj);
            break;
        case BaseObjType::Dude:
            result = addDudeObject(obj);
            break;
        default:
            break;
        }
    }
    return result;
}

//--------------------------------------------------------------------
BaseObj * ObjContainer::getObject(const CommonTypes::BaseObjType& type) const
//--------------------------------------------------------------------
{
    BaseObj* obj = nullptr;
    switch (type)
    {
    case BaseObjType::Tile:
        obj = mTileObj;
        break;
    case BaseObjType::Field:
        obj = getFieldObject();
        break;
    case BaseObjType::Cookie:
        obj = mCookieObj;
        break;
    case BaseObjType::Dude:
        obj = mDudeObj;
        break;
    default:
        break;
    }
    return obj;
}

//--------------------------------------------------------------------
FieldObj* ObjContainer::getFieldObject() const
//--------------------------------------------------------------------
{
    FieldObj* obj = nullptr;
    if (mFieldObjects.size() > 0)
        obj = mFieldObjects.front();
    return obj;
}

//--------------------------------------------------------------------
std::list<FieldObj*>& ObjContainer::getFieldObjects()
//--------------------------------------------------------------------
{
    return mFieldObjects;
}

//--------------------------------------------------------------------
bool ObjContainer::isContainObjForChain()
//--------------------------------------------------------------------
{
    if (mCookieObj || mDudeObj || getFieldObject()) {
        return true;
    }
    return false;
}

//--------------------------------------------------------------------
BaseObj* ObjContainer::getObjectForChain()
//--------------------------------------------------------------------
{
    BaseObj* obj = nullptr;
    auto fieldObj = getFieldObject();
    if (mCookieObj) {
        obj = mCookieObj;
    }
    else if (fieldObj) {
        obj = fieldObj;
    }
    else if (mDudeObj) {
        obj = mDudeObj;
    }
    return obj;
}

//--------------------------------------------------------------------
bool ObjContainer::removeObject(const CommonTypes::BaseObjType& type)
//--------------------------------------------------------------------
{
    bool result = false;
    switch (type)
    {
    case BaseObjType::Tile:
        CC_SAFE_RELEASE_NULL(mTileObj);
        break;
    case BaseObjType::Field:
        CC_SAFE_RELEASE(mFieldObjects.front());
        mFieldObjects.pop_front();
        break;
    case BaseObjType::Cookie:
        CC_SAFE_RELEASE_NULL(mCookieObj);
        break;
    case BaseObjType::Dude:
        CC_SAFE_RELEASE_NULL(mDudeObj);
        break;
    default:
        break;
    }
    return result;
}


//--------------------------------------------------------------------
bool ObjContainer::isEmptyTileAt()
//--------------------------------------------------------------------
{
    return mTileObj ? mTileObj->isEmptyTile() : false;
}

//--------------------------------------------------------------------
bool ObjContainer::isPossibleToAddCookie()
//--------------------------------------------------------------------
{
    // If there's a tile at a position but no cookie, then there's a hole.
    auto isEmptyTile = isEmptyTileAt();
    
    if (!isEmptyTile && mCookieObj == nullptr) {
        auto dudeObj = mDudeObj;
        if (!dudeObj) {
            auto fieldObj = getFieldObject();
            if (!fieldObj) {
                return true;
            }
            if (fieldObj->isContainer()) {
                return true;
            }
            if (fieldObj->isHpEnded() && fieldObj->getFieldObjChangeState()) {
                return true;
            }
        }

    }
    return false;
}

//--------------------------------------------------------------------
bool ObjContainer::isSameTypeOfCookieAt(int type)
//--------------------------------------------------------------------
{
    if (!mCookieObj)
        return false;

    if (mCookieObj->getTypeAsInt() != type)
        return false;

    return true;
}

//--------------------------------------------------------------------
bool ObjContainer::addDudeObject(BaseObj* obj)
//--------------------------------------------------------------------
{
    auto dudeObj = dynamic_cast<DudeObj*>(obj);
    if (dudeObj) {
        mDudeObj = dudeObj;
        CC_SAFE_RETAIN(mDudeObj);
        return true;
    }
    return false;
}

//--------------------------------------------------------------------
bool ObjContainer::addTileObject(BaseObj* obj)
//--------------------------------------------------------------------
{
    auto tileObj = dynamic_cast<TileObj*>(obj);
    if (tileObj) {
        mTileObj = tileObj;
        CC_SAFE_RETAIN(mTileObj);
        return true;
    }
    return false;
}

//--------------------------------------------------------------------
bool ObjContainer::addFieldObject(BaseObj* obj)
//--------------------------------------------------------------------
{
    auto fieldObj = dynamic_cast<FieldObj*>(obj);
    if (fieldObj) {
        mFieldObjects.push_back(fieldObj);
        CC_SAFE_RETAIN(fieldObj);
        return true;
    }
    return false;
}

//--------------------------------------------------------------------
bool ObjContainer::addCookieObject(BaseObj* obj)
//--------------------------------------------------------------------
{
    auto cookieObj = dynamic_cast<CookieObj*>(obj);
    if (cookieObj) {
        mCookieObj = cookieObj;
        CC_SAFE_RETAIN(mCookieObj);
        return true;
    }
    return false;
}

//--------------------------------------------------------------------
void ObjContainer::onRemoveCookie(BaseObj* obj)
//--------------------------------------------------------------------
{
    if (mCookieObj == obj) {
        if (mCookieObj->getSpriteNode()) {
            mCookieObj->getSpriteNode()->removeFromParent();
            mCookieObj->setSpriteNode(nullptr);
        }

        if (mCookieObj->getParent()) {
            mCookieObj->removeFromParent();
        }
        SmartFactory->recycle(mCookieObj);
        removeObject(BaseObjType::Cookie);
    }
}

//--------------------------------------------------------------------
void ObjContainer::onRemoveDude(BaseObj * obj)
//--------------------------------------------------------------------
{
    if (mDudeObj == obj) {
        if (mDudeObj->getSpriteNode()) {
            mDudeObj->getSpriteNode()->removeFromParent();
            mDudeObj->setSpriteNode(nullptr);
        }

        if (mDudeObj->getParent()) {
            mDudeObj->removeFromParent();
        }

        auto eraseDirectionsFunc = mDudeObj->getEraseDirectionsCallback();
        if (eraseDirectionsFunc) {
            eraseDirectionsFunc(obj);
        }

        SmartFactory->recycle(mDudeObj);
        removeObject(BaseObjType::Dude);
    }
}

//--------------------------------------------------------------------
void ObjContainer::onFieldObjChangeState(BaseObj* obj, std::function<void(FieldObj*)> createSpriteFunc)
//--------------------------------------------------------------------
{
    auto fieldObj = getFieldObject();
    if (fieldObj == obj) {
        if (fieldObj->getSpriteNode()) {
            fieldObj->getSpriteNode()->removeFromParent();
            fieldObj->setSpriteNode(nullptr);
        }

        if (fieldObj->getHP() > 0) {
            createSpriteFunc(fieldObj);
        }
        else if (fieldObj->isHpEnded()) {
            SmartFactory->recycle(fieldObj);

            removeObject(BaseObjType::Field);

            for (auto it = mFieldObjects.begin(); it != mFieldObjects.end(); ++it) {
                auto obj = dynamic_cast<FieldObj*>(*it);
                auto prio = obj->getPriority() - 1;
                obj->setPriority(prio);
                obj->setReadyToUpdatePriority(true);
            }
        }
    }
}