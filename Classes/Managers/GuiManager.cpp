/**
* @file Managers/GuiManager.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Managers/GuiManager.h"

#include "Utils/Helpers/Helper.h"
#include "Scenes/GameplayScene.h"
#include "GameObjects/Level/LevelGoalComponent.h"
#include "Utils/Localization.h"
#include "Utils/GameResources.h"
#include "Utils/Helpers/VisibleRect.h"

#include "cocos2d/cocos/ui/UIText.h"
#include "cocos2d/cocos/ui/UILayout.h"
#include "cocos2d/cocos/ui/UIButton.h"
#include "cocos2d/cocos/ui/UIWidget.h"

//#include "cocos2d/cocos/editor-support/cocostudio/WidgetReader/TextReader/TextReader.h"

USING_NS_CC;
using namespace CommonTypes;
using ui::Text;
using ui::Layout;
using ui::Button;
using ui::Widget;

//--------------------------------------------------------------------
bool _GuiManager::initWithScene(cocos2d::Scene* scene, LevelGoalComponent* levelGoal)
//--------------------------------------------------------------------
{
    cocos2d::log("GuiManager::initWithScene:");
    if (scene && levelGoal) {
        mCurrentScene = scene;
        mLevelGoalComponent = levelGoal;

        crateInfoLayer();
        createShuffleButton();
        createScoreBar();
        createLevelGoals();

        return true;
    }
    return false;
}

//--------------------------------------------------------------------
void _GuiManager::crateInfoLayer()
//--------------------------------------------------------------------
{
    int8_t fontSize = 32;

    Vec2 viewOrigin = Director::getInstance()->getVisibleOrigin();
    Size viewSize = Director::getInstance()->getVisibleSize();
    auto height = (viewSize.height - (GameResources::TileHeight * CommonTypes::NumColumns)) / 3;

    mTopGuiLayer = LayerColor::create(Color4B(53, 53, 53, 0));
    mTopGuiLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    mTopGuiLayer->setContentSize(Size(viewSize.width, height));
    mTopGuiLayer->setPosition(Vec2(0.0f, viewSize.height - height));
    mCurrentScene->addChild(mTopGuiLayer);

    mBottomGuiLayer = LayerColor::create(Color4B(255, 53, 53, 0));
    mBottomGuiLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    mBottomGuiLayer->setContentSize(Size(viewSize.width, height));
    mCurrentScene->addChild(mBottomGuiLayer);

    TextLabelInfo scoreTitleInfo = { Localization::scoreTitle.c_str(), fontSize, 0.1f, 0.35f };
    TextLabelInfo movesTitleInfo = { Localization::movesTitle.c_str(), fontSize, 0.5f, 0.75f };
    mTopGuiLayer->addChild(createLabel(scoreTitleInfo));
    mTopGuiLayer->addChild(createLabel(movesTitleInfo));    

    const char* tmpStr = "999999999";
    TextLabelInfo scoreInfo = { tmpStr, fontSize + 4, 0.335f, 0.35f };
    TextLabelInfo movesInfo = { tmpStr, fontSize + 8, 0.5f, 0.35f };
    
    mScoreLabel = createLabel(scoreInfo);
    mMovesLabel = createLabel(movesInfo);
    mMovesLabel->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);


    mTopGuiLayer->addChild(mScoreLabel);
    mTopGuiLayer->addChild(mMovesLabel);
}

//--------------------------------------------------------------------
void _GuiManager::createShuffleButton()
//--------------------------------------------------------------------
{
    mShuffleButton = ui::Button::create(GameResources::s_ButtonImg.getCString());
    mShuffleButton->setPositionType(Widget::PositionType::PERCENT);
    mShuffleButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    mShuffleButton->setPositionPercent(Vec2(0.5f, 0.5f));
    mShuffleButton->setTitleFontName(GameResources::s_fontYellow.getCString());
    mShuffleButton->setTitleText(Localization::shuffleTitle);
    mShuffleButton->setTitleFontSize(28);    
    mShuffleButton->setScale9Enabled(true);
    mShuffleButton->setScale(0.8f);
    mBottomGuiLayer->addChild(mShuffleButton);
}

//--------------------------------------------------------------------
void _GuiManager::createScoreBar()
//--------------------------------------------------------------------
{
    auto background = cocos2d::Sprite::create(GameResources::s_scoreBarBackgroundImg.getCString());
    auto border = cocos2d::Sprite::create(GameResources::s_scoreBarForegroundImg.getCString());
    border->setAnchorPoint(Vec2(0.0, 0.0));
    border->setPosition(Vec2(0.0, 0.0));    

    mScoreBar = ProgressTimer::create(background);
    mScoreBar->setType(ProgressTimerType::BAR);
    mScoreBar->setAnchorPoint(Vec2(0.0f, 0.5f));
    mScoreBar->setPosition(Vec2(0.0f, 0.0f));
    mScoreBar->setBarChangeRate(Vec2(1, 0));
    mScoreBar->setMidpoint(Vec2(0.0, 0.0));
    mScoreBar->setPercentage(5);
    mScoreBar->addChild(border, 10);// , kBar);

    auto widget = cocos2d::ui::Widget::create();
    widget->setPositionType(Widget::PositionType::PERCENT);
    widget->setPositionPercent(Vec2(0.025f, 0.75f));
    widget->setAnchorPoint(Vec2(-0.5f, 0));
    widget->addChild(mScoreBar, 5);
    widget->setScale(0.45f);
    
    mTopGuiLayer->addChild(widget, 5);// , kBorder);
}

//--------------------------------------------------------------------
void _GuiManager::createLevelGoals()
//--------------------------------------------------------------------
{
    int8_t fontSize = 28;
    auto widget = cocos2d::ui::Widget::create();
    widget->setPositionType(Widget::PositionType::PERCENT);
    widget->setPositionPercent(Vec2(0.85f, 0.65f));
    widget->setAnchorPoint(Vec2(0, 0));
    widget->setScale(0.9f);
    mTopGuiLayer->addChild(widget, 5);// , kBorder);

    auto levelGoals = mLevelGoalComponent->getLevelGoals();
    auto goalsCount = levelGoals.goalsCount;
    auto collectGoals = levelGoals.collectGoals;

    for (uint8_t i = 0; i < collectGoals.size(); i++) {
        auto goal = collectGoals.at(i);
        auto goalSprite = createSprite(goal.baseObjectType, goal.objectType);
        auto pos = getPosForGoalSprite(i, goalsCount, goalSprite->getContentSize());
        goalSprite->setPosition(pos);
        goalSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        widget->addChild(goalSprite, 5);

        auto tmpStr = cocos2d::String::createWithFormat("%d", goal.targetCount)->getCString();
        TextLabelInfo goalInfo = { tmpStr, fontSize + 4, 0, 0 };

        auto label = createLabel(goalInfo);
        label->setAnchorPoint(Vec2(0.5f, 1.9f));
        label->setPositionType(ui::Widget::PositionType::ABSOLUTE);
        label->setPosition(Vec2(pos.x, pos.y));
        label->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        widget->addChild(label, 10);
        mGoalsLabels.push_back(label);
    }
}

//--------------------------------------------------------------------
void _GuiManager::setShuffleButtonCallback(std::function<void()> touchEndedCallback)
//--------------------------------------------------------------------
{
    if (mShuffleButton) {
        mShuffleButton->addTouchEventListener([touchEndedCallback](Ref* pSender, ui::Widget::TouchEventType type) {
            if (type == Widget::TouchEventType::ENDED) {
                touchEndedCallback();
            }
        });
    }
}

//--------------------------------------------------------------------
void _GuiManager::updateScore(uint32_t value, float percentage)
//--------------------------------------------------------------------
{
    if (mScoreLabel) {
        mScoreLabel->setString(StringUtils::toString(value));
    }
    
    if (mScoreBar) {
        percentage *= 100.0f;
        if (percentage > 100.0f) percentage = 100.0f;
        if (percentage < 5.0f) percentage = 5.0f;
        
        mScoreBar->runAction(ProgressFromTo::create(1.0f, mScoreBar->getPercentage(), percentage));
    }   
}

//--------------------------------------------------------------------
void _GuiManager::updateMovesLabel(int value)
//--------------------------------------------------------------------
{
    if (!mMovesLabel)
        return;

    mMovesLabel->setString(StringUtils::toString(value));
}

//--------------------------------------------------------------------
void _GuiManager::updateLevelGoals(std::vector<CommonTypes::CollectGoalInfo>& levelGoals)
//--------------------------------------------------------------------
{
    for (uint8_t i = 0; i < mGoalsLabels.size(); i++) {
        auto goalLabel = mGoalsLabels.at(i);
        auto goalInfo = levelGoals.at(i);
        auto targetObjLeft = goalInfo.targetCount - goalInfo.currentCount;
        if (targetObjLeft < 0) targetObjLeft = 0;

        auto tmpStr = cocos2d::String::createWithFormat("%d", targetObjLeft)->getCString();
        goalLabel->setString(tmpStr);
    }
}

//--------------------------------------------------------------------
cocos2d::Sprite * _GuiManager::createSprite(int baseType, int objType)
//--------------------------------------------------------------------
{
    cocos2d::Sprite* sprite = nullptr;
    cocos2d::String* str = nullptr;
    auto baseObjType = static_cast<CommonTypes::BaseObjType>(baseType);
    switch (baseObjType)
    {
    case CommonTypes::BaseObjType::Cookie:
        str = &GameResources::s_cookieSpriteNames.at(objType);
        break;
    case CommonTypes::BaseObjType::Field:
    case CommonTypes::BaseObjType::Dude:
        str = Helper::getSpriteNameByTileType(objType);
        break;
    case CommonTypes::BaseObjType::Tile:
    case CommonTypes::BaseObjType::Unknown:
        break;
    default:
        break;
    }

    if (str) {
        sprite = cocos2d::Sprite::create(str->getCString());
        sprite->setAnchorPoint(Vec2(0.0, 0.0));
        sprite->setPosition(Vec2(0.0, 0.0));
    }
    return sprite;
}

//--------------------------------------------------------------------
cocos2d::Vec2 _GuiManager::getPosForGoalSprite(int currGoal, int goalsCount, const cocos2d::Size& spriteSize)
//--------------------------------------------------------------------
{
    cocos2d::Vec2 pos = cocos2d::Vec2::ZERO;
    if (goalsCount == 1) {
        pos = cocos2d::Vec2(-spriteSize.width * 0.25f, 0);
    }
    if (goalsCount == 2) {
        if (currGoal == 0) {
            pos = cocos2d::Vec2(-spriteSize.width * 0.75f, 0);
        }
        else if (currGoal == 1) {
            pos = cocos2d::Vec2(spriteSize.width * 0.75f, 0);
        }
    }
    else if (goalsCount == 3) {
        if (currGoal == 0) {
            pos = cocos2d::Vec2(-spriteSize.width * 1.1f, 0);
        }
        else if (currGoal == 2) {
            pos = cocos2d::Vec2(spriteSize.width * 1.1f, 0);
        }
    }
    return pos;
}

 //--------------------------------------------------------------------
cocos2d::ui::Text * _GuiManager::createLabel(const CommonTypes::TextLabelInfo& info)
//--------------------------------------------------------------------
{
    //TODO: use in future cocostudio::GUIReader::shareReader()->widgetFromJsonFile()

    Text* text = Text::create(info.text, GameResources::s_fontYellow.getCString(), info.fontSize);
    text->ignoreContentAdaptWithSize(false);
    text->setTextColor(Color4B::GRAY);
    text->setPositionType(ui::Widget::PositionType::PERCENT);
    text->setPositionPercent(Vec2(info.posXPercent, info.posYPercent));
    text->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    return text;
}
