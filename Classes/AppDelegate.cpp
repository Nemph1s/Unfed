#include "AppDelegate.h"
#include "Common/GlobalInfo/GlobalInfo.h"
#include "Controller/ViewController.h"

USING_NS_CC;

//--------------------------------------------------------------------
AppDelegate::AppDelegate()
    : mController(nullptr)
//--------------------------------------------------------------------
{
}

//--------------------------------------------------------------------
AppDelegate::~AppDelegate()
//--------------------------------------------------------------------
{
    CC_SAFE_RELEASE_NULL(mController);
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
//--------------------------------------------------------------------
void AppDelegate::initGLContextAttrs()
//--------------------------------------------------------------------
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
//--------------------------------------------------------------------
static int register_all_packages()
//--------------------------------------------------------------------
{
    return 0; //flag for packages manager
}

//--------------------------------------------------------------------
bool AppDelegate::applicationDidFinishLaunching()
//-------------------------------------------------------------------- 
{
    GlobInfo->init();

    register_all_packages();

    // create a scene. it's an autorelease object
    mController = ViewController::create();
    
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
//--------------------------------------------------------------------
void AppDelegate::applicationDidEnterBackground()
//--------------------------------------------------------------------
{
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
//--------------------------------------------------------------------
void AppDelegate::applicationWillEnterForeground()
//--------------------------------------------------------------------
{
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
