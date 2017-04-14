/**
* @file Controller/ObjectController/Dude/DudeHelper.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"
#include "Common/CommonTypes.h"

class DudeObj;
class DudeController;

class DudeHelper : public cocos2d::Ref
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~DudeHelper();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static DudeHelper* createWithDudeObject(DudeObj* obj);

    bool initWithDudeObject(DudeObj* obj);

    CT::Set* getChainByDirection(const CT::Direction& direction, bool isPreview = false);

    void clear();

protected:
    // Nodes should be created using create();
    DudeHelper();

    //---Class Attributes-------------------------------------------------

    CC_SYNTHESIZE(DudeObj*, mDudeObj, DudeObj);
    CC_SYNTHESIZE(DudeController*, mDudeCtrl, DudeController);

    CC_PROPERTY(CT::Set*, mTopChain, TopChain);
    CC_PROPERTY(CT::Set*, mBottomChain, BottomChain);
    CC_PROPERTY(CT::Set*, mLeftChain, LeftChain);
    CC_PROPERTY(CT::Set*, mRightChain, RightChain);
    CC_PROPERTY(CT::Set*, mHorizontalChain, HorizontalChain);
    CC_PROPERTY(CT::Set*, mVerticalChain, VerticalChain);
    CC_PROPERTY(CT::Set*, mXChain, XChain);
};