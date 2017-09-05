#include "ScoreScene.h"
#include "GameScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
#define database UserDefault::getInstance()
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

USING_NS_CC;

cocos2d::Scene* ScoreScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ScoreScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool ScoreScene::init() {
	if (!Layer::init())
	{
		return false;
	}

	Size size = Director::getInstance()->getVisibleSize();
	visibleHeight = size.height;
	visibleWidth = size.width;

	// Todo: Ìí¼Ó±³¾°Í¼Æ¬
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto bgSprite = Sprite::create("background.jpg");
	bgSprite->setPosition(Vec2(origin.x + visibleWidth / 2, origin.y + visibleHeight / 2));
	this->addChild(bgSprite, 0);

	ScoreScene_field = TextField::create("", "Arial", 30);
	ScoreScene_field->setText(Global::score);
	ScoreScene_field->setPosition(Size(visibleWidth / 4, visibleHeight / 4 * 3));
	this->addChild(ScoreScene_field, 2);

	submit_button = Button::create();
	submit_button->setTitleText("Submit");
	submit_button->setTitleFontSize(30);
	submit_button->setPosition(Size(visibleWidth / 4, visibleHeight / 4));
	submit_button->addClickEventListener(CC_CALLBACK_1(ScoreScene::submitRequest, this));
	this->addChild(submit_button, 2);

	rank_field = TextField::create("", "Arial", 30);
	rank_field->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4 * 3));
	this->addChild(rank_field, 2);

	rank_button = Button::create();
	rank_button->setTitleText("Rank");
	rank_button->setTitleFontSize(30);
	rank_button->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4));
	rank_button->addClickEventListener(CC_CALLBACK_1(ScoreScene::rankRequest, this));
	this->addChild(rank_button, 2);

	play_again = Button::create();
	play_again->setTitleText("Play Again");
	play_again->setTitleFontSize(30);
	play_again->setPosition(Size(visibleWidth / 5 * 2.5, visibleHeight / 6));
	play_again->addClickEventListener(CC_CALLBACK_1(ScoreScene::PlayAgain, this));
	this->addChild(play_again, 2);

	return true;
}

void ScoreScene::submitRequest(Ref * pSender)
{
	if (ScoreScene_field->getString() != "") {
		HttpRequest* request = new HttpRequest();
		request->setUrl("http://localhost:3000/score");
		request->setRequestType(HttpRequest::Type::POST);
		request->setResponseCallback(CC_CALLBACK_2(ScoreScene::submitRequestCompleted, this));

		string postdata = "{\"username\":\"" + Global::userName + "\",\"cookie\":\""
			+ Global::gameSessionId + "\",\"score\":\"" + ScoreScene_field->getString() + "\"}";

		request->setRequestData(postdata.c_str(), postdata.size());

		vector<string> headers;
		headers.push_back("Cookie: session=" + Global::gameSessionId + "\"");
		headers.push_back("Content-Type: application/json");
		request->setHeaders(headers);

		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
	}

}

void ScoreScene::submitRequestCompleted(HttpClient * sender, HttpResponse * response)
{
	if (!response) return;
	if (!response->isSucceed()) {
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	string res_header(response->getResponseHeader()->begin(), response->getResponseHeader()->end());
	string res_body(response->getResponseData()->begin(), response->getResponseData()->end());
	string sss = res_body;
}

void ScoreScene::rankRequest(Ref * pSender)
{
	HttpRequest * request = new HttpRequest();
	request->setUrl("http://localhost:3000/score");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(ScoreScene::rankRequestCompleted, this));

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void ScoreScene::rankRequestCompleted(HttpClient * sender, HttpResponse * response)
{
	if (response == nullptr) return;
	if (response->isSucceed()) {
		string res_header(response->getResponseHeader()->begin(), response->getResponseHeader()->end());
		string res_body(response->getResponseData()->begin(), response->getResponseData()->end());

		string result = res_body.c_str();
		for (int i = 0; i < result.size(); i++) {
			if (result[i] == '|') {
				result[i] = '\n';
			}
		}
		rank_field->setText(result);
	}
	else {
		rank_field->setText("");
		log("error buffer: %s", response->getErrorBuffer());
	}
}

void ScoreScene::PlayAgain(Ref * pSender)
{
	auto game = GameScene::createScene();
	game->init();
	Director::getInstance()->replaceScene(TransitionFade::create(0.1, game));
}
