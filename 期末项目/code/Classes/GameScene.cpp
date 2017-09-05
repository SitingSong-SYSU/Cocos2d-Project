#include "GameScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "SimpleAudioEngine.h"
#include <regex>
#define database UserDefault::getInstance()
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
using namespace CocosDenshion;

USING_NS_CC;

cocos2d::Scene* GameScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameScene::init() {
    if (!Layer::init())
    {
        return false;
    }

	invincible = 0;
	maxBlockNum = 0;
	difficulity = 0;
	cnt = 0;
	countBlock = 0;
	wait = 0;
	a = false;
	d = false;
    Size size = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
    visibleHeight = size.height;
    visibleWidth = size.width;
	schedule(schedule_selector(GameScene::update), 0.05f, kRepeatForever, 0);
	// Todo: 添加背景音乐
	auto Audio = SimpleAudioEngine::getInstance();
	Audio->playBackgroundMusic("music/bgm.mp3", true);
	// Todo: 添加背景图片
	auto bgSprite = Sprite::create("background.jpg");
	bgSprite->setPosition(Vec2(origin.x + visibleWidth / 2, origin.y + visibleHeight / 2));
	this->addChild(bgSprite, 0);
	// Todo: 添加界面元素
	redBall = Sprite::create("circle/red_circle.png");
	blueBall = Sprite::create("circle/blue_circle.png");
	redBall->setPosition(Vec2(origin.x + visibleWidth * 3 / 4, origin.y + visibleHeight * 1 / 4));
	blueBall->setPosition(Vec2(origin.x + visibleWidth * 1 / 4, origin.y + visibleHeight * 1 / 4));
	this->addChild(redBall, 1);
	this->addChild(blueBall, 1);

	// Todo: 添加键盘事件
	addListener();

	//显示分数
	score = 0;
	scoreNum = Label::createWithTTF("Score:            0", "fonts/arial.TTF", 20);
	scoreNum->setColor(Color3B(255, 255, 255));
	scoreNum->setPosition(Vec2(origin.x + 80, origin.y + visibleHeight - 80));
	this->addChild(scoreNum, 1);

	maxScore = 0;
	maxScoreNum = Label::createWithTTF("MaxScore:            0", "fonts/arial.TTF", 20);
	maxScoreNum->setColor(Color3B(255, 255, 255));
	maxScoreNum->setPosition(Vec2(origin.x + 100, origin.y + visibleHeight - 120));
	this->addChild(maxScoreNum, 1);
    return true;
}

void GameScene::addListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	// 小球旋转事件，传进来的参数决定旋转方向
	switch ((int)code) {
	case 124:
	case 26:
		a = true;
		break;
	case 127:
	case 27:
		d = true;
		break;
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch ((int)code) {
	case 124:
	case 26:
		a = false;
		break;
	case 127:
	case 27:
		d = false;
		break;
	}
}
