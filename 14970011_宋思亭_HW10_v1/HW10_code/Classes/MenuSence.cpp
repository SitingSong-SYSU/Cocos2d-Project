#include "MenuSence.h"
#include "GameSence.h"
USING_NS_CC;

Scene* MenuSence::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MenuSence::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuSence::init()
{
	if (!Layer::init()) {
		return false;
	}

	//创建背景
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float winw = visibleSize.width; //获取屏幕宽度
	auto background_sky = Sprite::create("menu-background-sky.jpg");
	background_sky->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 150));
	float maxWidth = background_sky->getTextureRect().getMaxX();
	background_sky->setScaleX(winw / maxWidth); //设置缩放比例
	this->addChild(background_sky, 0);

	auto background = Sprite::create("menu-background.png");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 50));
	maxWidth = background->getTextureRect().getMaxX();
	background->setScaleX(winw / maxWidth); //设置缩放比例
	this->addChild(background, 0);

	//创建人物
	auto miner = Sprite::create("menu-miner.png");
	miner->setPosition(Vec2(150 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(miner, 1);

	//创建腿部动画
	auto leg = Sprite::createWithSpriteFrameName("miner-leg-0.png");
	Animate* legAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("legAnimation"));
	leg->runAction(RepeatForever::create(legAnimate));
	leg->setPosition(110 + origin.x, origin.y + 102);
	this->addChild(leg, 1);

	//创建Logo
	auto logo = Sprite::create("gold-miner-text.png");
	logo->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - logo->getContentSize().height - 80));
	this->addChild(logo, 1);

	//创建开始按钮底部
	auto gold = Sprite::create("menu-start-gold.png");
	gold->setPosition(Vec2(origin.x + visibleSize.width - 200, origin.y + gold->getContentSize().height / 2));
	this->addChild(gold, 1);

	//创建开始按钮，点击开始按钮跳转
	auto start_button = MenuItemImage::create(
		"start-0.png",
		"start-1.png",
		CC_CALLBACK_1(MenuSence::startMenuCallback, this));
	start_button->setPosition(Vec2(origin.x + visibleSize.width - 200, origin.y + gold->getContentSize().height / 2 + 50));
	auto start = Menu::create(start_button, NULL);
	start->setPosition(Vec2::ZERO);
	this->addChild(start, 2);

	return true;
}

void MenuSence::startMenuCallback(cocos2d::Ref * pSendera)
{
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, GameSence::createScene()));
}
