/**
* @file GameObjects/TileObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/TileObj.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

//--------------------------------------------------------------------
TileObj::TileObj()
    : mColumn(0)
    , mRow(0)
    , mType(CommonTypes::TileType::Unknown)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
TileObj::~TileObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
TileObj * TileObj::create(const CommonTypes::TileInfo & info)
//--------------------------------------------------------------------
{
    TileObj * ret = new (std::nothrow) TileObj();
    if (ret && ret->init(info)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool TileObj::init(const CommonTypes::TileInfo & info)
//--------------------------------------------------------------------
{
    if (!Node::init()) {
        cocos2d::log("TileObj::init: can't init Node inctance");
        return false;
    }
    mColumn = info.column;
    mRow = info.row;
    mType = info.tileType;

    return true;
}

//--------------------------------------------------------------------
std::string TileObj::spriteName() const
//--------------------------------------------------------------------
{
    return GameResources::s_TileImg;
}

//--------------------------------------------------------------------
int TileObj::getTypeAsInt() const
//--------------------------------------------------------------------
{
    return Helper::getInstance()->to_underlying(mType);
}

//--------------------------------------------------------------------
bool TileObj::isVisibleTile()
//--------------------------------------------------------------------
{
    if (mType == CommonTypes::TileType::Empty) {
        return false;
    } else {
        return true;
    }    
}
