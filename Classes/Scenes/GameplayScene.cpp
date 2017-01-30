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

#include "Utils/Helpers/VisibleRect.h"
#include "Utils/Helpers/Helper.h"
#include "Utils/GameResources.h"

USING_NS_CC;

//--------------------------------------------------------------------
GameplayScene::GameplayScene()
    : mLevel(nullptr)
    , mGameLayer(nullptr)
    , mCookiesLayer(nullptr)
    , mListener(nullptr)
   //--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
GameplayScene * GameplayScene::createWithSize(const cocos2d::Size & size)
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
    CCLOGINFO("GameplayScene::~GameplayScene: deallocing CookieObj: %p - tag: %i", this, _tag);
}

//--------------------------------------------------------------------
bool GameplayScene::initWithSize(const Size& size)
//--------------------------------------------------------------------
{
    if (!Scene::initWithSize(size)) {
        CCLOGERROR("GameplayScene::initWithSize: can't init Scene inctance");
        return false;
    }
    clearTouchedCookie();

    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setPosition(VisibleRect::center());

    auto bg = Sprite::create(GameResources::s_backgroundImg);
    auto scaleFactor = std::min(bg->getContentSize().width / size.width, bg->getContentSize().height / size.height);
    bg->setScale(1.0f / scaleFactor);
    this->addChild(bg, 0);

    mGameLayer = Layer::create();
    this->addChild(mGameLayer);

    Vec2 layerPos = Vec2(-TileWidth * NumColumns / 2, -TileHeight * NumRows / 2);

    mTilesLayer = Layer::create();
    mTilesLayer->setPosition(layerPos);
    mGameLayer->addChild(mTilesLayer);

    mCookiesLayer = Layer::create();
    mCookiesLayer->setPosition(layerPos);
    mGameLayer->addChild(mCookiesLayer);

    return true;
}

//--------------------------------------------------------------------
void GameplayScene::onEnter()
//--------------------------------------------------------------------
{
	Scene::onEnter();
	CCLOGINFO("GameplayScene::onEnter:");

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
	CCLOGINFO("GameplayScene::onExit:");

	_eventDispatcher->removeEventListener(mListener);
	mListener = nullptr;
}

//--------------------------------------------------------------------
void GameplayScene::addTiles()
//--------------------------------------------------------------------
{
	CCLOGINFO("GameplayScene::addTiles:");
	for (int row = 0; row < CommonTypes::NumRows; row++) {
		for (int column = 0; column < CommonTypes::NumColumns; column++) {
            TileObj* tile = mLevel->tileAt(column, row);
			if (!tile) {
				continue;
			}
			auto tileSprite = Sprite::create(GameResources::s_TileImg);
            tileSprite->setPosition(pointForColumnAndRow(column, row));
            tileSprite->setOpacity(127);
			mTilesLayer->addChild(tileSprite);
		}
	}
}

