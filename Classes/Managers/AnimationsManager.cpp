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
#include "Managers/ActionsManager.h"
#include "Managers/AudioManager.h"

#include "Controller/SwapController/SwapObj.h"
#include "Controller/ChainController/ChainObj.h"
#include "GameObjects/TileObjects/CookieObj.h"
#include "GameObjects/TileObjects/TileObj.h"
#include "Controller/ObjectController/Dude/DudeObj.h"
#include "Controller/ObjectController/Enemy/EnemyObj.h"
#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"

#include "GameObjects/Level/LevelObj.h"
#include "Controller/ObjectController/ObjectController.h"

#include "Utils/Helpers/Helper.h"
#include "Utils/GameResources.h"
#include "Common/GameObjTypes.h"
#include "Common/Factory/SmartObjFactory.h"
#include "Common/GlobalInfo/GlobalInfo.h"

#include "Utils/Helpers/ScoreHelper.h"

#include "Scenes/GameplayScene.h"
#include "Layers/CookiesLayer.h"

#include "cocos2d/cocos/ui/UIText.h"

USING_NS_CC;
using namespace CT;
using namespace GOT;
using ui::Text;

//--------------------------------------------------------------------
bool _AnimationsManager::initWithScene(cocos2d::Scene * scene)
//--------------------------------------------------------------------
{
    ActionsManager->init();
    if (scene) {
        mCurrentScene = scene;
        mInitialScenePos = mCurrentScene->getPosition();
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
    
    auto cookieA = swap->getObjectA()->getSpriteNode();
    auto cookieB = swap->getObjectB()->getSpriteNode();
    auto swapObjA = ActionsManager->actionSwapObj(swap->getObjectA(), swap->getObjectB(), true);
    auto swapObjB = ActionsManager->actionSwapObj(swap->getObjectB(), swap->getObjectA());
    cookieA->runAction(swapObjA);
    cookieB->runAction(swapObjB);

    CC_ASSERT(mCurrentScene);
    const float duration = 0.3f;
    mCurrentScene->runAction(Sequence::createWithTwoActions(DelayTime::create(duration), completion));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateInvalidSwap(SwapObj* swap, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(swap);
    CC_ASSERT(completion);
    // Put the cookie you started with on top.

    auto cookieA = swap->getObjectA()->getSpriteNode();
    auto cookieB = swap->getObjectB()->getSpriteNode();

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
void _AnimationsManager::animateMatchObject(BaseObj * obj, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    if (!obj) {
        return;
    }

    switch (obj->getType())
    {
    case BaseObjType::Cookie:
    case BaseObjType::Dude:
    case BaseObjType::Enemy:
        animateMatchObj(obj, completion);
        break;
    case BaseObjType::Field:
        animateMatchFieldObj(dynamic_cast<FieldObj*>(obj), completion);
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------------
void _AnimationsManager::animateMatching(CT::Set* chains, cocos2d::CallFunc* completion)
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

        auto objects = chain->getChainObjects();
        for (auto it = objects->begin(); it != objects->end(); it++) {

            auto obj = dynamic_cast<BaseObj*>(*it);
            animateMatchObject(obj);
        }
    }
    CC_ASSERT(mCurrentScene);

    mCurrentScene->runAction(Sequence::create(DelayTime::create(duration + 0.05f), completion, nullptr));
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

        for (auto itArr = array->begin(); itArr != array->end(); itArr++) {

            auto obj = dynamic_cast<BaseObj*>(*itArr);
            CC_ASSERT(obj);

            // The higher up the cookie is, the bigger the delay on the animation. That looks more dynamic than dropping all the cookies at the same time.
            // This calculation works because fillHoles guarantees that lower cookies are first in the array.
            auto sprite = obj->getSpriteNode();
            auto startCell = Helper::cellFromPoint(sprite->getPosition());
            float duration = Helper::getDurationToTile(startCell, obj->getCell());

            // Calculate which animation is the longest. This is the time the game has to wait before it may continue.
            longestDuration = MAX(longestDuration, duration);

            // Perform the animation, which consists of a delay, a movement and a sound effect.
            auto moveCallback = CallFunc::create([=]() {
                obj->updateZOrder();
                AnimationsManager->animateBouncingObj(obj);
                AudioManager->playSound(CT::SoundType::FallingCookieSound);
            });
            sprite->runAction(ActionsManager->actionFallObject(obj, moveCallback, duration));
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

        int startRow = -5;
        int rowIdx = 0;
        for (auto itArr = array->begin(); itArr != array->end(); itArr++, rowIdx++) {
            auto row = array->count() - rowIdx - 1;
            auto obj = dynamic_cast<BaseObj*>(*itArr);
            CC_ASSERT(obj);

            auto startCell = Cell(obj->getCell(), startRow);
            scene->createSpriteWithObj(obj, startCell);
            auto sprite = obj->getSpriteNode();

            // The higher up the cookie is, the bigger the delay on the animation. That looks more dynamic than dropping all the cookies at the same time.
            // This calculation works because fillHoles guarantees that lower cookies are first in the array.
            const float tileOffset = 0.09f;
            float delay = tileOffset + 0.1f + (0.15f * row);
            float duration = Helper::getDurationToTile(startCell, obj->getCell());

            // You calculate which animation is the longest. This is the time the game has to wait before it may continue.
            auto animateBouncingObjDelay = 0.25f;
            longestDuration = MAX(longestDuration, duration + delay + animateBouncingObjDelay);

            auto callback = CallFunc::create([=]() {
                auto moveCallback = CallFunc::create([=]() {
                    obj->updateZOrder();
                    AnimationsManager->animateBouncingObj(obj);
                    AudioManager->playSound(CT::SoundType::AddCookieSound);
                });
                sprite->runAction(ActionsManager->actionFallObject(obj, moveCallback, duration));
            });

            sprite->runAction(Sequence::create(DelayTime::create(delay), callback, nullptr));
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

    auto objects = chain->getChainObjects(); //chain->getChainObjectsForScoreAnimation();
    CC_ASSERT(objects);

    for (auto itObj = objects->begin(); itObj != objects->end(); itObj++) {
        auto obj = dynamic_cast<BaseObj*>(*itObj);

        auto scene = dynamic_cast<GameplayScene*>(mCurrentScene);
        CC_ASSERT(scene);

        auto objCtrl = scene->getLevel()->getObjectController();

        Vec2 spritePos = Vec2::ZERO;
        if (objCtrl) {
            auto tileObj = objCtrl->tileAt(obj->getCell());
            spritePos = tileObj->getSpriteNode()->getPosition();
        }
        else {
            spritePos = obj->getSpriteNode()->getPosition();
        }        
        
        Vec2 centerPosition = Vec2(spritePos.x, spritePos.y);// - 8);

        auto color = Helper::getScoreColorByObj(obj);
        int score = ScoreHelper::getScoreByObj(obj, chain->getIsCreatedByDude());

        // Add a label for the score that slowly floats up.
        auto fontSize = 80;
        auto str = StringUtils::format("%d", score);
        Text* scoreLabel = Text::create(str, GameResources::s_fontYellow.getCString(), fontSize);
        scoreLabel->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        scoreLabel->setPosition(centerPosition);
        scoreLabel->setLocalZOrder(300);
        scoreLabel->setTextColor(Color4B::WHITE);
        scoreLabel->enableOutline(color, 2);
        scoreLabel->setScale(0.5f);

        scene->getInfoLayer()->addChild(scoreLabel);

        auto duration = 0.75f;
        //auto scaleAction = ScaleTo::create(duration, 2.0f);
        auto moveAction = MoveBy::create(duration, Vec2(0.0f, GlobInfo->getTileHeight() /2 ));
        auto easeOut = EaseIn::create(moveAction, duration);
        auto fadeOut = FadeOut::create(duration);

        auto callback = CallFunc::create([scoreLabel]() {
            if (scoreLabel) {
                scoreLabel->removeFromParent();
            }
        });
        scoreLabel->runAction(easeOut);
        scoreLabel->runAction(Sequence::create(DelayTime::create(duration * 0.75f), fadeOut, callback, nullptr));
    }
}

//--------------------------------------------------------------------
void _AnimationsManager::animateScoreForFieldObj(BaseObj * obj)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);

    Vec2 centerPosition = obj->getSpriteNode()->getPosition();

    auto color = Helper::getScoreColorByObj(obj);
    auto score = ScoreHelper::getScoreByObj(obj, false);

    // Add a label for the score that slowly floats up.
    auto fontSize = 80;
    auto str = StringUtils::format("%d", score);
    Text* scoreLabel = Text::create(str, GameResources::s_fontYellow.getCString(), fontSize);
    scoreLabel->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    scoreLabel->setPosition(centerPosition);
    scoreLabel->setLocalZOrder(300);
    scoreLabel->setTextColor(Color4B::WHITE);
    scoreLabel->enableOutline(color, 2);
    scoreLabel->setScale(0.5f);

    auto scene = dynamic_cast<GameplayScene*>(mCurrentScene);
    CC_ASSERT(scene);

    scene->getInfoLayer()->addChild(scoreLabel);

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
void _AnimationsManager::animateThrowDownAnObj(BaseObj* obj, CT::Cell& destPos, cocos2d::CallFunc* completion, bool animateShakingScreen)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);

    auto sprite = obj->getSpriteNode();

    auto easeAction = ActionsManager->actionThrowDownAnObj(obj, destPos);

    auto moveCallback = CallFunc::create([=]() {

        float duration = 0.3f;
        auto bounceIn = ActionsManager->actionBounceInHeavy(obj, duration);
        auto bounceOut = ActionsManager->actionBounceOut(obj, duration);
        auto seq = Sequence::create(DelayTime::create(duration), bounceOut, nullptr);

        sprite->runAction(bounceIn);        
        sprite->runAction(seq);

        if (animateShakingScreen) {
            AnimationsManager->animateShakeScreen();
        }
    });

    auto speed = 2.0f;
    auto waitDelay = DelayTime::create(0.2f);
    auto seq = Sequence::create(waitDelay, easeAction, moveCallback, completion, nullptr);
    sprite->runAction(Speed::create(seq, speed));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateReboundAfterThrowingObj(CT::Cell& destPos, CT::Set* chains, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(chains);

    for (auto itChain = chains->begin(); itChain != chains->end(); ++itChain) {
        auto chain = dynamic_cast<ChainObj*>(*itChain);
        if (!chain) {
            continue;
        }
        auto objects = chain->getChainObjects();
        if (!objects) {
            continue;
        }
        for (auto it = objects->begin(); it != objects->end(); ++it) {
            auto obj = dynamic_cast<BaseObj*>(*it);
            if (!obj) {
                continue;
            }
            int8_t col = obj->getColumn();
            int8_t row = obj->getRow();
            if (obj->isContainer() || (destPos.column == col && destPos.row == row)) {
                continue;
            }

            auto distance = Helper::getDistanceBetweenObjects(destPos, Cell(col, row));
            auto jumpHeight = GlobInfo->getTileHeight() / 3.0f;
            AnimationsManager->animateJumpWithBouncing(obj, distance * 0.2f, jumpHeight / distance);
        }
    }


}

//--------------------------------------------------------------------
void _AnimationsManager::animateJumpWithBouncing(BaseObj* obj, float delay, float heigthInPixel)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);

    auto bounceIn = ActionsManager->actionBounceInNormal(obj);

    auto speed = 1.0f;
    float duration = heigthInPixel / 100.0f;

    auto sprite = obj->getSpriteNode();
    auto jumpAction = cocos2d::JumpBy::create(duration, Vec2::ZERO, heigthInPixel, 1);
    auto seq = Sequence::create(DelayTime::create(delay), jumpAction, bounceIn, nullptr);
    sprite->runAction(Speed::create(seq, speed));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateBouncingObj(BaseObj* obj)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);
    
    auto bounceIn = ActionsManager->actionBounceInNormal(obj);
    auto bounceOut = ActionsManager->actionBounceOut(obj);

    auto sprite = obj->getSpriteNode();
    sprite->runAction(bounceIn);
    auto seq = Sequence::create(DelayTime::create(0.2f), bounceOut, nullptr);
    sprite->runAction(seq);
}

