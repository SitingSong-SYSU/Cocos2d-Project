#include "GameScene.h"
#include "ScoreScene.h"
#include <iostream>
#include "Block.h"
#include "SimpleAudioEngine.h"
#include <cmath>
#include <WinBase.h>
#define PI 3.14159265
using namespace cocos2d::ui;
using namespace CocosDenshion;
USING_NS_CC;
/*
ÿ0.05���һ����ײ
ÿ5һ�η������
*/
void GameScene::update(float t) {
	decideWhichToFall();
	keyHandle();
	checkMeet();

	//���·���
	char str[40];
	sprintf(str, "Score: %12d", score++);
	scoreNum->setString(str);
	if (score > maxScore) {
		maxScore = score;
		sprintf(str, "MaxScore: %12d", maxScore);
		maxScoreNum->setString(str);
	}
	//�����Ѷ�
	difficulity = score / 500;
	if (invincible > 0) invincible--;
}

/*
����Ƿ���Ҫ��ת
*/
void GameScene::keyHandle() {
	static int cnt = 0;
	if (cnt >= 0) {
		cnt = 0;
		if (a) {
			// ��ʱ����ת
			// ��ɫ��
			{
				auto x = redBall->getPosition().x - origin.x - visibleWidth / 2;
				auto y = redBall->getPosition().y - origin.x - visibleHeight / 4;
				float num;
				if (x / (visibleWidth / 4) > -1 && x / (visibleWidth / 4) < 1) {
					num = x / (visibleWidth / 4);
				}
				else if (x / (visibleWidth / 4) <= -1) {
					num = -1;
				}
				else {
					num = 1;
				}
				auto angle = acos(num) * 180 / PI;
				if (y < 0) {
					angle = 360 - angle;
				}
				//CCLOG("-%f %f %f", x, y, angle);
				// ÿ����ת10��
				angle += 10;
				x = visibleWidth / 4 * cos(angle * PI / 180) + origin.x + visibleWidth / 2;
				y = visibleWidth / 4 * sin(angle * PI / 180) + origin.y + visibleHeight / 4;
				//CCLOG("%f %f %f", x, y, angle);
				redBall->runAction(MoveTo::create(0.05f, Vec2(x, y)));
			}
			// ��ɫ��
			{
				auto x = blueBall->getPosition().x - origin.x - visibleWidth / 2;
				auto y = blueBall->getPosition().y - origin.x - visibleHeight / 4;
				float num;
				if (x / (visibleWidth / 4) > -1 && x / (visibleWidth / 4) < 1) {
					num = x / (visibleWidth / 4);
				}
				else if (x / (visibleWidth / 4) <= -1) {
					num = -1;
				}
				else {
					num = 1;
				}
				auto angle = acos(num) * 180 / PI;
				if (y <= 0) {
					angle = 360 - angle;
				}
				//CCLOG("-%f %f %f", x, y, angle);
				// ÿ����ת10��
				angle += 10;
				x = visibleWidth / 4 * cos(angle * PI / 180) + origin.x + visibleWidth / 2;
				y = visibleWidth / 4 * sin(angle * PI / 180) + origin.y + visibleHeight / 4;
				//CCLOG("%f %f %f", x, y, angle);
				blueBall->runAction(MoveTo::create(0.05f, Vec2(x, y)));
			}
			return;
		}
		if (d) {
			// ˳ʱ����ת
			// ��ɫ��
			{
				auto x = redBall->getPosition().x - origin.x - visibleWidth / 2;
				auto y = redBall->getPosition().y - origin.x - visibleHeight / 4;
				float num;
				if (x / (visibleWidth / 4) > -1 && x / (visibleWidth / 4) < 1) {
					num = x / (visibleWidth / 4);
				}
				else if (x / (visibleWidth / 4) <= -1) {
					num = -1;
				}
				else {
					num = 1;
				}
				auto angle = acos(num) * 180 / PI;
				if (y < 0) {
					angle = 360 - angle;
				}
				//CCLOG("-%f %f %f", x, y, angle);
				// ÿ����ת10��
				angle -= 10;
				x = visibleWidth / 4 * cos(angle * PI / 180) + origin.x + visibleWidth / 2;
				y = visibleWidth / 4 * sin(angle * PI / 180) + origin.y + visibleHeight / 4;
				//CCLOG("%f %f %f", x, y, angle);
				redBall->runAction(MoveTo::create(0.05f, Vec2(x, y)));
			}
			// ��ɫ��
			{
				auto x = blueBall->getPosition().x - origin.x - visibleWidth / 2;
				auto y = blueBall->getPosition().y - origin.x - visibleHeight / 4;
				float num;
				if (x / (visibleWidth / 4) > -1 && x / (visibleWidth / 4) < 1) {
					num = x / (visibleWidth / 4);
				}
				else if (x / (visibleWidth / 4) <= -1) {
					num = -1;
				}
				else {
					num = 1;
				}
				auto angle = acos(num) * 180 / PI;
				if (y <= 0) {
					angle = 360 - angle;
				}
				//CCLOG("-%f %f %f", x, y, angle);
				// ÿ����ת10��
				angle -= 10;
				x = visibleWidth / 4 * cos(angle * PI / 180) + origin.x + visibleWidth / 2;
				y = visibleWidth / 4 * sin(angle * PI / 180) + origin.y + visibleHeight / 4;
				//CCLOG("%f %f %f", x, y, angle);
				blueBall->runAction(MoveTo::create(0.05f, Vec2(x, y)));
			}
			return;
		}
	}
	else {
		cnt++;
	}
}

