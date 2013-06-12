#ifndef OBSTACLESPRITE_H
#define OBSTACLESPRITE_H

#include "hgesprite.h"

typedef struct vertexPos
{
	float x;
	float y;
}VertexPos;

class ObstacleSprite 
{
public:
	ObstacleSprite(hgeSprite * sprite,float x, float y);
	hgeSprite * getSpriteUnit(){return spr;};
	VertexPos getObstaclePos(){return vObstaclePos;};
	void render();
private:
	hgeSprite *	spr;
	VertexPos vObstaclePos;
};

#endif