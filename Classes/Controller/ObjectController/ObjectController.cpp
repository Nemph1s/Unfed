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
#include "Controller/ObjectController/Enemy/EnemyController.h"

#include "Common/Factory/SmartObjFactory.h"

#include "Utils/Parser/JsonParser.h"
#include "Utils/Helpers/Helper.h"

#include "GameObjects/TileObjects/TileObj.h"
#include "GameObjects/Level/LevelObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "Controller/ObjectController/Dude/DudeObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"

using namespace CT;
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
void ObjectController::createObjContainers()
//--------------------------------------------------------------------
{
    auto levelInfo = mLevel->getLevelInfo();
    for (int column = 0; column < _GlobalInfo::NumColumns; column++) {
        for (int row = 0; row < _GlobalInfo::NumRows; row++) {

            mObjects[column][row] = ObjContainer::create();

            auto tile = createTile(Cell(column, row), levelInfo.tiles[column][row]);
            mLevel->addChild(tile);
        }
    }
}

//--------------------------------------------------------------------
ObjContainer * ObjectController::getContainer(Cell& cell)
//--------------------------------------------------------------------
{
    if (Helper::isValidCell(cell)) {
        return mObjects[cell.column][cell.row];
    }
    cocos2d::log("ObjectController::getContainer: not valid column=%d or row=%d", cell.column, cell.row);
    return nullptr;
}

//--------------------------------------------------------------------
BaseObj * ObjectController::getObjectForChain(Cell& cell)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        return obj->getObjectForChain();
    }
    return nullptr;
}

//--------------------------------------------------------------------
CT::Set* ObjectController::getObjectsForChain(Cell& cell)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        return obj->getObjectsForChain();
    }
    return nullptr;
}

//--------------------------------------------------------------------
void ObjectController::synchronizeObjectAt(Cell& cell)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        obj->synchronizeTilePos();
    }
}

//--------------------------------------------------------------------
CT::Set* ObjectController::createInitialFieldObjects()
//--------------------------------------------------------------------
{
    auto levelInfo = mLevel->getLevelInfo();
    auto set = CT::Set::create();
    for (auto obj : levelInfo.fieldObjects) {
        for (uint8_t i = 0; i < obj.fieldType.size(); i++) {
            auto type = obj.fieldType.at(i);
            if (type > 0) {
                auto cell = Cell(obj.baseInfo.column, obj.baseInfo.row);
                auto fieldObj = createFieldObject(cell, type, i);
                set->addObject(fieldObj);
            }
        }
    }
    return set;
}

//--------------------------------------------------------------------
CT::Set * ObjectController::createInitialCookies()
//--------------------------------------------------------------------
{
    cocos2d::log("ObjectController::createInitialCookies:");
    auto set = CT::Set::create();
    auto createdString = cocos2d::String::create("");

    for (int row = 0; row < _GlobalInfo::NumRows; row++) {
        for (int column = 0; column < _GlobalInfo::NumColumns; column++) {
            auto cell = Cell(column, row);
            if (isPossibleToAddCookie(cell)) {
                int cookieType = getRandomCookieType(cell);
                auto cookie = createCookie(cell, cookieType);
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
BaseObj * ObjectController::createTile(Cell& cell, int type)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Tile, cell };
    TileInfo info = { baseInfo, static_cast<TileType>(type) };
    auto tile = SmartObjFactory->createTileObj(info);
    CC_ASSERT(tile);
    auto obj = getContainer(cell);
    if (obj) {
        obj->addObject(tile);
    }
    return tile;
}

//--------------------------------------------------------------------
BaseObj * ObjectController::createCookie(Cell& cell, int type)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Cookie, cell };
    CookieInfo info = { baseInfo, static_cast<CookieType>(type) };
    auto cookie = SmartObjFactory->createCookieObj(info);
    CC_ASSERT(cookie);
    auto obj = getContainer(cell);
    if (obj) {
        obj->addObject(cookie);
    }
    return cookie;
}

