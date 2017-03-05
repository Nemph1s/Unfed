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
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

//--------------------------------------------------------------------
FieldObj::FieldObj()
    : BaseObj()
    , mFieldType(ObjTypes::FieldType::Unknown)
    , mDebugLabel(nullptr)
    , mHP(0)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
FieldObj::~FieldObj()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
FieldObj * FieldObj::create(const ObjTypes::FieldInfo &info)
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
bool FieldObj::init(const ObjTypes::FieldInfo &info)
//--------------------------------------------------------------------
{
    if (!BaseObj::init(info.baseInfo)) {
        cocos2d::log("FieldObj::init: can't init Node inctance");
        return false;
    }
    mFieldType = info.fieldType;

    if (!mDebugLabel && mType != CommonTypes::BaseObjectType::FieldObj) {
#ifdef COCOS2D_DEBUG
        mDebugLabel = cocos2d::Label::create();
        mDebugLabel->setBMFontSize(16);
        mDebugLabel->setDimensions(32, 32);
        mDebugLabel->setHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        mDebugLabel->setVerticalAlignment(cocos2d::TextVAlignment::BOTTOM);
        mDebugLabel->setPosition(cocos2d::Vec2(GameResources::TileWidth * 0.8f, GameResources::TileHeight * 0.2f));
        mDebugLabel->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        mDebugLabel->setTextColor(cocos2d::Color4B::MAGENTA);
        mDebugLabel->setGlobalZOrder(1000);
        CC_SAFE_RETAIN(mDebugLabel);
        //mSpriteNode->addChild(mDebugLabel, 10);

        int col = mColumn == -1 ? 0 : mColumn;
        int row = mRow == -1 ? 0 : mRow;

        auto text = cocos2d::StringUtils::format("[%d,%d]", col, row);
        mDebugLabel->setString(text);
#endif //UNFED_ENABLE_DEBUG
    }

    return true;
}

//--------------------------------------------------------------------
cocos2d::String & FieldObj::description() const
//--------------------------------------------------------------------
{
    return *cocos2d::String::createWithFormat("type:%d square:(%d,%d)", getTypeAsInt(), mColumn, mRow);
}

//--------------------------------------------------------------------
void FieldObj::setSpriteNode(cocos2d::Sprite * var)
//--------------------------------------------------------------------
{
    mSpriteNode = var;
    if (mSpriteNode && mDebugLabel) {
        if (!mDebugLabel->getParent()) {
            mSpriteNode->addChild(mDebugLabel, 10);
        }
    }
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
        mFieldType = static_cast<ObjTypes::FieldType>(getTypeAsInt() - 1);
    }
}

//--------------------------------------------------------------------
void FieldObj::clear()
//--------------------------------------------------------------------
{
    BaseObj::clear();
    mFieldType = ObjTypes::FieldType::Unknown;
    mHP = 0;
    if (mDebugLabel) {
        if (mDebugLabel->getParent()) {
            mDebugLabel->removeFromParent();
        }        
        CC_SAFE_RELEASE_NULL(mDebugLabel);
    }    
}

//--------------------------------------------------------------------
bool FieldObj::checkMatchingCondition(int column, int row)
//--------------------------------------------------------------------
{
    return false;
}

//--------------------------------------------------------------------
bool FieldObj::isReadyToRemove() const
//--------------------------------------------------------------------
{
    bool result = false;
    if (isRemovable()) {
        result = (mHP <= 0);
    }
    return result;
}

//--------------------------------------------------------------------
void FieldObj::updateDebugLabel()
//--------------------------------------------------------------------
{
    if (mDebugLabel) {
        int col = mColumn == -1 ? 0 : mColumn;
        int row = mRow == -1 ? 0 : mRow;

        auto text = cocos2d::StringUtils::format("[%d,%d]", col, row);
        mDebugLabel->setString(text);
    }
}
