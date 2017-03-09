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
    : mTileObj(nullptr)
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
        case BaseObjectType::TileObj:
            result = addTileObject(obj);
            break;
        case BaseObjectType::FieldObj:
            result = addFieldObject(obj);
            break;
        case BaseObjectType::CookieObj:
            result = addCookieObject(obj);
            break;
        case BaseObjectType::DudeObj:
            result = addDudeObject(obj);
            break;
        default:
            break;
        }
    }
    return result;
}

//--------------------------------------------------------------------
BaseObj * ObjContainer::getObject(const CommonTypes::BaseObjectType& type) const
//--------------------------------------------------------------------
{
    BaseObj* obj = nullptr;
    switch (type)
    {
    case BaseObjectType::TileObj:
        obj = mTileObj;
        break;
    case BaseObjectType::FieldObj:
        obj = getFieldObject();
        break;
    case BaseObjectType::CookieObj:
        obj = mCookieObj;
        break;
    case BaseObjectType::DudeObj:
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
bool ObjContainer::removeObject(const CommonTypes::BaseObjectType& type)
//--------------------------------------------------------------------
{
    bool result = false;
    switch (type)
    {
    case BaseObjectType::TileObj:
        mTileObj = nullptr;
        break;
    case BaseObjectType::FieldObj:
        mFieldObjects.pop_front();
        break;
    case BaseObjectType::CookieObj:
        mCookieObj = nullptr;
        break;
    case BaseObjectType::DudeObj:
        mDudeObj = nullptr;
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
        //fieldObj->setPriority(mFieldObjects.size() + 1);
        mFieldObjects.push_back(fieldObj);
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
        return true;
    }
    return false;
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
            //scene->createSpriteWithFieldObj(fieldObj); use callback instead of
        }
        else if (fieldObj->isHpEnded()) {
            removeObject(BaseObjectType::FieldObj);

            SmartFactory->recycle(fieldObj);

            for (auto it = mFieldObjects.begin(); it != mFieldObjects.end(); ++it) {
                auto obj = dynamic_cast<FieldObj*>(*it);
                auto prio = obj->getPriority() - 1;
                obj->setPriority(prio);
                obj->setReadyToUpdatePriority(true);
            }
        }
    }
    
}