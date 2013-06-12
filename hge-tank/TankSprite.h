#ifndef TANKSPRITE_H
#define TANKSPRITE_H

#include "hge.h"
#include "hgefont.h"
#include "hgeanim.h"

#include "BulletManager.h"
#include <math.h>

#define SQRT2  1.4

typedef struct keyStruct
{
	int up;
	int down;
	int left;
	int right;
	bool isFire;
}KeyStruct;

class TankSprite
{
private:
	int				iTank_ID;			//坦克代号
	string			sTank_Name;			//坦克名
	int				iTank_Blood;		//坦克总血量
	int				iTank_RTBlood;		//坦克剩余血量
	float			fTank_Speed;		//坦克移动速度
	VertexPos		vTank_Pos;			//坦克位置
	VertexDirec		vTank_Direction;	//坦克朝向
	string			player;				//玩家
	float			fDetalTime;			//detal时间
	float			rot;
	HTEXTURE		animtex;
	HEFFECT			movesnd;			//坦克移动音效
	hgeAnimation*	pAnim;				//动画精灵元
	int				nframes;			//动画的帧数
	hgeRect *		rect1;
	hgeRect *		rect2;
	hgeRect *		rect3;
	KeyStruct		validKeys;			//用于标识坦克实际响应的按键：0不响应，1响应
	float			tank_tex_width;		//坦克贴图的宽
	float			tank_tex_height;	//坦克贴图的高
	hgeFont			*fnt;
	int				iBloodPercent;		//血量的百分值
	
public:
	BulletManager	bulletManager;		//子弹管理器

public:
	TankSprite();
	TankSprite(const TankSprite & tankSprite);
	~TankSprite(){};
	/*LOAD&INIT*/
	void loadResource(int iTank_ID, string sTank_Name, int iTank_Blood, float fTank_Speed, HTEXTURE animtex,int nframes);
	void initTank();

	/*SET functions*/
	void setBulletManager(Bullet bullet, int amount); //setBulletManager
	void setStartPosAndDirection(float vTank_PosX,float vTank_PosY,int vTank_DirectionX,int vTank_DirectionY);
	void setPlayer(string playername);	

	/*GET functions*/
	string getTankName(){return sTank_Name;};
	int getTank_RTBlood(){return iTank_RTBlood;};
	bool isGameOver(){return (iTank_RTBlood <= 0);};
	hgeRect * getTankRect();
	VertexPos getTankPos(){return vTank_Pos;};
	
	/*UPDATE*/
	void update(float fDetalTime,KeyStruct* pKeys,TankSprite* pEnemy,vector<ObstacleSprite>* pObstacles);
	
	/*RENDER*/
	void render();

	/*COLLISION*/
	bool collisionWithTank(TankSprite* pEnemy);		//包含与炮弹和坦克的碰撞检测
	bool collisionWithObstacles(vector<ObstacleSprite>* pObstacles);//包含与障碍物的碰撞检测

private:
	void decreaseBlood(int bulletPower);
	void keyHandle(KeyStruct* pKeys);		//处理按键:上、下、左、右、开火
	bool collisionWithRect(hgeRect * rect1,hgeRect * rect2);	//检测两rect是否相碰
	int getCollisionSuface(VertexPos vSelfHotSpot, VertexPos vOtherHotSpot);//获得碰撞面
	void directionToRotation();			//方向向量转角度
	VertexDirec rotationToDirection();		//旋转角度转方向向量
	VertexPos getAccurateBulletOutPos();		//获取精确的子弹发出位置
	void lockKey(int direction);		//忽略并锁定指定方向上的按键
	void unlockKey();		//解锁恢复所有按键的可按性
};

#endif