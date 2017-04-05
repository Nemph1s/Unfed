/**
* @file Common/GlobalInfo/GlobalInfo.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Common/GlobalInfo/GlobalInfo.h"
#include "Utils/Parser/JsonParser.h"

using cocos2d::Size;

//--------------------------------------------------------------------
_GlobalInfo & _GlobalInfo::Instance()
//--------------------------------------------------------------------
{
    static _GlobalInfo myInstance;
    return myInstance;
}

//--------------------------------------------------------------------
_GlobalInfo * _GlobalInfo::getInstance()
//--------------------------------------------------------------------
{
    return &_GlobalInfo::Instance();
}

//--------------------------------------------------------------------
_GlobalInfo::~_GlobalInfo()
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
void _GlobalInfo::init()
//--------------------------------------------------------------------
{
    cocos2d::log("//--------------------------------------------------------------------");
    cocos2d::log("GlobalInfo::init:");

    mDesignResolutionSize = mMediumResolutionSize;

    initGlView();
    initGlobalInfo();
    initScoreValues();
}

//--------------------------------------------------------------------
const std::map<uint16_t, uint16_t>& _GlobalInfo::getDudeScoreMap() const
//--------------------------------------------------------------------
{
    return mDudeScoreMap;
}

//--------------------------------------------------------------------
void _GlobalInfo::initGlView()
//--------------------------------------------------------------------
{
    cocos2d::log("//--------------------------------------------------------------------");
    cocos2d::log("GlobalInfo::initGlView:");
    cocos2d::log(" -  designResolutionSize: %.2fx%.2f", mDesignResolutionSize.width, mDesignResolutionSize.height);
    cocos2d::log("//--------------------------------------------------------------------");

    auto designSize = mDesignResolutionSize;

    auto sdSize = mSmallResolutionSize;
    auto hdsize = mMediumResolutionSize;
    auto hdrSize = mLargeResolutionSize;

    // initialize director
    auto director = cocos2d::Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = cocos2d::GLViewImpl::createWithRect("Unfed", cocos2d::Rect(0, 0, designSize.width, designSize.height));
#else
        glview = cocos2d::GLViewImpl::create("Unfed");
#endif
        director->setOpenGLView(glview);
    }
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::NO_BORDER);

    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > hdsize.height) {
        mScaleFactor = MIN(hdrSize.height / designSize.height, hdrSize.width / designSize.width);
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > sdSize.height) {
        mScaleFactor = MIN(hdsize.height / designSize.height, hdsize.width / designSize.width);
    }
    // if the frame's height is smaller than the height of medium size.
    else {
        mScaleFactor = MIN(sdSize.height / designSize.height, sdSize.width / designSize.width);
    }
    director->setContentScaleFactor(mScaleFactor);
}

//--------------------------------------------------------------------
void _GlobalInfo::initGlobalInfo()
//--------------------------------------------------------------------
{
    cocos2d::log("//--------------------------------------------------------------------");
    cocos2d::log("GlobalInfo::initGlobalInfo:");
    cocos2d::log(" - scaleFactor: %.2f", mScaleFactor);
    JsonParser->parseGlobalInfo();
    CC_ASSERT(JsonParser->checkGlobalInfoStatus());

    auto info = JsonParser->getJsonGlobalInfo();
    mSwVersion = info.swVersion;
    mImagePackType = info.imgPackType;
    mImagePackVersion = info.imgPackVersion;
    mTileWidth = info.tileWidth * mScaleFactor;
    mTileHeight = info.tileHeigth * mScaleFactor;
    mLevelsCount = info.levelsCount;

    cocos2d::log(" - swVersion: %s", mSwVersion);
    cocos2d::log(" - imagePackType: %s", mImagePackType);
    cocos2d::log(" - imagePackVersion: %s", mImagePackVersion);
    cocos2d::log(" - tileWidth: %.2f", mTileWidth);
    cocos2d::log(" - tileHeigth: %.2f", mTileHeight);
    cocos2d::log(" - levelsCount: %d", mLevelsCount);
    cocos2d::log("//--------------------------------------------------------------------");
}

//--------------------------------------------------------------------
void _GlobalInfo::initScoreValues()
//--------------------------------------------------------------------
{
    cocos2d::log("//--------------------------------------------------------------------");
    cocos2d::log("GlobalInfo::initScoreValues:");
    JsonParser->parseScoreValues();
    CC_ASSERT(JsonParser->checkScoreValuesStatus());
    JsonParser->updateScoreValues();

    cocos2d::log(" - cookieMinimalScore: %d", mCookieMinScore);
    cocos2d::log(" - cookieDefaultScore: %d", mCookieDefaultScore);
    cocos2d::log(" - fieldObjScore: %d", mFieldObjScore);
    cocos2d::log(" - enemyScore: %d", mEnemyScore);
    for (auto dudeScore : mDudeScoreMap) {
        cocos2d::log(" - dude%d: %d", dudeScore.first, dudeScore.second);
    }
    cocos2d::log("//--------------------------------------------------------------------");
}

//--------------------------------------------------------------------
void _GlobalInfo::increaseComboMultiplier()
//--------------------------------------------------------------------
{
    mComboMultiplier++;
}

//--------------------------------------------------------------------
void _GlobalInfo::resetComboMultiplier()
//--------------------------------------------------------------------
{
    mComboMultiplier = 1;
}