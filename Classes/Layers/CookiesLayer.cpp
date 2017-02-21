/**
* @file Layers/CookiesLayer.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Layers/CookiesLayer.h"

#include "Common/CommonTypes.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

#include "Managers/AnimationsManager.h"
#include "Controller/ObjectController.h"

#include "GameObjects/LevelObj.h"
#include "GameObjects/TileObjects/CookieObj.h"

USING_NS_CC;
using namespace GameResources;

//--------------------------------------------------------------------
CookiesLayer::CookiesLayer()
    : mListener(nullptr)
    , mLevel(nullptr)
    , mSelectionSprite(nullptr)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
CookiesLayer * CookiesLayer::create()
//--------------------------------------------------------------------
{
    CookiesLayer *ret = new (std::nothrow) CookiesLayer();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

//--------------------------------------------------------------------
CookiesLayer::~CookiesLayer()
//--------------------------------------------------------------------
{
    cocos2d::log("CookiesLayer::~CookiesLayer: deallocing CookiesLayer: %p - tag: %i", this, _tag);
    CC_SAFE_RELEASE_NULL(mSelectionSprite);
}

//--------------------------------------------------------------------
bool CookiesLayer::init()
//--------------------------------------------------------------------
{
    if (!Layer::init()) {
        cocos2d::log("CookiesLayer::initWithSize: can't init Scene inctance");
        return false;
    }
    clearTouchedCookie();

    mSelectionSprite = Sprite::create();
    CC_SAFE_RETAIN(mSelectionSprite);

    return true;
}

//--------------------------------------------------------------------
void CookiesLayer::onEnter()
//--------------------------------------------------------------------
{
    Layer::onEnter();
    cocos2d::log("CookiesLayer::onEnter:");

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(CookiesLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(CookiesLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(CookiesLayer::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(CookiesLayer::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    mListener = listener;
}

//--------------------------------------------------------------------
void CookiesLayer::onExit()
//--------------------------------------------------------------------
{
    Layer::onExit();
    cocos2d::log("CookiesLayer::onExit:");

    _eventDispatcher->removeEventListener(mListener);
    mListener = nullptr;
}

//--------------------------------------------------------------------
void CookiesLayer::addSpritesForCookies(Set* cookies)
//--------------------------------------------------------------------
{
    cocos2d::log("CookiesLayer::addSpritesForCookies:");
    auto it = cookies->begin();
    for (it; it != cookies->end(); it++) {
        auto cookie = dynamic_cast<CookieObj*>(*it);
        CC_ASSERT(cookie);

        createSpriteWithCookie(cookie, cookie->getColumn(), cookie->getRow());

        auto sprite = cookie->getSpriteNode();
        AnimationsManager->animateNewCookieSprite(sprite);
    }
}

//--------------------------------------------------------------------
bool CookiesLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());

    if (Helper::convertPointToTilePos(locationInNode, mSwipeFromColumn, mSwipeFromRow)) {
        auto objCtrl = mLevel->getObjectController();
        CookieObj* cookie = objCtrl->cookieAt(mSwipeFromColumn, mSwipeFromRow);
        if (cookie) {
            showSelectionIndicatorForCookie(cookie);
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------
void CookiesLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    if (!isCookieTouched())
        return;

    Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());

    int column = -1, row = -1;
    if (Helper::convertPointToTilePos(locationInNode, column, row)) {

        int horzDelta = 0, vertDelta = 0;
        updateSwipeDelta(column, row, horzDelta, vertDelta);

        if (horzDelta != 0 || vertDelta != 0) {
            if (!mTrySwapCookieCallback) {
                return;
            }

            if (mTrySwapCookieCallback(horzDelta, vertDelta)) {
                hideSelectionIndicator();
                clearTouchedCookie();
            }
        }
    }
}

//--------------------------------------------------------------------
void CookiesLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    if (mSelectionSprite->getParent() != nullptr && isCookieTouched()) {
        hideSelectionIndicator();
    }
    if (!isCookieTouched())
        return;

    clearTouchedCookie();
}

//--------------------------------------------------------------------
void CookiesLayer::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    cocos2d::log("GameplayScene::onTouchCancelled:");
    onTouchEnded(touch, event);
}

//--------------------------------------------------------------------
void CookiesLayer::userInteractionEnabled()
//--------------------------------------------------------------------
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
}

//--------------------------------------------------------------------
void CookiesLayer::userInteractionDisabled()
//--------------------------------------------------------------------
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
}

//--------------------------------------------------------------------
void CookiesLayer::showSelectionIndicatorForCookie(CookieObj* cookie)
//--------------------------------------------------------------------
{
    if (mSelectionSprite->getParent() != nullptr) {
        mSelectionSprite->removeFromParent();
    }

    auto img = new Image();
    img->initWithImageFile(cookie->highlightedSpriteName().getCString());

    auto texture = new Texture2D();
    texture->initWithImage(img);
    mSelectionSprite->initWithTexture(texture);

    auto size = cookie->getSpriteNode()->getContentSize();
    mSelectionSprite->setPosition(Vec2(size.width / 2, size.height / 2));
    mSelectionSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mSelectionSprite->setContentSize(size);

    cookie->getSpriteNode()->addChild(mSelectionSprite);
}

//--------------------------------------------------------------------
void CookiesLayer::hideSelectionIndicator()
//--------------------------------------------------------------------
{
    auto sprite = mSelectionSprite;
    auto callbackFunc = CallFunc::create([sprite]() {
        if (sprite->getParent() != nullptr) {
            sprite->removeFromParent();
        }
    });
    const float duration = 0.3f;
    auto seq = Sequence::create(FadeOut::create(duration), DelayTime::create(0.5f), callbackFunc, nullptr);
    //TODO: fix fade animation
    mSelectionSprite->runAction(seq);
}

//--------------------------------------------------------------------
void CookiesLayer::removeAllCookieSprites()
//--------------------------------------------------------------------
{
    this->removeAllChildren();
}

//--------------------------------------------------------------------
bool CookiesLayer::isCookieTouched()
//--------------------------------------------------------------------
{
    if (mSwipeFromColumn == -1 || mSwipeFromRow == -1)
        return false;
    return true;
}

//--------------------------------------------------------------------
void CookiesLayer::clearTouchedCookie()
//--------------------------------------------------------------------
{
    mSwipeFromColumn = -1;
    mSwipeFromRow = -1;
}

//--------------------------------------------------------------------
void CookiesLayer::updateSwipeDelta(int column, int row, int& horzDelta, int& vertDelta)
//--------------------------------------------------------------------
{
    if (column < mSwipeFromColumn) { // swipe left
        horzDelta = -1;
    }
    else if (column > mSwipeFromColumn) { // swipe right
        horzDelta = 1;
    }
    else if (row < mSwipeFromRow) { // swipe down
        vertDelta = -1;
    }
    else if (row > mSwipeFromRow) { // swipe up
        vertDelta = 1;
    }
}

//--------------------------------------------------------------------
void CookiesLayer::createSpriteWithCookie(CookieObj * cookie, int column, int row)
//--------------------------------------------------------------------
{
    auto sprite = Sprite::create(cookie->spriteName().getCString());
    sprite->setPosition(Helper::pointForColumnAndRow(column, row));
    cookie->setSpriteNode(sprite);
    cookie->updateDebugTileLabel();

    this->addChild(sprite);
    this->addChild(cookie);
}