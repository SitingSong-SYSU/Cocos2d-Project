#include "AppDelegate.h"
#include "MenuSence.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(960, 640);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Demo", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Demo");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);
    Size frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();



	// load game resource
	char frameName1[25];
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("general-sheet.plist");
	Animation* legAnimation = Animation::create();
	for (int i = 0; i < 3; i++) {
		sprintf(frameName1, "miner-leg-%d.png", i);
		legAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName1));
	}
	legAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(legAnimation, "legAnimation");

	char frameName2[25];
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("level-sheet.plist");
	Animation* mouseAnimation = Animation::create();
	for (int i = 0; i < 8; i++) {
		sprintf(frameName2, "gem-mouse-%d.png", i);
		mouseAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName2));
	}
	mouseAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(mouseAnimation, "mouseAnimation");

	char frameName3[25];
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("level-sheet.plist");
	Animation* stoneAnimation = Animation::create();
	for (int i = 0; i < 2; i++) {
		sprintf(frameName3, "stone-%d.png", i);
		stoneAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName3));
	}
	stoneAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(stoneAnimation, "stoneAnimation");
   
	char frameName4[25];
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("level-sheet.plist");
	Animation* diamondAnimation = Animation::create();
	for (int i = 0; i < 7; i++) {
		sprintf(frameName4, "diamond-%d.png", i);
		diamondAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName4));
	}
	diamondAnimation->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(diamondAnimation, "diamondAnimation");

	// create a scene. it's an autorelease object
    auto scene = MenuSence::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
