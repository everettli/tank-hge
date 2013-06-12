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
	int				iTank_ID;			//̹�˴���
	string			sTank_Name;			//̹����
	int				iTank_Blood;		//̹����Ѫ��
	int				iTank_RTBlood;		//̹��ʣ��Ѫ��
	float			fTank_Speed;		//̹���ƶ��ٶ�
	VertexPos		vTank_Pos;			//̹��λ��
	VertexDirec		vTank_Direction;	//̹�˳���
	string			player;				//���
	float			fDetalTime;			//detalʱ��
	float			rot;
	HTEXTURE		animtex;
	HEFFECT			movesnd;			//̹���ƶ���Ч
	hgeAnimation*	pAnim;				//��������Ԫ
	int				nframes;			//������֡��
	hgeRect *		rect1;
	hgeRect *		rect2;
	hgeRect *		rect3;
	KeyStruct		validKeys;			//���ڱ�ʶ̹��ʵ����Ӧ�İ�����0����Ӧ��1��Ӧ
	float			tank_tex_width;		//̹����ͼ�Ŀ�
	float			tank_tex_height;	//̹����ͼ�ĸ�
	hgeFont			*fnt;
	int				iBloodPercent;		//Ѫ���İٷ�ֵ
	
public:
	BulletManager	bulletManager;		//�ӵ�������

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
	bool collisionWithTank(TankSprite* pEnemy);		//�������ڵ���̹�˵���ײ���
	bool collisionWithObstacles(vector<ObstacleSprite>* pObstacles);//�������ϰ������ײ���

private:
	void decreaseBlood(int bulletPower);
	void keyHandle(KeyStruct* pKeys);		//������:�ϡ��¡����ҡ�����
	bool collisionWithRect(hgeRect * rect1,hgeRect * rect2);	//�����rect�Ƿ�����
	int getCollisionSuface(VertexPos vSelfHotSpot, VertexPos vOtherHotSpot);//�����ײ��
	void directionToRotation();			//��������ת�Ƕ�
	VertexDirec rotationToDirection();		//��ת�Ƕ�ת��������
	VertexPos getAccurateBulletOutPos();		//��ȡ��ȷ���ӵ�����λ��
	void lockKey(int direction);		//���Բ�����ָ�������ϵİ���
	void unlockKey();		//�����ָ����а����Ŀɰ���
};

#endif