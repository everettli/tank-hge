#include "ObstacleSprite.h"

ObstacleSprite::ObstacleSprite(hgeSprite * sprite,float x, float y)
{
	spr = new hgeSprite(0,0,0,0,0);
	memcpy(spr,sprite,sizeof(hgeSprite));
	this->vObstaclePos.x = x;
	this->vObstaclePos.y = y;
}

void ObstacleSprite::render()
{
	spr->Render(vObstaclePos.x,vObstaclePos.y);
}
