/**
* @file Controller/ObjectController.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"

class BaseObj;
class LevelObj;

class ObjectController : public cocos2d::Ref
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~ObjectController();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease" and "retain".
    */
    static ObjectController* create();

    bool init();

    void createInitialTiles();
    BaseObj* createTile(int column, int row, int type);

    cocos2d::Set* createInitialCookies();
    BaseObj* createCookie(int column, int row, int type);
    int getRandomCookieType(int column, int row);


protected:
    // Nodes should be created using create();
    ObjectController();

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(LevelObj*, mLevel, Level);
};