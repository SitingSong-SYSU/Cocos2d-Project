#include "GameScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

USING_NS_CC;

cocos2d::Scene* GameScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::create(0);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameScene::init(int num) {
    if (!Layer::init())
    {
        return false;
    }
	//创建背景图片
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto background = Sprite::create("bg.jpg");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(background, 0);

	scoreNumber = num;
	char c[10];
	itoa(scoreNumber, c, 10);
	tempNum = c;
	Label* label1 = Label::createWithTTF("Scores: " + tempNum, "fonts/arial.TTF", 50);
	label1->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4 * 3 + 50));
	this->addChild(label1, 2);

	submit_button = Button::create();
	submit_button->setTitleText("Submit");
	submit_button->setTitleFontName("fonts/arial.TTF");
	submit_button->setTitleFontSize(50);
	submit_button->setPosition(Size(visibleSize.width / 4, visibleSize.height / 4));
	submit_button->addClickEventListener(CC_CALLBACK_1(GameScene::submitButtonCallBack, this));
	this->addChild(submit_button, 2);

	rank_field = TextField::create("", "fonts/arial.TTF", 30);
	rank_field->setPosition(Size(visibleSize.width / 4 * 3, visibleSize.height / 4 * 3));
	this->addChild(rank_field, 2);

	rank_button = Button::create();
	rank_button->setTitleText("Rank");
	rank_button->setTitleFontName("fonts/arial.TTF");
	rank_button->setTitleFontSize(50);
	rank_button->setPosition(Size(visibleSize.width / 4 * 3, visibleSize.height / 4));
	rank_button->addClickEventListener(CC_CALLBACK_1(GameScene::rankButtonCallBack, this));
	this->addChild(rank_button, 2);

    return true;
}


void GameScene::submitButtonCallBack(cocos2d::Ref* pSender) {
	//发送网络请求，方式为POST
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/submit");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequestCompleted, this));

	//发送分数
	string post_str = "score=" + tempNum;
	const char * postData = post_str.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST Submit");

	//发送GAMESESSIONID
	vector<string> headers;
	headers.push_back("Cookie: GAMESESSIONID=" + Global::gameSessionId);
	request->setHeaders(headers);
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameScene::rankButtonCallBack(cocos2d::Ref* pSender) {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/rank?top=10");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequestCompleted, this));
	request->setTag("GET Rank");

	vector<string> headers;
	headers.push_back("Cookie: GAMESESSIONID=" + Global::gameSessionId);
	request->setHeaders(headers);

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameScene::onHttpRequestCompleted(HttpClient* sender, HttpResponse* response) {
	//submit 信息处理
	if (!response) {
		return;
	}
	if (!response->isSucceed()) {
		log("response failed!");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	std::vector<char> * buffer = response->getResponseData();
	string responseData = Global::toString(buffer);
	log(responseData.c_str());

	//rank 信息处理
	const char* tag_type = response->getHttpRequest()->getTag();
	if (!strcmp(tag_type, "GET Rank")) {
		rapidjson::Document doc;
		doc.Parse<0>(responseData.c_str());
		if (doc.HasParseError()) {
			log("GetParseError: %s", doc.GetParseError());
		}
		if (doc.IsObject() && doc.HasMember("info")) {
			string rank_info = doc["info"].GetString();
			log(rank_info.c_str());

			vector<string> rankInfo_vec;
			string tempstr = rank_info.substr(1);
			int index = 0;
			while (tempstr != "") {
				index = tempstr.find('|');
				string find_str = tempstr.substr(0, index);
				rankInfo_vec.push_back(find_str);
				tempstr = tempstr.substr(index + 1);
			}
			string rank_field_text = "";
			for (index = 0; index < rankInfo_vec.size(); index++) {
				rank_field_text += (rankInfo_vec[index] + "\n");
			}
			rank_field->setText(rank_field_text);
		}
	}
}