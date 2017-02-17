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

#include "Scenes/GameplayScene.h"
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
bool _GuiManager::initWithScene(cocos2d::Scene* scene)
//--------------------------------------------------------------------
{
    cocos2d::log("GuiManager::initWithScene:");
    if (scene) {
        mCurrentScene = scene;

        crateInfoLayer();
        createShuffleButton();

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
    auto width = (viewSize.width - (GameResources::TileWidth * CommonTypes::NumColumns)) / 2.2f;

    mLeftGuiLayer = LayerColor::create(Color4B(53, 53, 53, 0));
    mLeftGuiLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    mLeftGuiLayer->setContentSize(Size(width, viewSize.height));
    mCurrentScene->addChild(mLeftGuiLayer);

    mRightGuiLayer = LayerColor::create(Color4B(53, 53, 53, 0));
    mRightGuiLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    mRightGuiLayer->setPosition(Vec2(viewSize.width - width, 0.0f));
    mRightGuiLayer->setContentSize(Size(width, viewSize.height));
    mCurrentScene->addChild(mRightGuiLayer);

    TextLabelInfo scoreTitleInfo = { Localization::scoreTitle.c_str(), fontSize, 0.5f, 0.85f };
    TextLabelInfo targetTitleInfo = { Localization::targetTitle.c_str(), fontSize, 0.5f, 0.725f };
    TextLabelInfo movesTitleInfo = { Localization::movesTitle.c_str(), fontSize, 0.5f, 0.6f };
    mLeftGuiLayer->addChild(createLabel(scoreTitleInfo));
    mLeftGuiLayer->addChild(createLabel(targetTitleInfo));
    mLeftGuiLayer->addChild(createLabel(movesTitleInfo));

    const char* tmpStr = "999999999";
    TextLabelInfo scoreInfo = { tmpStr, fontSize + 4, 0.5f, 0.8f };
    TextLabelInfo targetInfo = { tmpStr, fontSize + 4, 0.5f, 0.675f };
    TextLabelInfo movesInfo = { tmpStr, fontSize + 4, 0.5f, 0.55f };
    
    mScoreLabel = createLabel(scoreInfo);
    mTargetLabel = createLabel(targetInfo);
    mMovesLabel = createLabel(movesInfo);

    mLeftGuiLayer->addChild(mScoreLabel);
    mLeftGuiLayer->addChild(mTargetLabel);
    mLeftGuiLayer->addChild(mMovesLabel);
}

//--------------------------------------------------------------------
void _GuiManager::createShuffleButton()
//--------------------------------------------------------------------
{
    mShuffleButton = ui::Button::create(GameResources::s_ButtonImg.getCString());
    mShuffleButton->setPositionType(Widget::PositionType::PERCENT);
    mShuffleButton->setPositionPercent(Vec2(0.5f, 0.1f));
    mShuffleButton->setTitleFontName(GameResources::s_fontYellow.getCString());
    mShuffleButton->setTitleText(Localization::shuffleTitle);
    mShuffleButton->setTitleFontSize(28);    
    mShuffleButton->setScale9Enabled(true);
    mShuffleButton->setScale(0.8f);
    mLeftGuiLayer->addChild(mShuffleButton);
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
void _GuiManager::updateScoreLabel(int value)
//--------------------------------------------------------------------
{
    if (!mScoreLabel)
        return;

    mScoreLabel->setString(StringUtils::toString(value));
}

//--------------------------------------------------------------------
void _GuiManager::updateTargetScoreLabel(int value)
//--------------------------------------------------------------------
{
    if (!mTargetLabel)
        return;

    mTargetLabel->setString(StringUtils::toString(value));
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
cocos2d::ui::Text * _GuiManager::createLabel(const CommonTypes::TextLabelInfo& info)
//--------------------------------------------------------------------
{
    //TODO: use in future cocostudio::GUIReader::shareReader()->widgetFromJsonFile()

    Text* text = Text::create(info.text, GameResources::s_fontYellow.getCString(), info.fontSize);
    text->ignoreContentAdaptWithSize(false);
    text->setContentSize(Size(280, 150));
    text->setTextColor(Color4B::GRAY);
    text->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    text->setPositionType(ui::Widget::PositionType::PERCENT);
    text->setPositionPercent(Vec2(info.posXPercent, info.posYPercent));

    return text;
}
