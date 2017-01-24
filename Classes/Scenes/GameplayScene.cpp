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

#include "Utils/VisibleRect.h"
#include "Utils/GameResources.h"

USING_NS_CC;

//--------------------------------------------------------------------
Scene* GameplayScene::createScene()
//--------------------------------------------------------------------
{
    auto scene = Scene::create();
    auto layer = GameplayScene::create();
    scene->addChild(layer);
    return scene;
}

//--------------------------------------------------------------------
bool GameplayScene::init()
//--------------------------------------------------------------------
{
    if (!Layer::init()) {
        return false;
    }
    
    auto sprite = Sprite::create(GameResources::s_backgroundImg);
    sprite->setPosition(VisibleRect::center());
    this->addChild(sprite, 0);
    
    return true;
}

