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
    , mSwipeFromColumn(-1)
    , mSwipeFromRow(-1)
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

	listener->onTouchBegan = [=](Touch* touch, Event* event) {
		int column, row = -1;
		Vec2 locationInNode = mCookiesLayer->convertToNodeSpace(touch->getLocation());
		
		if (convertPointToTilePos(locationInNode, column, row)) {
			CookieObj* cookie = mLevel->cookieAt(column, row);
			if (cookie) {
				cookie->getSpriteNode()->setColor(Color3B::RED);
				mSwipeFromColumn = column;
                mSwipeFromRow = row;
				return true;
			}
		}
		return false;
	};

	listener->onTouchEnded = [this](Touch* touch, Event* event) {
		CookieObj* cookie = mLevel->cookieAt(mSwipeFromColumn, mSwipeFromRow);
		if (cookie) {
			cookie->getSpriteNode()->setColor(Color3B::WHITE);
		}
        mSwipeFromColumn = -1;
        mSwipeFromRow = -1;
	};

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
        cookie->updateTilePosLabel();
        mCookiesLayer->addChild(cookie);
	}
}

//--------------------------------------------------------------------
Vec2 GameplayScene::pointForColumnAndRow(int column, int row)
//--------------------------------------------------------------------
{
   return Vec2(column * TileWidth + TileWidth / 2, row * TileHeight + TileHeight / 2);
}

//--------------------------------------------------------------------
bool GameplayScene::convertPointToTilePos(cocos2d::Vec2& point, int& column, int& row)
//--------------------------------------------------------------------
{
	CCLOGINFO("GameplayScene::convertPointToTilePos: point: x=%.2f y=%.2f", point.x, point.y);
	if (point.x >= 0 && point.x < NumColumns*TileWidth && point.y >= 0 && point.y < NumRows*TileHeight) {
        column = point.x / TileWidth;
        row = point.y / TileHeight;
		CCLOGINFO("GameplayScene::addSpritesForCookies: touch founed! column=%d row=%d", column, row);
		return true;
	} 
	return false;
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