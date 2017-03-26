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

#include "Utils/GameResources.h"
#include "Utils/Helpers/Helper.h"

#include "Common/Factory/SpritesFactory.h"
#include "Scenes/GameplayScene.h"

#include "Managers/AnimationsManager.h"
#include "Controller/ObjectController/ObjectController.h"

#include "GameObjects/Level/LevelObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "Controller/ObjectController/Dude/DudeObj.h"

USING_NS_CC;
using namespace GameResources;
using namespace CommonTypes;

//--------------------------------------------------------------------
CookiesLayer::CookiesLayer()
    : mListener(nullptr)
    , mLevel(nullptr)
    , mTouchedObj(nullptr)
    , mSelectionSprite(nullptr)
    , mDudesLayer(nullptr)
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
    clearTouchedObj();
    mListener = nullptr;
    mLevel = nullptr;
    mDudesLayer = nullptr;
}

//--------------------------------------------------------------------
bool CookiesLayer::init()
//--------------------------------------------------------------------
{
    if (!Layer::init()) {
        cocos2d::log("CookiesLayer::initWithSize: can't init Scene inctance");
        return false;
    }
    clearTouchedObj();

    mSelectionSprite = Sprite::create();
    CC_SAFE_RETAIN(mSelectionSprite);

    mDudesLayer = Layer::create();
    this->addChild(mDudesLayer);
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
void CookiesLayer::addSpritesForObjects(CommonTypes::Set * set)
//--------------------------------------------------------------------
{
    cocos2d::log("CookiesLayer::addSpritesForDudes:");
    auto it = set->begin();
    for (it; it != set->end(); it++) {
        auto obj = dynamic_cast<BaseObj*>(*it);
        CC_ASSERT(obj);

        if (obj->getType() == BaseObjType::Field) {
            auto fieldObj = dynamic_cast<FieldObj*>(*it);
            createSpriteWithFieldObj(fieldObj, fieldObj->getColumn(), fieldObj->getRow());
        } else {
            createSpriteWithObj(obj, obj->getColumn(), obj->getRow());
        }

        obj->updateDebugLabel();

        auto sprite = obj->getSpriteNode();
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
            if (cookie->isSwappable()) {
                showSelectionIndicatorForCookie(cookie);
                mTouchedObj = cookie;
                return true;
            }            
        }
        BaseObj* dudeObj = objCtrl->dudeObjectAt(mSwipeFromColumn, mSwipeFromRow);
        if (dudeObj) {
            if (dudeObj->isSwappable()) {
                mTouchedObj = dudeObj;
                return true;
            }
        }
    }
    return false;
}

//--------------------------------------------------------------------
void CookiesLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    if (!isObjTouched())
        return;

    Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());

    int column = -1, row = -1;
    if (Helper::convertPointToTilePos(locationInNode, column, row)) {

        auto direction = getSwipeDirection(column, row);

        if (mTouchedObj->getType() == BaseObjType::Dude) {
            auto dir = static_cast<CommonTypes::Direction>(direction);
            if (dir != mHintPreviewDirection) {

                removeChainPreviewSprites();

                auto set = mUpdateDirectionCallback(mTouchedObj, direction);
                if (set) {
                    createChainPreviewSprites(set);
                }
                mHintPreviewDirection = dir;
            }
            return;
        }

        if (direction != Helper::to_underlying(Direction::Unknown)) {
            if (!mTrySwapCookieCallback) {
                return;
            }

            if (mTrySwapCookieCallback(mSwipeFromColumn, mSwipeFromRow, direction)) {
                hideSelectionIndicator();
                clearTouchedObj();
            }
        }
    }
}

//--------------------------------------------------------------------
void CookiesLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    if (mTouchedObj && mCanActivateDudeCallback) {
        if (mTouchedObj->getType() == BaseObjType::Dude) {
            Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());

            int column = -1, row = -1;
            if (Helper::convertPointToTilePos(locationInNode, column, row)) {

                removeChainPreviewSprites();

                auto direction = getSwipeDirection(column, row);
                if (mCanActivateDudeCallback(mSwipeFromColumn, mSwipeFromRow, direction)) {
                    hideSelectionIndicator();
                    clearTouchedObj();
                    return;
                }
            }
        }
    }
    if (mSelectionSprite->getParent() != nullptr && isObjTouched()) {
        hideSelectionIndicator();
    }
    if (!isObjTouched())
        return;

    clearTouchedObj();
}

//--------------------------------------------------------------------
void CookiesLayer::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    cocos2d::log("GameplayScene::onTouchCancelled:");
    onTouchEnded(touch, event);
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
    if (!mSelectionSprite) {
        return;
    }
    auto sprite = mSelectionSprite;
    auto callbackFunc = CallFunc::create([sprite]() {
        if (sprite->getParent() != nullptr) {
            sprite->removeFromParent();
        }
    });
    const float duration = 0.3f;
    auto seq = Sequence::create(FadeOut::create(duration), DelayTime::create(0.5f), callbackFunc, nullptr);
    mSelectionSprite->runAction(seq);
}

