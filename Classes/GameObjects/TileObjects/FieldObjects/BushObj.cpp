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

using CommonTypes::FieldType;

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
BushObj * BushObj::create(const CommonTypes::FieldInfo & info)
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
bool BushObj::init(const CommonTypes::FieldInfo & info)
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
    mScoreValue = 100;

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
bool BushObj::checkMatchingCondition(int column, int row)
//--------------------------------------------------------------------
{
    if (column < 0 || column >= CommonTypes::NumColumns || row < 0 || row >= CommonTypes::NumColumns) {
        return false;
    }
    bool objectOnTop = (mColumn == column && mRow == row - 1);
    bool objectOnBot = (mColumn == column && mRow == row + 1);
    bool objectOnLeft = (mColumn == column - 1 && mRow == row);
    bool objectOnRight = (mColumn == column + 1 && mRow == row);
    return objectOnTop || objectOnBot || objectOnLeft || objectOnRight;
}