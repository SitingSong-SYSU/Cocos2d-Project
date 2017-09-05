#include "LoginScene.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Global.h"
#include "Thunder.h"
#include <regex>
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
USING_NS_CC;

using namespace cocostudio::timeline;

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;

Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoginScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	//创建背景图片
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto background = Sprite::create("bg.jpg");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(background, 0);

	//创建用户名输入框
	textField = TextField::create("Input your name", "fonts/arial.TTF", 50);
	textField->setPosition(Size(visibleSize.width / 2, visibleSize.height / 4 * 3));
	this->addChild(textField, 2);

	//创建登录按钮
	auto button = Button::create();
	button->setTitleText("Login");
	button->setTitleFontName("fonts/arial.TTF");
	button->setTitleFontSize(50);
	button->setPosition(Size(visibleSize.width / 2, visibleSize.height / 2));
	button->addClickEventListener(CC_CALLBACK_1(LoginScene::loginButtonCallBack, this));
	this->addChild(button, 2);

    return true;
}


void LoginScene::loginButtonCallBack(cocos2d::Ref* pSender) {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/login");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onHttpRequestCompleted, this));
	string textField_str = textField->getString();
	string post_str = "username=" + textField_str;
	const char * postData = post_str.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST Login");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void LoginScene::onHttpRequestCompleted(HttpClient* sender, HttpResponse* response) {
	//处理response数据保存到global中
	if (!response) {
		return;
	}
	if (!response->isSucceed()) {
		log("response failed!\nerror buffer: %s", response->getErrorBuffer());
		return;
	}
	std::vector<char> * buffer = response->getResponseData();
	std::vector<char> * headertmp = response->getResponseHeader();
	string responseData = Global::toString(buffer);
	string header = Global::toString(headertmp);
	Global::gameSessionId = Global::getSessionIdFromHeader(header);
	log(responseData.c_str());

	//跳转到游戏场景
	auto scene = Scene::create();
	auto thunder = Thunder::create();
	scene->addChild(thunder);
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

