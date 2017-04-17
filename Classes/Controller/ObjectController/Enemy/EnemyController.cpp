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
BaseObj * EnemyController::createEnemy(Cell& cell, int type, int priority)
//--------------------------------------------------------------------
{
    BaseObjInfo baseInfo = { BaseObjType::Enemy, cell };
    EnemyInfo info = { baseInfo, static_cast<EnemyType>(type) };
    auto enemyObj = SmartObjFactory->createEnemyObj(info);
    CC_ASSERT(enemyObj);
    auto obj = mObjCtrl->getContainer(cell);
    if (obj) {
        obj->addObject(enemyObj);
    }
    return enemyObj;
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
    if (obj == enemyObj) {
        std::function<void(BaseObj*)> onRemoveEnemyCallback;
        onRemoveEnemyCallback = std::bind(&ObjContainer::onRemoveEnemy, objContainer, _1);
        enemyObj->setRemoveEnemyCallback(onRemoveEnemyCallback);
    }
    else {
        return false;
    }
    return true;
}