//��Ϸ����
void GameScene::gameOver()
{
	//��ֹ���е���
	unschedule(schedule_selector(GameScene::update));
	this->getEventDispatcher()->removeAllEventListeners();
	scoreNum->setVisible(false);
	maxScoreNum->setVisible(false);
	//�رձ�������
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	SimpleAudioEngine::getInstance()->playEffect("music/sound.mp3");
	//��������
	auto overBackground = Sprite::create("gameOver.png");
	overBackground->setPosition(Vec2(origin.x + visibleWidth / 2, origin.y + visibleHeight / 2));
	this->addChild(overBackground, 2);
	//��������label
	auto FinalScore = Label::createWithTTF("FinalScore: ", "fonts/arial.TTF", 40);
	FinalScore->setColor(Color3B(0, 0, 0));
	FinalScore->setPosition(Vec2(origin.x + visibleWidth / 2, origin.y + visibleHeight / 2));
	this->addChild(FinalScore, 3);
	char str[40];
	sprintf(str, "FinalScore: %d", maxScore);
	FinalScore->setString(str);
	std::stringstream ss;
	ss << maxScore;
	ss >> Global::score;
	//������Ϸ����label3             
	auto gameOver = Label::createWithTTF("Game Over!", "fonts/arial.TTF", 60);
	gameOver->setColor(Color3B(255, 255, 255));
	gameOver->setPosition(Vec2(origin.x + visibleWidth / 2, origin.y + visibleHeight * 7 / 8));
	this->addChild(gameOver, 3);
	
	//���CLICK TO SUBMIT�����ϴ��ύ��������
	auto clickToSubmit = Button::create();
	clickToSubmit->setTitleText("CLICK TO SUBMIT");
	clickToSubmit->setTitleFontSize(30);
	clickToSubmit->setPosition(Vec2(origin.x + visibleWidth / 2, origin.y + visibleHeight * 7 / 10));
	clickToSubmit->addClickEventListener(CC_CALLBACK_1(GameScene::toTheScoreScene, this));
	this->addChild(clickToSubmit, 3);
}

void GameScene::gameRestart()
{
	auto block = Factory::getInstance();
	auto blocks = block->getBlocks();
	for (auto it = blocks.begin(); it != blocks.end();) {
		Sprite* removeBlock = *it;
		removeChild(removeBlock);
		it = blocks.erase(it);
	}
	scoreNum->setVisible(true);
	maxScoreNum->setVisible(true);
	redBall->setPosition(Vec2(origin.x + visibleWidth * 3 / 4, origin.y + visibleHeight * 1 / 4));
	blueBall->setPosition(Vec2(origin.x + visibleWidth * 1 / 4, origin.y + visibleHeight * 1 / 4));
	
	score = 0;
	cnt = 0;
	countBlock = difficulity + 3;
	wait = 0;
	difficulity = 0;
}

/*
�����ĸ�������䣬������������ʱ��
*/
void GameScene::decideWhichToFall() {
	// Todo: ������Ⱥ���
	if (cnt >= 25) {
		cnt = 0;
		if (countBlock < difficulity + 3) {
			countBlock++;
			if (CCRANDOM_0_1() < 0.2) //20%����̷���
				fall('1', (int)(CCRANDOM_0_1() * 3) - 1, 20.0 / (float)(difficulity + 10)); //�������ҵ���̷���
			else 
				fall('0', CCRANDOM_0_1() * 2 - 1 > 0 ? 1 : -1, 20.0 / (float)(difficulity + 10)); //�����ҵ��䳤����
		} else {
			if (wait < 2) {
				wait++;
			} else {
				wait = 0;
				countBlock = 0;
			}
		}
		return;
	} else {
		cnt++;
	}
}

/*
����Ƿ�����ײ
*/
void GameScene::checkMeet() {
	// ����Ƿ�����ײ
	auto factory = Factory::getInstance();
	if (invincible == 0 && (factory->collide(redBall->getBoundingBox()) || factory->collide(blueBall->getBoundingBox()))) {
		gameOver();
	}
}

/*
type �����������ַ���
dir ��������λ��
time ������������ʱ��
*/
void GameScene::fall(char type, int dir, float time)
{
	auto block = Factory::getInstance();
	auto m = block->createMonster(type);
	m->setPosition(Vec2(origin.x + visibleWidth / 2 + dir * visibleWidth / 4, origin.y + visibleHeight));
	this->addChild(m, 1);
	block->Fall(m, time);
}

void GameScene::toTheScoreScene(Ref* pSender)
{
	auto scoreScene = ScoreScene::createScene();
	scoreScene->init();
	Director::getInstance()->replaceScene(scoreScene);
}