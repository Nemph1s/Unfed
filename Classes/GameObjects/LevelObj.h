/**
* @file GameObjects/LevelObj.hpp
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
#include "Utils/PlatformMacros.h"

using namespace CommonTypes;

class BaseObj;
class SwapObj;
class TileObj;
class CookieObj;
class ChainObj;

class LevelObj : public cocos2d::Node
{
    friend SwapController;
    friend ObjectController;

CC_CONSTRUCTOR_ACCESS:
    virtual ~LevelObj();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease".
    */
    static LevelObj * createWithId(const int16_t& levelId);

    virtual bool initWithId(const int16_t& levelId);

    cocos2d::Set* shuffle();

    TileObj* tileAt(int column, int row);
    CookieObj* cookieAt(int column, int row);

    bool isEmptyTileAt(int column, int row);

    cocos2d::Set* removeMatches();

    cocos2d::Array* useGravityToFillHoles();
    cocos2d::Array* fillTopUpHoles();

    void resetComboMultiplier();

protected:
    // Nodes should be created using create();
    LevelObj() {};

    void createInitialTiles();
    BaseObj* createTile(int column, int row, int type);

    cocos2d::Set* createInitialCookies();
    BaseObj* createCookie(int column, int row, int type);
    int getRandomCookieType(int column, int row);

    bool isSameTypeOfCookieAt(int column, int row, int type);
 
    bool hasChainAt(int column, int row);
    void addChainsFromSetToSet(cocos2d::Set* from, cocos2d::Set* to);

    cocos2d::Set* detectHorizontalMatches();
    cocos2d::Set* detectVerticalMatches();
    cocos2d::Set* detectDifficultMatches(cocos2d::Set* horizontal, cocos2d::Set* vertical);

    ChainObj* detectLChainMatches(ChainObj* horzChain, ChainObj* vertChain);
    ChainObj* detectTChainMatches(ChainObj* horzChain, ChainObj* vertChain);

    void removeCookies(cocos2d::Set* chains);
    void calculateScore(cocos2d::Set* chains);

#ifdef COCOS2D_DEBUG
    void logDebugChains(cocos2d::Set* horizontal, cocos2d::Set* vertical, cocos2d::Set* difficult);
#endif // COCOS2D_DEBUG 

    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<bool()>, mDetectPossibleSwapsCallback, DetectPossibleSwapsCallback);

    CC_SYNTHESIZE_READONLY(int, mComboMultiplier, ComboMultiplier);
    CC_SYNTHESIZE_READONLY(LevelInfo, mLevelInfo, LevelInfo);

    BaseObj* mTiles[NumColumns][NumRows] = { nullptr };
    BaseObj* mCookies[NumColumns][NumRows] = { nullptr };
};
