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
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"

#include "Utils/Helpers/Helper.h"
#include "Utils/GameResources.h"
#include "Common/CommonTypes.h"
#include "Common/Factory/SmartFactory.h"

#include "Scenes/GameplayScene.h"
#include "Layers/CookiesLayer.h"

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

    cookieA->setLocalZOrder(100);
    cookieB->setLocalZOrder(90);

    const float duration = 0.3f;

    auto moveA = MoveTo::create(duration, cookieB->getPosition());
    auto easeA = EaseOut::create(moveA, duration); // maybe change rate?
    cookieA->runAction(Sequence::create(easeA, completion, nullptr));

    auto moveB = MoveTo::create(duration, cookieA->getPosition());
    auto easeB = EaseOut::create(moveB, duration);
    cookieB->runAction(easeB);
    
    swap->getCookieA()->updateDebugLabel();
    swap->getCookieB()->updateDebugLabel();
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

    cookieA->setLocalZOrder(100);
    cookieB->setLocalZOrder(90);

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
void _AnimationsManager::animateMatching(CommonTypes::Set* chains, cocos2d::CallFunc* completion)
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

    mCurrentScene->runAction(Sequence::create(DelayTime::create(duration/1.5f), completion, nullptr));
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

        float colDelay = Helper::randomFloatBetween(0.06f, 0.07f);
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
            auto animateBouncingObjDelay = 0.25f;
            longestDuration = MAX(longestDuration, duration + delay + animateBouncingObjDelay);

            // Perform the animation, which consists of a delay, a movement and a sound effect.
            auto callback = CallFunc::create([=]() {

                obj->updateDebugLabel();

                auto moveCallback = CallFunc::create([=]() {
                    AnimationsManager->animateBouncingObj(obj);
                    AudioManager->playSound(CommonTypes::SoundType::FallingCookieSound);
                });
                
                auto sprite = obj->getSpriteNode();
                auto delta = newPos - sprite->getPosition();

                auto moveAction = MoveBy::create(duration, delta);
                auto easeAction = EaseOut::create(moveAction, duration);

                sprite->runAction(Sequence::create(easeAction, moveCallback, nullptr));
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

    for (auto it = colums->begin(); it != colums->end(); it++) {

        auto array = dynamic_cast<cocos2d::Array*>(*it);
        CC_ASSERT(array);

        int startRow = -1;
        int rowIdx = 0;
        for (auto itArr = array->begin(); itArr != array->end(); itArr++, rowIdx++) {
            auto cookie = dynamic_cast<CookieObj*>(*itArr);
            CC_ASSERT(cookie);
            scene->createSpriteWithCookie(cookie, cookie->getColumn(), startRow);
            cookie->getSpriteNode()->setOpacity(0);

            auto newPos = Helper::pointForTile(cookie);

            // The higher up the cookie is, the bigger the delay on the animation. That looks more dynamic than dropping all the cookies at the same time.
            // This calculation works because fillHoles guarantees that lower cookies are first in the array.
            float delay = 0.1f + 0.175f * (array->count() - rowIdx - 1);

            // Likewise, the duration of the animation is based on how far the cookie has to fall (0.1 seconds per tile). 
            // You can tweak these numbers to change the feel of the animation.
            float timeToTile = fabs(startRow - cookie->getRow());
            float duration = (timeToTile * 0.1f) + 0.125f;            

            // You calculate which animation is the longest. This is the time the game has to wait before it may continue.
            auto animateBouncingObjDelay = 0.25f;
            longestDuration = MAX(longestDuration, duration + delay + animateBouncingObjDelay);

            // You perform the animation, which consists of a delay, a movement and a sound effect.
            auto callback = CallFunc::create([=]() {

                cookie->updateDebugLabel();

                auto moveCallback = CallFunc::create([=]() {
                    AnimationsManager->animateBouncingObj(cookie);
                    AudioManager->playSound(CommonTypes::SoundType::AddCookieSound);
                });

                auto moveAction = MoveTo::create(duration, newPos);
                auto easeAction = EaseOut::create(moveAction, duration);
                auto fadeIn = FadeIn::create(0.0125f);
                auto delayAction = DelayTime::create(0.15f);

                cookie->getSpriteNode()->runAction(Sequence::create(delayAction, fadeIn, nullptr));
                cookie->getSpriteNode()->runAction(Sequence::create(easeAction, moveCallback, nullptr));
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

    for (auto itObj = cookies->begin(); itObj != cookies->end(); itObj++) {
        auto obj = dynamic_cast<BaseObj*>(*itObj);

        auto spritePos = obj->getSpriteNode()->getPosition();
        Vec2 centerPosition = Vec2(spritePos.x, spritePos.y);// - 8);

        auto color = Helper::getScoreColorByObj(obj);

        // Add a label for the score that slowly floats up.
        auto fontSize = 80;
        auto str = StringUtils::format("%d", chain->getScore() / cookies->count());
        Text* scoreLabel = Text::create(str, GameResources::s_fontYellow.getCString(), fontSize);
        scoreLabel->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        scoreLabel->setPosition(centerPosition);
        scoreLabel->setLocalZOrder(300);
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
        scoreLabel->runAction(Sequence::create(DelayTime::create(duration / 2), fadeOut, nullptr));
        scoreLabel->runAction(Sequence::create(easeOut, callback, nullptr));
    }
}

//--------------------------------------------------------------------
void _AnimationsManager::animateScoreForFieldObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);

    Vec2 centerPosition = obj->getSpriteNode()->getPosition();

    auto color = Helper::getScoreColorByObj(obj);

    // Add a label for the score that slowly floats up.
    auto fontSize = 80;
    auto str = StringUtils::format("%d", obj->getScoreValue());
    Text* scoreLabel = Text::create(str, GameResources::s_fontYellow.getCString(), fontSize);
    scoreLabel->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    scoreLabel->setPosition(centerPosition);
    scoreLabel->setLocalZOrder(300);
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
    scoreLabel->runAction(Sequence::create(DelayTime::create(duration / 2), fadeOut, nullptr));
    scoreLabel->runAction(Sequence::create(easeOut, callback, nullptr));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateBouncingObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);

    float duration = 0.2f;
    auto moveAction = MoveBy::create(duration, Vec2(0.0f, -10.0f));
    auto easeMoveOut = EaseOut::create(moveAction, duration);
    auto scaleXAction = ScaleTo::create(duration, 1.2f, 0.8f);
    auto easeScaleXOut = EaseOut::create(scaleXAction, duration);

    auto reverseScaleXCallback = CallFunc::create([obj]() {
        float duration = 0.2f;
        auto reverseScaleXAction = ScaleTo::create(duration, 1.0f, 1.0f);
        auto reverseEaseScaleXOut = EaseOut::create(reverseScaleXAction, duration);
        auto reverseMoveAction = MoveBy::create(duration, Vec2(0.0f, 10.0f));
        auto reverseEaseMoveOut = EaseOut::create(reverseMoveAction, duration);
        if (obj) {
            auto sprite = obj->getSpriteNode();
            if (sprite) {
                sprite->runAction(reverseEaseScaleXOut);
                sprite->runAction(reverseEaseMoveOut);
            }
        }
    });

    duration = 0.2f;
    auto scaleYAction = ScaleTo::create(duration, 0.8f, 1.2f);
    auto easeScaleYOut = EaseOut::create(scaleYAction, duration);

    auto reverseScaleYAction = ScaleTo::create(duration*2, 1.0f, 1.0f);
    auto reverseEaseScaleYOut = EaseOut::create(reverseScaleYAction, duration);

    auto speed = 2.0f;
    auto sprite = obj->getSpriteNode();
    sprite->runAction(Speed::create(easeMoveOut, speed));
    sprite->runAction(Speed::create(easeScaleXOut, speed));
    auto seq1 = Sequence::create(DelayTime::create(0.2f), reverseScaleXCallback, nullptr);
    sprite->runAction(Speed::create(seq1, speed));
    auto seq2 = Sequence::create(DelayTime::create(0.4f), easeScaleYOut, reverseEaseScaleYOut, nullptr);
    sprite->runAction(Speed::create(seq2, speed));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateRemovingFieldObjects(CommonTypes::Set * fieldObjects)
//--------------------------------------------------------------------
{
    CC_ASSERT(fieldObjects);

    const float duration = 0.3f;

    for (auto it = fieldObjects->begin(); it != fieldObjects->end(); it++) {

        auto obj = dynamic_cast<FieldObj*>(*it);
        if (!obj)
            continue;

        const float scaleFactor = 0.1f;

        auto scaleAction = ScaleTo::create(duration, scaleFactor);
        auto fadeOut = FadeOut::create(duration);
        auto easeOut = EaseOut::create(fadeOut, duration);

        animateScoreForFieldObj(obj);

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
            } else if (obj->isHpEnded()) {
                SmartFactory->recycle(obj);
            }
            
        });
        obj->getSpriteNode()->runAction(Sequence::create(easeOut, callback, nullptr));
    }
    CC_ASSERT(mCurrentScene);

    mCurrentScene->runAction(Sequence::create(DelayTime::create(duration), nullptr));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateRemoveDude(BaseObj * obj, cocos2d::CallFunc * completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);
    CC_ASSERT(completion);

    const float duration = 0.3f;

    const float scaleFactor = 0.1f;

    auto scaleAction = ScaleTo::create(duration, scaleFactor);
    auto easeOut = EaseOut::create(scaleAction, duration);

    auto sprite = obj->getSpriteNode();
    auto callback = CallFunc::create([sprite, obj]() {
        if (sprite) {
            sprite->removeFromParent();
            obj->setSpriteNode(nullptr);
        }
    });
    sprite->runAction(Sequence::create(easeOut, callback, nullptr));

    CC_ASSERT(mCurrentScene);
    mCurrentScene->runAction(Sequence::create(DelayTime::create(duration), completion, nullptr));
}
