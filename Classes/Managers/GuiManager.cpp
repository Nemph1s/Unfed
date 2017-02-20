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

    TextLabelInfo scoreTitleInfo = { Localization::scoreTitle.c_str(), fontSize, 0.075f, 0.75f };
    TextLabelInfo targetTitleInfo = { Localization::targetTitle.c_str(), fontSize, 0.09f, 0.55f };
    TextLabelInfo movesTitleInfo = { Localization::movesTitle.c_str(), fontSize, 0.85f, 0.75f };
    mTopGuiLayer->addChild(createLabel(scoreTitleInfo));
    mTopGuiLayer->addChild(createLabel(targetTitleInfo));
    mTopGuiLayer->addChild(createLabel(movesTitleInfo));    

    const char* tmpStr = "999999999";
    TextLabelInfo scoreInfo = { tmpStr, fontSize + 4, 0.315f, 0.75f };
    TextLabelInfo targetInfo = { tmpStr, fontSize + 4, 0.315f, 0.55f };
    TextLabelInfo movesInfo = { tmpStr, fontSize + 4, 1.05f, 0.75f };
    
    mScoreLabel = createLabel(scoreInfo);
    mTargetLabel = createLabel(targetInfo);
    mMovesLabel = createLabel(movesInfo);

    mTopGuiLayer->addChild(mScoreLabel);
    mTopGuiLayer->addChild(mTargetLabel);
    mTopGuiLayer->addChild(mMovesLabel);
}

//--------------------------------------------------------------------
void _GuiManager::createShuffleButton()
//--------------------------------------------------------------------
{
    mShuffleButton = ui::Button::create(GameResources::s_ButtonImg.getCString());
    mShuffleButton->setPositionType(Widget::PositionType::PERCENT);
    mShuffleButton->setPositionPercent(Vec2(0.5f, 0.5f));
    mShuffleButton->setTitleFontName(GameResources::s_fontYellow.getCString());
    mShuffleButton->setTitleText(Localization::shuffleTitle);
    mShuffleButton->setTitleFontSize(28);    
    mShuffleButton->setScale9Enabled(true);
    mShuffleButton->setScale(0.8f);
    mBottomGuiLayer->addChild(mShuffleButton);
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
void _GuiManager::updateScoreLabel(uint32_t value)
//--------------------------------------------------------------------
{
    if (!mScoreLabel)
        return;

    mScoreLabel->setString(StringUtils::toString(value));
}

//--------------------------------------------------------------------
void _GuiManager::updateTargetScoreLabel(uint32_t value)
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
    text->setTextColor(Color4B::GRAY);
    text->setPositionType(ui::Widget::PositionType::PERCENT);
    text->setPositionPercent(Vec2(info.posXPercent, info.posYPercent));
    text->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    return text;
}
