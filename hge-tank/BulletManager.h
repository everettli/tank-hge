#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include "Bullet.h"

class BulletManager
{
private:
	int		iTank_Bullet_Amount;	//̹�˵���װ����
	int		iTank_SPBullet_Amount;
public:
	vector<Bullet>	bullets;			//̹�˵�ҩ��
public :
	BulletManager();
	~BulletManager(){};
	void initBulletManager(Bullet bullet, int amount);
	void shoot(vertexPos vBullet_Pos,VertexDirec vBullet_direction);				//�����Ϊ���ȥ���ӵ����ñ�Ҫ�Ĳ���
	//void recollectBullet();						//�����ӵ�����������
	void update(float fDetalTime,vector<ObstacleSprite>* pObstacles);				//���´��ȥ���ӵ�������
	void render();
	int getTankBulletAmount(){return iTank_Bullet_Amount;};
	int getTankSPBulletAmount();
};

#endif