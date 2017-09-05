#include"Monster.h"
USING_NS_CC;

Factory* Factory::factory = NULL;
Factory::Factory() {
	initSpriteFrame();
}
Factory* Factory::getInstance() {
	if (factory == NULL) {
		factory = new Factory();
	}
	return factory;
}
void Factory::initSpriteFrame(){
	auto texture = Director::getInstance()->getTextureCache()->addImage("Monster.png");
	monsterDead.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(258-48*i,0,42,42)));
		monsterDead.pushBack(frame);
	}
}

Sprite* Factory::createMonster() {
	Sprite* mons = Sprite::create("Monster.png", CC_RECT_PIXELS_TO_POINTS(Rect(364,0,42,42)));
	monster.pushBack(mons);
	return mons;
}

void Factory::removeMonster(Sprite* sp) {
	Vector<Sprite*>::iterator it = monster.begin();
	for (; it != monster.end();) {
		if (sp == (*it)) {
			it = monster.erase(it);
		}
		else {
			it++;
		}
	}
}
void Factory::moveMonster(Vec2 playerPos,float time){
	Vector<Sprite*>::iterator it = monster.begin();
	for (; it != monster.end(); it++) {
		Vec2 mp = (*it)->getPosition();
		Vec2 direaction = playerPos - mp;
		direaction.normalize();
		(*it)->runAction(MoveBy::create(time, direaction * 30));
	}
}

Sprite* Factory::collider(Rect rect) {
	Vector<Sprite*>::iterator it = monster.begin();
	for (; it != monster.end(); it++) {
		if (rect.containsPoint((*it)->getPosition())) {
			return *it;
		}
	}
	return NULL;
}