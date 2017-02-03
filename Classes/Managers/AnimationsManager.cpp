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

#include "GameObjects/SwapObj.h"
#include "GameObjects/ChainObj.h"
#include "GameObjects/CookieObj.h"

#include "Utils/Helpers/Helper.h"
#include "Utils/GameResources.h"

#include "Scenes/GameplayScene.h"

USING_NS_CC;

//--------------------------------------------------------------------
bool AnimationsManager::initWithScene(cocos2d::Scene * scene)
//--------------------------------------------------------------------
{
    if (scene) {
        mCurrentScene = scene;
        return true;
    }
    return false;
}

//--------------------------------------------------------------------
void AnimationsManager::animateSwap(SwapObj* swap, cocos2d::CallFunc* completion)
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
void AnimationsManager::animateInvalidSwap(SwapObj* swap, cocos2d::CallFunc* completion)
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
void AnimationsManager::animateMatching(cocos2d::Set* chains, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(chains);
    CC_ASSERT(completion);

    const float duration = 0.3f;

    for (auto itChain = chains->begin(); itChain != chains->end(); itChain++) {

        auto chain = dynamic_cast<ChainObj*>(*itChain);
        if (!chain)
            continue;
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
void AnimationsManager::animateFallingCookies(cocos2d::Array* colums, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(colums);
    CC_ASSERT(completion);

    float longestDuration = 0;
    for (auto it = colums->begin(); it != colums->end(); it++) {

        auto array = dynamic_cast<cocos2d::Array*>(*it);
        CC_ASSERT(array);

        float colDelay = Helper::randomFloatBetween(0.05f, 0.15f) * 1;
        for (auto itArr = array->begin(); itArr != array->end(); itArr++) {

            auto cookie = dynamic_cast<CookieObj*>(*itArr);
            CC_ASSERT(cookie);

            auto newPos = Helper::pointForCookie(cookie);
            // The higher up the cookie is, the bigger the delay on the animation. That looks more dynamic than dropping all the cookies at the same time.
            // This calculation works because fillHoles guarantees that lower cookies are first in the array.

            float delay = (0.05f + 0.15f * colDelay);

            // Likewise, the duration of the animation is based on how far the cookie has to fall (0.1 seconds per tile). 
            // You can tweak these numbers to change the feel of the animation.
            float timeToTile = (cookie->getSpriteNode()->getPositionY() - newPos.y) / GameResources::TileHeight;
            float duration = (timeToTile * 0.1f) + colDelay * 1.5f;
            if (colums->count() == 1) {
                duration = (timeToTile * 0.125f);
            }

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

                AudioManager::getInstance()->playSound(CommonTypes::SoundType::FallingCookieSound);
            });

            cookie->getSpriteNode()->runAction(Sequence::create(DelayTime::create(delay), callback, nullptr));
        }
    }
    
    CC_ASSERT(mCurrentScene);
    // You wait until all the cookies have fallen down before allowing the gameplay to continue.
    mCurrentScene->runAction(Sequence::create(DelayTime::create(longestDuration), completion, nullptr));
}

//--------------------------------------------------------------------
void AnimationsManager::animateNewCookies(cocos2d::Array* colums, cocos2d::CallFunc* completion)
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

        auto startCookie = dynamic_cast<CookieObj*>(array->objectAtIndex(0));
        int startRow = startCookie ? startCookie->getRow() - 1 : -1;

        int index = 1;
        for (auto itArr = array->begin(); itArr != array->end(); itArr++, index++) {
            auto cookie = dynamic_cast<CookieObj*>(*itArr);
            CC_ASSERT(cookie);
            scene->createSpriteWithCookie(cookie, cookie->getColumn(), startRow);
            cookie->getSpriteNode()->setOpacity(0);

            auto newPos = Helper::pointForCookie(cookie);

            // The higher up the cookie is, the bigger the delay on the animation. That looks more dynamic than dropping all the cookies at the same time.
            // This calculation works because fillHoles guarantees that lower cookies are first in the array.
            int arrCount = array->count();
            int colCount = colums->count();
            float delay = 0.1f + 0.2f * (colCount - index - 1);

            // Likewise, the duration of the animation is based on how far the cookie has to fall (0.1 seconds per tile). 
            // You can tweak these numbers to change the feel of the animation.
            float duration = fabs(((startRow + 1) - (cookie->getRow() + 1)) * 0.1f);

            // You calculate which animation is the longest. This is the time the game has to wait before it may continue.
            longestDuration = MAX(longestDuration, duration + delay);

            // You perform the animation, which consists of a delay, a movement and a sound effect.
            auto callback = CallFunc::create([=]() {

                cookie->updateDebugTileLabel();

                auto moveAction = MoveTo::create(duration, newPos);
                auto easeAction = EaseOut::create(moveAction, duration);
                auto fadeIn = FadeIn::create(0.05f);

                cookie->getSpriteNode()->runAction(fadeIn);
                cookie->getSpriteNode()->runAction(easeAction);
                
                AudioManager::getInstance()->playSound(CommonTypes::SoundType::AddCookieSound);
            });

            cookie->getSpriteNode()->runAction(Sequence::create(DelayTime::create(delay), callback, nullptr));
        }
    }

    // You wait until all the cookies have fallen down before allowing the gameplay to continue.
    scene->runAction(Sequence::create(DelayTime::create(longestDuration), completion, nullptr));
}