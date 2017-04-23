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
#include "Controller/ObjectController/Dude/DudeObj.h"
#include "Controller/ObjectController/Enemy/EnemyObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"

#include "Common/Factory/SmartObjFactory.h"
#include "Common/Factory/SpritesFactory.h"

using namespace CT;
using namespace GOT;

//--------------------------------------------------------------------
ObjContainer::ObjContainer()
    : cocos2d::Node()
    , mTileObj(nullptr)
    , mCookieObj(nullptr)
    , mDudeObj(nullptr)
    , mEnemyObj(nullptr)
    , mFieldObjects()
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

    if (mEnemyObj) {
        SmartObjFactory->recycle(mEnemyObj);
        mEnemyObj = nullptr;
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
        case BaseObjType::Enemy:
            result = addEnemyObject(obj);
            break;
        default:
            break;
        }
    }
    return result;
}

//--------------------------------------------------------------------
BaseObj * ObjContainer::getObject(const GOT::BaseObjType& type) const
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
    case BaseObjType::Enemy:
        obj = mEnemyObj;
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
bool ObjContainer::isContainGameObj()
//--------------------------------------------------------------------
{
    if (mCookieObj || mDudeObj || mEnemyObj || getFieldObject()) {
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
    else if (mDudeObj) {
        obj = mDudeObj;
    }
    else if (mEnemyObj) {
        obj = mEnemyObj;
    }
    else if (fieldObj) {
        obj = fieldObj;
    }
    
    return obj;
}

//--------------------------------------------------------------------
CT::Set* ObjContainer::getObjectsForChain()
//--------------------------------------------------------------------
{
    auto set = CT::Set::create();
    auto fieldObj = getFieldObject();

    if (mCookieObj) set->addObject(mCookieObj);
    else if (mDudeObj) set->addObject(mDudeObj);
    else if (mEnemyObj) set->addObject(mEnemyObj);
    if (fieldObj) set->addObject(fieldObj);    

    if (set->count() == 0) {
        set = nullptr;
    }
    return set;
}

//--------------------------------------------------------------------
bool ObjContainer::removeObject(const GOT::BaseObjType& type)
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
    case BaseObjType::Enemy:
        CC_SAFE_RELEASE_NULL(mEnemyObj);
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
    else if (currObj->getType() == BaseObjType::Enemy) {
        mEnemyObj = nullptr;
    }

    if (newObj->getType() == BaseObjType::Cookie) {
        mCookieObj = dynamic_cast<CookieObj*>(newObj);
    }
    else if (newObj->getType() == BaseObjType::Dude) {
        mDudeObj = dynamic_cast<DudeObj*>(newObj);
    }
    else if (newObj->getType() == BaseObjType::Enemy) {
        mEnemyObj = dynamic_cast<EnemyObj*>(newObj);
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
            mDudeObj->updateZOrder();
            
        }
        if (mEnemyObj) {
            mEnemyObj->setColumn(mTileObj->getColumn());
            mEnemyObj->setRow(mTileObj->getRow());
            mEnemyObj->updateZOrder();

        }
        if (mCookieObj) {
            mCookieObj->setColumn(mTileObj->getColumn());
            mCookieObj->setRow(mTileObj->getRow());
            mCookieObj->updateZOrder();          
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
        if (!mDudeObj && !mEnemyObj) {
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
bool ObjContainer::isContainChainPreviewSprite() const
//--------------------------------------------------------------------
{
    return (mTileObj->getChainPreviewSpriteNode() != nullptr);
}

//--------------------------------------------------------------------
void ObjContainer::setChainPreviewSprite(cocos2d::Sprite* sprite)
//--------------------------------------------------------------------
{
    mTileObj->setChainPreviewSpriteNode(sprite);
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
bool ObjContainer::addEnemyObject(BaseObj * obj)
//--------------------------------------------------------------------
{
    auto enemyObj = dynamic_cast<EnemyObj*>(obj);
    if (enemyObj) {
        mEnemyObj = enemyObj;
        CC_SAFE_RETAIN(mEnemyObj);
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
void ObjContainer::onRemoveEnemy(BaseObj * obj)
//--------------------------------------------------------------------
{
    if (mEnemyObj == obj) {
        if (mEnemyObj->getParent()) {
            mEnemyObj->removeFromParent();
        }

        auto eraseDirectionsFunc = mEnemyObj->getEraseEnemyHelperCallback();
        if (eraseDirectionsFunc) {
            eraseDirectionsFunc(obj);
        }
        mObjectInChain = nullptr;
        SpritesFactory->recycle(mEnemyObj->getSpriteNode(), mEnemyObj);
        if (mEnemyObj->getSpriteNode()) {
            mEnemyObj->setSpriteNode(nullptr);
        }
        SmartObjFactory->recycle(mEnemyObj);
        removeObject(BaseObjType::Enemy);
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