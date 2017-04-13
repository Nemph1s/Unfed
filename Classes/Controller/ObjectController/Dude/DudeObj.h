/**
* @file GameObjects/TileObjects/DudeObj.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "GameObjects/TileObjects/FieldObjects/Base/FieldObj.h"
#include "Utils/PlatformMacros.h"

class DudeObj : public FieldObj
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~DudeObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static DudeObj * create(const CT::FieldInfo &info);

    bool init(const CT::FieldInfo &info);

    cocos2d::String& spriteName() const override;

    void activate();
    void deactivate();


protected:
    // Nodes should be created using create();
    DudeObj();

    //---Class Attributes-------------------------------------------------

    CC_SYNTHESIZE(std::function<void(BaseObj*)>, mRemoveDudeCallback, RemoveDudeCallback);
    CC_SYNTHESIZE(std::function<void(BaseObj*)>, mEraseDirectionsCallback, EraseDirectionsCallback);

    /**
    * @brief A parameter that checks is the dude object was touched
    * If it doesn't, when next dude was founded in chain, then it will create chain for all horizontal or vertical fields
    */
    SYNTHESIZE_IS_READONLY(bool, mIsActivated, Activated, false);
};

