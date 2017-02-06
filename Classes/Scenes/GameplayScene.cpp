/**
* @file Scenes/GameplayScene.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Scenes/GameplayScene.h"

#include "GameObjects/LevelObj.h"
#include "GameObjects/CookieObj.h"
#include "GameObjects/SwapObj.h"
#include "GameObjects/ChainObj.h"

#include "Utils/Helpers/VisibleRect.h"
#include "Utils/Helpers/Helper.h"
#include "Utils/GameResources.h"

#include "Managers/AudioManager.h"
#include "Managers/AnimationsManager.h"
#include "Managers/GuiManager.h"
#include <math.h>

USING_NS_CC;
using namespace GameResources;

//--------------------------------------------------------------------
GameplayScene::GameplayScene()
    : mLevel(nullptr)
    , mGameLayer(nullptr)
    , mCookiesLayer(nullptr)
    , mListener(nullptr)
    , mSelectionSprite(nullptr)
   //--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
GameplayScene * GameplayScene::createWithSize(const cocos2d::Size& size)
//--------------------------------------------------------------------
{
    GameplayScene *ret = new (std::nothrow) GameplayScene();
    if (ret && ret->initWithSize(size))
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
GameplayScene::~GameplayScene()
//--------------------------------------------------------------------
{
    cocos2d::log("GameplayScene::~GameplayScene: deallocing CookieObj: %p - tag: %i", this, _tag);
    CC_SAFE_RELEASE_NULL(mSelectionSprite);
}

//--------------------------------------------------------------------
bool GameplayScene::initWithSize(const Size& size)
//--------------------------------------------------------------------
{
    if (!Scene::initWithSize(size)) {
        cocos2d::log("GameplayScene::initWithSize: can't init Scene inctance");
        return false;
    }
    clearTouchedCookie();

    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setPosition(VisibleRect::leftBottom());

    auto bg = Sprite::create(GameResources::s_backgroundImg);
    auto scaleFactor = std::min(bg->getContentSize().width / size.width, bg->getContentSize().height / size.height);
    bg->setScale(1.0f / scaleFactor);
    bg->setPosition(VisibleRect::center());
    this->addChild(bg, 0);

    mGameLayer = Layer::create();
    mGameLayer->setPosition(VisibleRect::center());
    this->addChild(mGameLayer);

    Vec2 layerPos = Vec2(-TileWidth * NumColumns / 2, -TileHeight * NumRows / 2);

    mTilesLayer = Layer::create();
    mTilesLayer->setPosition(layerPos);
    mGameLayer->addChild(mTilesLayer);

    mCookiesLayer = Layer::create();
    mCookiesLayer->setPosition(layerPos);
    mGameLayer->addChild(mCookiesLayer);

    mSelectionSprite = new Sprite();
    mSelectionSprite->retain();

    AudioManager::getInstance()->playBGMusic();

    return true;
}

//--------------------------------------------------------------------
void GameplayScene::onEnter()
//--------------------------------------------------------------------
{
	Scene::onEnter();
	cocos2d::log("GameplayScene::onEnter:");

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(GameplayScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameplayScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameplayScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameplayScene::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, mCookiesLayer);
	mListener = listener;
}

//--------------------------------------------------------------------
void GameplayScene::onExit()
//--------------------------------------------------------------------
{
	Scene::onExit();
	cocos2d::log("GameplayScene::onExit:");

	_eventDispatcher->removeEventListener(mListener);
	mListener = nullptr;
}

//--------------------------------------------------------------------
void GameplayScene::addTiles()
//--------------------------------------------------------------------
{
	cocos2d::log("GameplayScene::addTiles:");
	for (int row = 0; row < CommonTypes::NumRows; row++) {
		for (int column = 0; column < CommonTypes::NumColumns; column++) {
            TileObj* tile = mLevel->tileAt(column, row);
			if (!tile) {
				continue;
			}
			auto tileSprite = Sprite::create(GameResources::s_TileImg);
            tileSprite->setPosition(Helper::pointForColumnAndRow(column, row));
            tileSprite->setOpacity(127);
			mTilesLayer->addChild(tileSprite);
		}
	}
}

//--------------------------------------------------------------------
void GameplayScene::addSpritesForCookies(Set* cookies)
//--------------------------------------------------------------------
{
	cocos2d::log("GameplayScene::addSpritesForCookies:");
	auto it = cookies->begin();
	for (it; it != cookies->end(); it++) {
		auto cookie = dynamic_cast<CookieObj*>(*it);
        CC_ASSERT(cookie);
        createSpriteWithCookie(cookie, cookie->getColumn(), cookie->getRow());
	}
}

//--------------------------------------------------------------------
bool GameplayScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    Vec2 locationInNode = mCookiesLayer->convertToNodeSpace(touch->getLocation());

    if (Helper::convertPointToTilePos(locationInNode, mSwipeFromColumn, mSwipeFromRow)) {
        CookieObj* cookie = mLevel->cookieAt(mSwipeFromColumn, mSwipeFromRow);
        if (cookie) {
            showSelectionIndicatorForCookie(cookie);
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------
void GameplayScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    if (!isCookieTouched())
        return;

    Vec2 locationInNode = mCookiesLayer->convertToNodeSpace(touch->getLocation());

    int column = -1, row = -1;
    if (Helper::convertPointToTilePos(locationInNode, column, row)) {

        int horzDelta = 0, vertDelta = 0;
        updateSwipeDelta(column, row, horzDelta, vertDelta);

        if (horzDelta != 0 || vertDelta != 0) {
            if (trySwapCookieTo(horzDelta, vertDelta)) {
                hideSelectionIndicator();
                clearTouchedCookie();
            }
        }
    }
}

//--------------------------------------------------------------------
void GameplayScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
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
void GameplayScene::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    cocos2d::log("GameplayScene::onTouchCancelled:");
    onTouchEnded(touch, event);
}

//--------------------------------------------------------------------
void GameplayScene::userInteractionEnabled()
//--------------------------------------------------------------------
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(mCookiesLayer);
}

//--------------------------------------------------------------------
void GameplayScene::userInteractionDisabled()
//--------------------------------------------------------------------
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(mCookiesLayer);
}

//--------------------------------------------------------------------
void GameplayScene::showSelectionIndicatorForCookie(CookieObj* cookie)
//--------------------------------------------------------------------
{
    if (mSelectionSprite->getParent() != nullptr) {
        mSelectionSprite->removeFromParent();
    }

    auto img = new Image();
    img->initWithImageFile(cookie->highlightedSpriteName());

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
void GameplayScene::hideSelectionIndicator()
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
bool GameplayScene::isCookieTouched()
//--------------------------------------------------------------------
{
    if (mSwipeFromColumn == -1 || mSwipeFromRow == -1) 
        return false;
    return true;
}

//--------------------------------------------------------------------
void GameplayScene::clearTouchedCookie()
//--------------------------------------------------------------------
{
    mSwipeFromColumn = -1;
    mSwipeFromRow = -1;
}

//--------------------------------------------------------------------
void GameplayScene::updateSwipeDelta(int column, int row, int& horzDelta, int& vertDelta)
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
bool GameplayScene::trySwapCookieTo(int horzDelta, int vertDelta)
//--------------------------------------------------------------------
{
    cocos2d::log("GameplayScene::trySwapCookieTo: horzDelta=%d; vertDelta=%d;", horzDelta, vertDelta);
    int toColumn = mSwipeFromColumn + horzDelta;
    int toRow = mSwipeFromRow + vertDelta;

    if (toColumn < 0 || toColumn >= NumColumns) 
        return false;
    if (toRow < 0 || toRow >= NumRows) 
        return false;

    CookieObj* toCookie = mLevel->cookieAt(toColumn, toRow);
    if (!toCookie)
        return false;

    CookieObj* fromCookie = mLevel->cookieAt(mSwipeFromColumn, mSwipeFromRow);
   
    cocos2d::log("GameplayScene::trySwapCookieTo: fromCookie=[%d,%d]; toCookie=[%d][%d];"
        , fromCookie->getColumn(), fromCookie->getRow(), toCookie->getColumn(), toCookie->getRow());

    if (!mSwapCallback)
        return false;
    
    SwapObj* swap = SwapObj::createWithCookies(fromCookie, toCookie);
    if (!swap)
        return false;

    mSwapCallback(swap);
    
    return true;
}

//--------------------------------------------------------------------
void GameplayScene::createSpriteWithCookie(CookieObj * cookie, int column, int row)
//--------------------------------------------------------------------
{
    auto sprite = Sprite::create(cookie->spriteName());
    sprite->setPosition(Helper::pointForColumnAndRow(column, row));
    cookie->setSpriteNode(sprite);
    cookie->setDebugLabel(Label::create());

    mCookiesLayer->addChild(sprite);
    mCookiesLayer->addChild(cookie);
}

//--------------------------------------------------------------------
LevelObj* GameplayScene::getLevel() const
//--------------------------------------------------------------------
{
    return mLevel;
}

//--------------------------------------------------------------------
void GameplayScene::setLevel(LevelObj* var) 
//--------------------------------------------------------------------
{
    if (!mLevel) {
        this->addChild(var);
    } else {
        this->removeChildByName("Level");
        this->addChild(var);
    }
   mLevel = var;
}