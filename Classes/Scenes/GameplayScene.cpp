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

USING_NS_CC;

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

    AudioManager::getInstance()->init();

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
	cocos2d::log("GameplayScene::addSpritesForCookies:");
	auto it = cookies->begin();
	for (it; it != cookies->end(); it++) {
		auto cookie = dynamic_cast<CookieObj*>(*it);
		if (!cookie) {
			cocos2d::log("GameplayScene::addSpritesForCookies: can't cast Ref* to CookieObj*");
			CC_ASSERT(cookie);
			continue;
		}
		auto* sprite = Sprite::create(cookie->spriteName());
		sprite->setPosition(pointForColumnAndRow(cookie->getColumn(), cookie->getRow()));
		mCookiesLayer->addChild(sprite);
        cookie->setSpriteNode(sprite);

        auto label = Label::create();
        label->setBMFontSize(16);
        label->setDimensions(32, 32);
        label->setHorizontalAlignment(TextHAlignment::LEFT);
        label->setVerticalAlignment(TextVAlignment::TOP);
        auto size = sprite->getContentSize();
        label->setPosition(Vec2(size.width / 4, (size.height / 1.25f)));
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprite->addChild(label, 10);
        cookie->setDebugLabel(label);
        
        cookie->updateDebugTileLabel();
        mCookiesLayer->addChild(cookie);
	}
}

//--------------------------------------------------------------------
Vec2 GameplayScene::pointForColumnAndRow(int column, int row)
//--------------------------------------------------------------------
{
    //temporary solution to set correct pos for [col, row]
//     auto temp = column;
//     column = row;
//     row = temp;
//    return Vec2(column * TileWidth + TileWidth / 2, (NumRows - row - 1) * TileHeight + TileHeight / 2);
    return Vec2(column * TileWidth + TileWidth / 2, (NumRows - row - 1) * TileHeight + TileHeight / 2);
}

//--------------------------------------------------------------------
bool GameplayScene::convertPointToTilePos(cocos2d::Vec2& point, int& column, int& row)
//--------------------------------------------------------------------
{
	if (point.x >= 0 && point.x < NumColumns*TileWidth && point.y >= 0 && point.y < NumRows*TileHeight) {
        //temporary solution to set correct pos for [col, row]
//         row = point.x / TileWidth;
//         column = NumRows - (point.y / TileHeight);
        column = point.x / TileWidth;
        row = NumColumns - (point.y / TileHeight);
		return true;
	} 
	return false;
}

