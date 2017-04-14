/**
* @file GameObjects/Level/LevelObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/Level/LevelObj.h"

#include "GameObjects/TileObjects/Base/BaseObj.h"
#include "GameObjects/TileObjects/TileObj.h"
#include "Controller/ObjectController/Dude/DudeObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"

#include "Controller/ChainController/ChainObj.h"
#include "Controller/SwapController/SwapObj.h"

#include "Utils/Helpers/Helper.h"
#include "Utils/Helpers/ScoreHelper.h"
#include "Utils/Parser/JsonParser.h"

#include "Common/Factory/SmartObjFactory.h"
#include "Controller/ObjectController/ObjContainer.h"
#include "Controller/ObjectController/ObjectController.h"
#include "Controller/ObjectController/Dude/DudeController.h"
#include "Controller/ChainController/ChainController.h"


using namespace CT;

//--------------------------------------------------------------------
LevelObj::LevelObj()
    : mObjCtrl(nullptr)
    , mChainCtrl(nullptr)
    , mDudeCtrl(nullptr)
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
    CC_SAFE_RELEASE_NULL(mDudeCtrl);
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
    if (!JsonParser->checkLevelInfoStatus()) {
        cocos2d::log("LevelObj::initWithId: can't parse json file");
        return false;
    }
    mLevelInfo = JsonParser->getJsonLevelInfo();

    return true;
}

//--------------------------------------------------------------------
CT::Set* LevelObj::shuffle()
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::shuffle:");
    auto set = mObjCtrl->createInitialCookies();
    bool isSwapsDetected = mDetectPossibleSwapsCallback();

    while (!isSwapsDetected) {
        mObjCtrl->removeAllCookies();
        set = mObjCtrl->createInitialCookies();
        isSwapsDetected = mDetectPossibleSwapsCallback();
    }

    return set;
}

//--------------------------------------------------------------------
SearchEmptyHoles LevelObj::skipFillTopUpHoles(CT::Cell& cell, bool& filledTileFouned)
//--------------------------------------------------------------------
{
    SearchEmptyHoles res;
    if (!mObjCtrl->isEmptyTileAt(cell)) {
        filledTileFouned = true;
        res = SearchEmptyHoles::ObjFounded;
    }
    else {
        if ((cell.row <= (_GlobalInfo::NumRows / 2) + 2)  && !filledTileFouned) {
            res = SearchEmptyHoles::ContinueSearch;
        }
        else {
            res = SearchEmptyHoles::ContinueSearch;
        }
    }
    return res;
}

//--------------------------------------------------------------------
bool LevelObj::useGravityOnObject(cocos2d::Array* colArr, cocos2d::Array* rowArr, BaseObj* obj, int destinationRow)
//--------------------------------------------------------------------
{
    if (!colArr || !obj) {
        return false;
    }
    if (obj->getType() == BaseObjType::Dude || obj->getType() == BaseObjType::Field) {
        if (!obj->isMovable() && !obj->isContainer())
            return true;
    }

    if (obj->isMovable()) {
        auto lookupCell = obj->getCell();
        auto destCell = Cell(obj->getColumn(), destinationRow);
        // If find another cookie, move that cookie to the hole. This effectively moves the cookie down.
        mObjCtrl->removeObjectAt(lookupCell, obj->getType());
        mObjCtrl->updateObjectAt(destCell, obj);
        obj->setRow(destinationRow);

        // Lazy creation of array
        if (rowArr == nullptr) {
            rowArr = cocos2d::Array::createWithCapacity(_GlobalInfo::NumRows);
            colArr->addObject(rowArr);
        }
        rowArr->addObject(obj);

        // Once you found a cookie, you don�t need to scan up any farther so you break out of the inner loop.
        return true;
    }
    return false;
}

//--------------------------------------------------------------------
CT::Set* LevelObj::detectFieldObjects(CT::Set* chains)
//--------------------------------------------------------------------
{
    auto set = CT::Set::create();

    for (int row = 0; row < _GlobalInfo::NumRows; row++) {
        for (int column = 0; column < _GlobalInfo::NumColumns; column++) {

            auto cell = Cell(column, row);
            auto obj = mObjCtrl->fieldObjectAt(cell);
            if (!obj) {
                continue;
            }
            if (obj->isRemovable()) {

                if (mChainCtrl->checkMathicngFieldObjWithChain(chains, obj)) {
                    if (mObjCtrl->matchFieldObject(obj)) {
                        set->addObject(obj);
                        continue;
                    }
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
    auto columns = cocos2d::Array::createWithCapacity(_GlobalInfo::NumColumns);
    // loop through the rows, from bottom to top
    for (int column = 0; column < _GlobalInfo::NumColumns; column++) {

        cocos2d::Array* array = nullptr;
        for (int row = _GlobalInfo::NumRows - 1; row >= 0; row--) {

            auto cell = Cell(column, row);
            auto fieldObjects = mObjCtrl->fieldObjectsAt(cell);
            if (fieldObjects) {
                for (auto it = fieldObjects->begin(); it != fieldObjects->end(); ++it) {
                    auto obj = dynamic_cast<FieldObj*>(*it);
                    if (obj->getReadyToUpdatePriority()) {
                        // Lazy creation of array
                        if (array == nullptr) {
                            array = cocos2d::Array::createWithCapacity(_GlobalInfo::NumRows);
                            columns->addObject(array);
                        }
                        array->addObject(obj);
                        obj->setReadyToUpdatePriority(false);
                    }
                }
            }
            if (mObjCtrl->isPossibleToAddCookie(cell)) {

                // Scan upward to find the cookie that sits directly above the hole
                for (int lookup = row - 1; lookup >= 0; lookup--) {

                    auto lookupCell = Cell(column, lookup);
                    if (!mLevelInfo.skipEmptyHoles && mObjCtrl->isEmptyTileAt(lookupCell)) {
                        continue;
                    }                    
                    auto dudeObj = mObjCtrl->dudeAt(lookupCell);
                    if (useGravityOnObject(columns, array, dudeObj, row)) {
                        break;
                    }
                    auto fieldObj = mObjCtrl->fieldObjectAt(lookupCell);
                    if (useGravityOnObject(columns, array, fieldObj, row)) {
                        break;
                    }
                    auto cookie = mObjCtrl->cookieAt(lookupCell);
                    if (useGravityOnObject(columns, array, cookie, row)) {
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
    auto columns = cocos2d::Array::createWithCapacity(_GlobalInfo::NumColumns);
    //auto createdString = cocos2d::String::create("");
    int cookieType = -1;
    // loop through the rows, from top to bottom
    for (int column = 0; column < _GlobalInfo::NumColumns; column++) {

        bool filledTileFouned = false;

        cocos2d::Array* array = nullptr;
        for (int row = 0; row < _GlobalInfo::NumRows; row++) {
                        
            auto cell = Cell(column, row);
 //--------------------------------------------------------------------       
            if (!mLevelInfo.skipEmptyHoles) {
                auto skipRow = skipFillTopUpHoles(cell, filledTileFouned);
                if (skipRow == SearchEmptyHoles::ContinueSearch) {
                    continue;
                }
                else if (skipRow == SearchEmptyHoles::BreakSearch) {
                    break;
                }
            }                        
//--------------------------------------------------------------------
            auto fieldObj = mObjCtrl->fieldObjectAt(cell);
            if (fieldObj) {
                if (!fieldObj->isMovable() && !fieldObj->isContainer())
                    break;
            }
            if (mObjCtrl->isPossibleToAddCookie(cell)) {

                BaseObj* cookie = mObjCtrl->createRandomCookie(cell);

                if (array == nullptr) {
                    array = cocos2d::Array::createWithCapacity(_GlobalInfo::NumRows);
                    columns->addObject(array);
                }
                array->addObject(cookie);
                //createdString->appendWithFormat("\t%s\n", cookie->description());
            }
        }
        //createdString->append("}\n");
    }
    //cocos2d::log("LevelObj::fillTopUpHoles:  new created cookies: {\n%s", createdString->getCString());
    return columns;
}

//--------------------------------------------------------------------
void LevelObj::disablePredefinedCookies()
//--------------------------------------------------------------------
{
    if (mLevelInfo.isPredefinedCookies) {
        mLevelInfo.isPredefinedCookies = false;
    }
}

//--------------------------------------------------------------------
void LevelObj::removeDudeMatches(CT::Set* set)
//--------------------------------------------------------------------
{
    cocos2d::log("LevelObj::removeDudeMatches:");
    if (set) {
        ScoreHelper::calculateScore(set);
        mChainCtrl->matchChains(set);
    }
}
