/**
* @file GameObjects/TileObjects/FieldObjects/BushObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/TileObjects/FieldObjects/BushObj.h"

#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

using GOT::FieldType;

//--------------------------------------------------------------------
BushObj::BushObj()
    : FieldObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
BushObj::~BushObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
BushObj * BushObj::create(const GOT::FieldInfo & info)
//--------------------------------------------------------------------
{
    BushObj * ret = new (std::nothrow) BushObj();
    if (ret && ret->init(info)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool BushObj::init(const GOT::FieldInfo & info)
//--------------------------------------------------------------------
{
    if (!FieldObj::init(info)) {
        cocos2d::log("BushObj::init: can't init FieldObj inctance");
        return false;
    }

    if (info.fieldType >= FieldType::Bush && info.fieldType <= FieldType::Bush_HP2) {
        mHP = (getTypeAsInt() - Helper::to_underlying(FieldType::Bush)) + 1;
    }
    mIsRemovable = true;
    mIsMovable = false;
    mIsContainer = false;

    return true;
}

//--------------------------------------------------------------------
cocos2d::String& BushObj::spriteName() const
//--------------------------------------------------------------------
{
    const int normalState = 2;
    const int corruptedState = 1;
    cocos2d::String* str = nullptr;
    switch (mHP)
    {
    case corruptedState:
        str = &GameResources::s_BushCorruptedImg;
        break;
    case normalState:
    default:
        str = &GameResources::s_BushNormalImg;
        break;
    }
    return *str;
}

//--------------------------------------------------------------------
bool BushObj::checkMatchingCondition(CT::Cell& cell)
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