//--------------------------------------------------------------------
BaseObj * ObjectController::createFieldObject(Cell& cell, int type, int priority)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Field, cell };
    FieldInfo info = { baseInfo, static_cast<FieldType>(type), priority };
    auto fieldObj = SmartObjFactory->createFieldObj(info);
    CC_ASSERT(fieldObj);
    auto obj = getContainer(cell);
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
BaseObj * ObjectController::createRandomCookie(Cell& cell)
//--------------------------------------------------------------------
{
    int type = getRandomCookieType(cell);
    return createCookie(cell, type);
}

//--------------------------------------------------------------------
int ObjectController::getRandomCookieType(Cell& cell)
//--------------------------------------------------------------------
{
    auto levelInfo = mLevel->getLevelInfo();
    int type = 0;
    int randomCounter = 0;
    static const int randomCounterMax = 3; 
    bool findNextType = false;
    do {
        if (levelInfo.isPredefinedCookies) {
            type = (randomCounter > randomCounterMax) ? getRandomCookieType(cell) : levelInfo.cookies[cell.column][cell.row];
        } else {
            type = getAllowedRandomCookieType();
        }
        auto isCookiesToTheLeft = (cell.column >= 2 && // there are already two cookies of this type to the left
            isSameTypeOfCookieAt(cell.column - 1, cell.row, type) &&
            isSameTypeOfCookieAt(cell.column - 2, cell.row, type));
        auto isCookiesBelow = (cell.row >= 2 && // or there are already two cookies of this type below
            isSameTypeOfCookieAt(cell.column, cell.row - 1, type) &&
            isSameTypeOfCookieAt(cell.column, cell.row - 2, type));
        findNextType = (isCookiesToTheLeft || isCookiesBelow);
        randomCounter++;
    } while (findNextType);

    return type;
}

//--------------------------------------------------------------------
TileObj* ObjectController::tileAt(Cell& cell)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        return obj->getTile();
    }
    return nullptr;
}

//--------------------------------------------------------------------
CookieObj* ObjectController::cookieAt(Cell& cell)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        return obj->getCookie();
    }
    return nullptr;
}

//--------------------------------------------------------------------
bool ObjectController::hasChainAt(Cell& cell)
//--------------------------------------------------------------------
{
    if (!cookieAt(cell))
        return false;

    int type = cookieAt(cell)->getTypeAsInt();
    int fieldSize = _GlobalInfo::NumColumns;

    int horzLength = 1;

    for (int i = cell.column - 1; i >= 0 && isSameTypeOfCookieAt(i, cell.row, type); i--, horzLength++);
    for (int i = cell.column + 1; i < fieldSize && isSameTypeOfCookieAt(i, cell.row, type); i++, horzLength++);
    if (horzLength >= 3)
        return true;

    int vertLength = 1;

    for (int i = cell.row - 1; i >= 0 && isSameTypeOfCookieAt(cell.column, i, type); i--, vertLength++);
    for (int i = cell.row + 1; i < fieldSize && isSameTypeOfCookieAt(cell.column, i, type); i++, vertLength++);

    return (vertLength >= 3);
}

//--------------------------------------------------------------------
BaseObj* ObjectController::fieldObjectAt(Cell& cell)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        return obj->getFieldObject();
    }
    return nullptr;
}

//--------------------------------------------------------------------
std::list<FieldObj*>* ObjectController::fieldObjectsAt(Cell& cell)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        return &obj->getFieldObjects();
    }
    return nullptr;
}

//--------------------------------------------------------------------
DudeObj* ObjectController::dudeAt(Cell& cell)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        return obj->getDude();
    }
    return nullptr;
}

//--------------------------------------------------------------------
EnemyObj* ObjectController::enemyAt(CT::Cell & cell)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        return obj->getEnemy();
    }
    return nullptr;
}

//--------------------------------------------------------------------
bool ObjectController::isEmptyTileAt(Cell& cell)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        return obj->isEmptyTileAt();
    }
    return nullptr;
}

