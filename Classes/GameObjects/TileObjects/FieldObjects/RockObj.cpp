/**
* @file GameObjects/TileObjects/FieldObjects/BushObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/TileObjects/FieldObjects/RockObj.h"

#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

using GOT::FieldType;

//--------------------------------------------------------------------
RockObj::RockObj()
    : FieldObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
RockObj::~RockObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
RockObj* RockObj::create(const GOT::FieldInfo & info)
//--------------------------------------------------------------------
{
    RockObj* ret = new (std::nothrow) RockObj();
    if (ret && ret->init(info)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool RockObj::init(const GOT::FieldInfo & info)
//--------------------------------------------------------------------
{
    if (!FieldObj::init(info)) {
        cocos2d::log("RockObj::init: can't init FieldObj inctance");
        return false;
    }

    if (info.fieldType >= FieldType::RockWall && info.fieldType <= FieldType::RockWall) {
        mHP = (getTypeAsInt() - Helper::to_underlying(FieldType::RockWall)) + 1;
    }
    mIsRemovable = true;
    mIsMovable = false;
    mIsContainer = false;

    return true;
}

//--------------------------------------------------------------------
cocos2d::String& RockObj::spriteName() const
//--------------------------------------------------------------------
{
    const int weakState = 1;
    cocos2d::String* str = nullptr;
    switch (mHP)
    {
    case weakState:
    default:
        str = &GameResources::s_RockImg;
        break;
    }
    return *str;
}

//--------------------------------------------------------------------
bool RockObj::checkMatchingCondition(CT::Cell& cell)
//--------------------------------------------------------------------
{
    if (!Helper::isValidCell(cell)) {
        return false;
    }
    bool objectOnTop = (mColumn == cell.column && mRow == cell.row - 1);
    bool objectOnBot = (mColumn == cell.column && mRow == cell.row + 1);
    bool objectOnLeft = (mColumn == cell.column - 1 && mRow == cell.row);
    bool objectOnRight = (mColumn == cell.column + 1 && mRow == cell.row);
    return objectOnTop || objectOnBot || objectOnLeft || objectOnRight;
}
