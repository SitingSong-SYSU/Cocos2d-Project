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
	Label* scoreNum; //得分
	int maxScore;
	Label* maxScoreNum; //最大得分
	int maxBlockNum; //最大掉落方块数量
	int difficulity; //难度
	int invincible; //无效状态，大于零时不检测碰撞

	//方块掉落调度参数
	int cnt;
	int countBlock;
	int wait;
};

