/**
* @file GameObjects/TileObjects/FieldObjects/Base/FieldObj.h
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "GameObjects/TileObjects/Base/BaseObj.h"

class FieldObj : public BaseObj
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~FieldObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static FieldObj * create(const GOT::FieldInfo &info);

    bool init(const GOT::FieldInfo &info);

    virtual cocos2d::String& description() const override;

    virtual int getTypeAsInt() const override;

    virtual void match() override;
    virtual void clear() override;
    
    virtual bool isHpEnded() const override;

protected:
    // Nodes should be created using create();
    FieldObj();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE_READONLY(GOT::FieldType, mFieldType, FieldType);

    CC_SYNTHESIZE(bool, mReadyToUpdatePriority, ReadyToUpdatePriority);
    CC_SYNTHESIZE(std::function<void(BaseObj*, std::function<void(FieldObj*)>)>, mFieldObjChangeState, FieldObjChangeState);

    CC_SYNTHESIZE(int, mHP, HP);
};

