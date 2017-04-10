/**
* @file Managers/AnimationsManager.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Managers/ActionsManager.h"

#include "GameObjects/TileObjects/Base/BaseObj.h"

USING_NS_CC;


//--------------------------------------------------------------------
bool _ActionsManager::init()
//--------------------------------------------------------------------
{
    return true;
}

//--------------------------------------------------------------------
cocos2d::ActionInstant* _ActionsManager::actionBounceIn(BaseObj* obj)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);

    auto bounceCallback = CallFunc::create([obj]() {

        float duration = 0.2f;
        auto moveAction = MoveBy::create(duration, Vec2(0.0f, -10.0f));
        auto easeMoveOut = EaseOut::create(moveAction, duration);
        auto scaleXAction = ScaleTo::create(duration, 1.2f, 0.8f);
        auto easeScaleXOut = EaseOut::create(scaleXAction, duration);

        auto reverseScaleXCallback = CallFunc::create([obj, duration]() {

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

        auto speed = 2.0f;
        auto sprite = obj->getSpriteNode();
        sprite->runAction(Speed::create(easeMoveOut, speed));
        sprite->runAction(Speed::create(easeScaleXOut, speed));
        auto seq1 = Sequence::create(DelayTime::create(0.2f), reverseScaleXCallback, nullptr);
        sprite->runAction(Speed::create(seq1, speed));
    });

    return bounceCallback;
}

//--------------------------------------------------------------------
cocos2d::ActionInstant* _ActionsManager::actionBounceOut(BaseObj* obj)
//--------------------------------------------------------------------
{
    CC_ASSERT(obj);

    auto bounceCallback = CallFunc::create([obj]() {
        float duration = 0.2f;
        auto scaleYAction = ScaleTo::create(duration, 0.8f, 1.2f);
        auto easeScaleYOut = EaseOut::create(scaleYAction, duration);

        auto reverseScaleYAction = ScaleTo::create(duration * 2, 1.0f, 1.0f);
        auto reverseEaseScaleYOut = EaseOut::create(reverseScaleYAction, duration);

        if (obj) {
            auto sprite = obj->getSpriteNode();
            if (sprite) {
                auto speed = 2.0f;
                auto seq = Sequence::create(easeScaleYOut, reverseEaseScaleYOut, nullptr);
                sprite->runAction(Speed::create(seq, speed));
            }
        }
    });

    return bounceCallback;
}

//--------------------------------------------------------------------
cocos2d::ActionInstant* _ActionsManager::actionSwapObj(BaseObj* objA, BaseObj* objB, bool isFirstSwapObject)
//--------------------------------------------------------------------
{
    CC_ASSERT(objA);
    CC_ASSERT(objB);

    auto swapCallback = CallFunc::create([objA, objB, isFirstSwapObject]() {

        auto updateZOrderCallback = CallFunc::create([objA]() {
            if (objA) {
                objA->updateZOrder();                
            }
        });

        auto spriteA = objA->getSpriteNode();
        auto spriteB = objB->getSpriteNode();
        const float duration = 0.3f;
        auto movingCallback = CallFunc::create([objA, spriteA, spriteB, duration, isFirstSwapObject]() {
            // Put the cookie you started with on top.
            auto zOrder = isFirstSwapObject ? 100 : 90;
            auto move = MoveTo::create(duration, spriteB->getPosition());
            auto ease = EaseOut::create(move, duration);
            spriteA->setLocalZOrder(zOrder);
            spriteA->runAction(ease);
        });

        spriteA->runAction(movingCallback);
        spriteA->runAction(Sequence::createWithTwoActions(DelayTime::create(duration), updateZOrderCallback));
    });

    return swapCallback;
}
