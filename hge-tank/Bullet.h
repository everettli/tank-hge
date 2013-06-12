#ifndef BULLET_H
#define BULLET_H

#include <string>
#include <vector>

using namespace std;

#include "hge.h"
#include "hgefont.h"
#include "hgeanim.h"
#include "hgeparticle.h"



#include "ObstacleSprite.h"

/*以屏幕方向一致，顺时针方向*/
#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4

//rect偏移缩小量
#define OFFSET_VALUE 10



typedef struct vertexDirec
{
	int x;
	int y;
}VertexDirec;

class Bullet
{
private:
	int				iBullet_ID;				//子弹ID
	string			sBullet_Name;			//子弹名
	VertexPos		vBullet_Pos;			//子弹位置
	VertexDirec		vBullet_direction;		//朝向
	int				iBullet_Bounce_Amount;	//子弹可反弹次数
	int				iBullet_SPBounce_Amount;//子弹剩余反弹次数
	float			fBullet_Speed;			//子弹的速度
	int				iBullet_Power;			//子弹威力
	bool			isExplosed;				
	bool			isFired;				//是否被射出去
	hgeSprite*		sprite;					//精灵元
	HTEXTURE		tex;						
	HEFFECT			explosnd,firesnd;		//音效
	char *			exploparfilename;
	char *			moveparfilename;
	hgeParticleSystem*	explopar;			//爆炸粒子
	hgeParticleSystem*	movepar;			//飞行例子
	hgeRect*		rect1;
	hgeRect*		rect2;
	float			bullet_tex_width;		//纹理的宽
	float			bullet_tex_height;		//纹理的高
	hgeFont			*fnt;
public:
	Bullet();
	Bullet(const Bullet & bullet);
	~Bullet(){};
	/*LOAD*/
	//void loadResource(int iBullet_ID,string sBullet_Name,int iBullet_Bounce_Amount,int iBullet_Power,float fBullet_Speed,HTEXTURE	tex,HEFFECT	explosnd,HEFFECT firesnd,hgeParticleSystem*	explopar,hgeParticleSystem*	movepar);
	void loadResource(int iBullet_ID,string sBullet_Name,int iBullet_Bounce_Amount,int iBullet_Power,float fBullet_Speed,HTEXTURE	tex,HEFFECT	explosnd,HEFFECT firesnd,char* exploparfilename, char * moveparfilename);
	void initBulllet();
	/*SET*/
	void setStartPos(VertexPos vBullet_Pos);
	void setDirections(VertexDirec vBullet_direction);

	/*COLLISION*/
	//bool collisionWithTank(TankSprite *enemy);//与敌方坦克的碰撞检测，执行爆炸操作
	bool collisionWithObstacles(vector<ObstacleSprite>* pObstacles);//包含与障碍物的碰撞检测，执行检测是否达到爆炸点或反转方向
	
	void shot(vertexPos vBullet_Pos,VertexDirec vBullet_direction);
	void update(float fDetalTime,vector<ObstacleSprite>* pObstacles);			//更新子弹位置，粒子效果更新，执行碰撞检测
	void render();							//渲染

	/*GET*/
	int getBullet_Power(){return iBullet_Power;};	//得到子弹的威力
	hgeRect * getBulletRect();						//得到子弹的RECT
	//bool getIsExplosed(){return isExplosed; };	//是否子弹爆炸，用于子弹管理器中回收子弹
	bool getIsFired(){return isFired;};				//是否子弹被射出去，用于子弹管理器中回收子弹
	void bulletBoomb();							//爆炸，被碰撞检测方法调用
private:
	//void bulletBoomb();						//爆炸，被碰撞检测方法调用
	void reserveDirection(vector<ObstacleSprite>::iterator obstaclesprite);			//反转方向
	void decreaseSPBounceAmount();				//减少剩余碰撞次数
	bool isBulletSPBounceAmountOver();			//检查是否达到起爆点
	bool collisionWithRect(hgeRect * rect1,hgeRect * rect2);	//检测两rect是否有重叠部分
	int getCollisionSuface(VertexPos vSelfHotSpot, VertexPos vOtherHotSpot); //获取子弹与物体碰撞时，子弹的碰撞面
	
};

#endif