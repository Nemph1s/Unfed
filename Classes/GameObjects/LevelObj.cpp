/**
* @file GameObjects/LevelObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/LevelObj.h"
#include "GameObjects/TileObjects/TileObj.h"
#include "GameObjects/TileObjects/Base/BaseObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/Chain/ChainObj.h"
#include "GameObjects/Swap/SwapObj.h"

#include "Utils/Helpers/Helper.h"
#include "Utils/Parser/JsonParser.h"

#include "Common/Factory/SmartFactory.h"
#include "Controller/ObjectController.h"
#include "Controller/ChainController.h"

using namespace CommonTypes;

//--------------------------------------------------------------------
LevelObj::LevelObj()
    : mObjCtrl(nullptr)
    , mChainCtrl(nullptr)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
LevelObj::~LevelObj()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::~LevelObj: deallocing CookieObj: %p - tag: %i", this, _tag);
    CC_SAFE_RELEASE_NULL(mObjCtrl);
    CC_SAFE_RELEASE_NULL(mChainCtrl);
}

//--------------------------------------------------------------------
LevelObj* LevelObj::createWithId(const int16_t& levelId)
//--------------------------------------------------------------------
{
    LevelObj* ret = new (std::nothrow) LevelObj();
    if (ret && ret->initWithId(levelId)) {
        CC_SAFE_RETAIN(ret);
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool LevelObj::initWithId(const int16_t& levelId)
//--------------------------------------------------------------------
{
    if (!Node::init()) {
        cocos2d::log("LevelObj::initWithId: can't init Node inctance");
        return false;
    }

    JsonParser->parseLevelInfo(levelId);
    if (!JsonParser->checkStatus()) {
        cocos2d::log("LevelObj::initWithId: can't parse json file");
        return false;
    }
    mLevelInfo = JsonParser->getLevelInfo();

    initObjectController();
    initChainController();

    return true;
}

//--------------------------------------------------------------------
void LevelObj::initObjectController()
//--------------------------------------------------------------------
{
    mObjCtrl = ObjectController::create();
    mObjCtrl->setLevel(this);

    mObjCtrl->createInitialTiles();
    mObjCtrl->createInitialFieldObjects();
}

//--------------------------------------------------------------------
void LevelObj::initChainController()
//--------------------------------------------------------------------
{
    mChainCtrl = ChainController::create();
    mChainCtrl->setLevel(this);
    mChainCtrl->setObjectController(mObjCtrl);
}

//--------------------------------------------------------------------
cocos2d::Set* LevelObj::shuffle()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::shuffle:");
    auto set = mObjCtrl->createInitialCookies();
    bool isSwapsDetected = mDetectPossibleSwapsCallback();

    while (!isSwapsDetected) {
        set = mObjCtrl->createInitialCookies();
        isSwapsDetected = mDetectPossibleSwapsCallback();
    }

    return set;
}

//--------------------------------------------------------------------
cocos2d::Set * LevelObj::removeMatches()
//--------------------------------------------------------------------
{
    auto set = mChainCtrl->removeMatches();
    return set;
}

//--------------------------------------------------------------------
cocos2d::Set* LevelObj::removeChainAt(CommonTypes::ChainType& type, cocos2d::Vec2& pos)
//--------------------------------------------------------------------
{
    auto set = mChainCtrl->removeChainAt(type, pos);
    return set;
}

//--------------------------------------------------------------------
void LevelObj::removeCookies(cocos2d::Set * chains)
//--------------------------------------------------------------------
{
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);

        auto cookies = chain->getCookies();
        for (auto it = cookies->begin(); it != cookies->end(); it++) {
            auto cookie = dynamic_cast<CookieObj*>(*it);
            CC_ASSERT(cookie);

            mObjCtrl->removeCookie(cookie->getColumn(), cookie->getRow());
        }
    }
}

//--------------------------------------------------------------------
bool LevelObj::checkMathicngFieldObjWithChain(cocos2d::Set * chains, BaseObj * obj)
//--------------------------------------------------------------------
{
    auto result = false;
    auto fieldObj = dynamic_cast<TileObj*>(obj);
    if (!fieldObj) {
        return result;
    }
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);

        auto cookies = chain->getCookies();
        for (auto it = cookies->begin(); it != cookies->end(); it++) {
            auto cookie = dynamic_cast<CookieObj*>(*it);
            CC_ASSERT(cookie);

            int col = cookie->getColumn();
            int row = cookie->getRow();
            if (fieldObj->checkMatchingCondition(col, row)) {
                result = true;
                break;
            }
        }
        if (result)
            break;
    }
    return result;
}

//--------------------------------------------------------------------
bool LevelObj::isPossibleToAddCookie(int column, int row)
//--------------------------------------------------------------------
{
    return mObjCtrl->isPossibleToAddCookie(column, row);
}

//--------------------------------------------------------------------
cocos2d::Set* LevelObj::detectFieldObjects(cocos2d::Set * chains)
//--------------------------------------------------------------------
{
    auto set = new cocos2d::Set();

    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns; column++) {

            auto obj = mObjCtrl->fieldObjectAt(column, row);
            if (!obj) {
                continue;
            }
            if (checkMathicngFieldObjWithChain(chains, obj)) {
                if (mObjCtrl->matchFieldObject(obj)) {
                    set->addObject(obj);
                    continue;
                }
            }
        }
    }
    return set;
}

//--------------------------------------------------------------------
cocos2d::Array* LevelObj::useGravityToFillHoles()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::useGravityToFillHoles:");
    auto columns = cocos2d::Array::createWithCapacity(NumColumns);
    // loop through the rows, from bottom to top
    for (int column = 0; column < NumColumns; column++) {

        cocos2d::Array* array = nullptr;
        for (int row = NumRows - 1; row >= 0; row--) {

            if (isPossibleToAddCookie(column, row)) {

                // Scan upward to find the cookie that sits directly above the hole
                for (int lookup = row - 1; lookup >= 0; lookup--) {

                    if (mObjCtrl->isEmptyTileAt(column, lookup)) {
                        break;
                    }
                    auto fieldObj = mObjCtrl->fieldObjectAt(column, lookup);
                    if (fieldObj) {
                        if (!fieldObj->getIsMovable())
                            break;

                        // If find another cookie, move that cookie to the hole. This effectively moves the cookie down.
                        mObjCtrl->updateObjectAt(column, lookup, nullptr, fieldObj->getType());
                        mObjCtrl->updateObjectAt(column, row, fieldObj, fieldObj->getType());
                        fieldObj->setRow(row);

                        // Lazy creation of array
                        if (array == nullptr) {
                            array = cocos2d::Array::createWithCapacity(NumRows);
                            columns->addObject(array);
                        }
                        array->addObject(fieldObj);

                        // Once you’ve found a cookie, you don’t need to scan up any farther so you break out of the inner loop.
                        break;
                    }
                    auto cookie = mObjCtrl->cookieAt(column, lookup);
                    if (cookie) {
                        if (!cookie->getIsMovable())
                            continue;

                        // If find another cookie, move that cookie to the hole. This effectively moves the cookie down.
                        mObjCtrl->updateObjectAt(column, lookup, nullptr, cookie->getType());
                        mObjCtrl->updateObjectAt(column, row, cookie, cookie->getType());
                        cookie->setRow(row);

                        // Lazy creation of array
                        if (array == nullptr) {
                            array = cocos2d::Array::createWithCapacity(NumRows);
                            columns->addObject(array);
                        }
                        array->addObject(cookie);

                        // Once you’ve found a cookie, you don’t need to scan up any farther so you break out of the inner loop.
                        break;
                    }
                }
            }
        }
    }
    return columns;
}

//--------------------------------------------------------------------
cocos2d::Array * LevelObj::fillTopUpHoles()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::fillTopUpHoles:");
    auto columns = cocos2d::Array::createWithCapacity(NumColumns);
    auto createdString = cocos2d::String("");
    int cookieType = -1;
    // loop through the rows, from top to bottom
    for (int column = 0; column < NumColumns; column++) {
        for (int row = 0; row < NumRows; row++) {

            cocos2d::Array* array = nullptr;
            if (mObjCtrl->isEmptyTileAt(column, row)) {
                break;
            }
            auto fieldObj = mObjCtrl->fieldObjectAt(column, row);
            if (fieldObj) {
                if (!fieldObj->getIsMovable() && !fieldObj->getIsContainer())
                    break;
            }
            if (isPossibleToAddCookie(column, row)) {

                BaseObj* cookie = mObjCtrl->createRandomCookie(column, row);

                if (array == nullptr) {
                    array = cocos2d::Array::createWithCapacity(NumRows);
                    columns->addObject(array);
                }
                array->addObject(cookie);
                createdString.appendWithFormat("\t%s\n", cookie->description());
            }
        }
        createdString.append("}\n");
    }
    cocos2d::log("LevelObj::fillTopUpHoles:  new created cookies: {\n%s", createdString.getCString());
    return columns;
}

//--------------------------------------------------------------------
void LevelObj::calculateScore(cocos2d::Set * chains)
//--------------------------------------------------------------------
{
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);
        auto cookies = chain->getCookies();

        //TODO: move to other location
        int chainValue = 60;
        switch (chain->getType())
        {
        case ChainType::ChainTypeL:
            chainValue = 70;
            break;
        case ChainType::ChainTypeT:
            chainValue = 80;
        default:
            break;
        }

        //TODO: make more intelligent way to get score value
        chain->setScore(chainValue * (cookies->count() - 2) * mComboMultiplier);
        mComboMultiplier++;
    }
}

//--------------------------------------------------------------------
void LevelObj::resetComboMultiplier()
//--------------------------------------------------------------------
{
    mComboMultiplier = 1;
}
