/**
* @file Common/GlobalInfo/GlobalInfo.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"
#include "Common/GlobalInfo/GlobalInfoTypes.h"

#include "Utils/PlatformMacros.h"

class _GlobalInfo
{
public: 
    static _GlobalInfo& Instance();
    static _GlobalInfo* getInstance();

    void init();

protected:
    explicit _GlobalInfo() {};
    ~_GlobalInfo();
    _GlobalInfo(_GlobalInfo const&) = delete;
    _GlobalInfo(_GlobalInfo &&) = delete;
    _GlobalInfo & operator=(_GlobalInfo const&) = delete;
    _GlobalInfo & operator=(_GlobalInfo &&) = delete;

    void initGlView();
    void initGlobalInfo();

private:
    SYNTHESIZE_READONLY(const char*, mSwVersion, SwVersion, nullptr)
    SYNTHESIZE_READONLY(const char*, mImagePackType, ImagePackType, nullptr)
    SYNTHESIZE_READONLY(const char*, mImagePackVersion, ImagePackVersion, nullptr)
    SYNTHESIZE_READONLY_PASS_BY_REF(double, mTileWidth, TileWidth, 0.0f)
    SYNTHESIZE_READONLY_PASS_BY_REF(double, mTileHeight, TileHeight, 0.0f)
    SYNTHESIZE_READONLY_PASS_BY_REF(uint16_t, mLevelsCount, LevelsCount, 0)

    SYNTHESIZE_READONLY_PASS_BY_REF(float, mScaleFactor, ScaleFactor, 0.0f)
    SYNTHESIZE_READONLY_PASS_BY_REF(cocos2d::Size, mDesignResolutionSize, DesignResolutionSize, cocos2d::Size::ZERO)
    const cocos2d::Size mSmallResolutionSize = cocos2d::Size(480, 320);
    const cocos2d::Size mMediumResolutionSize = cocos2d::Size(768, 1024);
    const cocos2d::Size mLargeResolutionSize = cocos2d::Size(2048, 1536);
};

#define GlobalInfo _GlobalInfo::getInstance()