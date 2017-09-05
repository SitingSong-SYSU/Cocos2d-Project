#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include "Global.h"
#include "FallBlock.h"
using namespace cocos2d::ui;

using std::string;

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

	void fall(char type, int dir, float time);
	void update(float t);
	void checkMeet();
	void decideWhichToFall();
	void keyHandle();
	void gameOver();
	void gameRestart();
	void toTheScoreScene(Ref* pSender);
	void addListener();
	void onKeyPressed(EventKeyboard::KeyCode code, Event * event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event * event);

private:
    float visibleHeight;
    float visibleWidth;
	Vec2 origin;
	Sprite* redBall;
	Sprite* blueBall;
	bool a, d;
	int score;
	Label* scoreNum; //�÷�
	int maxScore;
	Label* maxScoreNum; //���÷�
	int maxBlockNum; //�����䷽������
	int difficulity; //�Ѷ�
	int invincible; //��Ч״̬��������ʱ�������ײ

	//���������Ȳ���
	int cnt;
	int countBlock;
	int wait;
};

