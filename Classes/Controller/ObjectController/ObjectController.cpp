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

#include "Common/Factory/SmartFactory.h"

#include "Utils/Parser/JsonParser.h"
#include "Utils/Helpers/Helper.h"

#include "GameObjects/TileObjects/TileObj.h"
#include "GameObjects/Level/LevelObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/TileObjects/DudeObj.h"
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
    for (int column = 0; column < NumColumns; column++) {
        for (int row = 0; row < NumRows; row++) {
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
    for (int column = 0; column < NumColumns; column++) {
        for (int row = 0; row < NumRows; row++) {

            mObjects[column][row] = ObjContainer::create();

            auto tile = createTile(column, row, levelInfo.tiles[column][row]);
            mLevel->addChild(tile);
        }
    }
}

//--------------------------------------------------------------------
ObjContainer* ObjectController::getObject(int column, int row)
//--------------------------------------------------------------------
{
    return mObjects[column][row];
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

    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns; column++) {
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
BaseObj * ObjectController::createTile(int column, int row, int type)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Tile, column, row };
    TileInfo info = { baseInfo, static_cast<TileType>(type) };
    auto tile = SmartFactory->createTileObj(info);
    CC_ASSERT(tile);
    //mTiles[column][row] = tile;
    getObject(column, row)->addObject(tile);
    return tile;
}

//--------------------------------------------------------------------
BaseObj * ObjectController::createCookie(int column, int row, int type)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Cookie, column, row };
    CookieInfo info = { baseInfo, static_cast<CookieType>(type) };
    auto cookie = SmartFactory->createCookieObj(info);
    CC_ASSERT(cookie);
    //mCookies[column][row] = cookie;
    getObject(column, row)->addObject(cookie);
    return cookie;
}

//--------------------------------------------------------------------
BaseObj * ObjectController::createFieldObject(int column, int row, int type, int priority)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Field, column, row };
    FieldInfo info = { baseInfo, static_cast<FieldType>(type), priority };
    auto obj = SmartFactory->createFieldObj(info);
    CC_ASSERT(obj);
    //mFieldObjects[column][row].push_back(obj);
    getObject(column, row)->addObject(obj);
    return obj;
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
    bool validColumn = column >= 0 && column < NumColumns;
    bool validRow = row >= 0 && row < NumColumns;
    if (!validColumn) {
        cocos2d::log("ObjectController::tileAt: Invalid column : %d", column);
        CC_ASSERT(validColumn);
    }
    if (!validRow) {
        cocos2d::log("ObjectController::tileAt: Invalid row: %d", row);
        CC_ASSERT(validRow);
    }
    return getObject(column, row)->getTileObj();
//    return dynamic_cast<TileObj*>(mTiles[column][row]);
}

//--------------------------------------------------------------------
CookieObj* ObjectController::cookieAt(int column, int row)
//--------------------------------------------------------------------
{
    bool validColumn = column >= 0 && column < NumColumns;
    bool validRow = row >= 0 && row < NumColumns;
    if (!validColumn || !validRow) {
        cocos2d::log("ObjectController::cookieAt: Invalid cookie at column = %d, row = %d", column, row);
        return nullptr;
    }
    return getObject(column, row)->getCookieObj();
//    return dynamic_cast<CookieObj*>(mCookies[column][row]);
}

//--------------------------------------------------------------------
bool ObjectController::hasChainAt(int column, int row)
//--------------------------------------------------------------------
{
    if (!cookieAt(column, row))
        return false;

    int type = cookieAt(column, row)->getTypeAsInt();
    int fieldSize = NumColumns;

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
BaseObj * ObjectController::fieldObjectAt(int column, int row)
//--------------------------------------------------------------------
{
    bool invalidColumn = column >= 0 && column < NumColumns;
    bool invalidRow = row >= 0 && row < NumColumns;
    if (!invalidColumn) {
        cocos2d::log("ObjectController::fieldObjectAt: Invalid column : %d", column);
        CC_ASSERT(invalidColumn);
    }
    if (!invalidRow) {
        cocos2d::log("ObjectController::fieldObjectAt: Invalid row: %d", row);
        CC_ASSERT(invalidRow);
    }
//     auto fieldsList = mFieldObjects[column][row];
//     if (fieldsList.size() == 0) return nullptr;
//     return fieldsList.front();
    return getObject(column, row)->getObject(BaseObjType::Field);
}

//--------------------------------------------------------------------
std::list<FieldObj*>& ObjectController::fieldObjectsAt(int column, int row)
//--------------------------------------------------------------------
{
    bool invalidColumn = column >= 0 && column < NumColumns;
    bool invalidRow = row >= 0 && row < NumColumns;
    if (!invalidColumn) {
        cocos2d::log("ObjectController::fieldObjectsAt: Invalid column : %d", column);
        CC_ASSERT(invalidColumn);
    }
    if (!invalidRow) {
        cocos2d::log("ObjectController::fieldObjectsAt: Invalid row: %d", row);
        CC_ASSERT(invalidRow);
    }
    return getObject(column, row)->getFieldObjects();
}

//--------------------------------------------------------------------
DudeObj* ObjectController::dudeObjectAt(int column, int row)
//--------------------------------------------------------------------
{
    return getObject(column, row)->getDudeObj();
}

//--------------------------------------------------------------------
bool ObjectController::isEmptyTileAt(int column, int row)
//--------------------------------------------------------------------
{
    return getObject(column, row)->isEmptyTileAt();
}

//--------------------------------------------------------------------
bool ObjectController::isPossibleToAddCookie(int column, int row)
//--------------------------------------------------------------------
{
    return getObject(column, row)->isPossibleToAddCookie();
}

//--------------------------------------------------------------------
bool ObjectController::isSameTypeOfCookieAt(int column, int row, int type)
//--------------------------------------------------------------------
{
    return getObject(column, row)->isSameTypeOfCookieAt(type);
}

//--------------------------------------------------------------------
bool ObjectController::matchFieldObject(BaseObj * obj)
//--------------------------------------------------------------------
{
    obj->match();

    auto objContainer = getObject(obj->getColumn(), obj->getRow());
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
    auto objContainer = getObject(obj->getColumn(), obj->getRow());
    auto cookieObj = objContainer->getCookieObj();
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
    auto objContainer = getObject(obj->getColumn(), obj->getRow());
    auto dudeObj = objContainer->getDudeObj();

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
    getObject(column, row)->addObject(cookie);
//    mCookies[column][row] = cookie;
}

//--------------------------------------------------------------------
void ObjectController::updateObjectAt(int column, int row, BaseObj * obj)
//--------------------------------------------------------------------
{
    getObject(column, row)->addObject(obj);
}

//--------------------------------------------------------------------
void ObjectController::removeObjectAt(int column, int row, CommonTypes::BaseObjType type)
//--------------------------------------------------------------------
{
    getObject(column, row)->removeObject(type);
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
    SmartFactory->recycle(cookie);
    getObject(column, row)->removeObject(BaseObjType::Cookie);
}

//--------------------------------------------------------------------
void ObjectController::removeAllCookies()
//--------------------------------------------------------------------
{
    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns; column++) {
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