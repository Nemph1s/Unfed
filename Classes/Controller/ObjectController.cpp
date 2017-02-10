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

#include "Utils/Parser/JsonParser.h"

#include "GameObjects/LevelObj.h"

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
    mLevel->mLevelInfo = JsonParser->getLevelInfo();
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
    mLevel->mTiles[column][row] = tile;
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
            if (!mLevel->isEmptyTileAt(column, row)) {
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
    mLevel->mCookies[column][row] = cookie;
    return cookie;
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
            mLevel->isSameTypeOfCookieAt(column - 1, row, type) &&
            mLevel->isSameTypeOfCookieAt(column - 2, row, type));
        auto isCookiesBelow = (row >= 2 && // or there are already two cookies of this type below
            mLevel->isSameTypeOfCookieAt(column, row - 1, type) &&
            mLevel->isSameTypeOfCookieAt(column, row - 2, type));
        findNextType = (isCookiesToTheLeft || isCookiesBelow);
    } while (findNextType);

    return type;
}

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
