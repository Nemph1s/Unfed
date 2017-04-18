/**
* @file Controller/ObjectController/Enemy/EnemyController.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Controller/ObjectController/Enemy/EnemyController.h"
#include "Controller/ObjectController/Enemy/EnemyHelper.h"
#include "Controller/ObjectController/Enemy/EnemyObj.h"
#include "Controller/ObjectController/ObjectController.h"
#include "Controller/ObjectController/ObjContainer.h"
#include "GameObjects/Level/LevelObj.h"


#include "Controller/ObjectController/Dude/DudeController.h"

#include "Common/Factory/SmartObjFactory.h"

#include "Utils/Parser/JsonParser.h"
#include "Utils/Helpers/Helper.h"

#include "GameObjects/TileObjects/TileObj.h"

#include "GameObjects/TileObjects/CookieObj.h"
#include "Controller/ObjectController/Dude/DudeObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"

using namespace CT;
using namespace std::placeholders;

//--------------------------------------------------------------------
EnemyController::EnemyController()
    : mObjCtrl(nullptr)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
EnemyController::~EnemyController()
//--------------------------------------------------------------------
{
    mObjCtrl = nullptr;
}

//--------------------------------------------------------------------
EnemyController * EnemyController::create()
//--------------------------------------------------------------------
{
    EnemyController * ret = new (std::nothrow) EnemyController();
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
bool EnemyController::init()
//--------------------------------------------------------------------
{
    return true;
}

//--------------------------------------------------------------------
CT::Set* EnemyController::createInitialEnemies()
//--------------------------------------------------------------------
{
    auto level = mObjCtrl->getLevel();
    auto levelInfo = level->getLevelInfo();
    auto set = CT::Set::create();
    for (auto obj : levelInfo.enemyObjects) {

        auto cell = Cell(obj.baseInfo.column, obj.baseInfo.row);
        auto enemyObj = createEnemy(cell, obj.enemyType);
        set->addObject(enemyObj);
    }
    return set;
}

//--------------------------------------------------------------------
void EnemyController::detectDirectionsForEnemies()
//--------------------------------------------------------------------
{
    for (auto it = mEnemyDirections.begin(); it != mEnemyDirections.end(); ++it) {
        auto enemyPair = *it;
        EnemyObj* enemy = enemyPair.first;
        EnemyHelper* helper = enemyPair.second;
        if (enemy && helper) {
            updateDirectionsForEnemy(enemy, helper);
        }
    }
}

//--------------------------------------------------------------------
void EnemyController::eraseEnemyHelper(EnemyObj* obj)
//--------------------------------------------------------------------
{
    if (!obj) {
        cocos2d::log("EnemyController::eraseEnemyHelper: empty enemy ptr");
        return;
    }
    mEnemyDirections.erase(obj);
}

//--------------------------------------------------------------------
bool EnemyController::beginEnemiesTurn(bool updateDirections /* = true */)
//--------------------------------------------------------------------
{
    bool result = false;
    cocos2d::log("EnemyController::beginEnemiesTurn:");
    for (auto it = mEnemyDirections.begin(); it != mEnemyDirections.end(); ++it) {
        
        auto enemyPair = *it;
        EnemyObj* enemy = enemyPair.first;
        if (updateDirections) {
            EnemyHelper* helper = enemyPair.second;
            if (enemy && helper) {
                updateDirectionsForEnemy(enemy, helper);
            }
        }
        if (runAction(enemy)) {
            result = true;
        }        
    }
    return result;
}

//--------------------------------------------------------------------
BaseObj * EnemyController::createEnemy(Cell& cell, int type, int priority)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Enemy, cell };
    EnemyInfo info = { baseInfo, static_cast<EnemyType>(type) };
    auto obj = dynamic_cast<EnemyObj*>(SmartObjFactory->createEnemyObj(info));
    CC_ASSERT(obj);
    auto container = mObjCtrl->getContainer(cell);
    if (container) {
        container->addObject(obj);
    }
    auto helper = EnemyHelper::createWithEnemyObject(obj);
    helper->setEnemyController(this);
    helper->setObjectController(mObjCtrl);

    mEnemyDirections.insert(obj, helper);
    return obj;
}

//--------------------------------------------------------------------
bool EnemyController::runAction(EnemyObj* obj)
//--------------------------------------------------------------------
{
    bool result = true;
    auto helper = mEnemyDirections.at(obj);
    if (!helper) {
        result = false;
    }
    if (helper->isReadyForAction()) {
        helper->runEnemyAction();
    } else {
        result = false;
    }
    //TODO: change state in different way
    obj->updateState();
    return result;
}

//--------------------------------------------------------------------
void EnemyController::updateDirectionsForEnemy(EnemyObj* obj, EnemyHelper* helper)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);
    CC_ASSERT(helper);

    int column = obj->getColumn();
    int row = obj->getRow();

    auto topObj = mObjCtrl->getObjectForChain(Cell(column, row - 1));
    auto botObj = mObjCtrl->getObjectForChain(Cell(column, row + 1));
    auto leftObj = mObjCtrl->getObjectForChain(Cell(column - 1, row));
    auto rightObj = mObjCtrl->getObjectForChain(Cell(column + 1, row));

    updateNotAllowedDirectionToObj(topObj);
    updateNotAllowedDirectionToObj(botObj);
    updateNotAllowedDirectionToObj(leftObj);
    updateNotAllowedDirectionToObj(rightObj);

    helper->updateNearbyObjects(topObj, botObj, leftObj, rightObj);
}

//--------------------------------------------------------------------
void EnemyController::updateNotAllowedDirectionToObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    if (obj) {
        if (!obj->isSwappable() && obj->isContainer()) {
            obj = nullptr;
        }
    }
}

//--------------------------------------------------------------------
bool EnemyController::matchEnemyObject(BaseObj* obj)
//--------------------------------------------------------------------
{
    auto objContainer = mObjCtrl->getContainer(obj->getCell());
    if (!objContainer) {
        return false;
    }

    obj->match();

    auto enemyObj = objContainer->getEnemy();
    if (obj != enemyObj) {
        return false;
    }

    std::function<void(BaseObj*)> func = [&](BaseObj* obj) {
        auto enemy = dynamic_cast<EnemyObj*>(obj);
        if (enemy) {
            eraseEnemyHelper(enemy);
        }
    };
    enemyObj->setEraseEnemyHelperCallback(func);

    std::function<void(BaseObj*)> onRemoveEnemyCallback;
    onRemoveEnemyCallback = std::bind(&ObjContainer::onRemoveEnemy, objContainer, _1);
    enemyObj->setRemoveEnemyCallback(onRemoveEnemyCallback);
    return true;
}
