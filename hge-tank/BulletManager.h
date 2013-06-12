#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include "Bullet.h"

class BulletManager
{
private:
	int		iTank_Bullet_Amount;	//坦克的总装弹量
	int		iTank_SPBullet_Amount;
public:
	vector<Bullet>	bullets;			//坦克弹药库
public :
	BulletManager();
	~BulletManager(){};
	void initBulletManager(Bullet bullet, int amount);
	void shoot(vertexPos vBullet_Pos,VertexDirec vBullet_direction);				//射出，为打出去的子弹设置必要的参数
	//void recollectBullet();						//回收子弹，用于重用
	void update(float fDetalTime,vector<ObstacleSprite>* pObstacles);				//更新打出去的子弹的属性
	void render();
	int getTankBulletAmount(){return iTank_Bullet_Amount;};
	int getTankSPBulletAmount();
};

#endif