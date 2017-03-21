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

#include "Common/Factory/SmartObjFactory.h"
#include "Common/Factory/SpritesFactory.h"

// #include "Controller/ObjectController/Dude/DudeController.h"
// 
// #include "Common/Factory/SmartObjFactory.h"
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
    , mObjectInChain(false)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
ObjContainer::~ObjContainer()
//--------------------------------------------------------------------
{
    if (mTileObj) {
        SmartObjFactory->recycle(mTileObj);
        mTileObj = nullptr;
    }
    
    if (mCookieObj) {
        SmartObjFactory->recycle(mCookieObj);
        mCookieObj = nullptr;
    }
    
    if (mDudeObj) {
        SmartObjFactory->recycle(mDudeObj);
        mDudeObj = nullptr;
    }
    
    while (mFieldObjects.size() > 0) {
        if (mFieldObjects.front()) {
            SmartObjFactory->recycle(mFieldObjects.front());
        }        
        mFieldObjects.pop_front();
    }
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
void ObjContainer::updateObjectWith(BaseObj* currObj, BaseObj* newObj)
//--------------------------------------------------------------------
{
    CC_ASSERT(currObj);
    CC_ASSERT(newObj);
    //You can swap only cookie or dude
    if (currObj->getType() == BaseObjType::Cookie) {
        mCookieObj = nullptr;
    }
    else if (currObj->getType() == BaseObjType::Dude) {
        mDudeObj = nullptr;
    }

    if (newObj->getType() == BaseObjType::Cookie) {
        mCookieObj = dynamic_cast<CookieObj*>(newObj);
    }
    else if (newObj->getType() == BaseObjType::Dude) {
        mDudeObj = dynamic_cast<DudeObj*>(newObj);
    }
}

//--------------------------------------------------------------------
void ObjContainer::synchronizeTilePos()
//--------------------------------------------------------------------
{
    if (mTileObj) {
        if (mDudeObj) {
            mDudeObj->setColumn(mTileObj->getColumn());
            mDudeObj->setRow(mTileObj->getRow());
            mDudeObj->updateDebugLabel();
        }
        if (mCookieObj) {
            mCookieObj->setColumn(mTileObj->getColumn());
            mCookieObj->setRow(mTileObj->getRow());
            mCookieObj->updateDebugLabel();
        }
    }
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
        if (mCookieObj->getParent()) {
            mCookieObj->removeFromParent();
        }
        mObjectInChain = nullptr;
        SpritesFactory->recycle(mCookieObj->getSpriteNode(), mCookieObj);
        if (mCookieObj->getSpriteNode()) {
            mCookieObj->setSpriteNode(nullptr);
        }
        SmartObjFactory->recycle(mCookieObj);        
        removeObject(BaseObjType::Cookie);
    }
}

//--------------------------------------------------------------------
void ObjContainer::onRemoveDude(BaseObj * obj)
//--------------------------------------------------------------------
{
    if (mDudeObj == obj) {
        if (mDudeObj->getParent()) {
            mDudeObj->removeFromParent();
        }

        auto eraseDirectionsFunc = mDudeObj->getEraseDirectionsCallback();
        if (eraseDirectionsFunc) {
            eraseDirectionsFunc(obj);
        }
        mObjectInChain = nullptr;
        SpritesFactory->recycle(mDudeObj->getSpriteNode(), mDudeObj);
        if (mDudeObj->getSpriteNode()) {
            mDudeObj->setSpriteNode(nullptr);
        }
        SmartObjFactory->recycle(mDudeObj);        
        removeObject(BaseObjType::Dude);
    }
}

//--------------------------------------------------------------------
void ObjContainer::onFieldObjChangeState(BaseObj* obj, std::function<void(FieldObj*)> createSpriteFunc)
//--------------------------------------------------------------------
{
    auto fieldObj = getFieldObject();
    if (fieldObj == obj) {
        mObjectInChain = nullptr;
        if (fieldObj->getHP() > 0) {
            createSpriteFunc(fieldObj);
        }
        else if (fieldObj->isHpEnded()) {
            SpritesFactory->recycle(fieldObj->getSpriteNode(), fieldObj);
            if (fieldObj->getSpriteNode()) {
                fieldObj->setSpriteNode(nullptr);
            }
            SmartObjFactory->recycle(fieldObj);
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