//--------------------------------------------------------------------
bool GameplayScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
//--------------------------------------------------------------------
{
    Vec2 locationInNode = mCookiesLayer->convertToNodeSpace(touch->getLocation());

    if (convertPointToTilePos(locationInNode, mSwipeFromColumn, mSwipeFromRow)) {
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
    if (convertPointToTilePos(locationInNode, column, row)) {

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
void GameplayScene::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event)
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
void GameplayScene::animateSwap(SwapObj * swap, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(swap);
    CC_ASSERT(completion);
    // Put the cookie you started with on top.

    auto cookieA = swap->getCookieA()->getSpriteNode();
    auto cookieB = swap->getCookieB()->getSpriteNode();
    
    cookieA->setZOrder(100);
    cookieB->setZOrder(90);
    
    const float duration = 0.3;

    auto moveA = MoveTo::create(duration, cookieB->getPosition());
    auto easeA = EaseOut::create(moveA, duration); // maybe change rate?
    cookieA->runAction(Sequence::create(easeA, completion, nullptr));

    auto moveB = MoveTo::create(duration, cookieA->getPosition());
    auto easeB = EaseOut::create(moveB, duration);
    cookieB->runAction(easeB);

    AudioManager::getInstance()->playSound(SoundType::SwapSound);

    swap->getCookieA()->updateDebugTileLabel();
    swap->getCookieB()->updateDebugTileLabel();
}

//--------------------------------------------------------------------
void GameplayScene::animateInvalidSwap(SwapObj * swap, cocos2d::CallFunc * completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(swap);
    CC_ASSERT(completion);
    // Put the cookie you started with on top.

    auto cookieA = swap->getCookieA()->getSpriteNode();
    auto cookieB = swap->getCookieB()->getSpriteNode();

    cookieA->setZOrder(100);
    cookieB->setZOrder(90);

    const float duration = 0.3;
    auto deltaA = cookieB->getPosition() - cookieA->getPosition();
    auto deltaB = cookieA->getPosition()- cookieB->getPosition();

    auto moveA = MoveBy::create(duration, deltaA);
    auto easeA = EaseOut::create(moveA, duration);
    auto moveB = MoveBy::create(duration, deltaB);
    auto easeB = EaseOut::create(moveB, duration);

    auto rotate = RotateBy::create(0.05f, 10);
    auto seq = Sequence::create(rotate, rotate->reverse(), rotate, rotate->reverse(), nullptr);

    cookieB->runAction(Sequence::create(easeB, easeB->reverse(), nullptr));
    cookieA->runAction(Sequence::create(easeA, easeA->reverse(), seq, completion, nullptr));

    AudioManager::getInstance()->playSound(SoundType::InvalidSwapSound);
}

//--------------------------------------------------------------------
void GameplayScene::animateMatching(cocos2d::Set* chains, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(chains);
    CC_ASSERT(completion);

    const float duration = 0.3;

    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {

        auto chain = dynamic_cast<ChainObj*>(*itChain);
        if (!chain)
            continue;
        auto cookies = chain->getCookies();
        for (auto it = cookies->begin(); it != cookies->end(); it++) {

            auto cookie = dynamic_cast<CookieObj*>(*it);
            if (cookie != nullptr) {

                const float scaleFactor = 0.1;

                auto scaleAction = ScaleTo::create(duration, scaleFactor);
                auto easeOut = EaseOut::create(scaleAction, duration);

                auto sprite = cookie->getSpriteNode();
                auto callback = CallFunc::create([sprite, cookie]() {
                    if (sprite) {
                        sprite->removeFromParent();
                        cookie->setSpriteNode(nullptr);
                    }
                });

                cookie->getSpriteNode()->runAction(Sequence::create(easeOut, callback, nullptr));
            }
        }
    }

    AudioManager::getInstance()->playSound(SoundType::MatchSound);

    this->runAction(Sequence::create(DelayTime::create(duration), completion, nullptr));
}

//--------------------------------------------------------------------
void GameplayScene::animateFallingCookies(cocos2d::Array * colums, cocos2d::CallFunc * func)
//--------------------------------------------------------------------
{
    CC_ASSERT(colums);
    CC_ASSERT(func);
    // As with the other animation methods, you should only call the completion block after all the animations are finished. 
    // Because the number of falling cookies may vary, you can’t hardcode this total duration but instead have to compute it.
    float longestDuration = 0;
    
    for (auto it = colums->begin(); it != colums->end(); it++) {

        auto array = dynamic_cast<cocos2d::Array*>(*it);
        CC_ASSERT(array);

        int index = 1;
        for (auto itArr = array->begin(); itArr != array->end(); itArr++, index++) {

            auto cookie = dynamic_cast<CookieObj*>(*itArr);
            CC_ASSERT(cookie);
            auto newPos = pointForColumnAndRow(cookie->getColumn(), cookie->getRow());
            // The higher up the cookie is, the bigger the delay on the animation. That looks more dynamic than dropping all the cookies at the same time.
            // This calculation works because fillHoles guarantees that lower cookies are first in the array.
            float delay = 0.05f + 0.15f * index;

            auto sprite = cookie->getSpriteNode();
            // Likewise, the duration of the animation is based on how far the cookie has to fall (0.1 seconds per tile). 
            // You can tweak these numbers to change the feel of the animation.
            float duration = ((sprite->getPositionY() - newPos.y) / TileHeight) * 0.1f;

            // You calculate which animation is the longest. This is the time the game has to wait before it may continue.
            longestDuration = MAX(longestDuration, duration + delay);

            // You perform the animation, which consists of a delay, a movement and a sound effect.
            auto callback = CallFunc::create([=]() {

                cookie->updateDebugTileLabel();

                auto moveAction = MoveTo::create(duration, newPos);
                auto easeAction = EaseOut::create(moveAction, duration);
                auto sprite = cookie->getSpriteNode();
                sprite->runAction(easeAction);
                AudioManager::getInstance()->playSound(SoundType::FallingCookieSound);
            });

            sprite->runAction(Sequence::create(DelayTime::create(delay), callback, nullptr));
        }
    }

    // You wait until all the cookies have fallen down before allowing the gameplay to continue.
    this->runAction(Sequence::create(DelayTime::create(longestDuration), func, nullptr));
}

//--------------------------------------------------------------------
void GameplayScene::showSelectionIndicatorForCookie(CookieObj * cookie)
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