#include "BulletManager.h"

BulletManager::BulletManager()
{
	this->iTank_Bullet_Amount = this->iTank_SPBullet_Amount = 0;
}

//初始化子弹管理器
void BulletManager::initBulletManager(Bullet bullet, int amount)
{
	for(int i=0; i < amount; i++)
	{
	/*	Bullet tempBullet(bullet);*/
		bullets.push_back(Bullet(bullet));
	}
	this->iTank_Bullet_Amount = this->iTank_SPBullet_Amount = amount;
}

//射出一枚子弹
void BulletManager::shoot(vertexPos vBullet_Pos,VertexDirec vBullet_direction)
{
	for(int i = 0; i < iTank_Bullet_Amount; i++)
	{
		if(!bullets[i].getIsFired())
		{
			bullets[i].shot(vBullet_Pos,vBullet_direction);
			//decreaseSPBulletAmount();
			break;
		}
	}
}

//更新发射出去的子弹
void BulletManager::update(float fDetalTime,vector<ObstacleSprite>* pObstacles)
{
	for(int i = 0; i < iTank_Bullet_Amount; i++)
	{
		bullets[i].update(fDetalTime,pObstacles);
	}
}

//渲染发射出去的子弹
void BulletManager::render()
{
	for(int i = 0; i < iTank_Bullet_Amount; i++)
	{
		bullets[i].render();
	}
}

int BulletManager::getTankSPBulletAmount()
{
	int sum = 0;
	for(int i = 0; i< iTank_Bullet_Amount;i++)
	{
		if(!bullets[i].getIsFired())
		{
			sum++;
		}
	}
	return sum;
}