//--------------------------------------------------------------------
void GameplayScene::addSpritesForCookies(Set* cookies)
//--------------------------------------------------------------------
{
	CCLOGINFO("GameplayScene::addSpritesForCookies:");
	auto it = cookies->begin();
	for (it; it != cookies->end(); it++) {
		auto cookie = dynamic_cast<CookieObj*>(*it);
		if (!cookie) {
			CCLOGERROR("GameplayScene::addSpritesForCookies: can't cast Ref* to CookieObj*");
			CC_ASSERT(cookie);
			continue;
		}
		auto* sprite = Sprite::create(cookie->spriteName());
		sprite->setPosition(pointForColumnAndRow(cookie->getColumn(), cookie->getRow()));
		mCookiesLayer->addChild(sprite);
        cookie->setSpriteNode(sprite);

        auto label = cocos2d::Label::create("", "fonts/Arial", 16,
            cocos2d::Size(32, 32), cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
        auto size = sprite->getContentSize();
        label->setPosition(cocos2d::Vec2(size.width / 4, (size.height / 1.25f)));
        label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        sprite->addChild(label);
        cookie->setDebugLabel(label);
        
        cookie->updateDebugTileLabel();
        mCookiesLayer->addChild(cookie);
	}
}

//--------------------------------------------------------------------
Vec2 GameplayScene::pointForColumnAndRow(int column, int row)
//--------------------------------------------------------------------
{
   return Vec2(column * TileWidth + TileWidth / 2, (NumRows - row - 1) * TileHeight + TileHeight / 2);
}

//--------------------------------------------------------------------
bool GameplayScene::convertPointToTilePos(cocos2d::Vec2& point, int& column, int& row)
//--------------------------------------------------------------------
{
	CCLOGINFO("GameplayScene::convertPointToTilePos: point: x=%.2f y=%.2f", point.x, point.y);
	if (point.x >= 0 && point.x < NumColumns*TileWidth && point.y >= 0 && point.y < NumRows*TileHeight) {
        column = point.x / TileWidth;
        row = NumRows - (point.y / TileHeight);
		CCLOGINFO("GameplayScene::addSpritesForCookies: touch founed! column=%d row=%d", column, row);
		return true;
	} 
	return false;
}

//--------------------------------------------------------------------
bool GameplayScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    CCLOGINFO("GameplayScene::onTouchBegan:");
    Vec2 locationInNode = mCookiesLayer->convertToNodeSpace(touch->getLocation());

    if (convertPointToTilePos(locationInNode, mSwipeFromColumn, mSwipeFromRow)) {
        CookieObj* cookie = mLevel->cookieAt(mSwipeFromColumn, mSwipeFromRow);
        if (cookie) {
//             cookie->getSpriteNode()->setVisible(false);
//             cookie->getHighLightedSpriteNode()->setVisible(true);
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
    if (convertPointToTilePos(locationInNode, column, row)) {

        int horzDelta = 0, vertDelta = 0;
        updateSwipeDelta(column, row, horzDelta, vertDelta);

        if (horzDelta != 0 || vertDelta != 0) {
            if (trySwapCookieTo(horzDelta, vertDelta)) {
                clearTouchedCookie();
            }
        }
    }
}

//--------------------------------------------------------------------
void GameplayScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    if (!isCookieTouched())
        return;

    CCLOGINFO("GameplayScene::onTouchEnded:");
    CookieObj* cookie = mLevel->cookieAt(mSwipeFromColumn, mSwipeFromRow);
    if (cookie) {
//         cookie->getSpriteNode()->setVisible(true);
//         cookie->getHighLightedSpriteNode()->setVisible(false);
    }
    clearTouchedCookie();
}

//--------------------------------------------------------------------
void GameplayScene::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event)
//--------------------------------------------------------------------
{
    CCLOGINFO("GameplayScene::onTouchCancelled:");
    onTouchEnded(touch, event);
}

void GameplayScene::userInteractionEnabled()
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(mCookiesLayer);
}

void GameplayScene::userInteractionDisabled()
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(mCookiesLayer);
}

//--------------------------------------------------------------------
void GameplayScene::animateSwap(SwapObj * swap, cocos2d::CallFunc* func)
//--------------------------------------------------------------------
{
    CC_ASSERT(swap);
    CC_ASSERT(func);
    // Put the cookie you started with on top.

    auto cookieA = swap->getCookieA()->getSpriteNode();
    auto cookieB = swap->getCookieB()->getSpriteNode();
    
    cookieA->setZOrder(100);
    cookieB->setZOrder(90);
    
    const float duration = 0.3;

    auto moveA = MoveTo::create(duration, cookieB->getPosition());
    auto easeA = EaseOut::create(moveA, duration); // maybe change rate?
    cookieA->runAction(Sequence::create(easeA, func, nullptr));

    auto moveB = MoveTo::create(duration, cookieA->getPosition());
    auto easeB = EaseOut::create(moveB, duration);
    cookieB->runAction(easeB);

    swap->getCookieA()->updateDebugTileLabel();
    swap->getCookieB()->updateDebugTileLabel();
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
    CCLOGINFO("GameplayScene::trySwapCookieTo: horzDelta=%d; vertDelta=%d;", horzDelta, vertDelta);
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
   
    CCLOGINFO("GameplayScene::trySwapCookieTo: fromCookie=[%d,%d]; toCookie=[%d][%d];"
        , fromCookie->getColumn(), fromCookie->getRow(), toCookie->getColumn(), toCookie->getRow());

    if (mSwapCallback != nullptr) {
        SwapObj* swap = SwapObj::createWithCookies(fromCookie, toCookie);
        mSwapCallback(swap);
    }
    return true;
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