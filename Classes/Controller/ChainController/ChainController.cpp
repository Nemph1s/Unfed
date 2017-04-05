/**
* @file Controller/ChainController/ChainController.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ChainController/ChainController.h"
#include "Controller/ObjectController/ObjContainer.h"
#include "Controller/ObjectController/ObjectController.h"

#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"
#include "Controller/ChainController/ChainObj.h"
#include "GameObjects/Level/LevelObj.h"

#include "Utils/Helpers/Helper.h"
#include "Utils/Helpers/ScoreHelper.h"

#include "Common/CommonTypes.h"
#include "Common/GlobalInfo/GlobalInfo.h"

using namespace CommonTypes;

//--------------------------------------------------------------------
ChainController::ChainController()
    : mLevel(nullptr)
    , mObjCtrl(nullptr)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
ChainController::~ChainController()
//--------------------------------------------------------------------
{
    mLevel = nullptr;
    mObjCtrl = nullptr;
}

//--------------------------------------------------------------------
ChainController* ChainController::create()
//--------------------------------------------------------------------
{
    ChainController * ret = new (std::nothrow) ChainController();
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
bool ChainController::init()
//--------------------------------------------------------------------
{
    return true;
}

//--------------------------------------------------------------------
Set* ChainController::removeMatches()
//--------------------------------------------------------------------
{
    cocos2d::log("ChainController::removeMatches:");
    auto horizontalChains = detectHorizontalMatches();
    auto verticalChains = detectVerticalMatches();
    auto difficultChains = detectDifficultMatches(horizontalChains, verticalChains);

    auto set = Set::create();

    activateChains(horizontalChains);
    activateChains(verticalChains);
    activateChains(difficultChains);

    addChainsFromSetToSet(horizontalChains, set);
    addChainsFromSetToSet(verticalChains, set);
    addChainsFromSetToSet(difficultChains, set);

#ifdef COCOS2D_DEBUG
    //logDebugChains(horizontalChains, verticalChains, difficultChains);
#endif //COCOS2D_DEBUG

    ScoreHelper::calculateScore(set);
    matchChains(set);

    GlobInfo->increaseComboMultiplier();

    return set;
}

//--------------------------------------------------------------------
Set* ChainController::removeChainAt(ChainType& type, cocos2d::Vec2& pos)
//--------------------------------------------------------------------
{
    auto set = Set::create();

    int column = -1, row = -1;
    if (Helper::convertPointToTilePos(pos, column, row)) {
        Set* chainSet = nullptr;
        switch (type)
        {
        case ChainType::ChainTypeHorizontal:
            chainSet = createHorizontalChainAt(column, row);
            break;
        case ChainType::ChainTypeVertical:
            chainSet = createVerticalChainAt(column, row);
            break;
        case ChainType::ChainTypeX:
            chainSet = createXChainAt(column, row);
            break;
        case ChainType::ChainTypeAllOfOne:
            chainSet = createAllOfOneChain(column, row);
            break;
        default:
            break;
        }
        if (chainSet) {
            addChainsFromSetToSet(chainSet, set);

            ScoreHelper::calculateScore(chainSet);
            matchChains(chainSet);
        }
    }
    return set;
}

//--------------------------------------------------------------------
Set* ChainController::detectChainAt(int column, int row)
//--------------------------------------------------------------------
{
    Set* set = nullptr;
    if (!mObjCtrl->cookieAt(column, row))
        return set;

    set = Set::create();
    int type = mObjCtrl->cookieAt(column, row)->getTypeAsInt();
    int fieldSize = _GlobalInfo::NumColumns;

    int horzLength = 1;
    for (int i = column - 1; i >= 0 && mObjCtrl->isSameTypeOfCookieAt(i, row, type); i--, horzLength++) {
        set->addObject(mObjCtrl->cookieAt(i, row));
    };
    for (int i = column + 1; i < fieldSize && mObjCtrl->isSameTypeOfCookieAt(i, row, type); i++, horzLength++) {
        set->addObject(mObjCtrl->cookieAt(i, row));
    };
    if (horzLength >= 3) {
        set->addObject(mObjCtrl->cookieAt(column, row));
        return set;
    }
    set->removeAllObjects();

    int vertLength = 1;

    for (int i = row - 1; i >= 0 && mObjCtrl->isSameTypeOfCookieAt(column, i, type); i--, vertLength++) {
        set->addObject(mObjCtrl->cookieAt(column, i));
    };
    for (int i = row + 1; i < fieldSize && mObjCtrl->isSameTypeOfCookieAt(column, i, type); i++, vertLength++) {
        set->addObject(mObjCtrl->cookieAt(column, i));
    };
    if (vertLength >= 3) {
        set->addObject(mObjCtrl->cookieAt(column, row));
    }
    else {
        set->removeAllObjects();
        set = nullptr;
    }

    return set;
}

//--------------------------------------------------------------------
bool ChainController::getCellFromChainAndPrevSwapSet(int& column, int& row, ChainObj* chain, CommonTypes::Set* prevSwapObjs)
//--------------------------------------------------------------------
{
    CC_ASSERT(chain);
    if (!prevSwapObjs) {
        auto objects = chain->getChainObjects();
        auto randomObject = dynamic_cast<BaseObj*>(objects->getRandomObject());
        if (randomObject) {
            row = randomObject->getRow();
            column = randomObject->getColumn();
        }
    } 
    else {
        auto objects = chain->getObjects();
        for (auto it = prevSwapObjs->begin(); it != prevSwapObjs->end(); it++) {
            for (auto itChain = objects->begin(); itChain != objects->end(); itChain++) {
            
                if (*itChain == *it) {
                    auto container = dynamic_cast<ObjContainer*>(*itChain);
                    if (!container)
                        continue;

                    auto baseObj = container->getObjectForChain();
                    if (!baseObj)
                        continue;

                    row = baseObj->getRow();
                    column = baseObj->getColumn();
                    break;
                }
            }
            if (column != -1 && row != -1) {
                break;
            }
        }        
    }
    if (column == -1 || row == -1) {
        return false;
    }
    return true;
}

//--------------------------------------------------------------------
void ChainController::activateChains(Set* chains)
//--------------------------------------------------------------------
{
    cocos2d::log("ChainController::activateChains:");
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);
        chain->activateObjects();
    }
}

//--------------------------------------------------------------------
void ChainController::deactivateChains(Set* chains)
//--------------------------------------------------------------------
{
    cocos2d::log("ChainController::deactivateChains:");
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);
        chain->deactivateObjects();
    }
}

//--------------------------------------------------------------------
void ChainController::matchChains(Set* chains)
//--------------------------------------------------------------------
{
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);
        auto objects = chain->getObjects();
        if (!objects) {
            continue;
        }
        for (auto it = objects->begin(); it != objects->end(); it++) {
            auto container = dynamic_cast<ObjContainer*>(*it);
            CC_ASSERT(container);

            auto objects = container->getObjectsForChain();
            if (!objects) {
                continue;
            }
            for (auto itObj = objects->begin(); itObj != objects->end(); ++itObj) {
                auto object = dynamic_cast<BaseObj*>(*itObj);
                CC_ASSERT(object);

                if (object->getType() == BaseObjType::Cookie) {
                    mObjCtrl->matchCookieObject(object);
                }
                else if (object->getType() == BaseObjType::Field) {
                    mObjCtrl->matchFieldObject(object);
                }
                else if (object->getType() == BaseObjType::Dude) {
                    mObjCtrl->matchDudeObject(object);
                }
            }            
        }
    }
}

//--------------------------------------------------------------------
void ChainController::executeCollectGoalCallback(Set * chains)
//--------------------------------------------------------------------
{
    for (auto it = chains->begin(); it != chains->end(); it++) {
        auto chain = dynamic_cast<ChainObj*>(*it);
        if (chain)
            chain->executeCollectGoalCallback();
    }
}

//--------------------------------------------------------------------
bool ChainController::isPossibleToAddObjToChain(int col, int row, int & prevType, int & nextType)
//--------------------------------------------------------------------
{
    auto cookie = mObjCtrl->cookieAt(col, row);
    nextType = cookie ? cookie->getTypeAsInt() : -1;
    if (cookie != nullptr && nextType == prevType) {
        return true;
    }
    return false;
}

//--------------------------------------------------------------------
void ChainController::addObjToChain(ChainObj* chain, int col, int row)
//--------------------------------------------------------------------
{
    CC_ASSERT(chain);
    auto container = mObjCtrl->getContainer(col, row);
    if (container) {
        if (container->isContainGameObj()) {
            chain->addObjectToChain(container);
        }
    }
}

//--------------------------------------------------------------------
void ChainController::addChainToSet(ChainObj* chain, Set* set)
//--------------------------------------------------------------------
{
    CC_ASSERT(set);
    CC_ASSERT(chain);
    if (chain->getChainObjects()) {
        ScoreHelper::updateChainScore(chain);
        set->addObject(chain);
    }
}

//--------------------------------------------------------------------
Set* ChainController::detectVerticalMatches()
//--------------------------------------------------------------------
{
    auto set = Set::create();
    for (int column = 0; column < _GlobalInfo::NumColumns; column++) {
        for (int row = 0; row < _GlobalInfo::NumRows - 2;) {

            if (isNextTwoCookieSuitable(ChainType::ChainTypeVertical, column, row)) {
                int matchType = mObjCtrl->cookieAt(column, row)->getTypeAsInt();

                auto chain = ChainObj::createWithType(ChainType::ChainTypeVertical);
                chain->setUpdateGoalCallback(mUpdateGoalCallback);
                int newMatchType = -1;
                do {
                    if (isPossibleToAddObjToChain(column, row, matchType, newMatchType)) {
                        addObjToChain(chain, column, row);
                        row += 1;
                    }
                } while (row < _GlobalInfo::NumRows && newMatchType == matchType);
                addChainToSet(chain, set);
                continue;
            }
            //  If the next two cookies don�t match the current one or if there is an empty tile, 
            // then there is no chain, so you skip over the cookie.
            row += 1;
        }
    }
    return set;
}


//--------------------------------------------------------------------
Set* ChainController::detectHorizontalMatches()
//--------------------------------------------------------------------
{
    auto set = Set::create();
    for (int row = 0; row < _GlobalInfo::NumRows; row++) {
        for (int column = 0; column < _GlobalInfo::NumColumns - 2; ) {

            if (isNextTwoCookieSuitable(ChainType::ChainTypeHorizontal, column, row)) {
                int matchType = mObjCtrl->cookieAt(column, row)->getTypeAsInt();

                auto chain = ChainObj::createWithType(ChainType::ChainTypeHorizontal);
                chain->setUpdateGoalCallback(mUpdateGoalCallback);
                int newMatchType = -1;
                do {
                    if (isPossibleToAddObjToChain(column, row, matchType, newMatchType)) {
                        addObjToChain(chain, column, row);
                        column += 1;
                    }
                } while (column < _GlobalInfo::NumColumns && newMatchType == matchType);
                addChainToSet(chain, set);
                continue;
            }
            //  If the next two cookies don�t match the current one or if there is an empty tile, 
            // then there is no chain, so you skip over the cookie.
            column += 1;
        }
    }
    return set;
}

//--------------------------------------------------------------------
Set* ChainController::detectDifficultMatches(Set* horizontal, Set* vertical)
//--------------------------------------------------------------------
{
    auto set = Set::create();
    ChainObj* chainL = nullptr;
    ChainObj* chainX = nullptr;
    ChainObj* chainT = nullptr;    

    for (auto horzIt = horizontal->begin(); horzIt != horizontal->end(); ) {
        auto horzChain = dynamic_cast<ChainObj*>(*horzIt);
        CC_ASSERT(horzChain);

        for (auto vertIt = vertical->begin(); vertIt != vertical->end(); ) {
            auto vertChain = dynamic_cast<ChainObj*>(*vertIt);
            CC_ASSERT(vertChain);

            chainL = detectLChainMatches(horzChain, vertChain);
            chainX = detectXChainMatches(horzChain, vertChain);
            chainT = detectTChainMatches(horzChain, vertChain);

            if (chainL) {
                addChainToSet(chainL, set);
            }
            if (chainX) {
                addChainToSet(chainX, set);
            }
            if (chainT) {
                addChainToSet(chainT, set);
            }

            if (chainL || chainX || chainT) {
                vertIt++;
                vertical->removeObject(vertChain);
                continue;
            }
            vertIt++;
        }

        if (chainL || chainX || chainT) {
            horzIt++;
            horizontal->removeObject(horzChain);
            chainL = nullptr;
            chainX = nullptr;
            chainT = nullptr;
            continue;
        }
        horzIt++;
    }
    return set;
}

//--------------------------------------------------------------------
ChainObj* ChainController::detectLChainMatches(ChainObj * horzChain, ChainObj * vertChain)
//--------------------------------------------------------------------
{
    ChainObj* chain = nullptr;
    auto horzCookies = horzChain->getChainObjects();
    CC_ASSERT(horzCookies);
    auto vertCookies = vertChain->getChainObjects();
    CC_ASSERT(vertCookies);

    auto firstHorzCookie = dynamic_cast<CookieObj*>(horzCookies->getObjectAtIndex(0));
    auto lastHorzCookie = dynamic_cast<CookieObj*>(horzCookies->getLastObject());
    auto firstVertCookie = dynamic_cast<CookieObj*>(vertCookies->getObjectAtIndex(0));
    auto lastVertCookie = dynamic_cast<CookieObj*>(vertCookies->getLastObject());

    if (!firstHorzCookie || !lastHorzCookie || !firstVertCookie || !lastVertCookie) {
        return chain;
    }
    if (firstHorzCookie->getTypeAsInt() != firstVertCookie->getTypeAsInt()) {
        return chain;
    }

    if (firstHorzCookie == firstVertCookie || firstHorzCookie == lastVertCookie ||
        lastHorzCookie == firstVertCookie || lastHorzCookie == lastVertCookie) {
        chain = ChainObj::createWithType(ChainType::ChainTypeL);
        chain->setUpdateGoalCallback(mUpdateGoalCallback);
        chain->addObjectsFromChain(horzChain);
        chain->addObjectsFromChain(vertChain);
    }
    return chain;
}

//--------------------------------------------------------------------
ChainObj* ChainController::detectTChainMatches(ChainObj* horzChain, ChainObj* vertChain)
//--------------------------------------------------------------------
{
    ChainObj* chain = nullptr;
    auto horzCookies = horzChain->getChainObjects();
    CC_ASSERT(horzCookies);
    auto vertCookies = vertChain->getChainObjects();
    CC_ASSERT(vertCookies);

    auto firstHorzCookie = dynamic_cast<CookieObj*>(horzCookies->getObjectAtIndex(0));
    auto lastHorzCookie = dynamic_cast<CookieObj*>(horzCookies->getLastObject());
    auto firstVertCookie = dynamic_cast<CookieObj*>(vertCookies->getObjectAtIndex(0));
    auto lastVertCookie = dynamic_cast<CookieObj*>(vertCookies->getLastObject());

    if (!firstHorzCookie || !lastHorzCookie || !firstVertCookie || !lastVertCookie) {
        return chain;
    }
    if (firstHorzCookie->getTypeAsInt() != firstVertCookie->getTypeAsInt()) {
        return chain;
    }

    for (auto itHorz = horzCookies->begin() + 1; itHorz < horzCookies->end() - 1; itHorz++) {
        auto middleHorzCookie = dynamic_cast<CookieObj*>(*itHorz);
        if (!middleHorzCookie) {
            return chain;
        }
        for (auto itVert = vertCookies->begin() + 1; itVert < vertCookies->end() - 1; itVert++) {
            auto middleVertCookie = dynamic_cast<CookieObj*>(*itVert);
            if (!middleVertCookie) {
                return chain;
            }
            if (middleHorzCookie == firstVertCookie || middleHorzCookie == lastVertCookie ||
                middleVertCookie == firstHorzCookie || middleVertCookie == lastHorzCookie) {
                chain = ChainObj::createWithType(ChainType::ChainTypeT);
                chain->setUpdateGoalCallback(mUpdateGoalCallback);
                chain->addObjectsFromChain(horzChain);
                chain->addObjectsFromChain(vertChain);
                return chain;
            }
        }
    }
    return chain;
}

//--------------------------------------------------------------------
ChainObj* ChainController::detectXChainMatches(ChainObj* horzChain, ChainObj* vertChain)
//--------------------------------------------------------------------
{
    ChainObj* chain = nullptr;
    auto horzCookies = horzChain->getChainObjects();
    CC_ASSERT(horzCookies);
    auto vertCookies = vertChain->getChainObjects();
    CC_ASSERT(vertCookies);

    for (auto itHorz = horzCookies->begin() + 1; itHorz < horzCookies->end() - 1; itHorz++) {
        auto middleHorzCookie = dynamic_cast<CookieObj*>(*itHorz);
        if (!middleHorzCookie) {
            return chain;
        }
        for (auto itVert = vertCookies->begin() + 1; itVert < vertCookies->end() - 1; itVert++) {
            auto middleVertCookie = dynamic_cast<CookieObj*>(*itVert);
            if (!middleVertCookie) {
                return chain;
            }
            if (middleHorzCookie->getTypeAsInt() != middleVertCookie->getTypeAsInt()) {
                return chain;
            }
            if (middleHorzCookie == middleVertCookie ) {
                chain = ChainObj::createWithType(ChainType::ChainTypeX);
                chain->setUpdateGoalCallback(mUpdateGoalCallback);
                chain->addObjectsFromChain(horzChain);
                chain->addObjectsFromChain(vertChain);
                return chain;
            }
        }
    }
    return chain;
}

//--------------------------------------------------------------------
bool ChainController::isNextTwoCookieSuitable(const ChainType& type, int col, int row)
//--------------------------------------------------------------------
{
    bool result = false;
    auto cookie = mObjCtrl->cookieAt(col, row);
    // skip over any gaps in the level design.
    if (cookie != nullptr) {

        int matchType = cookie->getTypeAsInt();

        int horzDelta = type == ChainType::ChainTypeHorizontal ? 1 : 0;
        int vertDelta = type == ChainType::ChainTypeVertical ? 1 : 0;

        auto obj1 = mObjCtrl->cookieAt(col + (1 * horzDelta), row + (1 * vertDelta));
        auto obj2 = mObjCtrl->cookieAt(col + (2 * horzDelta), row + (2 * vertDelta));
        // check whether the next two columns have the same cookie type.
        if (obj1 != nullptr && obj2 != nullptr) {

            if (obj1->getTypeAsInt() == matchType
                && obj2->getTypeAsInt() == matchType) {
                //  There is a chain of at least three cookies but potentially there are more. 
                // This steps through all the matching cookies until it finds a cookie that breaks 
                // the chain or it reaches the end of the grid.
                result = true;
            }
        }
        
    }
    return result;
}

//--------------------------------------------------------------------
void ChainController::addChainsFromSetToSet(Set* from, Set* to, bool skipDudes)
//--------------------------------------------------------------------
{
    CC_ASSERT(from);
    CC_ASSERT(to);
    uint8_t index = 0;
    for (auto it = from->begin(); it != from->end(); it++, index++) {
        auto chain = dynamic_cast<ChainObj*>(*it);
        CC_ASSERT(chain);
        if (skipDudes) {
            chain->removeDudeObjectsFromChain();
        }
        to->addObject(chain);
    }
}

//--------------------------------------------------------------------
void ChainController::addObjectsFromChainToChain(Set* from, Set* to)
//--------------------------------------------------------------------
{
    CC_ASSERT(from);
    CC_ASSERT(to);
    auto toChain = dynamic_cast<ChainObj*>(to->anyObject());
    if (toChain) {
        for (auto it = from->begin(); it != from->end(); it++) {
            auto chain = dynamic_cast<ChainObj*>(*it);
            CC_ASSERT(chain);
            CC_ASSERT(toChain);
            toChain->addObjectsFromChain(chain);
            ScoreHelper::updateChainScore(toChain);
        }
    } else {
        addChainsFromSetToSet(from, to);
    }
}

//--------------------------------------------------------------------
void ChainController::addFieldOjbectsToChainSet(Set* fieldObjects, Set* chainSet)
//--------------------------------------------------------------------
{
    auto chain = ChainObj::createWithType(ChainType::ChainFieldObjects);
    chain->setUpdateGoalCallback(mUpdateGoalCallback);

    for (auto it = fieldObjects->begin(); it != fieldObjects->end(); it++) {
        auto obj = dynamic_cast<FieldObj*>(*it);
        if (obj)
            addObjToChain(chain, obj->getColumn(), obj->getRow());
    }
    addChainToSet(chain, chainSet);
}

//--------------------------------------------------------------------
bool ChainController::checkMathicngFieldObjWithChain(Set* chains, BaseObj* obj)
//--------------------------------------------------------------------
{
    auto result = false;
    auto fieldObj = dynamic_cast<FieldObj*>(obj);
    if (!fieldObj) {
        return result;
    }
    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);

        if (chain->getIsCreatedByDude()) {
            return result;
        }
        auto objects = chain->getChainObjects();
        for (auto it = objects->begin(); it != objects->end(); it++) {
            auto cookie = dynamic_cast<CookieObj*>(*it);
            if (cookie) {
                int col = cookie->getColumn();
                int row = cookie->getRow();
                if (fieldObj->checkMatchingCondition(col, row)) {
                    result = true;
                    break;
                }
            }
        }
        if (result)
            break;
    }
    return result;
}

//--------------------------------------------------------------------
Set* ChainController::createHorizontalChainAt(int startColumn, int startRow, bool isCreatedByDude)
//--------------------------------------------------------------------
{
    auto set = Set::create();

    if (!(startColumn >= 0 && startColumn < _GlobalInfo::NumColumns) 
        || !(startRow >= 0 && startRow < _GlobalInfo::NumRows)) {
        cocos2d::log("ChainController::createHorizontalChainAt: wrong destinationPos at column=%d, row=%d", startColumn, startRow);
        return set;
    }

    auto chain = ChainObj::createWithType(ChainType::ChainTypeHorizontal);
    chain->setUpdateGoalCallback(mUpdateGoalCallback);

    if (mObjCtrl->dudeAt(startColumn, startRow)) {
        addObjToChain(chain, startColumn, startRow);
        chain->setIsCreatedByDude(true);
    }
    for (int i = 0; i < _GlobalInfo::NumRows; i++) {
        if (startColumn == i && isCreatedByDude) {
            continue;
        }
        addObjToChain(chain, i, startRow);
    }
    addChainToSet(chain, set);
    return set;
}

//--------------------------------------------------------------------
Set* ChainController::createVerticalChainAt(int startColumn, int startRow, bool isCreatedByDude)
//--------------------------------------------------------------------
{
    auto set = Set::create();

    if (!(startColumn >= 0 && startColumn < _GlobalInfo::NumColumns) 
        || !(startRow >= 0 && startRow < _GlobalInfo::NumRows)) {
        cocos2d::log("ChainController::createVerticalChainAt: wrong destinationPos at column=%d, row=%d", startColumn, startRow);
        return set;
    }

    auto chain = ChainObj::createWithType(ChainType::ChainTypeVertical);
    chain->setUpdateGoalCallback(mUpdateGoalCallback);

    if (mObjCtrl->dudeAt(startColumn, startRow)) {
        addObjToChain(chain, startColumn, startRow);
        chain->setIsCreatedByDude(true);
    }
    for (int i = 0; i < _GlobalInfo::NumRows; i++) {
        if (startRow == i && isCreatedByDude) {
            continue;
        }
        addObjToChain(chain, startColumn, i);
    }
    addChainToSet(chain, set);
    return set;
}

//--------------------------------------------------------------------
Set* ChainController::createXChainAt(int column, int row, bool isCreatedByDude)
//--------------------------------------------------------------------
{
    auto set = Set::create();
    auto chain = ChainObj::createWithType(ChainType::ChainTypeX);
    chain->setUpdateGoalCallback(mUpdateGoalCallback);

    addObjToChain(chain, column, row);
    for (int i = 0; i < _GlobalInfo::NumColumns; i++) {
        for (int j = 0; j < _GlobalInfo::NumRows; j++) {
            if (column == i && row == j) {
                continue;
            }
            if (column == i) {
                addObjToChain(chain, column, j);
            }
            if (row == j) {
                addObjToChain(chain, i, row);
            }
        }
    }
    chain->setIsCreatedByDude(isCreatedByDude);
    addChainToSet(chain, set);

    return set;
}

//--------------------------------------------------------------------
Set* ChainController::createAllOfOneChain(int entryColumn, int entryRow, bool isCreatedByDude, BaseObj* dudeObj)
//--------------------------------------------------------------------
{
    auto set = Set::create();
    auto entryCookie = mObjCtrl->cookieAt(entryColumn, entryRow);
    if (!entryCookie) {
        return set;
    }

    auto chain = ChainObj::createWithType(ChainType::ChainTypeAllOfOne);
    chain->setUpdateGoalCallback(mUpdateGoalCallback);
    if (isCreatedByDude && dudeObj) {
        addObjToChain(chain, dudeObj->getColumn(), dudeObj->getRow());
    }    

    for (int column = 0; column < _GlobalInfo::NumColumns; column++) {
        for (int row = _GlobalInfo::NumRows - 1; row >= 0; row--) {
            auto cookie = mObjCtrl->cookieAt(row, column);
            // skip over any gaps in the level design.
            if (!cookie)
                continue;

            if (cookie->getCookieType() == entryCookie->getCookieType()) {
                addObjToChain(chain, row, column);
            }
        }
    }
    chain->setIsCreatedByDude(isCreatedByDude);
    addChainToSet(chain, set);
    return set;
}

//--------------------------------------------------------------------
Set* ChainController::createChainFromPosToPos(cocos2d::Vec2 from, cocos2d::Vec2 to, bool isCreatedByDude)
//--------------------------------------------------------------------
{
    int fromCol = -1; int fromRow = -1;
    int toCol = -1; int toRow = -1;
    auto set = Set::create();
    if (!Helper::convertPointToTilePos(from, fromCol, fromRow)) {
        return set;
    }
    if (!Helper::convertPointToTilePos(to, toCol, toRow)) {
        return set;
    }

    return createChainFromPosToPos(Direction::Unknown, fromCol, fromRow, toCol, toRow, isCreatedByDude);
}

//--------------------------------------------------------------------
Set* ChainController::createChainFromPosToPos(const Direction& direction, int fromCol, int fromRow, int toCol, int toRow, bool isCreatedByDude)
//--------------------------------------------------------------------
{
    auto set = Set::create();

    if (!(toCol >= 0 && toCol < _GlobalInfo::NumColumns) || !(toRow >= 0 && toRow < _GlobalInfo::NumColumns)) {
        cocos2d::log("ChainController::createChainFromPosToPos: wrong destinationPos at column=%d, row=%d", toCol, toRow);
        return set;
    }
 
    int i = fromCol; 
    int j = fromRow;

    auto chain = ChainObj::createWithType(ChainType::ChainFromAToB);
    chain->setUpdateGoalCallback(mUpdateGoalCallback);

    auto newDirection = direction;
    if (direction == Direction::Unknown) {
        auto dir = Helper::getDirectionByTileFromAToB(Helper::to_underlying(direction), fromCol, fromRow, toCol, toRow);
        newDirection = static_cast<Direction>(dir);
    }
    chain->setDirection(newDirection);

    addObjToChain(chain, i, j);
    do {
        if (fromCol != toCol) {
            i = fromCol > toCol ? i - 1 : i + 1;
        }
        do {
            if (fromRow != toRow) {
                j = fromRow > toRow ? j - 1 : j + 1;
            }
            addObjToChain(chain, i, j);                
        } while (j != toRow);
    } while (i != toCol);
    chain->setIsCreatedByDude(isCreatedByDude);
    addChainToSet(chain, set);
    return set;
}


#ifdef COCOS2D_DEBUG
// --------------------------------------------------------------------
// void ChainController::logDebugChains(CommonTypes::Set * horizontal, CommonTypes::Set * vertical, CommonTypes::Set * difficult)
// --------------------------------------------------------------------
// {
//     auto strHorizontalChains = cocos2d::String::createWithFormat("Horizontal matches: {\n");
//     auto horzIt = horizontal->begin();
//     for (horzIt; horzIt != horizontal->end(); horzIt++) {
//         auto chain = dynamic_cast<ChainObj*>(*horzIt);
//         CC_ASSERT(chain);
//         strHorizontalChains->appendWithFormat("%s\n", chain->description().c_str());
//     }
//     strHorizontalChains->append("}");
// 
//     auto strVerticalChains = cocos2d::String::createWithFormat("Vertical matches: {\n");
//     auto vertIt = vertical->begin();
//     for (vertIt; vertIt != vertical->end(); vertIt++) {
//         auto chain = dynamic_cast<ChainObj*>(*vertIt);
//         CC_ASSERT(chain);
//         strVerticalChains->appendWithFormat("%s\n", chain->description().c_str());
//     }
//     strVerticalChains->append("}");
// 
//     auto strtypeLChains = cocos2d::String::createWithFormat("type L matches: {\n");
//     auto strtypeTChains = cocos2d::String::createWithFormat("type T matches: {\n");
//     auto lIt = difficult->begin();
//     for (lIt; lIt != difficult->end(); lIt++) {
//         auto chain = dynamic_cast<ChainObj*>(*lIt);
//         CC_ASSERT(chain);
//         if (chain->getType() == ChainType::ChainTypeL) {
//             strtypeLChains->appendWithFormat("%s\n", chain->description().c_str());
//         }
//         else if (chain->getType() == ChainType::ChainTypeT) {
//             strtypeTChains->appendWithFormat("%s\n", chain->description().c_str());
//         }
//     }
//     strtypeLChains->append("}");
//     strtypeTChains->append("}");
// 
//     cocos2d::log("ChainController::logDebugChains: %s", strHorizontalChains->getCString());
//     cocos2d::log("ChainController::logDebugChains: %s", strVerticalChains->getCString());
//     cocos2d::log("ChainController::logDebugChains: %s", strtypeLChains->getCString());
//     cocos2d::log("ChainController::logDebugChains: %s", strtypeTChains->getCString());
// }
#endif // #ifdef COCOS2D_DEBUG
