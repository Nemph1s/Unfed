/**
* @file Managers/AnimationsManager.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Managers/AnimationsManager.h"
#include "Managers/AudioManager.h"

#include "GameObjects/Swap/SwapObj.h"
#include "GameObjects/Chain/ChainObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/TileObjects/TileObj.h"

#include "Utils/Helpers/Helper.h"
#include "Utils/GameResources.h"
#include "Common/CommonTypes.h"
#include "Common/Factory/SmartFactory.h"

#include "Scenes/GameplayScene.h"

#include "cocos2d/cocos/ui/UIText.h"

USING_NS_CC;
using namespace CommonTypes;
using ui::Text;

//--------------------------------------------------------------------
bool _AnimationsManager::initWithScene(cocos2d::Scene * scene)
//--------------------------------------------------------------------
{
    if (scene) {
        mCurrentScene = scene;
        return true;
    }
    return false;
}

//--------------------------------------------------------------------
void _AnimationsManager::animateSwap(SwapObj* swap, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(swap);
    CC_ASSERT(completion);
    // Put the cookie you started with on top.

    auto cookieA = swap->getCookieA()->getSpriteNode();
    auto cookieB = swap->getCookieB()->getSpriteNode();

    cookieA->setZOrder(100);
    cookieB->setZOrder(90);

    const float duration = 0.3f;

    auto moveA = MoveTo::create(duration, cookieB->getPosition());
    auto easeA = EaseOut::create(moveA, duration); // maybe change rate?
    cookieA->runAction(Sequence::create(easeA, completion, nullptr));

    auto moveB = MoveTo::create(duration, cookieA->getPosition());
    auto easeB = EaseOut::create(moveB, duration);
    cookieB->runAction(easeB);
    
    swap->getCookieA()->updateDebugTileLabel();
    swap->getCookieB()->updateDebugTileLabel();
}

//--------------------------------------------------------------------
void _AnimationsManager::animateInvalidSwap(SwapObj* swap, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(swap);
    CC_ASSERT(completion);
    // Put the cookie you started with on top.

    auto cookieA = swap->getCookieA()->getSpriteNode();
    auto cookieB = swap->getCookieB()->getSpriteNode();

    cookieA->setZOrder(100);
    cookieB->setZOrder(90);

    const float duration = 0.3f;
    auto deltaA = cookieB->getPosition() - cookieA->getPosition();
    auto deltaB = cookieA->getPosition() - cookieB->getPosition();

    auto moveA = MoveBy::create(duration, deltaA);
    auto easeA = EaseOut::create(moveA, duration);
    auto moveB = MoveBy::create(duration, deltaB);
    auto easeB = EaseOut::create(moveB, duration);

    auto rotate = RotateBy::create(0.05f, 10);
    auto seq = Sequence::create(rotate, rotate->reverse(), rotate, rotate->reverse(), nullptr);

    cookieB->runAction(Sequence::create(easeB, easeB->reverse(), nullptr));
    cookieA->runAction(Sequence::create(easeA, easeA->reverse(), seq, completion, nullptr));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateMatching(cocos2d::Set* chains, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(chains);
    CC_ASSERT(completion);

    const float duration = 0.3f;

    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {

        auto chain = dynamic_cast<ChainObj*>(*itChain);
        if (!chain)
            continue;

        animateScoreForChain(chain);

        auto cookies = chain->getCookies();
        for (auto it = cookies->begin(); it != cookies->end(); it++) {

            auto cookie = dynamic_cast<CookieObj*>(*it);
            if (cookie != nullptr) {

                const float scaleFactor = 0.1f;

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
    CC_ASSERT(mCurrentScene);

    mCurrentScene->runAction(Sequence::create(DelayTime::create(duration), completion, nullptr));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateFallingCookies(cocos2d::Array* colums, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(colums);
    CC_ASSERT(completion);

    float longestDuration = 0;
    for (auto it = colums->begin(); it != colums->end(); it++) {

        auto array = dynamic_cast<cocos2d::Array*>(*it);
        CC_ASSERT(array);

        float colDelay = Helper::randomFloatBetween(0.055f, 0.07f) * 1;
        for (auto itArr = array->begin(); itArr != array->end(); itArr++) {

            auto cookie = dynamic_cast<CookieObj*>(*itArr);
            CC_ASSERT(cookie);

            auto newPos = Helper::pointForTile(cookie);
            // The higher up the cookie is, the bigger the delay on the animation. That looks more dynamic than dropping all the cookies at the same time.
            // This calculation works because fillHoles guarantees that lower cookies are first in the array.

            float delay = (0.05f + 0.15f * colDelay);

            // Likewise, the duration of the animation is based on how far the cookie has to fall (0.1 seconds per tile). 
            // You can tweak these numbers to change the feel of the animation.
            float timeToTile = (cookie->getSpriteNode()->getPositionY() - newPos.y) / GameResources::TileHeight;
            float duration = (timeToTile * 0.1f) + colDelay * 1.5f;

            // Calculate which animation is the longest. This is the time the game has to wait before it may continue.
            longestDuration = MAX(longestDuration, duration + delay);

            // Perform the animation, which consists of a delay, a movement and a sound effect.
            auto callback = CallFunc::create([=]() {

                cookie->updateDebugTileLabel();

                auto sprite = cookie->getSpriteNode();
                auto delta = newPos - sprite->getPosition();

                auto moveAction = MoveBy::create(duration, delta);
                auto easeAction = EaseOut::create(moveAction, duration);
                sprite->runAction(easeAction);

                AudioManager->playSound(CommonTypes::SoundType::FallingCookieSound);
            });

            cookie->getSpriteNode()->runAction(Sequence::create(DelayTime::create(delay), callback, nullptr));
        }
    }
    
    CC_ASSERT(mCurrentScene);
    // You wait until all the cookies have fallen down before allowing the gameplay to continue.
    mCurrentScene->runAction(Sequence::create(DelayTime::create(longestDuration), completion, nullptr));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateFallingObjects(cocos2d::Array * colums, cocos2d::CallFunc * completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(colums);
    CC_ASSERT(completion);

    float longestDuration = 0;
    for (auto it = colums->begin(); it != colums->end(); it++) {

        auto array = dynamic_cast<cocos2d::Array*>(*it);
        CC_ASSERT(array);

        float colDelay = Helper::randomFloatBetween(0.055f, 0.07f) * 1;
        for (auto itArr = array->begin(); itArr != array->end(); itArr++) {

            auto obj = dynamic_cast<BaseObj*>(*itArr);
            CC_ASSERT(obj);

            auto newPos = Helper::pointForTile(obj);
            // The higher up the cookie is, the bigger the delay on the animation. That looks more dynamic than dropping all the cookies at the same time.
            // This calculation works because fillHoles guarantees that lower cookies are first in the array.

            float delay = (0.05f + 0.15f * colDelay);

            // Likewise, the duration of the animation is based on how far the cookie has to fall (0.1 seconds per tile). 
            // You can tweak these numbers to change the feel of the animation.
            float timeToTile = (obj->getSpriteNode()->getPositionY() - newPos.y) / GameResources::TileHeight;
            float duration = (timeToTile * 0.1f) + colDelay * 1.5f;

            // Calculate which animation is the longest. This is the time the game has to wait before it may continue.
            longestDuration = MAX(longestDuration, duration + delay);

            // Perform the animation, which consists of a delay, a movement and a sound effect.
            auto callback = CallFunc::create([=]() {

                if (obj->getType() == BaseObjectType::CookieObj) {
                    auto cookie = dynamic_cast<CookieObj*>(obj);
                    cookie->updateDebugTileLabel();
                }
                
                auto sprite = obj->getSpriteNode();
                auto delta = newPos - sprite->getPosition();

                auto moveAction = MoveBy::create(duration, delta);
                auto easeAction = EaseOut::create(moveAction, duration);
                sprite->runAction(easeAction);

                AudioManager->playSound(CommonTypes::SoundType::FallingCookieSound);
            });

            obj->getSpriteNode()->runAction(Sequence::create(DelayTime::create(delay), callback, nullptr));
        }
    }

    CC_ASSERT(mCurrentScene);
    // You wait until all the cookies have fallen down before allowing the gameplay to continue.
    mCurrentScene->runAction(Sequence::create(DelayTime::create(longestDuration), completion, nullptr));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateNewCookies(cocos2d::Array* colums, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(colums);
    CC_ASSERT(completion);
    // The duration of falling cookies
    float longestDuration = 0;

    auto scene = dynamic_cast<GameplayScene*>(mCurrentScene);
    CC_ASSERT(scene);

    int columnIdx = 0;
    for (auto it = colums->begin(); it != colums->end(); it++, columnIdx++) {

        auto array = dynamic_cast<cocos2d::Array*>(*it);
        CC_ASSERT(array);

        int startRow = -1;
        float colDelay = Helper::randomFloatBetween(0.055f, 0.07f) * 1;

        for (auto itArr = array->begin(); itArr != array->end(); itArr++) {
            auto cookie = dynamic_cast<CookieObj*>(*itArr);
            CC_ASSERT(cookie);
            scene->createSpriteWithCookie(cookie, cookie->getColumn(), startRow);
            cookie->getSpriteNode()->setOpacity(0);

            auto newPos = Helper::pointForTile(cookie);

            // The higher up the cookie is, the bigger the delay on the animation. That looks more dynamic than dropping all the cookies at the same time.
            // This calculation works because fillHoles guarantees that lower cookies are first in the array.
            float delay = 0.1f + 0.175f * (colums->count() - columnIdx - 1);

            // Likewise, the duration of the animation is based on how far the cookie has to fall (0.1 seconds per tile). 
            // You can tweak these numbers to change the feel of the animation.
            float timeToTile = fabs(startRow - cookie->getRow());
            float duration = (timeToTile * 0.1f) + colDelay;

            // You calculate which animation is the longest. This is the time the game has to wait before it may continue.
            longestDuration = MAX(longestDuration, duration + delay);

            // You perform the animation, which consists of a delay, a movement and a sound effect.
            auto callback = CallFunc::create([=]() {

                cookie->updateDebugTileLabel();

                auto moveAction = MoveTo::create(duration, newPos);
                auto easeAction = EaseOut::create(moveAction, duration);
                auto fadeIn = FadeIn::create(0.0125f);

                cookie->getSpriteNode()->runAction(Sequence::create(DelayTime::create(0.15f), fadeIn, nullptr));
                cookie->getSpriteNode()->runAction(easeAction);
                
                AudioManager->playSound(CommonTypes::SoundType::AddCookieSound);
            });

            cookie->getSpriteNode()->runAction(Sequence::create(DelayTime::create(delay), callback, nullptr));
        }
    }

    // You wait until all the cookies have fallen down before allowing the gameplay to continue.
    scene->runAction(Sequence::create(DelayTime::create(longestDuration), completion, nullptr));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateNewCookieSprite(cocos2d::Sprite * sprite)
//--------------------------------------------------------------------
{
    CC_ASSERT(sprite);

    sprite->setOpacity(0);
    sprite->setScale(0.5f);

    auto duration = Helper::randomFloatBetween(0.25f, 0.5f);
    auto callback = CallFunc::create([sprite]() {
        auto duration = 0.25f;
        sprite->runAction(FadeIn::create(duration));
        sprite->runAction(ScaleTo::create(duration, 1.0f));
    });

    sprite->runAction(Sequence::create(DelayTime::create(duration), callback, nullptr));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateScoreForChain(ChainObj * chain)
//--------------------------------------------------------------------
{
    CC_ASSERT(chain);
    // Figure out what the midpoint of the chain is.

    auto cookies = chain->getCookies();
    CC_ASSERT(cookies);

    auto firstCookie = dynamic_cast<CookieObj*>(cookies->getObjectAtIndex(0));
    auto lastCookie = dynamic_cast<CookieObj*>(cookies->getLastObject());
    CC_ASSERT(firstCookie);
    CC_ASSERT(lastCookie);

    auto firstSpritePos = firstCookie->getSpriteNode()->getPosition();
    auto lastSpritePos = lastCookie->getSpriteNode()->getPosition();

    Vec2 centerPosition = Vec2((firstSpritePos.x + lastSpritePos.x) / 2,
        (firstSpritePos.y + lastSpritePos.y) / 2);// - 8);

    //TODO: move to helper
    auto color = Color4B::WHITE;
    switch (lastCookie->getCookieType())
    {
    case CookieType::Croissant:
        color = Color4B::ORANGE;
        break;
    case CookieType::Cupcake:
        color = Color4B::RED;
        break;
    case CookieType::Danish:
        color = Color4B::BLUE;
        break;
    case CookieType::Donut:
        color = Color4B::MAGENTA;
        break;
    case CookieType::Macaron:
        color = Color4B::GREEN;
        break;
    case CookieType::SugarCookie:
        color = Color4B::YELLOW;
        break;
    default:
        break;
    }

    // Add a label for the score that slowly floats up.

    auto fontSize = 80;
    auto str = StringUtils::format("+%d", chain->getScore());
    Text* scoreLabel = Text::create(str, GameResources::s_fontYellow.getCString(), fontSize);
    scoreLabel->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    scoreLabel->setPosition(centerPosition);
    scoreLabel->setZOrder(300);
    scoreLabel->setTextColor(Color4B::WHITE);
    scoreLabel->enableOutline(color, 2);
    scoreLabel->setScale(0.5f);

    auto scene = dynamic_cast<GameplayScene*>(mCurrentScene);
    CC_ASSERT(scene);

    scene->getCookiesLayer()->addChild(scoreLabel);

    auto duration = 1.15f;
    //auto scaleAction = ScaleTo::create(duration, 2.0f);
    auto moveAction = MoveBy::create(duration, Vec2(0.0f, 10.0f));
    auto easeOut = EaseOut::create(moveAction, duration);
    auto fadeOut = FadeOut::create(0.5f);

    auto callback = CallFunc::create([scoreLabel]() {
        if (scoreLabel) {
            scoreLabel->removeFromParent();
        }
    });
    scoreLabel->runAction(Sequence::create(DelayTime::create(duration/2), fadeOut, nullptr));
    scoreLabel->runAction(Sequence::create(easeOut, callback, nullptr));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateRemovingFieldObjects(cocos2d::Set * fieldObjects, cocos2d::CallFunc * completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(fieldObjects);
    CC_ASSERT(completion);

    const float duration = 0.3f;

    for (auto it = fieldObjects->begin(); it != fieldObjects->end(); it++) {

        auto obj = dynamic_cast<TileObj*>(*it);
        if (!obj)
            continue;

        const float scaleFactor = 0.1f;

        auto scaleAction = ScaleTo::create(duration, scaleFactor);
        auto fadeOut = FadeOut::create(duration);
        auto easeOut = EaseOut::create(fadeOut, duration);

        auto scene = dynamic_cast<GameplayScene*>(mCurrentScene);
        CC_ASSERT(scene);

        auto sprite = obj->getSpriteNode();
        auto callback = CallFunc::create([scene, sprite, obj]() {
            if (sprite) {
                sprite->removeFromParent();
                obj->setSpriteNode(nullptr);
            }
            //TODO: move this to another place
            if (obj->getHP() > 0) {
                scene->createSpriteWithFieldObj(obj);
            } else if (obj->isReadyToRemove()) {
                SmartFactory->recycle(obj);
                obj->removeFromParent();
            }
            
        });
        obj->getSpriteNode()->runAction(Sequence::create(easeOut, callback, nullptr));
    }
    CC_ASSERT(mCurrentScene);

    mCurrentScene->runAction(Sequence::create(DelayTime::create(duration), completion, nullptr));
}
