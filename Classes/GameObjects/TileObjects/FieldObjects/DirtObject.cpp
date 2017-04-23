/**
* @file GameObjects/TileObjects/FieldObjects/DirtObject.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/TileObjects/FieldObjects/DirtObject.h"

#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

using GOT::FieldType;

//--------------------------------------------------------------------
DirtObject::DirtObject()
    : FieldObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
DirtObject::~DirtObject()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
DirtObject * DirtObject::create(const GOT::FieldInfo & info)
//--------------------------------------------------------------------
{
    DirtObject * ret = new (std::nothrow) DirtObject();
    if (ret && ret->init(info)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool DirtObject::init(const GOT::FieldInfo & info)
//--------------------------------------------------------------------
{
    if (!FieldObj::init(info)) {
        cocos2d::log("DirtObject::init: can't init FieldObj inctance");
        return false;
    }
 
    if (info.fieldType >= FieldType::Dirt && info.fieldType <= FieldType::Dirt_HP3) {
        mHP = (getTypeAsInt() - Helper::to_underlying(FieldType::Dirt)) + 1;
    }
    mIsRemovable = true;
    mIsContainer = true;
    mPriority = -1;

    return true;
}

//--------------------------------------------------------------------
cocos2d::String& DirtObject::spriteName() const
//--------------------------------------------------------------------
{
    const int x2State = 2;
    const int x1State = 1;
    cocos2d::String* str = nullptr;
    switch (mHP)
    {
    case x1State:
        str = &GameResources::s_DirtX2Img;
        break;
    case x2State:
    default:
        str = &GameResources::s_DirtImg;
        break;
    }
    return *str;
}

//--------------------------------------------------------------------
bool DirtObject::checkMatchingCondition(CT::Cell& cell)
//--------------------------------------------------------------------
{
    if (!Helper::isValidCell(cell)) {
        return false;
    }
    return (mColumn == cell.column && mRow == cell.row);
}
