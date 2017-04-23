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
#include "Controller/ObjectController/ObjContainer.h"
#include "Controller/ChainController/ChainObj.h"

#include "GameObjects/Level/LevelObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "Controller/ObjectController/Dude/DudeObj.h"

USING_NS_CC;
using namespace CT;

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
    log("CookiesLayer::~CookiesLayer: deallocing CookiesLayer: %p - tag: %i", this, _tag);
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
        log("CookiesLayer::initWithSize: can't init Scene inctance");
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
    log("CookiesLayer::onEnter:");

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
    log("CookiesLayer::onExit:");

    _eventDispatcher->removeEventListener(mListener);
    mListener = nullptr;
}

//--------------------------------------------------------------------
void CookiesLayer::addSpritesForObjects(CT::Set * set)
//--------------------------------------------------------------------
{
    log("CookiesLayer::addSpritesForDudes:");
    auto it = set->begin();
    for (it; it != set->end(); it++) {
        auto obj = dynamic_cast<BaseObj*>(*it);
        CC_ASSERT(obj);

        if (obj->getType() == BaseObjType::Field) {
            auto fieldObj = dynamic_cast<FieldObj*>(*it);
            createSpriteWithFieldObj(fieldObj, fieldObj->getCell());
        } else {
            createSpriteWithObj(obj, obj->getCell());
        }

        obj->updateZOrder();

        auto sprite = obj->getSpriteNode();
        AnimationsManager->animateNewCookieSprite(sprite);
    }
}

//--------------------------------------------------------------------
bool CookiesLayer::onTouchBegan(Touch* touch, Event* event)
//--------------------------------------------------------------------
{
    Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());

    if (Helper::convertPointToTilePos(locationInNode, mSwipeFromCell)) {
        auto objCtrl = mLevel->getObjectController();
        CookieObj* cookie = objCtrl->cookieAt(mSwipeFromCell);
        if (cookie) {
            if (cookie->isSwappable()) {
                showSelectionIndicatorForCookie(cookie);
                mTouchedObj = cookie;
                return true;
            }            
        }
        BaseObj* dudeObj = objCtrl->dudeAt(mSwipeFromCell);
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
void CookiesLayer::onTouchMoved(Touch* touch, Event* event)
//--------------------------------------------------------------------
{
    if (!isObjTouched())
        return;

    Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());

    auto cell = Cell();;
    if (Helper::convertPointToTilePos(locationInNode, cell)) {

        auto direction = getSwipeDirection(cell);
        if (isSameDirection(direction)) {
            return;
        }
        if (updateChainPreviewHint(cell, direction)) {
            return;
        }
        if (direction != Helper::to_underlying(Direction::Unknown)) {
            if (mTrySwapCookieCallback) {
                if (mTrySwapCookieCallback(mSwipeFromCell, direction)) {
                    hideSelectionIndicator();
                    clearTouchedObj();
                }
            }
        }
    }
}

