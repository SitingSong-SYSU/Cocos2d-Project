#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include "Global.h"
using namespace cocos2d::ui;

#include "network/HttpClient.h"
using namespace cocos2d::network;

using std::string;

class ScoreScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(ScoreScene);

	void submitRequest(Ref* pSender);
	void submitRequestCompleted(HttpClient* sender, HttpResponse *res);

	void rankRequest(Ref* pSender);
	void rankRequestCompleted(HttpClient* sender, HttpResponse *res);

	void PlayAgain(Ref* pSender);

private:
	float visibleHeight;
	float visibleWidth;
	TextField * ScoreScene_field;
	TextField * rank_field;
	Button *submit_button;
	Button *rank_button;
	Button *play_again;
};