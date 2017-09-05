#include "Block.h"
#include<string>
USING_NS_CC;

Factory* Factory::factory = NULL;
Factory::Factory() {
	Size size = Director::getInstance()->getVisibleSize();
	visibleHeight = size.height;
	visibleWidth = size.width;
	origin = Director::getInstance()->getVisibleOrigin();
}

Factory * Factory::getInstance()
{
	if (factory == NULL) {
		factory = new Factory();
	}
	return factory;
}

Sprite * Factory::createMonster(char type)
{
	std::string block("block/block");
	block.push_back(type);
	block += ".jpg";
	Sprite* blc = Sprite::create(block);
	Blocks.pushBack(blc);
	return blc;
}

void Factory::Fall(Sprite* sp, float time)
{
	float xpos = sp->getPosition().x;
	sp->runAction(MoveTo::create(time, Vec2(xpos, origin.y - visibleHeight / 3)));
}

bool Factory::collide(Rect rect) {
	Rect area = Rect(rect.getMinX() - 30, rect.getMinY() - 20, rect.getMaxX() - rect.getMinX() + 60, rect.getMaxY() - rect.getMinY() + 40);
	for each(Sprite* s in Blocks) {
		if (area.containsPoint(s->getPosition())) {
			return true;
		}
	}
	return false;
}

Vector<Sprite*> Factory::getBlocks()
{
	return Blocks;
}


