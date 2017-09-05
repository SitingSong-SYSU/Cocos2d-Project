#include "HelloWorldScene.h"
#include "Monster.h"
#include <string>

#pragma execution_character_set("utf-8")
#define database UserDefault::getInstance()
USING_NS_CC;
using namespace std;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a girl to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
	float winw = visibleSize.width;  // 获取屏幕宽度
	float winh = visibleSize.height; // 获取屏幕高度

	//创建地图
	TMXTiledMap* tmx = TMXTiledMap::create("background.tmx");
	tmx->setPosition(winw / 2, winh / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(tmx, 0);

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + winw / 2, origin.y + winh /2));
	addChild(player, 3);

	//击杀怪物数量
	killNum = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 40);
	//周期性调用调度器
	schedule(schedule_selector(HelloWorld::updateKill), 0.1f, kRepeatForever, 0);
	schedule(schedule_selector(HelloWorld::creatMonster), 5.0f, kRepeatForever, 0);
	schedule(schedule_selector(HelloWorld::moveMonster), 5.0f, kRepeatForever, 0);
	schedule(schedule_selector(HelloWorld::hitByMonster), 0.5f, kRepeatForever, 0);
	num = database->getIntegerForKey("killNum");
	killNum->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - killNum->getContentSize().height - 20));
	addChild(killNum);

	//设置hp条progressBar
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));
	timer = ProgressTimer::create(sp);
	timer->setScaleX(90);
	timer->setAnchorPoint(Vec2(0, 0));
	timer->setType(ProgressTimerType::BAR);
	timer->setBarChangeRate(Point(1, 0));
	timer->setMidpoint(Point(0, 1));
	timer->setPercentage(100);
	timer->setPosition(Vec2(origin.x + 14 * timer->getContentSize().width,origin.y + visibleSize.height - 2 * timer->getContentSize().height));
	addChild(timer,1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + timer->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0,0);

	//静态动画
	idle.reserve(1);
	idle.pushBack(frame0);

	//攻击动画
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113*i,0,113,113)));
		attack.pushBack(frame);
	}
	
	//死亡动画
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
	}
	
	//运动动画
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	run.reserve(9);
	for (int i = 0; i < 9; i++) {
		if (i < 8) {
			auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
			run.pushBack(frame);
		} else {
			auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 68, 101)));
			run.pushBack(frame);
		}
	}
	
	//Label
	auto menuLabel1 = Label::createWithTTF("W", "fonts/Marker Felt.ttf", 40);
	auto menuLabel2 = Label::createWithTTF("S", "fonts/Marker Felt.ttf", 40);
	auto menuLabel3 = Label::createWithTTF("A", "fonts/Marker Felt.ttf", 40);
	auto menuLabel4 = Label::createWithTTF("D", "fonts/Marker Felt.ttf", 40);
	auto menuLabel6 = Label::createWithTTF("Y", "fonts/Marker Felt.ttf", 40);
	//menuItem
	auto item1 = MenuItemLabel::create(menuLabel1, CC_CALLBACK_1(HelloWorld::moveEvent, this,'W'));
	auto item2 = MenuItemLabel::create(menuLabel2, CC_CALLBACK_1(HelloWorld::moveEvent, this,'S'));
	auto item3 = MenuItemLabel::create(menuLabel3, CC_CALLBACK_1(HelloWorld::moveEvent, this,'A'));
	auto item4 = MenuItemLabel::create(menuLabel4, CC_CALLBACK_1(HelloWorld::moveEvent, this,'D'));
	auto item6 = MenuItemLabel::create(menuLabel6, CC_CALLBACK_1(HelloWorld::actionEvent, this, 'Y'));
	//setPosition
	item3->setPosition(Vec2(origin.x+item3->getContentSize().width,origin.y+item3->getContentSize().height));
	item4->setPosition(Vec2(item3->getPosition().x + 3 * item4->getContentSize().width, item3->getPosition().y));
	item2->setPosition(Vec2(item3->getPosition().x + 1.5*item2->getContentSize().width, item3->getPosition().y));
	item1->setPosition(Vec2(item2->getPosition().x, item2->getPosition().y + item1->getContentSize().height));
	item6->setPosition(Vec2(origin.x+visibleSize.width-item6->getContentSize().width,item1->getPosition().y));
	
	auto menu = Menu::create(item1, item2, item3, item4, item6, NULL);
	menu->setPosition(Vec2(0, 0));
	addChild(menu, 1);
	
	return true;
}


