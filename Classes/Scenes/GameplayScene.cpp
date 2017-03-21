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

#include "Common/Factory/SpritesFactory.h"

#include "GameObjects/Level/LevelObj.h"
#include "GameObjects/TileObjects/TileObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"

#include "Utils/Helpers/VisibleRect.h"
#include "Utils/Helpers/Helper.h"
#include "Utils/GameResources.h"

#include "Layers/CookiesLayer.h"

#include "Managers/AudioManager.h"
#include "Controller/ObjectController/ObjectController.h"

#include "cocos2d/cocos/ui/UIScale9Sprite.h"

USING_NS_CC;
using namespace GameResources;

//--------------------------------------------------------------------
GameplayScene::GameplayScene()
    : mLevel(nullptr)
    , mCookiesLayer(nullptr)
    , mGameLayer(nullptr)
    , mTilesLayer(nullptr)
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
}

//--------------------------------------------------------------------
bool GameplayScene::initWithSize(const Size& size)
//--------------------------------------------------------------------
{
    if (!Scene::initWithSize(size)) {
        cocos2d::log("GameplayScene::initWithSize: can't init Scene inctance");
        return false;
    }

    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setPosition(VisibleRect::leftBottom());

    auto bgLayer = LayerColor::create(Color4B::WHITE);
    bgLayer->setPosition(VisibleRect::leftBottom());
    this->addChild(bgLayer);

    auto bg = ui::Scale9Sprite::create(GameResources::s_backgroundImg.getCString());
    auto scaleFactor = std::min(bg->getContentSize().width / size.width
        , bg->getContentSize().height / size.height);
    bg->setScale(1.0f / scaleFactor);
    bg->setScale(bg->getScale() * 2 );
    bg->setPosition(VisibleRect::center());
    bgLayer->addChild(bg, 0);

    mGameLayer = Layer::create();
    mGameLayer->setPosition(VisibleRect::center());
    this->addChild(mGameLayer);

    auto offset = -2.5f * CommonTypes::NumColumns / 2;
    Vec2 layerPos = Vec2(offset - TileWidth * CommonTypes::NumColumns / 2
        , offset - TileHeight * CommonTypes::NumRows / 2);

    mTilesLayer = Layer::create();
    mTilesLayer->setPosition(layerPos);
    mGameLayer->addChild(mTilesLayer);

    mCookiesLayer = CookiesLayer::create();
    mCookiesLayer->setPosition(layerPos);
    mGameLayer->addChild(mCookiesLayer);

    mFieldObjectsLayer = Layer::create();
    mFieldObjectsLayer->setPosition(layerPos);
    mGameLayer->addChild(mFieldObjectsLayer);

    AudioManager->playBGMusic();

    return true;
}

//--------------------------------------------------------------------
void GameplayScene::addTiles()
//--------------------------------------------------------------------
{
	cocos2d::log("GameplayScene::addTiles:");
	for (int row = 0; row < CommonTypes::NumRows; row++) {
		for (int column = 0; column < CommonTypes::NumColumns; column++) {
            auto objCtrl = mLevel->getObjectController();
			if (objCtrl->isEmptyTileAt(column, row)) {
				continue;
			}
            auto tile = objCtrl->tileAt(column, row);
            auto tileSprite = SpritesFactory->createWithBaseObject(tile);
            tileSprite->setVisible(true);
            tileSprite->setPosition(Helper::pointForColumnAndRow(column, row));
            tileSprite->setOpacity(127);
            tile->setSpriteNode(tileSprite);
			mTilesLayer->addChild(tileSprite);

		}
	}
}

//--------------------------------------------------------------------
void GameplayScene::addFieldObjectsAt(int column, int row)
//--------------------------------------------------------------------
{
    // Create Field objects
    auto objCtrl = mLevel->getObjectController();
    auto fieldObjects = objCtrl->fieldObjectsAt(column, row);
    if (fieldObjects.size() == 0) {
        return;
    }
    for (auto it = fieldObjects.begin(); it != fieldObjects.end(); ++it) {
        auto fieldObj = dynamic_cast<FieldObj*>(*it);    
        createSpriteWithFieldObj(fieldObj);
        fieldObj->updateDebugLabel();
    }
}

//--------------------------------------------------------------------
void GameplayScene::addSpritesForObjects(CommonTypes::Set* set)
//--------------------------------------------------------------------
{
    cocos2d::log("GameplayScene::addSpritesForObjects:");
    mCookiesLayer->addSpritesForObjects(set);
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
void GameplayScene::setSwapCookieCallback(std::function<bool(int fromCol, int fromRow, int direction)> func)
//--------------------------------------------------------------------
{
    mCookiesLayer->setTrySwapCookieCallback(func);
}

//--------------------------------------------------------------------
void GameplayScene::setDudeActivationCallback(std::function<bool(int fromCol, int fromRow, int direction)> func)
//--------------------------------------------------------------------
{
    mCookiesLayer->setCanActivateDudeCallback(func);
}

//--------------------------------------------------------------------
void GameplayScene::removeAllCookieSprites()
//--------------------------------------------------------------------
{
    mCookiesLayer->removeAllCookieSprites();
}

//--------------------------------------------------------------------
void GameplayScene::createSpriteWithCookie(CookieObj * cookie, int column, int row)
//--------------------------------------------------------------------
{
    mCookiesLayer->createSpriteWithObj(cookie, column, row);
}

//--------------------------------------------------------------------
void GameplayScene::createSpriteWithDude(BaseObj * dudeObj)
//--------------------------------------------------------------------
{
    mCookiesLayer->createSpriteWithObj(dudeObj, dudeObj->getColumn(), dudeObj->getRow());
}

//--------------------------------------------------------------------
void GameplayScene::createSpriteWithFieldObj(FieldObj * obj)
//--------------------------------------------------------------------
{
    mCookiesLayer->createSpriteWithFieldObj(obj, obj->getColumn(), obj->getRow());
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
    mCookiesLayer->setLevel(var);
    mLevel = var;
}