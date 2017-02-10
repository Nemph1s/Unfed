/**
* @file Scenes/ObjectController.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ObjectController.h"

#include "Common/Factory/SmartFactory.h"

#include "Utils/Parser/JsonParser.h"
#include "Utils/Helpers/Helper.h"

#include "GameObjects/TileObj.h"
#include "GameObjects/LevelObj.h"
#include "GameObjects/CookieObj.h"

using namespace CommonTypes;

//--------------------------------------------------------------------
ObjectController::ObjectController()
    : mLevel(nullptr)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
ObjectController::~ObjectController()
//--------------------------------------------------------------------
{
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
void ObjectController::createInitialTiles()
//--------------------------------------------------------------------
{
    auto levelInfo = mLevel->getLevelInfo();

    for (int column = 0; column < NumColumns; column++) {
        for (int row = 0; row < NumRows; row++) {

            int tileType = levelInfo.tiles[column][row];
            auto tile = createTile(column, row, tileType);
            mLevel->addChild(tile);
        }
    }
}

//--------------------------------------------------------------------
BaseObj * ObjectController::createTile(int column, int row, int type)
//--------------------------------------------------------------------
{
    BaseObjectInfo baseInfo = { BaseObjectType::TileObj, column, row };
    TileInfo info = { baseInfo, static_cast<TileType>(type) };
    BaseObj* tile = SmartFactory->createTileObj(info);
    CC_ASSERT(tile);
    mTiles[column][row] = tile;
    return tile;
}

//--------------------------------------------------------------------
cocos2d::Set * ObjectController::createInitialCookies()
//--------------------------------------------------------------------
{
    cocos2d::log("ObjectController::createInitialCookies:");
    cocos2d::Set* set = new cocos2d::Set();
    auto createdString = cocos2d::String("");

    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns; column++) {
            if (!isEmptyTileAt(column, row)) {
                int cookieType = getRandomCookieType(column, row);
                BaseObj* cookie = createCookie(column, row, cookieType);
                set->addObject(cookie);
                createdString.appendWithFormat("%d ", cookieType);
            }
        }
        createdString.append("\n");
    }
    cocos2d::log("ObjectController::createInitialCookies: created array=%s", createdString.getCString());
    return set;
}

//--------------------------------------------------------------------
BaseObj * ObjectController::createCookie(int column, int row, int type)
//--------------------------------------------------------------------
{
    BaseObjectInfo baseInfo = { BaseObjectType::CookieObj, column, row };
    CookieInfo info = { baseInfo, static_cast<CookieType>(type) };
    BaseObj* cookie = SmartFactory->createCookieObj(info);
    CC_ASSERT(cookie);
    mCookies[column][row] = cookie;
    return cookie;
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
    int cookieMax = Helper::getInstance()->to_underlying(CommonTypes::CookieType::CookieMax);
    auto levelInfo = mLevel->getLevelInfo();
    if (levelInfo.typesCount < cookieMax) {
        cookieMax = levelInfo.typesCount;
    }
    int type = 0;
    bool findNextType = false;
    do {
        type = Helper::getInstance()->random(0, cookieMax - 1);
        auto isCookiesToTheLeft = (column >= 2 && // there are already two cookies of this type to the left
            isSameTypeOfCookieAt(column - 1, row, type) &&
            isSameTypeOfCookieAt(column - 2, row, type));
        auto isCookiesBelow = (row >= 2 && // or there are already two cookies of this type below
            isSameTypeOfCookieAt(column, row - 1, type) &&
            isSameTypeOfCookieAt(column, row - 2, type));
        findNextType = (isCookiesToTheLeft || isCookiesBelow);
    } while (findNextType);

    return type;
}


//--------------------------------------------------------------------
TileObj* ObjectController::tileAt(int column, int row)
//--------------------------------------------------------------------
{
    bool invalidColumn = column >= 0 && column < NumColumns;
    bool invalidRow = row >= 0 && row < NumColumns;
    if (!invalidColumn) {
        cocos2d::log("ObjectController::tileAt: Invalid column : %d", column);
        CC_ASSERT(invalidColumn);
    }
    if (!invalidRow) {
        cocos2d::log("ObjectController::tileAt: Invalid row: %d", row);
        CC_ASSERT(invalidRow);
    }
    return dynamic_cast<TileObj*>(mTiles[column][row]);
}

//--------------------------------------------------------------------
CookieObj* ObjectController::cookieAt(int column, int row)
//--------------------------------------------------------------------
{
    bool invalidColumn = column >= 0 && column < NumColumns;
    bool invalidRow = row >= 0 && row < NumColumns;
    if (!invalidColumn || !invalidRow) {
        cocos2d::log("ObjectController::cookieAt: Invalid cookie at column = %d, row = %d", column, row);
        return nullptr;
    }
    return dynamic_cast<CookieObj*>(mCookies[column][row]);
}

//--------------------------------------------------------------------
bool ObjectController::hasChainAt(int column, int row)
//--------------------------------------------------------------------
{
    if (!cookieAt(column, row))
        return false;

    int type = cookieAt(column, row)->getTypeAsInt();
    int fieldSize = CommonTypes::NumColumns;

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
bool ObjectController::isEmptyTileAt(int column, int row)
//--------------------------------------------------------------------
{
    return tileAt(column, row) ? tileAt(column, row)->isEmptyTile() : false;
}

//--------------------------------------------------------------------
bool ObjectController::isSameTypeOfCookieAt(int column, int row, int type)
//--------------------------------------------------------------------
{
    auto cookie = cookieAt(column, row);
    if (!cookie)
        return false;

    if (cookie->getTypeAsInt() != type)
        return false;

    return true;
}

//--------------------------------------------------------------------
void ObjectController::updateCookieObjectAt(int column, int row, BaseObj* cookie)
//--------------------------------------------------------------------
{
    mCookies[column][row] = cookie;
}

//--------------------------------------------------------------------
void ObjectController::removeCookie(int column, int row)
//--------------------------------------------------------------------
{
    auto cookie = cookieAt(column, row);
    CC_ASSERT(cookie);
    cocos2d::log("ObjectController::removeCookies: remove %s", cookie->description());

    cookie->removeFromParent();
    SmartFactory->recycle(cookie);
    mCookies[column][row] = nullptr;
}