//--------------------------------------------------------------------
void CookiesLayer::removeAllCookieSprites()
//--------------------------------------------------------------------
{
    for (const auto& child : getChildren()) {
        auto obj = dynamic_cast<BaseObj*>(child);
        if (!obj) {
            continue;
        }
        if (obj->getType() != BaseObjType::Dude && obj->getType() != BaseObjType::Field) {
            this->removeChild(child);
        }
    }
}

//--------------------------------------------------------------------
void CookiesLayer::removeChainPreviewSprites()
//--------------------------------------------------------------------
{
    auto gameLayer = this->getParent();
    auto scene = dynamic_cast<GameplayScene*>(gameLayer->getParent());
    CC_ASSERT(scene);

    scene->removeAllChainPreviewSprites();
}

//--------------------------------------------------------------------
bool CookiesLayer::isObjTouched()
//--------------------------------------------------------------------
{
    if (mSwipeFromColumn == -1 || mSwipeFromRow == -1 || !mTouchedObj)
        return false;
    return true;
}

//--------------------------------------------------------------------
void CookiesLayer::clearTouchedObj()
//--------------------------------------------------------------------
{
    mSwipeFromColumn = -1;
    mSwipeFromRow = -1;
    mTouchedObj = nullptr;
}

//--------------------------------------------------------------------
int CookiesLayer::getSwipeDirection(int column, int row)
//--------------------------------------------------------------------
{
    auto direction = Direction::Unknown;
    if (column < mSwipeFromColumn) { // swipe left
        direction = Direction::Left;
    }
    else if (column > mSwipeFromColumn) { // swipe right
        direction = Direction::Right;
    }
    else if (row < mSwipeFromRow) { // swipe up
        direction = Direction::Up;
    }
    else if (row > mSwipeFromRow) { // swipe down
        direction = Direction::Down;
    }
    return Helper::to_underlying(direction);
}

//--------------------------------------------------------------------
void CookiesLayer::createSpriteWithObj(BaseObj* obj, int column, int row)
//--------------------------------------------------------------------
{
    if (obj) {
        auto sprite = SpritesFactory->createWithBaseObject(obj);
        sprite->setVisible(true);
        sprite->setPosition(Helper::pointForColumnAndRow(column, row));
        obj->setSpriteNode(sprite);

        auto zOrder = (row * 10);
        if (obj->getType() == BaseObjType::Dude) {
            mDudesLayer->addChild(sprite, zOrder);
            mDudesLayer->addChild(obj, zOrder);
        } else {
            this->addChild(sprite, zOrder);
            if (!obj->getParent()) {
                this->addChild(obj, zOrder);
            }
        }
        if (obj->getType() != BaseObjType::Tile) {
            obj->updateDebugLabel();
        }
    }    
}

//--------------------------------------------------------------------
void CookiesLayer::createSpriteWithFieldObj(FieldObj* obj, int column, int row)
//--------------------------------------------------------------------
{
    if (obj) {
        auto sprite = SpritesFactory->createWithBaseObject(obj);
        sprite->setVisible(true);
        obj->setSpriteNode(sprite);

        auto priority = obj->getPriority();
        auto zOrder = (row * 10) + priority;
        auto pos = Helper::pointForColumnAndRowWithPriority(column, row, priority);

        sprite->setPosition(pos);
        sprite->setScale(1);

        auto gameLayer = this->getParent();
        auto scene = dynamic_cast<GameplayScene*>(gameLayer->getParent());
        CC_ASSERT(scene);

        if (obj->getPriority() <= -1) {
            scene->getTilesLayer()->addChild(sprite, zOrder);
        } else {
            scene->getFieldObjectsLayer()->addChild(sprite, zOrder);
        }
        
        if (!obj->getParent()) {
            this->addChild(obj, zOrder);
        }       
    }
}

//--------------------------------------------------------------------
void CookiesLayer::createChainPreviewSprites(CommonTypes::Set* set)
//--------------------------------------------------------------------
{
    cocos2d::log("CookiesLayer::addSpritesForDudes:");
    auto it = set->begin();
    for (it; it != set->end(); it++) {
        auto obj = dynamic_cast<BaseObj*>(*it);
        CC_ASSERT(obj);

        auto sprite = SpritesFactory->createHintSprite();
        sprite->setVisible(true);
        sprite->setPosition(Helper::pointForColumnAndRow(obj->getColumn(), obj->getRow()));

        auto gameLayer = this->getParent();
        auto scene = dynamic_cast<GameplayScene*>(gameLayer->getParent());
        CC_ASSERT(scene);

        scene->getChainPreviewLayer()->addChild(sprite);
       
        AnimationsManager->animateNewCookieSprite(sprite);
    }
}
