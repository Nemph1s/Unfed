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
}

//--------------------------------------------------------------------
ObjContainer* ObjContainer::createWithObject(BaseObj * obj)
//--------------------------------------------------------------------
{
    ObjContainer * ret = new (std::nothrow) ObjContainer();
    if (ret && ret->initWithObject(obj)) {
        ret->autorelease();
        CC_SAFE_RETAIN(ret);
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool ObjContainer::initWithObject(BaseObj* obj)
//--------------------------------------------------------------------
{
    bool result = true;
    result = addObject(obj);

    return result;
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
BaseObj * ObjContainer::getObject(CommonTypes::BaseObjectType& type) const
//--------------------------------------------------------------------
{
    BaseObj* obj = nullptr;
    switch (type)
    {
    case BaseObjectType::TileObj:
        obj = mTileObj;
        break;
    case BaseObjectType::FieldObj:
        obj = mFieldObjects.front();
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
bool ObjContainer::removeObject(BaseObj* obj)
//--------------------------------------------------------------------
{
    return false;
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
        fieldObj->setPriority(mFieldObjects.size() + 1);
        mFieldObjects.push_back(obj);
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
bool ObjContainer::removeFieldObject(bool removeWithCleanup) // execute this method with false before start animation and with true after animation
//--------------------------------------------------------------------
{
    FieldObj* fieldObj = mFieldObjects.size() > 0 ? mFieldObjects.front() : nullptr;
    if (!fieldObj) {
        return false;
    }
    if (removeWithCleanup) {
        mFieldObjects.remove(fieldObj);

        if (fieldObj->getSpriteNode()) {
            fieldObj->getSpriteNode()->removeFromParent();
            fieldObj->setSpriteNode(nullptr);
        }

        if (obj->getHP() > 0) {
            //scene->createSpriteWithFieldObj(obj); use callback to scene
        }
        else if (fieldObj->isHpEnded()) {
            SmartFactory->recycle(fieldObj);
        }
    } else {
        // this flag is used to check after complete animation to remove obj 
        fieldObj->setReadyToChangeState(true); 
    }
    return true;
}
