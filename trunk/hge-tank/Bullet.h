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

/*����Ļ����һ�£�˳ʱ�뷽��*/
#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4

//rectƫ����С��
#define OFFSET_VALUE 10



typedef struct vertexDirec
{
	int x;
	int y;
}VertexDirec;

class Bullet
{
private:
	int				iBullet_ID;				//�ӵ�ID
	string			sBullet_Name;			//�ӵ���
	VertexPos		vBullet_Pos;			//�ӵ�λ��
	VertexDirec		vBullet_direction;		//����
	int				iBullet_Bounce_Amount;	//�ӵ��ɷ�������
	int				iBullet_SPBounce_Amount;//�ӵ�ʣ�෴������
	float			fBullet_Speed;			//�ӵ����ٶ�
	int				iBullet_Power;			//�ӵ�����
	bool			isExplosed;				
	bool			isFired;				//�Ƿ����ȥ
	hgeSprite*		sprite;					//����Ԫ
	HTEXTURE		tex;						
	HEFFECT			explosnd,firesnd;		//��Ч
	char *			exploparfilename;
	char *			moveparfilename;
	hgeParticleSystem*	explopar;			//��ը����
	hgeParticleSystem*	movepar;			//��������
	hgeRect*		rect1;
	hgeRect*		rect2;
	float			bullet_tex_width;		//����Ŀ�
	float			bullet_tex_height;		//����ĸ�
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
	//bool collisionWithTank(TankSprite *enemy);//��з�̹�˵���ײ��⣬ִ�б�ը����
	bool collisionWithObstacles(vector<ObstacleSprite>* pObstacles);//�������ϰ������ײ��⣬ִ�м���Ƿ�ﵽ��ը���ת����
	
	void shot(vertexPos vBullet_Pos,VertexDirec vBullet_direction);
	void update(float fDetalTime,vector<ObstacleSprite>* pObstacles);			//�����ӵ�λ�ã�����Ч�����£�ִ����ײ���
	void render();							//��Ⱦ

	/*GET*/
	int getBullet_Power(){return iBullet_Power;};	//�õ��ӵ�������
	hgeRect * getBulletRect();						//�õ��ӵ���RECT
	//bool getIsExplosed(){return isExplosed; };	//�Ƿ��ӵ���ը�������ӵ��������л����ӵ�
	bool getIsFired(){return isFired;};				//�Ƿ��ӵ������ȥ�������ӵ��������л����ӵ�
	void bulletBoomb();							//��ը������ײ��ⷽ������
private:
	//void bulletBoomb();						//��ը������ײ��ⷽ������
	void reserveDirection(vector<ObstacleSprite>::iterator obstaclesprite);			//��ת����
	void decreaseSPBounceAmount();				//����ʣ����ײ����
	bool isBulletSPBounceAmountOver();			//����Ƿ�ﵽ�𱬵�
	bool collisionWithRect(hgeRect * rect1,hgeRect * rect2);	//�����rect�Ƿ����ص�����
	int getCollisionSuface(VertexPos vSelfHotSpot, VertexPos vOtherHotSpot); //��ȡ�ӵ���������ײʱ���ӵ�����ײ��
	
};

#endif