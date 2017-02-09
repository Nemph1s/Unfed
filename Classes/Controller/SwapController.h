/**
* @file Controller/SwapController.hpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#pragma once

#include "cocos2d.h"
#include "Utils/PlatformMacros.h"

class SwapObj;
class LevelObj;
class CookieObj;
class GameplayScene;

class SwapController : public cocos2d::Ref
{
CC_CONSTRUCTOR_ACCESS:
    virtual ~SwapController();

public:
    /**
    * Allocates and initializes a node.
    * @return A initialized node which is marked as "autorelease" and "retain".
    */
    static SwapController* create();

    bool init();
    
    /**
    * @brief A method that checks is the cookie[column][row] type equal to forwarded type
    * It will step through the rows and columns of the 2 - D grid and simply swap each cookie with the one next to it, one at a time.
    * If swapping these two cookies creates a chain, it will add a new RWTSwap object to the list of possibleSwaps.
    * Then, it will swap these cookies back to restore the original state and continue with the next cookie until it has swapped them all.
    * It will go through the above steps twice : once to check all horizontal swaps and once to check all vertical swaps.
    */
    bool detectPossibleSwaps();

    bool isPossibleSwap(SwapObj* swap);
    void performSwap(SwapObj* swap);

    bool trySwapCookieTo(int horzDelta, int vertDelta);
                          
protected:
    // Nodes should be created using create();
    SwapController() {};
 
    //---Class Attributes-------------------------------------------------
    CC_SYNTHESIZE(std::function<void(SwapObj* swap)>, mSwapCallback, SwapCallback);

    CC_SYNTHESIZE_PTR(LevelObj*, mLevel, Level);
    CC_SYNTHESIZE_PTR(GameplayScene*, mGameplayScene, GameplayScene);
    CC_SYNTHESIZE_READONLY_PTR(cocos2d::Set*, mPossibleSwaps, PossibleSwaps);
};