//--------------------------------------------------------------------
void CookiesLayer::onTouchEnded(Touch* touch, Event* event)
//--------------------------------------------------------------------
{
    if (mTouchedObj && mCanActivateDudeCallback) {
        if (mTouchedObj->getType() == BaseObjType::Dude) {
            Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());

            auto cell = Cell();
            if (Helper::convertPointToTilePos(locationInNode, cell)) {

                removeChainPreviewSprites();

                auto direction = getSwipeDirection(cell);
                if (direction != Helper::to_underlying(Direction::Unknown)) {
                    if (mCanActivateDudeCallback(mSwipeFromCell, direction)) {
                        hideSelectionIndicator();
                        clearTouchedObj();
                        return;
                    }
                } else {
                    auto objCtrl = mLevel->getObjectController();
                    objCtrl->detectDirectionsForDudes();
                    if (isObjTouched()) {
                        AnimationsManager->animateBouncingObj(mTouchedObj);
                    }
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
void CookiesLayer::onTouchCancelled(Touch* touch, Event* event)
//--------------------------------------------------------------------
{
    log("GameplayScene::onTouchCancelled:");
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
    auto childs = getChildren();
    if (childs.size() == 0) {
        return;
    }
    for (const auto& child : childs) {
        auto obj = dynamic_cast<BaseObj*>(child);
        if (!obj) {
            continue;
        }
        if (obj->getType() != BaseObjType::Dude 
            && obj->getType() != BaseObjType::Field
            && obj->getType() != BaseObjType::Enemy) {
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
    if (mSwipeFromCell.column == -1 || mSwipeFromCell.row == -1 || !mTouchedObj)
        return false;
    return true;
}

//--------------------------------------------------------------------
void CookiesLayer::clearTouchedObj()
//--------------------------------------------------------------------
{
    mSwipeFromCell.column = -1;
    mSwipeFromCell.row = -1;
    mTouchedObj = nullptr;
}

//--------------------------------------------------------------------
int CookiesLayer::getSwipeDirection(CT::Cell& cell)
//--------------------------------------------------------------------
{
    auto direction = Direction::Unknown;
    if (cell.column < mSwipeFromCell.column) { // swipe left
        direction = Direction::Left;
    }
    else if (cell.column > mSwipeFromCell.column) { // swipe right
        direction = Direction::Right;
    }
    else if (cell.row < mSwipeFromCell.row) { // swipe up
        direction = Direction::Up;
    }
    else if (cell.row > mSwipeFromCell.row) { // swipe down
        direction = Direction::Down;
    }
    return Helper::to_underlying(direction);
}

//--------------------------------------------------------------------
bool CookiesLayer::isSameDirection(int direction)
//--------------------------------------------------------------------
{
    if (direction == Helper::to_underlying(mPreviousDirection)) {
        return true;
    }
    mPreviousDirection = static_cast<CT::Direction>(direction);
    return false;
}

//--------------------------------------------------------------------
bool CookiesLayer::updateChainPreviewHint(CT::Cell& cell, int direction)
//--------------------------------------------------------------------
{
    bool isDudeObject = mTouchedObj->getType() == BaseObjType::Dude;
    if (isDudeObject) {

        auto objCtrl = mLevel->getObjectController();
        removeChainPreviewSprites();
        objCtrl->detectDirectionsForDudes();

        if (direction != Helper::to_underlying(Direction::Unknown)) {
            auto set = mUpdateDirectionCallback(mTouchedObj, direction);
            if (set) {
                createChainPreviewSprites(set);
                CC_SAFE_RELEASE_NULL(set);
            }
        }
    }   
    return isDudeObject;
}

//--------------------------------------------------------------------
void CookiesLayer::createSpriteWithObj(BaseObj* obj, CT::Cell& cell)
//--------------------------------------------------------------------
{
    if (obj) {
        auto sprite = SpritesFactory->createWithBaseObject(obj);
        sprite->setVisible(true);
        sprite->setPosition(Helper::pointForCell(cell));
        obj->setSpriteNode(sprite);

        auto zOrder = (cell.row * 10);
        if (obj->getType() == BaseObjType::Dude) {
            this->addChild(sprite, zOrder);
            mDudesLayer->addChild(obj, zOrder);
        } else {
            this->addChild(sprite, zOrder);
            if (!obj->getParent()) {
                this->addChild(obj, zOrder);
            }
        }
        if (obj->getType() != BaseObjType::Tile) {
            obj->updateZOrder();
        }
    }    
}

//--------------------------------------------------------------------
void CookiesLayer::createSpriteWithFieldObj(FieldObj* obj, CT::Cell& cell)
//--------------------------------------------------------------------
{
    if (obj) {
        auto sprite = SpritesFactory->createWithBaseObject(obj);
        sprite->setVisible(true);
        obj->setSpriteNode(sprite);

        auto priority = obj->getPriority();
        auto zOrder = (cell.row * 10) + priority;
        auto pos = Helper::pointForCellWithPriority(cell, priority);

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
void CookiesLayer::createChainPreviewSprites(CT::Set* set)
//--------------------------------------------------------------------
{
    log("CookiesLayer::addSpritesForDudes:");
    for (auto itChain = set->begin(); itChain != set->end(); itChain++) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        CC_ASSERT(chain);

        DudeObj* dude = nullptr;

        auto objects = chain->getObjects();
        for (auto itObj = objects->begin(); itObj != objects->end(); itObj++) {
            auto container = dynamic_cast<ObjContainer*>(*itObj);
            CC_ASSERT(container);

            auto obj = container->getObjectForChain();
            CC_ASSERT(obj);

            if (!dude && obj->getType() == BaseObjType::Dude) {
                dude = dynamic_cast<DudeObj*>(obj);
            }
            if (container->isContainChainPreviewSprite()) {
                continue;
            }

            auto sprite = SpritesFactory->createHintSprite(chain->getChainColor());
            container->setChainPreviewSprite(sprite);

            sprite->setPosition(Helper::pointForCell(obj->getCell()));

            auto gameLayer = this->getParent();
            auto scene = dynamic_cast<GameplayScene*>(gameLayer->getParent());
            CC_ASSERT(scene);

            scene->getChainPreviewLayer()->addChild(sprite);
            
        }
    }
}
