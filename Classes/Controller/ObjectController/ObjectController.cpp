/**
* @file Controller/ObjectController/ObjectController.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ObjectController/ObjectController.h"
#include "Controller/ObjectController/ObjContainer.h"
#include "Controller/ObjectController/Dude/DudeController.h"

#include "Common/Factory/SmartObjFactory.h"

#include "Utils/Parser/JsonParser.h"
#include "Utils/Helpers/Helper.h"

#include "GameObjects/TileObjects/TileObj.h"
#include "GameObjects/Level/LevelObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "Controller/ObjectController/Dude/DudeObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"

using namespace CommonTypes;
using namespace std::placeholders;

//--------------------------------------------------------------------
ObjectController::ObjectController()
    : mLevel(nullptr)
    , mDudeCtrl(nullptr)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
ObjectController::~ObjectController()
//--------------------------------------------------------------------
{
    for (int column = 0; column < _GlobalInfo::NumColumns; column++) {
        for (int row = 0; row < _GlobalInfo::NumRows; row++) {
            CC_SAFE_RELEASE_NULL(mObjects[column][row]);
        }
    }
    mLevel = nullptr;
    mDudeCtrl = nullptr;
}

//--------------------------------------------------------------------
ObjectController * ObjectController::create()
//--------------------------------------------------------------------
{
    ObjectController * ret = new (std::nothrow) ObjectController();
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
bool ObjectController::init()
//--------------------------------------------------------------------
{
    return true;
}

//--------------------------------------------------------------------
void ObjectController::createObjects()
//--------------------------------------------------------------------
{
    auto levelInfo = mLevel->getLevelInfo();
    for (int column = 0; column < _GlobalInfo::NumColumns; column++) {
        for (int row = 0; row < _GlobalInfo::NumRows; row++) {

            mObjects[column][row] = ObjContainer::create();

            auto tile = createTile(column, row, levelInfo.tiles[column][row]);
            mLevel->addChild(tile);
        }
    }
}

//--------------------------------------------------------------------
ObjContainer* ObjectController::getContainer(int column, int row)
//--------------------------------------------------------------------
{
    if (Helper::isValidColumnAndRow(column, row)) {
        return mObjects[column][row];
    }
    cocos2d::log("ObjectController::getObject: not valid column=%d or row=%d", column, row);
    return nullptr;    
}

//--------------------------------------------------------------------
CommonTypes::Set* ObjectController::createInitialFieldObjects()
//--------------------------------------------------------------------
{
    auto levelInfo = mLevel->getLevelInfo();
    auto set = CommonTypes::Set::create();
    for (auto obj : levelInfo.fieldObjects) {
        for (uint8_t i = 0; i < obj.fieldType.size(); i++) {
            auto type = obj.fieldType.at(i);
            if (type > 0) {
                auto fieldObj = createFieldObject(obj.baseInfo.column, obj.baseInfo.row, type, i);
                set->addObject(fieldObj);
            }
        }
    }
    return set;
}

//--------------------------------------------------------------------
CommonTypes::Set * ObjectController::createInitialCookies()
//--------------------------------------------------------------------
{
    cocos2d::log("ObjectController::createInitialCookies:");
    auto set = CommonTypes::Set::create();
    auto createdString = cocos2d::String::create("");

    for (int row = 0; row < _GlobalInfo::NumRows; row++) {
        for (int column = 0; column < _GlobalInfo::NumColumns; column++) {
            if (isPossibleToAddCookie(column, row)) {
                int cookieType = getRandomCookieType(column, row);
                auto cookie = createCookie(column, row, cookieType);
                set->addObject(cookie);
                createdString->appendWithFormat("%d ", cookieType);
            }
        }
        createdString->append("\n");
    }
    mLevel->disablePredefinedCookies();
    cocos2d::log("ObjectController::createInitialCookies: created array=%s", createdString->getCString());
    return set;
}
//--------------------------------------------------------------------
void ObjectController::detectDirectionsForDudes()
//--------------------------------------------------------------------
{
    mDudeCtrl->detectDirectionsForDudes();
}

//--------------------------------------------------------------------
BaseObj * ObjectController::createTile(int column, int row, int type)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Tile, column, row };
    TileInfo info = { baseInfo, static_cast<TileType>(type) };
    auto tile = SmartObjFactory->createTileObj(info);
    CC_ASSERT(tile);
    auto obj = getContainer(column, row);
    if (obj) {
        obj->addObject(tile);
    }
    return tile;
}

//--------------------------------------------------------------------
BaseObj * ObjectController::createCookie(int column, int row, int type)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Cookie, column, row };
    CookieInfo info = { baseInfo, static_cast<CookieType>(type) };
    auto cookie = SmartObjFactory->createCookieObj(info);
    CC_ASSERT(cookie);
    auto obj = getContainer(column, row);
    if (obj) {
        obj->addObject(cookie);
    }
    return cookie;
}

//--------------------------------------------------------------------
BaseObj * ObjectController::createFieldObject(int column, int row, int type, int priority)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Field, column, row };
    FieldInfo info = { baseInfo, static_cast<FieldType>(type), priority };
    auto fieldObj = SmartObjFactory->createFieldObj(info);
    CC_ASSERT(fieldObj);
    auto obj = getContainer(column, row);
    if (obj) {
        obj->addObject(fieldObj);
    }
    return fieldObj;
}

//--------------------------------------------------------------------
int ObjectController::getAllowedRandomCookieType()
//--------------------------------------------------------------------
{
    int cookieMax = Helper::getInstance()->to_underlying(CookieType::CookieMax);
    if (mLevel->getLevelInfo().typesCount < cookieMax) {
        cookieMax = mLevel->getLevelInfo().typesCount;
    }
    int type = 0;
    bool findNextType = true;
    auto allowedTypes = mLevel->getLevelInfo().allowedCookieTypes;

    do {
        type = Helper::getInstance()->random(0, cookieMax - 1);

        if (allowedTypes.size() == 0) {
            break;
        }
        for (auto val : mLevel->getLevelInfo().allowedCookieTypes) {
            if (val == type) {
                findNextType = false;
                break;
            }
        }
    } while (findNextType);

    return type;
}

//--------------------------------------------------------------------
BaseObj * ObjectController::createRandomCookie(int column, int row)
//--------------------------------------------------------------------
{
    int type = getRandomCookieType(column, row);
    return createCookie(column, row, type);
}

//--------------------------------------------------------------------
int ObjectController::getRandomCookieType(int column, int row)
//--------------------------------------------------------------------
{
    auto levelInfo = mLevel->getLevelInfo();
    int type = 0;
    int randomCounter = 0;
    static const int randomCounterMax = 3; 
    bool findNextType = false;
    do {
        if (levelInfo.isPredefinedCookies) {
            type = (randomCounter > randomCounterMax) ? getRandomCookieType(column, row) : levelInfo.cookies[column][row];
        } else {
            type = getAllowedRandomCookieType();
        }
        auto isCookiesToTheLeft = (column >= 2 && // there are already two cookies of this type to the left
            isSameTypeOfCookieAt(column - 1, row, type) &&
            isSameTypeOfCookieAt(column - 2, row, type));
        auto isCookiesBelow = (row >= 2 && // or there are already two cookies of this type below
            isSameTypeOfCookieAt(column, row - 1, type) &&
            isSameTypeOfCookieAt(column, row - 2, type));
        findNextType = (isCookiesToTheLeft || isCookiesBelow);
        randomCounter++;
    } while (findNextType);

    return type;
}


//--------------------------------------------------------------------
TileObj* ObjectController::tileAt(int column, int row)
//--------------------------------------------------------------------
{
    auto obj = getContainer(column, row);
    if (obj) {
        return obj->getTile();
    }
    return nullptr;
}

//--------------------------------------------------------------------
CookieObj* ObjectController::cookieAt(int column, int row)
//--------------------------------------------------------------------
{
    auto obj = getContainer(column, row);
    if (obj) {
        return obj->getCookie();
    }
    return nullptr;
}

//--------------------------------------------------------------------
bool ObjectController::hasChainAt(int column, int row)
//--------------------------------------------------------------------
{
    if (!cookieAt(column, row))
        return false;

    int type = cookieAt(column, row)->getTypeAsInt();
    int fieldSize = _GlobalInfo::NumColumns;

    int horzLength = 1;

    for (int i = column - 1; i >= 0 && isSameTypeOfCookieAt(i, row, type); i--, horzLength++);
    for (int i = column + 1; i < fieldSize && isSameTypeOfCookieAt(i, row, type); i++, horzLength++);
    if (horzLength >= 3)
        return true;

    int vertLength = 1;

    for (int i = row - 1; i >= 0 && isSameTypeOfCookieAt(column, i, type); i--, vertLength++);
    for (int i = row + 1; i < fieldSize && isSameTypeOfCookieAt(column, i, type); i++, vertLength++);

    return (vertLength >= 3);
}

//--------------------------------------------------------------------
BaseObj* ObjectController::fieldObjectAt(int column, int row)
//--------------------------------------------------------------------
{
    auto obj = getContainer(column, row);
    if (obj) {
        return obj->getFieldObject();
    }
    return nullptr;
}

//--------------------------------------------------------------------
std::list<FieldObj*>* ObjectController::fieldObjectsAt(int column, int row)
//--------------------------------------------------------------------
{
    auto obj = getContainer(column, row);
    if (obj) {
        return &obj->getFieldObjects();
    }
    return nullptr;
}

//--------------------------------------------------------------------
DudeObj* ObjectController::dudeAt(int column, int row)
//--------------------------------------------------------------------
{
    auto obj = getContainer(column, row);
    if (obj) {
        return obj->getDude();
    }
    return nullptr;
}

//--------------------------------------------------------------------
bool ObjectController::isEmptyTileAt(int column, int row)
//--------------------------------------------------------------------
{
    auto obj = getContainer(column, row);
    if (obj) {
        return obj->isEmptyTileAt();
    }
    return nullptr;
}

//--------------------------------------------------------------------
bool ObjectController::isPossibleToAddCookie(int column, int row)
//--------------------------------------------------------------------
{
    auto obj = getContainer(column, row);
    if (obj) {
        return obj->isPossibleToAddCookie();
    }
    return nullptr;
}

//--------------------------------------------------------------------
bool ObjectController::isSameTypeOfCookieAt(int column, int row, int type)
//--------------------------------------------------------------------
{
    auto obj = getContainer(column, row);
    if (obj) {
        return obj->isSameTypeOfCookieAt(type);
    }
    return nullptr;
}

//--------------------------------------------------------------------
bool ObjectController::matchFieldObject(BaseObj * obj)
//--------------------------------------------------------------------
{
    obj->match();

    auto objContainer = getContainer(obj->getColumn(), obj->getRow());
    if (!objContainer) {
        return false;
    }
    auto fieldObj = objContainer->getFieldObject();
    if (obj == fieldObj) {
        std::function<void(BaseObj*, std::function<void(FieldObj*)>)> onFieldObjChangeStateCallback;
        onFieldObjChangeStateCallback = std::bind(&ObjContainer::onFieldObjChangeState, objContainer, _1, _2);
        fieldObj->setFieldObjChangeState(onFieldObjChangeStateCallback);
    }   

    return true;
}

//--------------------------------------------------------------------
bool ObjectController::matchCookieObject(BaseObj * obj)
//--------------------------------------------------------------------
{
    auto objContainer = getContainer(obj->getColumn(), obj->getRow());
    if (!objContainer) {
        return false;
    }
    auto cookieObj = objContainer->getCookie();
    if (obj == cookieObj) {
        std::function<void(BaseObj*)> onRemoveCookieCallback;
        onRemoveCookieCallback = std::bind(&ObjContainer::onRemoveCookie, objContainer, _1);
        cookieObj->setRemoveCookieCallback(onRemoveCookieCallback);
    }

    return true;
}

//--------------------------------------------------------------------
bool ObjectController::matchDudeObject(BaseObj * obj)
//--------------------------------------------------------------------
{
    auto objContainer = getContainer(obj->getColumn(), obj->getRow());
    if (!objContainer) {
        return false;
    }
    auto dudeObj = objContainer->getDude();
    if (obj == dudeObj) {
        std::function<void(BaseObj*)> func = [&](BaseObj* obj) {
            auto dudeObj = dynamic_cast<DudeObj*>(obj);
            if (dudeObj) {
                mDudeCtrl->eraseDirectionsForDude(dudeObj);
            }
        };
        dudeObj->setEraseDirectionsCallback(func);

        std::function<void(BaseObj*)> onRemoveDudeCallback;
        onRemoveDudeCallback = std::bind(&ObjContainer::onRemoveDude, objContainer, _1);
        dudeObj->setRemoveDudeCallback(onRemoveDudeCallback);
    }

    return true;
}

//--------------------------------------------------------------------
void ObjectController::updateCookieObjectAt(int column, int row, BaseObj* cookie)
//--------------------------------------------------------------------
{
    auto obj = getContainer(column, row);
    if (obj) {
        obj->addObject(cookie);
    }
}

//--------------------------------------------------------------------
void ObjectController::updateObjectAt(int column, int row, BaseObj* baseObj)
//--------------------------------------------------------------------
{
    auto obj = getContainer(column, row);
    if (obj) {
        obj->addObject(baseObj);
    }
}

//--------------------------------------------------------------------
void ObjectController::removeObjectAt(int column, int row, CommonTypes::BaseObjType type)
//--------------------------------------------------------------------
{
    auto obj = getContainer(column, row);
    if (obj) {
        obj->removeObject(type);
    }
}

//--------------------------------------------------------------------
void ObjectController::removeCookie(int column, int row)
//--------------------------------------------------------------------
{
    auto cookie = cookieAt(column, row);

    if (!cookie) {
        cocos2d::log("ObjectController::removeCookie: cookie at (%d,%d) already removed", column, row);
        return;
    }
    cocos2d::log("ObjectController::removeCookie: remove %s", cookie->description().getCString());
    if (cookie->getParent()) {
        cookie->removeFromParent();
    }
    SmartObjFactory->recycle(cookie);
    auto obj = getContainer(column, row);
    if (obj) {
        obj->removeObject(BaseObjType::Cookie);
    }
}

//--------------------------------------------------------------------
void ObjectController::removeAllCookies()
//--------------------------------------------------------------------
{
    for (int row = 0; row < _GlobalInfo::NumRows; row++) {
        for (int column = 0; column < _GlobalInfo::NumColumns; column++) {
            auto cookie = cookieAt(column, row);
            if (cookie) {
                cookie->clear();
                if (cookie->getSpriteNode()) {
                    if (cookie->getSpriteNode()->getParent()) {
                        cookie->getSpriteNode()->removeFromParent();
                    }
                    cookie->setSpriteNode(nullptr);
                }
            }
            removeCookie(column, row);
        }
    }
}