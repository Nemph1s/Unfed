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
#include <map>

class _JsonParser;
class _GlobalInfo
{
    friend _JsonParser;
public: 
    static _GlobalInfo& Instance();
    static _GlobalInfo* getInstance();

    void init();

    const std::map<uint16_t, uint16_t>& getDudeScoreMap() const;

protected:
    explicit _GlobalInfo() {};
    ~_GlobalInfo();
    _GlobalInfo(_GlobalInfo const&) = delete;
    _GlobalInfo(_GlobalInfo &&) = delete;
    _GlobalInfo & operator=(_GlobalInfo const&) = delete;
    _GlobalInfo & operator=(_GlobalInfo &&) = delete;

    void initGlView();
    void initGlobalInfo();

    void initScoreValues();

private:
    // Global info
    SYNTHESIZE_READONLY(const char*, mSwVersion, SwVersion, nullptr)
    SYNTHESIZE_READONLY(const char*, mImagePackType, ImagePackType, nullptr)
    SYNTHESIZE_READONLY(const char*, mImagePackVersion, ImagePackVersion, nullptr)
    SYNTHESIZE_READONLY_PASS_BY_REF(double, mTileWidth, TileWidth, 0.0f)
    SYNTHESIZE_READONLY_PASS_BY_REF(double, mTileHeight, TileHeight, 0.0f)
    SYNTHESIZE_READONLY_PASS_BY_REF(uint16_t, mLevelsCount, LevelsCount, 0)

    // GL view info
    SYNTHESIZE_READONLY_PASS_BY_REF(float, mScaleFactor, ScaleFactor, 0.0f)
    SYNTHESIZE_READONLY_PASS_BY_REF(cocos2d::Size, mDesignResolutionSize, DesignResolutionSize, cocos2d::Size::ZERO)

    const cocos2d::Size mSmallResolutionSize = cocos2d::Size(480, 320);
    const cocos2d::Size mMediumResolutionSize = cocos2d::Size(768, 1024);
    const cocos2d::Size mLargeResolutionSize = cocos2d::Size(2048, 1536);

    // Score values
    SYNTHESIZE_READONLY_PASS_BY_REF(uint16_t, mCookieMinScore, CookieMinScore, 0)
    SYNTHESIZE_READONLY_PASS_BY_REF(uint16_t, mCookieDefaultScore, CookieDefaultScore, 0)
    SYNTHESIZE_READONLY_PASS_BY_REF(uint16_t, mFieldObjScore, FieldObjScore, 0)
    SYNTHESIZE_READONLY_PASS_BY_REF(uint16_t, mEnemyScore, EnemyScore, 0)
    std::map<uint16_t, uint16_t> mDudeScoreMap;

    // Multipliers
    SYNTHESIZE_READONLY_PASS_BY_REF(uint16_t, mCookieScoreMultiplier, CookieScoreMultiplier, 0)
    //TODO: make multiplier for dude chain objects

public:
    static const uint8_t NumColumns = 9;
    static const uint8_t NumRows = 9;
};

#define GlobInfo _GlobalInfo::getInstance()