//--------------------------------------------------------------------
bool ObjectController::isPossibleToAddCookie(Cell& cell)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        return obj->isPossibleToAddCookie();
    }
    return nullptr;
}

//--------------------------------------------------------------------
bool ObjectController::isSameTypeOfCookieAt(int8_t column, int8_t row, int type)
//--------------------------------------------------------------------
{
    auto obj = getContainer(Cell(column, row));
    if (obj) {
        return obj->isSameTypeOfCookieAt(type);
    }
    return nullptr;
}

//--------------------------------------------------------------------
bool ObjectController::matchObject(BaseObj * obj)
//--------------------------------------------------------------------
{
    bool result = false;

    if (obj) {
        switch (obj->getType())
        {
        case BaseObjType::Cookie:
            result = matchCookieObject(obj);
            break;
        case BaseObjType::Field:
            result = matchFieldObject(obj);
            break;
        case BaseObjType::Dude:
            result = mDudeCtrl->matchDudeObject(obj);
            break;
        case BaseObjType::Enemy:
            result = mEnemyCtrl->matchEnemyObject(obj);
            break;
        default:
            break;
        }
    }
    return result;
}

//--------------------------------------------------------------------
bool ObjectController::matchFieldObject(BaseObj* obj)
//--------------------------------------------------------------------
{
    auto objContainer = getContainer(obj->getCell());
    if (!objContainer) {
        return false;
    }

    obj->match();

    auto fieldObj = objContainer->getFieldObject();
    if (obj != fieldObj) {
        return false;
    }
    std::function<void(BaseObj*, std::function<void(FieldObj*)>)> onFieldObjChangeStateCallback;
    onFieldObjChangeStateCallback = std::bind(&ObjContainer::onFieldObjChangeState, objContainer, _1, _2);
    fieldObj->setFieldObjChangeState(onFieldObjChangeStateCallback);
    return true;
}

//--------------------------------------------------------------------
bool ObjectController::matchCookieObject(BaseObj* obj)
//--------------------------------------------------------------------
{
    auto objContainer = getContainer(obj->getCell());
    if (!objContainer) {
        return false;
    }
    auto cookieObj = objContainer->getCookie();
    if (obj != cookieObj) {
        return false;
    }
    std::function<void(BaseObj*)> onRemoveCookieCallback;
    onRemoveCookieCallback = std::bind(&ObjContainer::onRemoveCookie, objContainer, _1);
    cookieObj->setRemoveObjectCallback(onRemoveCookieCallback);
    return true;
}

//--------------------------------------------------------------------
void ObjectController::updateCookieObjectAt(Cell& cell, BaseObj* cookie)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        obj->addObject(cookie);
    }
}

//--------------------------------------------------------------------
void ObjectController::updateObjectAt(Cell& cell, BaseObj* baseObj)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        obj->addObject(baseObj);
    }
}

//--------------------------------------------------------------------
void ObjectController::removeObjectAt(Cell& cell, CT::BaseObjType type)
//--------------------------------------------------------------------
{
    auto obj = getContainer(cell);
    if (obj) {
        obj->removeObject(type);
    }
}

//--------------------------------------------------------------------
void ObjectController::removeCookie(Cell& cell)
//--------------------------------------------------------------------
{
    auto cookie = cookieAt(cell);

    if (!cookie) {
        cocos2d::log("ObjectController::removeCookie: cookie at (%d,%d) already removed", cell.column, cell.row);
        return;
    }
    cocos2d::log("ObjectController::removeCookie: remove %s", cookie->description().getCString());
    if (cookie->getParent()) {
        cookie->removeFromParent();
    }
    SmartObjFactory->recycle(cookie);
    auto obj = getContainer(cell);
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
            auto cell = Cell(column, row);
            auto cookie = cookieAt(cell);
            if (cookie) {
                cookie->clear();
                if (cookie->getSpriteNode()) {
                    if (cookie->getSpriteNode()->getParent()) {
                        cookie->getSpriteNode()->removeFromParent();
                    }
                    cookie->setSpriteNode(nullptr);
                }
            }
            removeCookie(cell);
        }
    }
}