#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class Factory:public cocos2d::Ref {
public:
	static Factory* getInstance();
	Sprite* createMonster(char type);
	void Fall(Sprite* sp, float time);
	bool collide(Rect);
	Vector<Sprite*> getBlocks();

private:
	Factory();
	Vector<Sprite*> Blocks;
	cocos2d::Vector<SpriteFrame*> deadBlock;
	static Factory* factory; 
	float visibleHeight;
	float visibleWidth;
	Vec2 origin;
};