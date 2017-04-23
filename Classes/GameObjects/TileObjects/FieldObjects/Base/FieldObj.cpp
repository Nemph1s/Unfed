/**
* @file GameObjects/TileObjects/FieldObjects/Base/FieldObj.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"
#include "Common/GlobalInfo/GlobalInfo.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

using GOT::FieldType;

//--------------------------------------------------------------------
FieldObj::FieldObj()
    : BaseObj()
    , mFieldType(FieldType::Unknown)
    , mHP(0)
    , mReadyToUpdatePriority(false)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
FieldObj::~FieldObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
FieldObj * FieldObj::create(const GOT::FieldInfo &info)
//--------------------------------------------------------------------
{
    FieldObj * ret = new (std::nothrow) FieldObj();
    if (ret && ret->init(info)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//--------------------------------------------------------------------
bool FieldObj::init(const GOT::FieldInfo &info)
//--------------------------------------------------------------------
{
    if (!BaseObj::init(info.baseInfo)) {
        cocos2d::log("FieldObj::init: can't init Node inctance");
        return false;
    }
    mFieldType = info.fieldType;
    mPriority = info.priority;
    return true;
}

//--------------------------------------------------------------------
cocos2d::String & FieldObj::description() const
//--------------------------------------------------------------------
{
    return *cocos2d::String::createWithFormat("type:%d square:(%d,%d)", getTypeAsInt(), mColumn, mRow);
}

//--------------------------------------------------------------------
int FieldObj::getTypeAsInt() const
//--------------------------------------------------------------------
{
    return Helper::getInstance()->to_underlying(mFieldType);
}

//--------------------------------------------------------------------
void FieldObj::match()
//--------------------------------------------------------------------
{
    mHP--;
    if (mHP > 0) {
        mFieldType = static_cast<FieldType>(getTypeAsInt() - 1);
    } else if (mHP == 0) {
        mPriority--;
    }
}

//--------------------------------------------------------------------
void FieldObj::clear()
//--------------------------------------------------------------------
{
    BaseObj::clear();
    mFieldType = FieldType::Unknown;
    mHP = 0;
    mReadyToUpdatePriority = false;
}

//--------------------------------------------------------------------
bool FieldObj::isHpEnded() const
//--------------------------------------------------------------------
{
    bool result = false;
    if (isRemovable()) {
        result = (mHP <= 0);
    }
    return result;
}
