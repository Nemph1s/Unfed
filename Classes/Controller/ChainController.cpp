/**
* @file Scenes/SwapController.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ChainController.h"
#include "Controller/ObjectController/ObjContainer.h"
#include "Controller/ObjectController/ObjectController.h"

#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"
#include "GameObjects/Chain/ChainObj.h"
#include "GameObjects/Level/LevelObj.h"

#include "Utils/Helpers/Helper.h"

#include "Common/CommonTypes.h"

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
ChainController * ChainController::create()
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
CommonTypes::Set * ChainController::removeMatches()
//--------------------------------------------------------------------
{
    cocos2d::log("ChainController::removeMatches:");
    auto horizontalChains = detectHorizontalMatches();
    auto verticalChains = detectVerticalMatches();
    auto difficultChains = detectDifficultMatches(horizontalChains, verticalChains);

    auto set = CommonTypes::Set::create();

    addChainsFromSetToSet(horizontalChains, set);
    addChainsFromSetToSet(verticalChains, set);
    addChainsFromSetToSet(difficultChains, set);


#ifdef COCOS2D_DEBUG
    //logDebugChains(horizontalChains, verticalChains, difficultChains);
#endif //COCOS2D_DEBUG

    mLevel->calculateScore(horizontalChains);
    matchChains(horizontalChains);

    mLevel->calculateScore(verticalChains);
    matchChains(verticalChains);

    mLevel->calculateScore(difficultChains);
    matchChains(difficultChains);
    return set;
}

//--------------------------------------------------------------------
CommonTypes::Set * ChainController::removeChainAt(CommonTypes::ChainType & type, cocos2d::Vec2 & pos)
//--------------------------------------------------------------------
{
    auto set = CommonTypes::Set::create();

    int column = -1, row = -1;
    if (Helper::convertPointToTilePos(pos, column, row)) {
        CommonTypes::Set* chainSet = nullptr;
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

            mLevel->calculateScore(chainSet);
            matchChains(chainSet);
        }
    }
    return set;
}

//--------------------------------------------------------------------
void ChainController::matchChains(CommonTypes::Set* chains)
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

            auto object = container->getObjectForChain();
            if (!object) {
                continue;
            }
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

//--------------------------------------------------------------------
void ChainController::calculateChainScore(ChainObj* chain)
//--------------------------------------------------------------------
{
    CC_ASSERT(chain);
    int cookiesValue = 0;
    int chainValue = 0;
    auto objects = chain->getChainObjects();
    if (objects) {
        for (auto itObj = objects->begin(); itObj != objects->end(); itObj++) {
            auto obj = dynamic_cast<BaseObj*>(*itObj);
            int chainTypeMultiplier = 1;
            switch (chain->getType())
            {
            case ChainType::ChainTypeL:
                chainTypeMultiplier = 2;
                break;
            case ChainType::ChainTypeT:
                chainTypeMultiplier = 3;
            default:
                break;
            }
            int score = obj->getType() == CommonTypes::BaseObjType::Cookie ? obj->getScoreValue() : 0;
            cookiesValue = cookiesValue + score;
            chainValue = chainValue + obj->getScoreValue();
        }
        auto multiplier = !chain->getIsCreatedByDude() && objects->count() > 2 ? objects->count() - 2 : 1;
        chain->setCookiesScore(cookiesValue * multiplier);
        chain->setScore(chainValue * multiplier);
    }
}

//--------------------------------------------------------------------
void ChainController::executeCollectGoalCallback(CommonTypes::Set * chains)
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
    auto container = mObjCtrl->getObject(col, row);
    CC_ASSERT(container);
    if (container->isContainObjForChain()) {
        chain->addObjectToChain(container);
    }    
}

//--------------------------------------------------------------------
void ChainController::addChainToSet(ChainObj* chain, CommonTypes::Set* set)
//--------------------------------------------------------------------
{
    CC_ASSERT(set);
    CC_ASSERT(chain);
    if (chain->getChainObjects()) {

        calculateChainScore(chain);
        set->addObject(chain);
    }
}

//--------------------------------------------------------------------
CommonTypes::Set * ChainController::detectVerticalMatches()
//--------------------------------------------------------------------
{
    auto set = CommonTypes::Set::create();
    for (int column = 0; column < NumColumns; column++) {
        for (int row = 0; row < NumRows - 2;) {

            auto cookie = mObjCtrl->cookieAt(column, row);
            // skip over any gaps in the level design.
            if (cookie != nullptr) {
                int matchType = cookie->getTypeAsInt();

                auto other1 = mObjCtrl->cookieAt(column, row + 1);
                auto other2 = mObjCtrl->cookieAt(column, row + 2);
                // check whether the next two columns have the same cookie type.
                if (other1 != nullptr && other2 != nullptr) {

                    if (other1->getTypeAsInt() == matchType
                        && other2->getTypeAsInt() == matchType) {
                        //  There is a chain of at least three cookies but potentially there are more. This steps through all the matching cookies 
                        // until it finds a cookie that breaks the chain or it reaches the end of the grid.
                        auto chain = ChainObj::createWithType(ChainType::ChainTypeVertical);
                        chain->setUpdateGoalCallback(mUpdateGoalCallback);
                        int newMatchType = -1;
                        do {
                            if (isPossibleToAddObjToChain(column, row, matchType, newMatchType)) {
                                addObjToChain(chain, column, row);
                                row += 1;
                            }
                        } while (row < NumColumns && newMatchType == matchType);
                        addChainToSet(chain, set);
                        continue;
                    }
                }
            }
            //  If the next two cookies don’t match the current one or if there is an empty tile, 
            // then there is no chain, so you skip over the cookie.
            row += 1;
        }
    }
    return set;
}


//--------------------------------------------------------------------
CommonTypes::Set * ChainController::detectHorizontalMatches()
//--------------------------------------------------------------------
{
    auto set = CommonTypes::Set::create();
    for (int row = 0; row < NumRows; row++) {
        for (int column = 0; column < NumColumns - 2; ) {

            auto cookie = mObjCtrl->cookieAt(column, row);
            // skip over any gaps in the level design.
            if (cookie != nullptr) {
                int matchType = cookie->getTypeAsInt();

                auto other1 = mObjCtrl->cookieAt(column + 1, row);
                auto other2 = mObjCtrl->cookieAt(column + 2, row);
                // check whether the next two columns have the same cookie type.
                if (other1 != nullptr && other2 != nullptr) {

                    if (other1->getTypeAsInt() == matchType
                        && other2->getTypeAsInt() == matchType) {
                        //  There is a chain of at least three cookies but potentially there are more. This steps through all the matching cookies 
                        // until it finds a cookie that breaks the chain or it reaches the end of the grid.
                        auto chain = ChainObj::createWithType(ChainType::ChainTypeHorizontal);
                        chain->setUpdateGoalCallback(mUpdateGoalCallback);
                        int newMatchType = -1;
                        do {
                            if (isPossibleToAddObjToChain(column, row, matchType, newMatchType)) {
                                addObjToChain(chain, column, row);
                                column += 1;
                            }
                        } while (column < NumColumns && newMatchType == matchType);
                        addChainToSet(chain, set);
                        continue;
                    }
                }
            }
            //  If the next two cookies don’t match the current one or if there is an empty tile, 
            // then there is no chain, so you skip over the cookie.
            column += 1;
        }
    }
    return set;
}

//--------------------------------------------------------------------
CommonTypes::Set * ChainController::detectDifficultMatches(CommonTypes::Set * horizontal, CommonTypes::Set * vertical)
//--------------------------------------------------------------------
{
    auto set = CommonTypes::Set::create();
    ChainObj* chainL = nullptr;
    ChainObj* chainT = nullptr;

    for (auto horzIt = horizontal->begin(); horzIt != horizontal->end(); ) {
        auto horzChain = dynamic_cast<ChainObj*>(*horzIt);
        CC_ASSERT(horzChain);

        for (auto vertIt = vertical->begin(); vertIt != vertical->end(); ) {
            auto vertChain = dynamic_cast<ChainObj*>(*vertIt);
            CC_ASSERT(vertChain);

            chainL = detectLChainMatches(horzChain, vertChain);
            chainT = detectTChainMatches(horzChain, vertChain);

            if (chainL) {
                addChainToSet(chainL, set);
            }
            if (chainT) {
                addChainToSet(chainT, set);
            }

            if (chainL || chainT) {
                vertIt++;
                vertical->removeObject(vertChain);
                continue;
            }
            vertIt++;
        }

        if (chainL || chainT) {
            horzIt++;
            horizontal->removeObject(horzChain);
            chainL = nullptr;
            chainT = nullptr;
            continue;
        }
        horzIt++;
    }
    return set;
}

//--------------------------------------------------------------------
ChainObj * ChainController::detectLChainMatches(ChainObj * horzChain, ChainObj * vertChain)
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
ChainObj * ChainController::detectTChainMatches(ChainObj * horzChain, ChainObj * vertChain)
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

    int horzMiddlePos = firstHorzCookie->getColumn() + horzCookies->count() / 2;
    int vertMiddlePos = firstVertCookie->getRow() + vertCookies->count() / 2;
    auto middleHorzCookie = mObjCtrl->cookieAt(horzMiddlePos, firstHorzCookie->getRow());
    auto middleVertCookie = mObjCtrl->cookieAt(firstVertCookie->getColumn(), vertMiddlePos);

    if (middleHorzCookie == firstVertCookie || middleHorzCookie == lastVertCookie ||
        middleVertCookie == firstHorzCookie || middleVertCookie == lastHorzCookie) {
        chain = ChainObj::createWithType(ChainType::ChainTypeT);
        chain->setUpdateGoalCallback(mUpdateGoalCallback);
        chain->addObjectsFromChain(horzChain);
        chain->addObjectsFromChain(vertChain);
    }
    return chain;
}

//--------------------------------------------------------------------
void ChainController::addChainsFromSetToSet(CommonTypes::Set* from, CommonTypes::Set* to, bool skipDudes)
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
void ChainController::addObjectsFromChainToChain(CommonTypes::Set * from, CommonTypes::Set * to)
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
            toChain->setCookiesScore(toChain->getCookiesScore() + chain->getCookiesScore());
            toChain->setScore(toChain->getScore() + chain->getScore());
        }
        
    } else {
        addChainsFromSetToSet(from, to);
    }
}

//--------------------------------------------------------------------
void ChainController::addFieldOjbectsToChainSet(CommonTypes::Set* fieldObjects, CommonTypes::Set* chainSet)
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
bool ChainController::checkMathicngFieldObjWithChain(CommonTypes::Set * chains, BaseObj * obj)
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
CommonTypes::Set* ChainController::createHorizontalChainAt(int startColumn, int startRow, bool isCreatedByDude)
//--------------------------------------------------------------------
{
    auto set = CommonTypes::Set::create();

    if (!(startColumn >= 0 && startColumn < NumColumns) || !(startRow >= 0 && startRow < NumRows)) {
        cocos2d::log("ChainController::createHorizontalChainAt: wrong destinationPos at column=%d, row=%d", startColumn, startRow);
        return set;
    }

    auto chain = ChainObj::createWithType(ChainType::ChainTypeHorizontal);
    chain->setUpdateGoalCallback(mUpdateGoalCallback);

    if (mObjCtrl->getObject(startColumn, startRow)->getDudeObj()) {
        addObjToChain(chain, startColumn, startRow);
        chain->setIsCreatedByDude(true);
    }
    for (int i = 0; i < NumRows; i++) {
        if (startColumn == i && isCreatedByDude) {
            continue;
        }
        addObjToChain(chain, i, startRow);
    }
    addChainToSet(chain, set);
    return set;
}

//--------------------------------------------------------------------
CommonTypes::Set* ChainController::createVerticalChainAt(int startColumn, int startRow, bool isCreatedByDude)
//--------------------------------------------------------------------
{
    auto set = CommonTypes::Set::create();

    if (!(startColumn >= 0 && startColumn < NumColumns) || !(startRow >= 0 && startRow < NumRows)) {
        cocos2d::log("ChainController::createVerticalChainAt: wrong destinationPos at column=%d, row=%d", startColumn, startRow);
        return set;
    }

    auto chain = ChainObj::createWithType(ChainType::ChainTypeVertical);
    chain->setUpdateGoalCallback(mUpdateGoalCallback);

    if (mObjCtrl->getObject(startColumn, startRow)->getDudeObj()) {
        addObjToChain(chain, startColumn, startRow);
        chain->setIsCreatedByDude(true);
    }
    for (int i = 0; i < NumRows; i++) {
        if (startRow == i && isCreatedByDude) {
            continue;
        }
        addObjToChain(chain, startColumn, i);
    }
    addChainToSet(chain, set);
    return set;
}

//--------------------------------------------------------------------
CommonTypes::Set* ChainController::createXChainAt(int column, int row, bool isCreatedByDude)
//--------------------------------------------------------------------
{
    auto set = CommonTypes::Set::create();
    auto chain = ChainObj::createWithType(ChainType::ChainTypeX);
    chain->setUpdateGoalCallback(mUpdateGoalCallback);

    addObjToChain(chain, column, row);
    for (int i = 0; i < NumColumns; i++) {
        for (int j = 0; j < NumRows; j++) {
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
CommonTypes::Set* ChainController::createAllOfOneChain(int entryColumn, int entryRow, bool isCreatedByDude)
//--------------------------------------------------------------------
{
    auto set = CommonTypes::Set::create();
    auto entryCookie = mObjCtrl->cookieAt(entryColumn, entryRow);
    if (!entryCookie) {
        return set;
    }

    auto chain = ChainObj::createWithType(ChainType::ChainTypeAllOfOne);
    chain->setUpdateGoalCallback(mUpdateGoalCallback);
    for (int column = 0; column < NumColumns; column++) {
        for (int row = NumRows - 1; row >= 0; row--) {
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
CommonTypes::Set * ChainController::createChainFromPosToPos(cocos2d::Vec2 from, cocos2d::Vec2 to, bool isCreatedByDude)
//--------------------------------------------------------------------
{
    int fromCol = -1; int fromRow = -1;
    int toCol = -1; int toRow = -1;
    auto set = CommonTypes::Set::create();
    if (!Helper::convertPointToTilePos(from, fromCol, fromRow)) {
        return set;
    }
    if (!Helper::convertPointToTilePos(to, toCol, toRow)) {
        return set;
    }

    return createChainFromPosToPos(Direction::Unknown, fromCol, fromRow, toCol, toRow, isCreatedByDude);
}

//--------------------------------------------------------------------
CommonTypes::Set * ChainController::createChainFromPosToPos(const CommonTypes::Direction& direction, int fromCol, int fromRow, int toCol, int toRow, bool isCreatedByDude)
//--------------------------------------------------------------------
{
    auto set = CommonTypes::Set::create();

    if (!(toCol >= 0 && toCol < NumColumns) || !(toRow >= 0 && toRow < NumColumns)) {
        cocos2d::log("ChainController::createChainFromPosToPos: wrong destinationPos at column=%d, row=%d", toCol, toRow);
        return set;
    }
 
    int i = fromCol; 
    int j = fromRow;

    auto chain = ChainObj::createWithType(ChainType::ChainFromAToB);
    chain->setUpdateGoalCallback(mUpdateGoalCallback);

    auto dir = Helper::getDirectionByTileFromAToB(fromCol, fromRow, toCol, toRow);
    auto newDirection = direction;
    if (direction == Direction::Unknown) {
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