void HelloWorld::moveEvent(Ref*, char cid) {
	if (actionOn && !isEnd) {
		actionOn = false;
		auto s = Director::getInstance()->getWinSize();
		auto position = player->getPosition();
		auto animation_run = Animation::createWithSpriteFrames(run, 0.05f);
		auto animate_run = Animate::create(animation_run);
		int x = position.x;
		int y = position.y;
		switch (cid) {
			case 'W':
				y += 50;
				if (y > s.height - 20) {
					y = s.height - 20;
				}
				break;
			case 'A':
				x -= 50;
				if (x < origin.x + 20) {
					x = origin.x + 20;
				}
				if (!rotate) {
					player->setRotationY(180);
					rotate = true;
				}
				break;
			case 'S':
				y -= 50;
				if (y < origin.y + 20) {
					y = origin.y + 20;
				}
				break;
			case 'D':
				x = x + 50;
				if (x >= s.width - 20) {
					x = s.width - 20;
				}
				if (rotate) {
					player->setRotationY(0);
					rotate = false;
				}
				break;
		}
		FiniteTimeAction *runAction = MoveTo::create(0.5, Point(x, y));
		FiniteTimeAction *repeatRunAction = Repeat::create(animate_run, 1);
		auto stopAction = CallFunc::create(CC_CALLBACK_0(HelloWorld::stopAc, this));
		player->runAction(Sequence::create(Spawn::create(runAction, repeatRunAction, NULL), stopAction, NULL));
	}
}

void HelloWorld::actionEvent(Ref*, char cid) {
	if (actionOn && !isEnd) {
		FiniteTimeAction *deadAction = Repeat::create(Animate::create(Animation::createWithSpriteFrames(dead, 0.1f)), 1);
		FiniteTimeAction *attackAction = Repeat::create(Animate::create(Animation::createWithSpriteFrames(attack, 0.1f)), 1);
		FiniteTimeAction *idleAction = Repeat::create(Animate::create(Animation::createWithSpriteFrames(idle, 0.1f)), 1);
		auto stopAction = CallFunc::create(CC_CALLBACK_0(HelloWorld::stopAc, this));
		if (actionOn) {
			actionOn = false;
			player->runAction(Sequence::create(attackAction, idleAction, stopAction, NULL));
			if (attackmonster()) {
				if (hp < 100) {
					hp = hp + 20 >= 100 ? 100 : hp + 20;
				}
				num++;
				database->setIntegerForKey("killNum", num);
			}
		}
		CCProgressTo* progress = CCProgressTo::create(2, hp);
		timer->runAction(progress);
	}
}

void HelloWorld::stopAc() {
	actionOn = true;
}

void HelloWorld::updateKill(float dt) {
	String* s = String::createWithFormat("%d", num);
	string str = s->getCString();
	killNum->setString(str);
}

void HelloWorld::creatMonster(float dt) {
	auto fac = Factory::getInstance();
	auto newMonster = fac->createMonster();
	newMonster->setPosition(random(origin.x, visibleSize.width), random(origin.y, visibleSize.height));
	addChild(newMonster, 2);
}

void HelloWorld::moveMonster(float dt) {
	auto position = player->getPosition();
	Factory::getInstance()->moveMonster(position, 4.0f);
}

void HelloWorld::hitByMonster(float dt) {
	auto fac = Factory::getInstance();
	Sprite* collision = fac->collider(player->getBoundingBox());
	if (collision != NULL) {
		actionOn = true;
		FiniteTimeAction *deadAction = Repeat::create(Animate::create(Animation::createWithSpriteFrames(dead, 0.1f)), 1);
		FiniteTimeAction *idleAction = Repeat::create(Animate::create(Animation::createWithSpriteFrames(idle, 0.1f)), 1);
		auto stopAction = CallFunc::create(CC_CALLBACK_0(HelloWorld::stopAc, this));
		if (hp > 0) {
			hp = hp - 20 <= 0 ? 0 : hp - 20;
		} 
		if (hp <= 0) {
			timer->runAction(Sequence::create(CCProgressTo::create(2, 0), CallFunc::create([this]() {
				player->runAction(Sequence::create(ScaleTo::create(2.0, 1.0), FadeOut::create(1.0), nullptr)); // 人物消失
				auto over = Sprite::create("over.png");
				float winw = visibleSize.width;  // 获取屏幕宽度
				float winh = visibleSize.height; // 获取屏幕高度
				over->setPosition(Vec2(winw / 2 + origin.x, winh / 2 + origin.y));
				float spx = over->getTextureRect().getMaxX();
				over->setScaleX(winw / spx);//  背景缩放
				over->setScaleY(winw / spx);
				this->addChild(over, 2);
				isEnd = true;
				num = 0;
				database->setIntegerForKey("killNum", num);
			}), nullptr));
			return;
		}
		fac->removeMonster(collision);
		CCProgressTo* progress = CCProgressTo::create(2, hp);
		timer->runAction(progress);
		player->runAction(Sequence::create(deadAction, idleAction, stopAction, NULL));
		removeChild(collision);
	}
}

bool HelloWorld::attackmonster() {
	Rect girlRect = player->getBoundingBox();
	Rect attackRect = Rect(girlRect.getMinX() - 40, girlRect.getMinY(),
		girlRect.getMaxX() - girlRect.getMinX() + 80, girlRect.getMaxY() - girlRect.getMinY());
	Sprite* collision = Factory::getInstance()->collider(attackRect);
	if (collision != NULL) {
		removeChild(collision);
		Factory::getInstance()->removeMonster(collision);
	}
	return collision != NULL;
}