//--------------------------------------------------------------------
void _AnimationsManager::animateHintJump(BaseObj* obj)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);
    
    auto bounceIn = ActionsManager->actionBounceInNormal(obj);
    auto bounceOut = ActionsManager->actionBounceOut(obj);

    auto sprite = obj->getSpriteNode();
    sprite->runAction(bounceIn);
    auto seq = Sequence::create(DelayTime::create(0.2f), bounceOut, nullptr);
    sprite->runAction(seq);

    auto speed = 2.0f;
    auto jumpAction = cocos2d::JumpBy::create(0.4f, Vec2::ZERO, 40.0f, 1);
    auto seq1 = Sequence::create(DelayTime::create(0.5f), jumpAction, nullptr);
    sprite->runAction(Speed::create(seq1, speed));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateHintSwap(CT::Set* objects, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    CC_ASSERT(objects);

    for (auto itObj = objects->begin(); itObj != objects->end(); itObj++) {
        auto obj = dynamic_cast<BaseObj*>(*itObj);
        if (obj) {
            animateHintJump(obj);
        }
    }

    const float duration = 0.4f;
    mCurrentScene->runAction(Sequence::create(DelayTime::create(duration), completion, nullptr));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateMatchObj(BaseObj* obj, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    if (!obj) {
        return;
    }
    const float duration = 0.3f;
    const float scaleFactor = 0.1f;

    auto scaleAction = ScaleTo::create(duration, scaleFactor);
    auto easeOut = EaseOut::create(scaleAction, duration);

    auto baseObj = dynamic_cast<BaseObj*>(obj);
    auto callback = CallFunc::create([baseObj, obj]() {

        auto func = obj->getRemoveObjectCallback();
        if (func) {
            func(baseObj);
        }
    });
    obj->getSpriteNode()->runAction(Sequence::create(easeOut, callback, completion, nullptr));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateMatchFieldObj(FieldObj * obj, cocos2d::CallFunc* completion)
//--------------------------------------------------------------------
{
    if (!obj) {
        return;
    }
    const float duration = 0.3f;

    auto fadeOut = FadeOut::create(duration);
    auto easeOut = EaseOut::create(fadeOut, duration);

    auto scene = dynamic_cast<GameplayScene*>(mCurrentScene);
    CC_ASSERT(scene);

    std::function<void(FieldObj*)> createSpriteCallback = [scene](FieldObj* obj) {
        scene->createSpriteWithFieldObj(obj);
    };
    auto baseObj = dynamic_cast<BaseObj*>(obj);
    auto callback = CallFunc::create([scene, baseObj, obj, createSpriteCallback]() {

        auto func = obj->getFieldObjChangeState();
        if (func) {
            func(baseObj, createSpriteCallback);
        }
    });
    obj->getSpriteNode()->runAction(Sequence::create(easeOut, callback, completion, nullptr));
}

//--------------------------------------------------------------------
void _AnimationsManager::animateShakeScreen()
//--------------------------------------------------------------------
{
    if (mShakeScreenDuration <= 0) {
        auto selector = schedule_selector(_AnimationsManager::shakeScreen);
        cocos2d::Director::getInstance()->getScheduler()->schedule(selector, this, 0, CC_REPEAT_FOREVER, 0.0f, false);
        mShakeScreenDuration = GlobInfo->getShakeScreenDuration();
    }
    else {
        cocos2d::log("_AnimationsManager::animateShakeScreen: ERROR! can't run animation while it is still running!");
    }
}

//--------------------------------------------------------------------
void _AnimationsManager::shakeScreen(float dt)
//--------------------------------------------------------------------
{
    float offset = mShakeScreenDuration / GlobInfo->getMinShakeTime();
    float randx = Helper::rangeRandom(-offset, offset);
    float randy = Helper::rangeRandom(-offset, offset);

    mCurrentScene->setPosition(Point(randx, randy));
    mCurrentScene->setPosition(Point(mInitialScenePos.x + randx, mInitialScenePos.y + randy));

    mShakeScreenDuration -= 1;

    if (mShakeScreenDuration <= 0) {
        mCurrentScene->setPosition(Point(mInitialScenePos.x, mInitialScenePos.y));
        auto selector = schedule_selector(_AnimationsManager::shakeScreen);
        cocos2d::Director::getInstance()->getScheduler()->unschedule(selector, this);
    }
}