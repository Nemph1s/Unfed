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

#include <algorithm>


using cocos2d::Set;
using cocos2d::Size;
using cocos2d::Vec2;
using cocos2d::Sprite;
using cocos2d::Layer;

//--------------------------------------------------------------------
GameplayScene::GameplayScene()
   : mLevel(nullptr)
   , mGameLayer(nullptr)
   , mCookiesLayer(nullptr)
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
    bg->setScale(1.0f/scaleFactor);
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
void GameplayScene::addTiles()
//--------------------------------------------------------------------
{
	CCLOGINFO("GameplayScene::addTiles:");
	for (int8_t row = 0; row < CommonTypes::NumRows; row++) {
		for (int8_t column = 0; column < CommonTypes::NumColumns; column++) {
			if (mLevel->tileAt(column, row) == nullptr) {
				continue;
			}
			auto tile = Sprite::create(GameResources::s_TileImg);
			tile->setPosition(pointForColumnAndRow(column, row));
			tile->setOpacity(127);
			mTilesLayer->addChild(tile);
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
   }
}

//--------------------------------------------------------------------
Vec2 GameplayScene::pointForColumnAndRow(int8_t column, int8_t row)
//--------------------------------------------------------------------
{
   return Vec2(column * TileWidth + TileWidth / 2, row * TileHeight + TileHeight / 2);
}

