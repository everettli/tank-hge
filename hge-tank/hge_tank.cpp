/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut06 - Creating menus
*/


// Copy the files "menu.wav", "font1.fnt", "font1.png",
// "bg.png" and "cursor.png" from the folder "precompiled"
// to the folder with executable file. Also copy hge.dll
// and bass.dll to the same folder.


#include "hge.h"
#include "hgefont.h"
#include "hgegui.h"

#include "menuitem.h"

#include "TankSprite.h"

#include <math.h>

#define MAIN 0
#define SELECT 1
#define PLAY 2
#define PAUSE 3
#define STOP 4
#define TEST 100
#define CR_START 101
#define CR_EXIT 102
#define CR_BACK 103
#define CR_PLAY 104
#define CR_PAUSE 105
#define CR_STOP 106
#define CR_CONTINUE 107
#define CR_BACKTOMAIN 108
#define CR_STOPTOMAIN 109
#define MAX_TIMEPAUSE 350
#define PAUSETIME 50

//**************************************MAIN**************************************GLOBAL_DEF
// Pointer to the HGE interface.
// Helper classes require this to work.
HGE *hge=0;

// Some resource handles    纹理和声音句柄
HEFFECT				snd;
HSTREAM				bgmusic;
HTEXTURE			tex;
hgeQuad				Select_bg_quad;
hgeQuad				Main_bg_quad;

// Pointers to the HGE objects we will use    HGE对象定义
hgeGUI				*gui;
hgeFont				*fnt;
hgeFont				*infofnt;
//hgeSprite			*spr;

//状态定义
static int                 state;
//**************************************MAIN**************************************GLOBAL_DEF


//**************************************SELECT**************************************GLOBAL_DEF
// Handle for a sound effect
HEFFECT Selectsnd;

//************************SELECT_TANK************************
hgeQuad TankP1_quad1;
hgeQuad TankP1_quad2;
hgeQuad TankP1_quad3;

float TankP1_x1=90.0f, TankP1_y1=290.0f;
float TankP1_dpic1 = 0.0f;
float TankP1_pic1 = 90.0f;

float TankP1_x2=90.0f, TankP1_y2=400.0f;
float TankP1_dpic2 = 0.0f;
float TankP1_pic2 = 16.0f;

float TankP1_x3=90.0f, TankP1_y3=510.0f;
float TankP1_dpic3 = 0.0f;
float TankP1_pic3 = 16.0f;

int TankP1_timePause = 0;
int TankP1 = 0;

hgeQuad TankP2_quad1;
hgeQuad TankP2_quad2;
hgeQuad TankP2_quad3;

float TankP2_x1=710.0f, TankP2_y1=310.0f;
float TankP2_dpic1 = 0.0f;
float TankP2_pic1 = 90.0f;

float TankP2_x2=710.0f, TankP2_y2=410.0f;
float TankP2_dpic2 = 0.0f;
float TankP2_pic2 = 16.0f;

float TankP2_x3=710.0f, TankP2_y3=510.0f;
float TankP2_dpic3 = 0.0f;
float TankP2_pic3 = 16.0f;

int TankP2_timePause = 0;
int TankP2 = 0;
//************************SELECT_MAP************************
hgeQuad Map_quad1;
hgeQuad Map_quad2;
hgeQuad Map_quad3;
int Map = 0;

float map_x1=180.0f, map_y1=180.0f;
float map_dx1=0.0f, map_dy1=0.0f;

float map_x2=400.0f, map_y2=400.0f;
float map_dx2=0.0f, map_dy2=0.0f;

float map_x3=620.0f, map_y3=180.0f;
float map_dx3=0.0f, map_dy3=0.0f;

int map_timePause = 0;

float map_dpic1 = 0.0f;
float map_pic1 = 16.0f;

float map_dpic2= 0.0f;
float map_pic2 = 90.0f;

float map_dpic3 = 0.0f;
float map_pic3 = 16.0f;

const float speed=90;
const float friction=0.98f;
const float dt = 0.001f;
const float Mapdt=4.500f;

//**************************************SELECT**************************************GLOBAL_DEF

//**************************************SELECT**************************************GLOBAL_DEF
hgeQuad Pause_quad;
hgeQuad Stop_quad;
//**************************************SELECT**************************************GLOBAL_DEF

//**************************************PLAY**************************************GLOBAL_DEF
/*地图上障碍物的列表*/
vector<ObstacleSprite> *		pObstacles; 
hgeSprite *	sp_wall, * sp_edge_w, *sp_edge_h;
hgeSprite * sp_pig_wall_1, * sp_pig_wall_2, *sp_pig_wall_3, *sp_pig_wall_4;

KeyStruct * P1keyStruct;
KeyStruct * P2keyStruct;
/*玩家控制的坦克*/
TankSprite * pP1Tank = NULL;
TankSprite * pP2Tank = NULL;
/*坦克的弹夹*/
BulletManager * pP1BulletManager;
BulletManager * pP2BulletManager;

/*三种炮弹*/
Bullet	bullet_s;
Bullet  bullet_m;
Bullet  bullet_l;
/*三种坦克*/
TankSprite tank_s;
TankSprite tank_m;
TankSprite tank_l;

/*resource*/
HTEXTURE obstacle_wall,pig_wall;
HTEXTURE obstacle_edge_w,obstacle_edge_h;
HTEXTURE bullet_s_Tex,bullet_m_Tex,bullet_l_Tex;
HTEXTURE tank_s_animtex,tank_m_animtex,tank_l_animtex;
HEFFECT explosnd_s,explosnd_m,explosnd_l;
HEFFECT firesnd_s,firesnd_m,firesnd_l;

/*游戏背景音乐*/
HSTREAM playbgmusic;

//坦克一和坦克二的血量
int tank1blood,tank2blood;

string info="";

int pauseTime1;
int pauseTime2;

HTEXTURE bgtex;
hgeSprite  *bgspr;

//**************************************PLAY**************************************GLOBAL_DEF
void InitMap(int num)
{
	//设定边界
	pObstacles->push_back(ObstacleSprite(sp_edge_w,0,-800));
	pObstacles->push_back(ObstacleSprite(sp_edge_w,0,600));
	pObstacles->push_back(ObstacleSprite(sp_edge_h,-600,0));
	pObstacles->push_back(ObstacleSprite(sp_edge_h,800,0));
	switch(num)
	{
		case 1:
			//设定地图元素
			pObstacles->push_back(ObstacleSprite(sp_pig_wall_2,355,255));
			pObstacles->push_back(ObstacleSprite(sp_pig_wall_1,155,105));
			pObstacles->push_back(ObstacleSprite(sp_pig_wall_3,555,405));
			pObstacles->push_back(ObstacleSprite(sp_pig_wall_4,155,405));
			pObstacles->push_back(ObstacleSprite(sp_pig_wall_2,555,105));
			//设定背景图片
			bgtex = hge->Texture_Load("Images/mapbg1.png");
			//设定坦克初始位置及方向
			pP1Tank->setStartPosAndDirection(100,270,0,0);
			pP2Tank->setStartPosAndDirection(680,270,0,0);
			break;
		case 2:
			//设定地图元素
			pObstacles->push_back(ObstacleSprite(sp_wall,385,0));
			pObstacles->push_back(ObstacleSprite(sp_wall,355,0));
			pObstacles->push_back(ObstacleSprite(sp_wall,415,0));
			pObstacles->push_back(ObstacleSprite(sp_wall,0,0));
			pObstacles->push_back(ObstacleSprite(sp_wall,385,30));
			pObstacles->push_back(ObstacleSprite(sp_wall,385,60));
			pObstacles->push_back(ObstacleSprite(sp_wall,385,160));
			pObstacles->push_back(ObstacleSprite(sp_wall,385,290));
			pObstacles->push_back(ObstacleSprite(sp_wall,385,410));
			pObstacles->push_back(ObstacleSprite(sp_wall,385,570));
			pObstacles->push_back(ObstacleSprite(sp_wall,385,540));
			pObstacles->push_back(ObstacleSprite(sp_wall,385,510));
			pObstacles->push_back(ObstacleSprite(sp_wall,355,570));
			pObstacles->push_back(ObstacleSprite(sp_wall,415,570));
			pObstacles->push_back(ObstacleSprite(sp_wall,770,0));
			pObstacles->push_back(ObstacleSprite(sp_wall,770,570));
			pObstacles->push_back(ObstacleSprite(sp_wall,0,570));
			pObstacles->push_back(ObstacleSprite(sp_wall,163,466));
			pObstacles->push_back(ObstacleSprite(sp_wall,607,466));
			pObstacles->push_back(ObstacleSprite(sp_wall,163,496));
			pObstacles->push_back(ObstacleSprite(sp_wall,607,496));
			pObstacles->push_back(ObstacleSprite(sp_wall,163,90));
			pObstacles->push_back(ObstacleSprite(sp_wall,607,90));
			pObstacles->push_back(ObstacleSprite(sp_wall,163,120));
			pObstacles->push_back(ObstacleSprite(sp_wall,607,120));
			pObstacles->push_back(ObstacleSprite(sp_wall,74,235));
			pObstacles->push_back(ObstacleSprite(sp_wall,496,334));

			//设定背景图片
			bgtex = hge->Texture_Load("Images/mapbg2.png");
			//设定坦克初始位置及方向
			pP1Tank->setStartPosAndDirection(300,400,0,0);
			pP2Tank->setStartPosAndDirection(500,400,0,0);
			break;

		case 3:
			//设定地图元素
			pObstacles->push_back(ObstacleSprite(sp_wall,185,300));
			pObstacles->push_back(ObstacleSprite(sp_wall,185,330));
			pObstacles->push_back(ObstacleSprite(sp_wall,185,360));
			pObstacles->push_back(ObstacleSprite(sp_wall,185,390));
			pObstacles->push_back(ObstacleSprite(sp_wall,185,420));
			pObstacles->push_back(ObstacleSprite(sp_wall,185,450));
			pObstacles->push_back(ObstacleSprite(sp_wall,185,480));
			pObstacles->push_back(ObstacleSprite(sp_wall,185,510));
			pObstacles->push_back(ObstacleSprite(sp_wall,185,540));
			pObstacles->push_back(ObstacleSprite(sp_wall,185,570));
			pObstacles->push_back(ObstacleSprite(sp_wall,585,0));
			pObstacles->push_back(ObstacleSprite(sp_wall,585,30));
			pObstacles->push_back(ObstacleSprite(sp_wall,585,60));
			pObstacles->push_back(ObstacleSprite(sp_wall,585,90));
			pObstacles->push_back(ObstacleSprite(sp_wall,585,120));
			pObstacles->push_back(ObstacleSprite(sp_wall,585,150));
			pObstacles->push_back(ObstacleSprite(sp_wall,585,180));
			pObstacles->push_back(ObstacleSprite(sp_wall,585,210));
			pObstacles->push_back(ObstacleSprite(sp_wall,585,240));
			pObstacles->push_back(ObstacleSprite(sp_wall,585,270));
			pObstacles->push_back(ObstacleSprite(sp_pig_wall_1,355,0));
			pObstacles->push_back(ObstacleSprite(sp_pig_wall_2,355,510));

			//设定背景图片
			bgtex = hge->Texture_Load("Images/mapbg3.png");
			//设定坦克初始位置及方向
			pP1Tank->setStartPosAndDirection(85,435,0,0);
			pP2Tank->setStartPosAndDirection(685,135,0,0);
			break;
	}
	bgspr = new hgeSprite(bgtex,0,0,800,600);
}

void InitPlayRes()
{
	if(pP1Tank != NULL)
	{
		free(pP1Tank);
	}
	if(pP2Tank != NULL)
	{
		free(pP2Tank);
	}
	if(!pObstacles->empty())
	{
		pObstacles->clear();
	}
	if(bgtex != 0)
	{
		hge->Texture_Free(bgtex);
	}
	switch(TankP1)
	{
		case 0:
			pP1Tank = new TankSprite(tank_s);
			break;
		case 1:
			pP1Tank = new TankSprite(tank_m);
			break;
		case 2:
			pP1Tank = new TankSprite(tank_l);
			break;
	}
	switch(TankP2)
	{
		case 0:
			pP2Tank = new TankSprite(tank_s);
			break;
		case 1:
			pP2Tank = new TankSprite(tank_m);
			break;
		case 2:
			pP2Tank = new TankSprite(tank_l);
			break;
	}
	switch(Map)
	{
		case 0:InitMap(1);
			break;
		case 1:InitMap(2);
			break;
		case 2:InitMap(3);
			break;
	}
	pP1Tank->setPlayer("P1");
	pP2Tank->setPlayer("P2");
	//暂停主背景音乐,开始播放游戏音乐
	if(hge->Channel_IsPlaying(bgmusic))
	{
		hge->Channel_Pause(bgmusic);
		hge->Stream_Play(playbgmusic,100);
	}
}

void TankP1Select()
{
	if (hge->Input_GetKeyState(HGEK_S))
		if(TankP1_timePause > MAX_TIMEPAUSE)
		{
			if(TankP1 == 0)
			{
				TankP1_dpic1 = -1.5;//缩小
				TankP1_dpic2 = 1.5;//放大
				TankP1_dpic3 = 0;
			}
			if(TankP1 == 1)
			{
				TankP1_dpic1 = 0;
				TankP1_dpic2 = -1.5;
				TankP1_dpic3 = 1.5;
			}
			if(TankP1 == 2)
			{
				TankP1_dpic1 = 1.5;
				TankP1_dpic2 = 0;
				TankP1_dpic3 = -1.5;
			}
			TankP1 += 1;
			if(TankP1 == 3)
				TankP1 = 0;
			TankP1_timePause = 0;
			hge->Effect_Play(Selectsnd);
		}

	if (hge->Input_GetKeyState(HGEK_W))
		if(TankP1_timePause > MAX_TIMEPAUSE)
		{
			if(TankP1 == 0)
			{
				TankP1_dpic1 = -1.5;//缩小
				TankP1_dpic2 = 0;//放大
				TankP1_dpic3 = 1.5;
			}
			if(TankP1 == 1)
			{
				TankP1_dpic1 = 1.5;
				TankP1_dpic2 = -1.5;
				TankP1_dpic3 = 0;
			}
			if(TankP1 == 2)
			{
				TankP1_dpic1 = 0;
				TankP1_dpic2 = 1.5;
				TankP1_dpic3 = -1.5;
			}
			TankP1 -= 1;
			if(TankP1 == -1)
				TankP1 = 2;
			TankP1_timePause = 0;
			hge->Effect_Play(Selectsnd);
		}

	TankP1_timePause += 1;

	if(TankP1_timePause <= MAX_TIMEPAUSE)
	{
		TankP1_dpic1 *= friction;TankP1_pic1 += TankP1_dpic1;
		TankP1_dpic2 *= friction;TankP1_pic2 += TankP1_dpic2;
		TankP1_dpic3 *= friction;TankP1_pic3 += TankP1_dpic3;
	}

	TankP1_quad1.v[0].x=TankP1_x1-TankP1_pic1; TankP1_quad1.v[0].y=TankP1_y1-TankP1_pic1;
	TankP1_quad1.v[1].x=TankP1_x1+TankP1_pic1; TankP1_quad1.v[1].y=TankP1_y1-TankP1_pic1;
	TankP1_quad1.v[2].x=TankP1_x1+TankP1_pic1; TankP1_quad1.v[2].y=TankP1_y1+TankP1_pic1;
	TankP1_quad1.v[3].x=TankP1_x1-TankP1_pic1; TankP1_quad1.v[3].y=TankP1_y1+TankP1_pic1;

	TankP1_quad2.v[0].x=TankP1_x2-TankP1_pic2; TankP1_quad2.v[0].y=TankP1_y2-TankP1_pic2;
	TankP1_quad2.v[1].x=TankP1_x2+TankP1_pic2; TankP1_quad2.v[1].y=TankP1_y2-TankP1_pic2;
	TankP1_quad2.v[2].x=TankP1_x2+TankP1_pic2; TankP1_quad2.v[2].y=TankP1_y2+TankP1_pic2;
	TankP1_quad2.v[3].x=TankP1_x2-TankP1_pic2; TankP1_quad2.v[3].y=TankP1_y2+TankP1_pic2;

	TankP1_quad3.v[0].x=TankP1_x3-TankP1_pic3; TankP1_quad3.v[0].y=TankP1_y3-TankP1_pic3;
	TankP1_quad3.v[1].x=TankP1_x3+TankP1_pic3; TankP1_quad3.v[1].y=TankP1_y3-TankP1_pic3;
	TankP1_quad3.v[2].x=TankP1_x3+TankP1_pic3; TankP1_quad3.v[2].y=TankP1_y3+TankP1_pic3;
	TankP1_quad3.v[3].x=TankP1_x3-TankP1_pic3; TankP1_quad3.v[3].y=TankP1_y3+TankP1_pic3;
}

void TankP2Select()
{
	if (hge->Input_GetKeyState(HGEK_DOWN))
		if(TankP2_timePause > MAX_TIMEPAUSE)
		{
			if(TankP2 == 0)
			{
				TankP2_dpic1 = -1.5;//缩小
				TankP2_dpic2 = 1.5;//放大
				TankP2_dpic3 = 0;
			}
			if(TankP2 == 1)
			{
				TankP2_dpic1 = 0;
				TankP2_dpic2 = -1.5;
				TankP2_dpic3 = 1.5;
			}
			if(TankP2 == 2)
			{
				TankP2_dpic1 = 1.5;
				TankP2_dpic2 = 0;
				TankP2_dpic3 = -1.5;
			}
			TankP2 += 1;
			if(TankP2 == 3)
				TankP2 = 0;
			TankP2_timePause = 0;
			hge->Effect_Play(Selectsnd);
		}

	if (hge->Input_GetKeyState(HGEK_UP))
		if(TankP2_timePause > MAX_TIMEPAUSE)
		{
			if(TankP2 == 0)
			{
				TankP2_dpic1 = -1.5;//缩小
				TankP2_dpic2 = 0;//放大
				TankP2_dpic3 = 1.5;
			}
			if(TankP2 == 1)
			{
				TankP2_dpic1 = 1.5;
				TankP2_dpic2 = -1.5;
				TankP2_dpic3 = 0;
			}
			if(TankP2 == 2)
			{
				TankP2_dpic1 = 0;
				TankP2_dpic2 = 1.5;
				TankP2_dpic3 = -1.5;
			}
			TankP2 -= 1;
			if(TankP2 == -1)
				TankP2 = 2;
			TankP2_timePause = 0;
			hge->Effect_Play(Selectsnd);
		}

	TankP2_timePause += 1;

	if(TankP2_timePause <= MAX_TIMEPAUSE)
	{
		TankP2_dpic1 *= friction;TankP2_pic1 += TankP2_dpic1;
		TankP2_dpic2 *= friction;TankP2_pic2 += TankP2_dpic2;
		TankP2_dpic3 *= friction;TankP2_pic3 += TankP2_dpic3;
	}

	TankP2_quad1.v[0].x=TankP2_x1-TankP2_pic1; TankP2_quad1.v[0].y=TankP2_y1-TankP2_pic1;
	TankP2_quad1.v[1].x=TankP2_x1+TankP2_pic1; TankP2_quad1.v[1].y=TankP2_y1-TankP2_pic1;
	TankP2_quad1.v[2].x=TankP2_x1+TankP2_pic1; TankP2_quad1.v[2].y=TankP2_y1+TankP2_pic1;
	TankP2_quad1.v[3].x=TankP2_x1-TankP2_pic1; TankP2_quad1.v[3].y=TankP2_y1+TankP2_pic1;

	TankP2_quad2.v[0].x=TankP2_x2-TankP2_pic2; TankP2_quad2.v[0].y=TankP2_y2-TankP2_pic2;
	TankP2_quad2.v[1].x=TankP2_x2+TankP2_pic2; TankP2_quad2.v[1].y=TankP2_y2-TankP2_pic2;
	TankP2_quad2.v[2].x=TankP2_x2+TankP2_pic2; TankP2_quad2.v[2].y=TankP2_y2+TankP2_pic2;
	TankP2_quad2.v[3].x=TankP2_x2-TankP2_pic2; TankP2_quad2.v[3].y=TankP2_y2+TankP2_pic2;

	TankP2_quad3.v[0].x=TankP2_x3-TankP2_pic3; TankP2_quad3.v[0].y=TankP2_y3-TankP2_pic3;
	TankP2_quad3.v[1].x=TankP2_x3+TankP2_pic3; TankP2_quad3.v[1].y=TankP2_y3-TankP2_pic3;
	TankP2_quad3.v[2].x=TankP2_x3+TankP2_pic3; TankP2_quad3.v[2].y=TankP2_y3+TankP2_pic3;
	TankP2_quad3.v[3].x=TankP2_x3-TankP2_pic3; TankP2_quad3.v[3].y=TankP2_y3+TankP2_pic3;
}

void MapSelect()
{
	if (hge->Input_GetKeyState(HGEK_SPACE))
		if(map_timePause > MAX_TIMEPAUSE)
		{
			if(Map == 0)
			{
				map_dx1+=Mapdt;//向右移一个单位
				map_dy1+=Mapdt;//向下移一个单位
				map_dpic1 = 1.5;//放大
				
				map_dx2+=Mapdt;//向右移一个单位
				map_dy2-=Mapdt;//向上移一个单位
				map_dpic2 = -1.5;//缩小

				map_dx3-=2*Mapdt;//向左移两个单位
				map_dpic3 = 0;
			}
			if(Map == 1)
			{
				map_dx1+=Mapdt;
				map_dy1-=Mapdt;
				map_dpic1 = -1.5;

				map_dx2-=2*Mapdt;
				map_dpic2 = 0;

				map_dx3+=Mapdt;
				map_dy3+=Mapdt;
				map_dpic3 = 1.5;
			}
			if(Map == 2)
			{
				map_dx1-=2*Mapdt;
				map_dpic1 = 0;

				map_dx2+=Mapdt;
				map_dy2+=Mapdt;
				map_dpic2 = 1.5;

				map_dx3+=Mapdt;
				map_dy3-=Mapdt;
				map_dpic3 = -1.5;
			}
			Map += 1;
			if(Map == 3)
				Map = 0;
			map_timePause = 0;
			hge->Effect_Play(Selectsnd);
		}

	map_timePause += 1;

	if(map_timePause <= MAX_TIMEPAUSE)
	{
		// Do some movement calculations and collision detection	
		map_dx1*=friction; map_dy1*=friction;
		map_x1+=map_dx1; map_y1+=map_dy1;
		// Set up quad's screen coordinates
		map_dpic1 *= friction;map_pic1 += map_dpic1;

		map_dx2*=friction; map_dy2*=friction;
		map_x2+=map_dx2; map_y2+=map_dy2;
		map_dpic2 *= friction;map_pic2 += map_dpic2;

		map_dx3*=friction; map_dy3*=friction;
		map_x3+=map_dx3; map_y3+=map_dy3;
		map_dpic3 *= friction;map_pic3 += map_dpic3;
	}

	if (hge->Input_GetKeyState(HGEK_M))
		if(map_timePause >= MAX_TIMEPAUSE)
		{
			if(Map == 0)
			{
				map_dx1+=2*Mapdt;
				map_dpic1 = 0;
				
				map_dx2-=Mapdt;
				map_dy2-=Mapdt;
				map_dpic2 = -1.5;

				map_dx3-=Mapdt;
				map_dy3+=Mapdt;
				map_dpic3 = 1.5;
			}
			if(Map == 1)
			{
				map_dx1-=Mapdt;
				map_dy1-=Mapdt;
				map_dpic1 = -1.5;

				map_dx2-=Mapdt;
				map_dy2+=Mapdt;
				map_dpic2 = 1.5;

				map_dx3+=2*Mapdt;
				map_dpic3 =0;
			}
			if(Map == 2)
			{
				map_dx1-=Mapdt;
				map_dy1+=Mapdt;
				map_dpic1 = 1.5;

				map_dx2+=2*Mapdt;
				map_dpic2 = 0;

				map_dx3-=Mapdt;
				map_dy3-=Mapdt;
				map_dpic3 = -1.5;
			}
			Map -= 1;
			if(Map == -1)
				Map = 2;
			map_timePause = 0;
			hge->Effect_Play(Selectsnd);
		}

	Map_quad1.v[0].x=map_x1-map_pic1; Map_quad1.v[0].y=map_y1-map_pic1;
	Map_quad1.v[1].x=map_x1+map_pic1; Map_quad1.v[1].y=map_y1-map_pic1;
	Map_quad1.v[2].x=map_x1+map_pic1; Map_quad1.v[2].y=map_y1+map_pic1;
	Map_quad1.v[3].x=map_x1-map_pic1; Map_quad1.v[3].y=map_y1+map_pic1;
	
	Map_quad2.v[0].x=map_x2-map_pic2; Map_quad2.v[0].y=map_y2-map_pic2;
	Map_quad2.v[1].x=map_x2+map_pic2; Map_quad2.v[1].y=map_y2-map_pic2;
	Map_quad2.v[2].x=map_x2+map_pic2; Map_quad2.v[2].y=map_y2+map_pic2;
	Map_quad2.v[3].x=map_x2-map_pic2; Map_quad2.v[3].y=map_y2+map_pic2;

	Map_quad3.v[0].x=map_x3-map_pic3; Map_quad3.v[0].y=map_y3-map_pic3;
	Map_quad3.v[1].x=map_x3+map_pic3; Map_quad3.v[1].y=map_y3-map_pic3;
	Map_quad3.v[2].x=map_x3+map_pic3; Map_quad3.v[2].y=map_y3+map_pic3;
	Map_quad3.v[3].x=map_x3-map_pic3; Map_quad3.v[3].y=map_y3+map_pic3;
}

bool MAIN_FrameFunc()
{
	float dt=hge->Timer_GetDelta();//获取时间间隔
	//static float t=0.0f;
	//float tx,ty;
	int id;
	static int lastid=0;

	// If ESCAPE was pressed, tell the GUI to finish
	if(hge->Input_GetKeyState(HGEK_ESCAPE)) { lastid=CR_EXIT   ; gui->Leave(); }
	
	// We update the GUI and take an action if
	// one of the menu items was selected
	id=gui->Update(dt);//更新GUI并处理用户输入,返回控件的标识,如果调用了Leave方法 并且所有的控件都完成了结束动画，Update将返回-1.其他情况返回0
	if(id == -1)
	{
		switch(lastid)
		{
			case CR_START:
				gui->ShowCtrl(CR_START,false);
				gui->ShowCtrl(CR_EXIT,false);
				gui->ShowCtrl(CR_BACK,true);
				gui->ShowCtrl(CR_PLAY,true);
				gui->ShowCtrl(CR_CONTINUE,false);
				gui->ShowCtrl(CR_BACKTOMAIN,false);
				gui->ShowCtrl(CR_STOPTOMAIN,false);
				gui->SetFocus(CR_PLAY);
				gui->SetNavMode(HGEGUI_LEFTRIGHT|HGEGUI_CYCLED);
				gui->Enter();
				state = SELECT;
				break;
			case CR_EXIT:
				return true;
		}
	}
	else if(id) 
	{ 
		lastid=id; 
		gui->Leave(); 
	}

	// Here we update our background animation
	// 让背景移动
	//t+=dt;
	//tx=50*cosf(t/60);
	//ty=50*sinf(t/60);

	Main_bg_quad.v[0].tx=0;   Main_bg_quad.v[0].ty=0;
	Main_bg_quad.v[1].tx=1;	  Main_bg_quad.v[1].ty=0;
	Main_bg_quad.v[2].tx=1;	  Main_bg_quad.v[2].ty=1;
	Main_bg_quad.v[3].tx=0;   Main_bg_quad.v[3].ty=1;

	return false;
}



bool SELECT_FrameFunc()
{
	float dt=hge->Timer_GetDelta();//获取时间间隔
	static float t=0.0f;
	float tx,ty;
	t+=dt;
	tx=50*cosf(t/60);
	ty=50*sinf(t/60);

	int id;
	static int lastid=0;

	// If ESCAPE was pressed, tell the GUI to finish
	if(hge->Input_GetKeyState(HGEK_ESCAPE)) { lastid=CR_BACK   ; gui->Leave(); }	
	
	// We update the GUI and take an action if
	// one of the menu items was selected
	id=gui->Update(dt);//更新GUI并处理用户输入,返回控件的标识,如果调用了Leave方法 并且所有的控件都完成了结束动画，Update将返回-1.其他情况返回0
	if(id == -1)
	{
		switch(lastid)
		{
			case CR_BACK:
				gui->ShowCtrl(CR_START,true);
				gui->ShowCtrl(CR_EXIT,true);
				gui->ShowCtrl(CR_BACK,false);
				gui->ShowCtrl(CR_PLAY,false);
				gui->ShowCtrl(CR_CONTINUE,false);
				gui->ShowCtrl(CR_BACKTOMAIN,false);
				gui->ShowCtrl(CR_STOPTOMAIN,false);
				gui->SetNavMode(HGEGUI_UPDOWN|HGEGUI_CYCLED);
				gui->SetFocus(CR_START);
				gui->Enter();
				state = MAIN;
				break;
			case CR_PLAY:
				gui->ShowCtrl(CR_START,false);
				gui->ShowCtrl(CR_EXIT,false);
				gui->ShowCtrl(CR_BACK,false);
				gui->ShowCtrl(CR_PLAY,false);
				gui->ShowCtrl(CR_CONTINUE,false);
				gui->ShowCtrl(CR_BACKTOMAIN,false);
				gui->ShowCtrl(CR_STOPTOMAIN,false);
				gui->SetNavMode(HGEGUI_NONAVKEYS);
				InitPlayRes();
				gui->Enter();
				state = PLAY;
				break;
		}
	}
	else if(id) 
	{ 
		lastid=id; 
		gui->Leave(); 
	}

	/*Select_bg_quad.v[0].tx=tx;        Select_bg_quad.v[0].ty=ty;
	Select_bg_quad.v[1].tx=tx+800/64; Select_bg_quad.v[1].ty=ty;
	Select_bg_quad.v[2].tx=tx+800/64; Select_bg_quad.v[2].ty=ty+600/64;
	Select_bg_quad.v[3].tx=tx;        Select_bg_quad.v[3].ty=ty+600/64;*/

	Select_bg_quad.v[0].tx=0;        Select_bg_quad.v[0].ty=0;
	Select_bg_quad.v[1].tx=1;		 Select_bg_quad.v[1].ty=0;
	Select_bg_quad.v[2].tx=1;		 Select_bg_quad.v[2].ty=1;
	Select_bg_quad.v[3].tx=0;        Select_bg_quad.v[3].ty=1;

	// Get the time elapsed since last call of FrameFunc().
	// This will help us to synchronize on different
	// machines and video modes.
	
	for(int a =0;a<6;a++)
	{
		MapSelect();
		TankP1Select();
		TankP2Select();
	}

	return false;
}

bool PLAY_FrameFunc()
{
	float dt=hge->Timer_GetDelta();//获取时间间隔
	//static float t=0.0f;
	//float tx,ty;
	//t+=dt;
	//tx=50*cosf(t/60);
	//ty=50*sinf(t/60);

	int id;
	static int lastid=0;

	// If ESCAPE was pressed, tell the GUI to pause
	if(hge->Input_GetKeyState(HGEK_ESCAPE)) { lastid=CR_PAUSE   ; gui->Leave(); }	
	//this is for test ,when you release the program,you need to remove it below
	//if(hge->Input_GetKeyState(HGEK_Q)) { lastid=CR_STOP   ; gui->Leave(); }	
	// We update the GUI and take an action if
	// one of the menu items was selected
	id=gui->Update(dt);//更新GUI并处理用户输入,返回控件的标识,如果调用了Leave方法 并且所有的控件都完成了结束动画，Update将返回-1.其他情况返回0
	if(id == -1)
	{
		switch(lastid)
		{
			case CR_PAUSE:
				gui->ShowCtrl(CR_START,false);
				gui->ShowCtrl(CR_EXIT,false);
				gui->ShowCtrl(CR_BACK,false);
				gui->ShowCtrl(CR_PLAY,false);
				gui->ShowCtrl(CR_CONTINUE,true);
				gui->ShowCtrl(CR_BACKTOMAIN,true);
				gui->ShowCtrl(CR_STOPTOMAIN,false);
				gui->SetFocus(CR_CONTINUE);
				gui->SetNavMode(HGEGUI_LEFTRIGHT|HGEGUI_CYCLED);
				gui->SetFocus(CR_CONTINUE);
				gui->Enter();
				state = PAUSE;
				break;
			case CR_STOP:
				gui->ShowCtrl(CR_START,false);
				gui->ShowCtrl(CR_EXIT,false);
				gui->ShowCtrl(CR_BACK,false);
				gui->ShowCtrl(CR_PLAY,false);
				gui->ShowCtrl(CR_CONTINUE,false);
				gui->ShowCtrl(CR_BACKTOMAIN,false);
				gui->ShowCtrl(CR_STOPTOMAIN,true);
				gui->SetFocus(CR_STOPTOMAIN);
				gui->Enter();
				state = STOP;
				break;
		}
	}
	else if(id) 
	{ 
		lastid=id; 
		gui->Leave(); 
	}

	/*添加代码*/
	//获取按键
	//Player 1
	if(hge->Input_GetKeyState(HGEK_W))
		P1keyStruct->up = 1;
	else
		P1keyStruct->up = 0;

	if(hge->Input_GetKeyState(HGEK_S))
		P1keyStruct->down = 1;
	else
		P1keyStruct->down = 0;

	if(hge->Input_GetKeyState(HGEK_A))
		P1keyStruct->left = 1;
	else
		P1keyStruct->left = 0;

	if(hge->Input_GetKeyState(HGEK_D))
		P1keyStruct->right = 1;
	else
		P1keyStruct->right = 0;

	if(hge->Input_GetKeyState(HGEK_J)|| hge->Input_GetKeyState(HGEK_G))
		if(pauseTime1>=PAUSETIME)
		{
			P1keyStruct->isFire = true;
			pauseTime1 = 0;
		}
		else
			P1keyStruct->isFire = false;
	else
		P1keyStruct->isFire = false;

	pauseTime1++;
	//player2
	if(hge->Input_GetKeyState(HGEK_UP))
		P2keyStruct->up = 1;
	else
		P2keyStruct->up = 0;

	if(hge->Input_GetKeyState(HGEK_DOWN))
		P2keyStruct->down = 1;
	else
		P2keyStruct->down = 0;

	if(hge->Input_GetKeyState(HGEK_LEFT))
		P2keyStruct->left = 1;
	else
		P2keyStruct->left = 0;

	if(hge->Input_GetKeyState(HGEK_RIGHT))
		P2keyStruct->right = 1;
	else
		P2keyStruct->right = 0;

	if(hge->Input_GetKeyState(HGEK_NUMPAD1) || hge->Input_GetKeyState(HGEK_ALT))
		if(pauseTime2 >= PAUSETIME)
		{
			P2keyStruct->isFire = true;
			pauseTime2 = 0;
		}
		else
			P2keyStruct->isFire = false;
	else
		P2keyStruct->isFire = false;

	pauseTime2++;

	
	pP1Tank->update(dt,P1keyStruct,pP2Tank,pObstacles);
	pP2Tank->update(dt,P2keyStruct,pP1Tank,pObstacles);
	/*tank1blood = pP1Tank->getTank_RTBlood();
	tank2blood = pP2Tank->getTank_RTBlood();*/
	if(pP1Tank->isGameOver() || pP2Tank->isGameOver())
	{
		pP1Tank->isGameOver()?info.assign("P2"):info.assign("P1");
		gui->ShowCtrl(CR_START,false);
		gui->ShowCtrl(CR_EXIT,false);
		gui->ShowCtrl(CR_BACK,false);
		gui->ShowCtrl(CR_PLAY,false);
		gui->ShowCtrl(CR_CONTINUE,false);
		gui->ShowCtrl(CR_BACKTOMAIN,false);
		gui->ShowCtrl(CR_STOPTOMAIN,true);
		gui->SetFocus(CR_STOPTOMAIN);
		gui->Enter();
		state = STOP;
	}
	return false;
}

bool PAUSE_FrameFunc()
{
	float dt=hge->Timer_GetDelta();//获取时间间隔
	static float t=0.0f;
	float tx,ty;
	t+=dt;
	tx=50*cosf(t/60);
	ty=50*sinf(t/60);

	int id;
	static int lastid=0;

	// If ESCAPE was pressed, tell the GUI to finish
	if(hge->Input_GetKeyState(HGEK_ESCAPE)) { lastid=CR_CONTINUE   ; gui->Leave(); }	
	// We update the GUI and take an action if
	// one of the menu items was selected
	id=gui->Update(dt);//更新GUI并处理用户输入,返回控件的标识,如果调用了Leave方法 并且所有的控件都完成了结束动画，Update将返回-1.其他情况返回0
	if(id == -1)
	{
		switch(lastid)
		{
			case CR_BACKTOMAIN:
				//播放背景音乐
				if(hge->Channel_IsPlaying(playbgmusic))
				{
					hge->Channel_Pause(playbgmusic);
				}
				hge->Channel_Resume(bgmusic);
				gui->ShowCtrl(CR_START,true);
				gui->ShowCtrl(CR_EXIT,true);
				gui->ShowCtrl(CR_BACK,false);
				gui->ShowCtrl(CR_PLAY,false);
				gui->ShowCtrl(CR_CONTINUE,false);
				gui->ShowCtrl(CR_BACKTOMAIN,false);
				gui->ShowCtrl(CR_STOPTOMAIN,false);
				gui->SetFocus(CR_START);
				gui->SetNavMode(HGEGUI_UPDOWN|HGEGUI_CYCLED);
				gui->Enter();
				state = MAIN;
				break;
			case CR_CONTINUE:
				gui->ShowCtrl(CR_START,false);
				gui->ShowCtrl(CR_EXIT,false);
				gui->ShowCtrl(CR_BACK,false);
				gui->ShowCtrl(CR_PLAY,false);
				gui->ShowCtrl(CR_CONTINUE,false);
				gui->ShowCtrl(CR_BACKTOMAIN,false);
				gui->ShowCtrl(CR_STOPTOMAIN,false);
				gui->Enter();
				state = PLAY;
				break;
		}
	}
	else if(id) 
	{ 
		lastid=id; 
		gui->Leave(); 
	}

	Pause_quad.v[0].tx=0;        Pause_quad.v[0].ty=0;
	Pause_quad.v[1].tx=1;		 Pause_quad.v[1].ty=0;
	Pause_quad.v[2].tx=1;		 Pause_quad.v[2].ty=1;
	Pause_quad.v[3].tx=0;        Pause_quad.v[3].ty=1;
	return false;
}

bool STOP_FrameFunc()
{
	float dt=hge->Timer_GetDelta();//获取时间间隔
	static float t=0.0f;
	float tx,ty;
	t+=dt;
	tx=50*cosf(t/60);
	ty=50*sinf(t/60);

	int id;
	static int lastid=0;

	// If ESCAPE was pressed, tell the GUI to finish
	if(hge->Input_GetKeyState(HGEK_ESCAPE)) { lastid=CR_EXIT   ; gui->Leave(); }	
	// We update the GUI and take an action if
	// one of the menu items was selected
	id=gui->Update(dt);//更新GUI并处理用户输入,返回控件的标识,如果调用了Leave方法 并且所有的控件都完成了结束动画，Update将返回-1.其他情况返回0
	if(id == -1)
	{
		switch(lastid)
		{
			case CR_STOPTOMAIN:
				//播放背景音乐
				if(hge->Channel_IsPlaying(playbgmusic))
				{
					hge->Channel_Pause(playbgmusic);
				}
				hge->Channel_Resume(bgmusic);
				gui->ShowCtrl(CR_START,true);
				gui->ShowCtrl(CR_EXIT,true);
				gui->ShowCtrl(CR_BACK,false);
				gui->ShowCtrl(CR_PLAY,false);
				gui->ShowCtrl(CR_CONTINUE,false);
				gui->ShowCtrl(CR_BACKTOMAIN,false);
				gui->ShowCtrl(CR_STOPTOMAIN,false);
				gui->SetFocus(CR_START);
				gui->SetNavMode(HGEGUI_UPDOWN|HGEGUI_CYCLED);
				gui->Enter();
				state = MAIN;
				break;
			case CR_EXIT:
				return true;
		}
	}
	else if(id) 
	{ 
		lastid=id; 
		gui->Leave(); 
	}
	return false;
}

bool FrameFunc()
{
	if(state == MAIN)
	{
		return MAIN_FrameFunc();
	}
	else if(state == SELECT)
	{	
		return SELECT_FrameFunc();
	}
	else if(state == PLAY)
	{	
		return PLAY_FrameFunc();
	}
	else if(state == PAUSE)
	{	
		return PAUSE_FrameFunc();
	}
	else if(state == STOP)
	{	
		return STOP_FrameFunc();
	}
	else
	{
		return false;
	}
}



bool MAIN_RenderFunc()
{
	// Render graphics
	hge->Gfx_BeginScene();	//开始渲染图形
	hge->Gfx_RenderQuad(&Main_bg_quad);//渲染一个矩形
	gui->Render();//将GUI渲染到屏幕上。
	//fnt->SetColor(0xFFFFFFFF);//将左上角显示设置为白色
	//fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d\nSTATE:%d", hge->Timer_GetDelta(), hge->Timer_GetFPS(),state);//左上角显示dt和FPS,STATE
	hge->Gfx_EndScene();   //结束渲染图形

	return false;
}

bool SELECT_RenderFunc()
{
	// Render graphics
	hge->Gfx_BeginScene();	//开始渲染图形
	hge->Gfx_RenderQuad(&Select_bg_quad);//渲染一个矩形
	hge->Gfx_RenderQuad(&Map_quad1);
	hge->Gfx_RenderQuad(&Map_quad2);
	hge->Gfx_RenderQuad(&Map_quad3);
	hge->Gfx_RenderQuad(&TankP1_quad1);
	hge->Gfx_RenderQuad(&TankP1_quad2);
	hge->Gfx_RenderQuad(&TankP1_quad3);
	hge->Gfx_RenderQuad(&TankP2_quad1);
	hge->Gfx_RenderQuad(&TankP2_quad2);
	hge->Gfx_RenderQuad(&TankP2_quad3);
	gui->Render();//将GUI渲染到屏幕上。
	fnt->SetColor(0xFFFFFFFF);
	fnt->SetScale(0.7);
	fnt->printf(0,0,HGETEXT_LEFT,"Tips:\nUse the space to switch map");
	fnt->SetColor(0xFFDAA520);
	fnt->SetScale(2.5);
	fnt->printf(90, 120, HGETEXT_CENTER, "P1");
	fnt->SetColor(0xFFFF5500);
	fnt->printf(710, 120, HGETEXT_CENTER, "P2");
	fnt->SetScale(1);
	hge->Gfx_EndScene();   //结束渲染图形

	return false;
}

bool PLAY_RenderFunc()
{
	// Render graphics
	hge->Gfx_BeginScene();	//开始渲染图形
	//绘制背景
	bgspr->Render(0,0);
	//绘制障碍物
	for(vector<ObstacleSprite>::iterator it = pObstacles->begin(); it != pObstacles->end(); it++)
	{
		it->render();
	}
	//绘制坦克精灵及子弹
	pP1Tank->render();
	pP2Tank->render();
	hge->Gfx_EndScene();   //结束渲染图形

	return false;
}

bool PAUSE_RenderFunc()
{
	//PLAY_RenderFunc();
	// Render graphics
	hge->Gfx_BeginScene();	//开始渲染图形
	//hge->Gfx_RenderQuad(&Select_bg_quad);//渲染一个矩形
	bgspr->Render(0,0);
	//绘制障碍物
	for(vector<ObstacleSprite>::iterator it = pObstacles->begin(); it != pObstacles->end(); it++)
	{
		it->render();
	}
	//绘制坦克精灵及子弹
	pP1Tank->render();
	pP2Tank->render();
	hge->Gfx_RenderQuad(&Stop_quad);//渲染一个矩形
	gui->Render();//将GUI渲染到屏幕上。
	//fnt->SetColor(0xFFFFFFFF);//将左上角显示设置为白色
	//fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d\nSTATE:%d", hge->Timer_GetDelta(), hge->Timer_GetFPS(),state);//左上角显示dt和FPS,STATE
	hge->Gfx_EndScene();   //结束渲染图形

	return false;
}

bool STOP_RenderFunc()
{
	//PLAY_RenderFunc();
	// Render graphics
	hge->Gfx_BeginScene();	//开始渲染图形
	//hge->Gfx_RenderQuad(&Select_bg_quad);//渲染一个矩形
	bgspr->Render(0,0);
	//绘制障碍物
	for(vector<ObstacleSprite>::iterator it = pObstacles->begin(); it != pObstacles->end(); it++)
	{
		it->render();
	}
	//绘制坦克精灵及子弹
	pP1Tank->render();
	pP2Tank->render();
	hge->Gfx_RenderQuad(&Stop_quad);//渲染一个矩形
	gui->Render();//将GUI渲染到屏幕上。
	infofnt->SetColor(0xFFFFA500);
	infofnt->SetScale(2);
	infofnt->printf(400,300,HGETEXT_CENTER,"%s  is the Winner",info.c_str());
	fnt->SetColor(0xFFFFFFFF);//将左上角显示设置为白色
	//fnt->printf(5, 5, HGETEXT_LEFT, "%s  is the Winner dt:%.3f\nFPS:%d\nSTATE:%d",info, hge->Timer_GetDelta(), hge->Timer_GetFPS(),state);//左上角显示dt和FPS,STATE
	hge->Gfx_EndScene();   //结束渲染图形

	return false;
}

bool RenderFunc()
{
	if(state == MAIN)
	{
		return MAIN_RenderFunc();
	}
	else if(state == SELECT)
	{	
		return SELECT_RenderFunc();
	}
	else if(state == PLAY)
	{	
		return PLAY_RenderFunc();
	}
	else if(state == PAUSE)
	{	
		return PAUSE_RenderFunc();
	}
	else if(state == STOP)
	{	
		return STOP_RenderFunc();
	}
	else
	{
		return false;
	}
}

void freeGame()
{
	delete gui;
	delete fnt;
	delete infofnt;
	//delete spr;
	hge->Effect_Free(snd);
	hge->Texture_Free(tex);
	hge->Texture_Free(Select_bg_quad.tex);
}

void Init()
{
	// Load sound and textures,载入声音 文理
	Main_bg_quad.tex=hge->Texture_Load("Images/bg1.png");
	Select_bg_quad.tex=hge->Texture_Load("Images/bg4.png");
	Pause_quad.tex=hge->Texture_Load("Images/bg1.png");
	Stop_quad.tex=hge->Texture_Load("Images/bg1.png");
	//tex=hge->Texture_Load("cursor.png");
	bgmusic = hge->Stream_Load("Sound/BgMusic.mp3",0);
	snd=hge->Effect_Load("Sound/menu.wav");
	if(!Select_bg_quad.tex || !Main_bg_quad.tex || !Pause_quad.tex || !snd)
	{
		// If one of the data files is not found, display
		// an error message and shutdown.
		//MessageBox(NULL, "Can't load *.PNG, CURSOR.PNG or MENU.WAV", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		hge->System_Shutdown();
		hge->Release();
		return;
	}

	// Set up the quad we will use for background animation
	Select_bg_quad.blend=BLEND_DEFAULT;
	Main_bg_quad.blend=BLEND_DEFAULT;
	Pause_quad.blend=BLEND_DEFAULT;
	Stop_quad.blend=BLEND_DEFAULT;

	for(int i=0;i<4;i++)//顶点的Z序（Z-order）范围从0.0到1.0如果使用了Z缓冲，此值将被忽略。顶点颜色和不透明度。
	{
		// Set up z-coordinate of vertices
		Select_bg_quad.v[i].z=0.5f;
		Main_bg_quad.v[i].z = 0.5f;
		Pause_quad.v[i].z = 0.5f;
		// Set up color. The format of DWORD col is 0xAARRGGBB
		Select_bg_quad.v[i].col=0xFFFFFFFF;
		Main_bg_quad.v[i].col = 0xFFFFFFFF;
		Pause_quad.v[i].col = 0x8FFFFFFF;
		Stop_quad.v[i].col = 0x8FFFFFFF;
	}

	
	//设置四个点x,y坐标 ,用于平铺纹理

	Select_bg_quad.v[0].x=0; Select_bg_quad.v[0].y=0;      //(0,0)
	Select_bg_quad.v[1].x=800; Select_bg_quad.v[1].y=0;    //(800,0)
	Select_bg_quad.v[2].x=800; Select_bg_quad.v[2].y=600;  //(800,600)
	Select_bg_quad.v[3].x=0; Select_bg_quad.v[3].y=600;    //(0,600)

	Main_bg_quad.v[0].x=0; Main_bg_quad.v[0].y=0;      //(0,0)
	Main_bg_quad.v[1].x=800; Main_bg_quad.v[1].y=0;    //(800,0)
	Main_bg_quad.v[2].x=800; Main_bg_quad.v[2].y=600;  //(800,600)
	Main_bg_quad.v[3].x=0; Main_bg_quad.v[3].y=600;    //(0,600)

	Pause_quad.v[0].x=0; Pause_quad.v[0].y=0;      //(0,0)
	Pause_quad.v[1].x=800; Pause_quad.v[1].y=0;    //(800,0)
	Pause_quad.v[2].x=800; Pause_quad.v[2].y=600;  //(800,600)
	Pause_quad.v[3].x=0; Pause_quad.v[3].y=600;    //(0,600)

	Stop_quad.v[0].x=0; Stop_quad.v[0].y=0;      //(0,0)
	Stop_quad.v[1].x=800; Stop_quad.v[1].y=0;    //(800,0)
	Stop_quad.v[2].x=800; Stop_quad.v[2].y=600;  //(800,600)
	Stop_quad.v[3].x=0; Stop_quad.v[3].y=600;    //(0,600)


	// Load the font, create the cursor sprite    设置字体,鼠标
	fnt=new hgeFont("Font/font1.fnt");
	infofnt = new hgeFont("Font/font1.fnt");
	//spr=new hgeSprite(tex,0,0,32,32);

	// Create and initialize the GUI    设置选项,创建并初始化GUI
	gui=new hgeGUI();

	

	gui->AddCtrl(new hgeGUIMenuItem(CR_START,fnt,snd,300,200,0.0f,"Start"));
	gui->AddCtrl(new hgeGUIMenuItem(CR_EXIT,fnt,snd,300,240,0.1f,"Exit"));
	gui->AddCtrl(new hgeGUIMenuItem(CR_BACK,fnt,snd,350,550,0.0f,"Back"));
	gui->AddCtrl(new hgeGUIMenuItem(CR_PLAY,fnt,snd,450,550,0.1f,"Play"));
	gui->AddCtrl(new hgeGUIMenuItem(CR_CONTINUE,fnt,snd,300,350,0.0f,"Continue"));
	gui->AddCtrl(new hgeGUIMenuItem(CR_BACKTOMAIN,fnt,snd,500,350,0.1f,"Back To Main"));
	gui->AddCtrl(new hgeGUIMenuItem(CR_STOPTOMAIN,fnt,snd,400,550,0.0f,"Press Enter To Main"));



	gui->ShowCtrl(CR_START,true);
	gui->ShowCtrl(CR_EXIT,true);
	gui->ShowCtrl(CR_BACK,false);
	gui->ShowCtrl(CR_PLAY,false);
	gui->ShowCtrl(CR_CONTINUE,false);
	gui->ShowCtrl(CR_BACKTOMAIN,false);
	gui->ShowCtrl(CR_STOPTOMAIN,false);

	gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);//设置键盘导航模式,上下循环导航
	//gui->SetCursor(spr);//设置鼠标精灵
	gui->SetFocus(CR_START);//设置键盘输入焦点,也就是默认在start上
	gui->Enter();//图形用户界面开始进入动画。

	/**/
	P1keyStruct = new KeyStruct();
	P2keyStruct = new KeyStruct();

	/*纹理资源*/
	/*子弹*/
	bullet_s_Tex = hge->Texture_Load("Images/bullet_s_Tex.png");
	bullet_m_Tex = hge->Texture_Load("Images/bullet_m_Tex.png");
	bullet_l_Tex = hge->Texture_Load("Images/bullet_l_Tex.png");
	/*坦克*/
	tank_s_animtex = hge->Texture_Load("Images/tank_s_animTex.png");
	tank_m_animtex = hge->Texture_Load("Images/tank_m_animTex.png");
	tank_l_animtex = hge->Texture_Load("Images/tank_l_animTex.png");
	/*障碍物*/
	obstacle_wall = hge->Texture_Load("Images/obstacle_wall.png");
	obstacle_edge_w = hge->Texture_Load("Images/obstacle_edge_w.png");
	obstacle_edge_h = hge->Texture_Load("Images/obstacle_edge_h.png");
	pig_wall = hge->Texture_Load("Images/pig_wall.png");

	/*声音资源*/
	explosnd_s = hge->Effect_Load("Sound/explosion_s.ogg");
	firesnd_s = hge->Effect_Load("Sound/canon_fire_s.ogg");
	explosnd_m = hge->Effect_Load("Sound/explosion_m.ogg");
	firesnd_m = hge->Effect_Load("Sound/canon_fire_m.ogg");
	explosnd_l = hge->Effect_Load("Sound/explosion_l.ogg");
	firesnd_l = hge->Effect_Load("Sound/canon_fire_l.ogg");
	//游戏背景音乐
	playbgmusic = hge->Stream_Load("Sound/playbgmusic.ogg");

	/*加载资源并初始化精灵*/
	/*三种炮弹*/
	//int iBullet_ID, string sBullet_Name, int iBullet_Bounce_Amount, int iBullet_Power,float fBullet_Speed, HTEXTURE tex, HEFFECT explosnd, HEFFECT firesnd, hgeParticleSystem *explopar, hgeParticleSystem *movepar
	bullet_s.loadResource(0,"bullet_s",1,15,170,bullet_s_Tex,explosnd_s,firesnd_s,"Par/explopar_s.psi","Par/movepar_s.psi");
	bullet_m.loadResource(1,"bullet_m",2,30,145,bullet_m_Tex,explosnd_m,firesnd_m,"Par/explopar_m.psi","Par/movepar_m.psi");
	bullet_l.loadResource(2,"bullet_l",3,45,100,bullet_l_Tex,explosnd_l,firesnd_l,"Par/explopar_l.psi","Par/movepar_l.psi");
	/*三种坦克*/
	//坦克ID、坦克名、坦克血量、坦克速度、坦克动画纹理、纹理中帧数
	tank_s.loadResource(0,"鸡式坦克",150,105,tank_s_animtex,2);
	tank_m.loadResource(1,"牛式坦克",175,75,tank_m_animtex,2);
	tank_l.loadResource(2,"虎式坦克",200,55,tank_l_animtex,2);

	/*为坦克加载弹夹*/
	//子弹类型、数量
	tank_s.setBulletManager(bullet_s,3);
	tank_m.setBulletManager(bullet_m,3);
	tank_l.setBulletManager(bullet_l,4);
	/*地图障碍物*/
	pObstacles = new vector<ObstacleSprite>();
	sp_wall = new hgeSprite(obstacle_wall,0,0,hge->Texture_GetWidth(obstacle_wall),hge->Texture_GetHeight(obstacle_wall));
	sp_edge_w = new hgeSprite(obstacle_edge_w,0,0,hge->Texture_GetWidth(obstacle_edge_w),hge->Texture_GetHeight(obstacle_edge_w));
	sp_edge_h = new hgeSprite(obstacle_edge_h,0,0,hge->Texture_GetWidth(obstacle_edge_h),hge->Texture_GetHeight(obstacle_edge_h));
	sp_pig_wall_1 = new hgeSprite(pig_wall,0,0,90,90);
	sp_pig_wall_2 = new hgeSprite(pig_wall,60,0,90,90);
	sp_pig_wall_3 = new hgeSprite(pig_wall,120,0,90,90);
	sp_pig_wall_4 = new hgeSprite(pig_wall,180,0,90,90);

}

void InitSelect()
{
	// Load sound and texture
	Selectsnd=hge->Effect_Load("Sound/menu.wav");
	Map_quad1.tex=hge->Texture_Load("Images/map2.png");
	Map_quad2.tex=hge->Texture_Load("Images/map1.png");
	Map_quad3.tex=hge->Texture_Load("Images/map3.png");
	TankP1_quad1.tex=hge->Texture_Load("Images/tanks.png");
	TankP1_quad2.tex=hge->Texture_Load("Images/tanks.png");
	TankP1_quad3.tex=hge->Texture_Load("Images/tanks.png");
	TankP2_quad1.tex=hge->Texture_Load("Images/tanks.png");
	TankP2_quad2.tex=hge->Texture_Load("Images/tanks.png");
	TankP2_quad3.tex=hge->Texture_Load("Images/tanks.png");
	if(!Selectsnd 
		|| !Map_quad1.tex || !Map_quad2.tex || !Map_quad3.tex 
		|| !TankP1_quad1.tex || !TankP1_quad2.tex || !TankP1_quad3.tex 
		|| !TankP2_quad1.tex || !TankP2_quad2.tex || !TankP2_quad3.tex)
	{
		// If one of the data files is not found, display
		// an error message and shutdown.
//		MessageBox(NULL, "Can't load MENU.WAV or PARTICLES.PNG", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		hge->System_Shutdown();
		hge->Release();
		return;
	}

	// Set up quad which we will use for rendering sprite
	Map_quad1.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;
	Map_quad2.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;
	Map_quad3.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;
	TankP1_quad1.blend=BLEND_DEFAULT;
	TankP1_quad2.blend=BLEND_DEFAULT;
	TankP1_quad3.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;
	TankP2_quad1.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;
	TankP2_quad2.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;
	TankP2_quad3.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;

	for(int i=0;i<4;i++)
	{
		// Set up z-coordinate of vertices
		Map_quad1.v[i].z=0.5f;
		Map_quad2.v[i].z=0.5f;
		Map_quad3.v[i].z=0.5f;
		// Set up color. The format of DWORD col is 0xAARRGGBB
		Map_quad1.v[i].col=0xFFFFFFFF;
		Map_quad2.v[i].col=0xFFFFFFFF;
		Map_quad3.v[i].col=0xFFFFFFFF;

		TankP1_quad1.v[i].z=0.5f;
		TankP1_quad2.v[i].z=0.5f;
		TankP1_quad3.v[i].z=0.5f;

		TankP2_quad1.v[i].z=0.5f;
		TankP2_quad2.v[i].z=0.5f;
		TankP2_quad3.v[i].z=0.5f;

		TankP1_quad1.v[i].col=0xFFFFFFFF;
		TankP1_quad2.v[i].col=0xFFFFFFFF;
		TankP1_quad3.v[i].col=0xFFFFFFFF;

		TankP2_quad1.v[i].col=0xFFFFFFFF;
		TankP2_quad2.v[i].col=0xFFFFFFFF;
		TankP2_quad3.v[i].col=0xFFFFFFFF;

	}

	// Set up quad's texture coordinates.
	// 0,0 means top left corner and 1,1 -
	// bottom right corner of the texture.
	Map_quad1.v[0].tx=0;  Map_quad1.v[0].ty=0; 
	Map_quad1.v[1].tx=440/440; Map_quad1.v[1].ty=0; 
	Map_quad1.v[2].tx=440/440; Map_quad1.v[2].ty=330/330;
	Map_quad1.v[3].tx=0;  Map_quad1.v[3].ty=330/330;

	Map_quad2.v[0].tx=0;  Map_quad2.v[0].ty=0; 
	Map_quad2.v[1].tx=440/440; Map_quad2.v[1].ty=0; 
	Map_quad2.v[2].tx=440/440; Map_quad2.v[2].ty=330/330;
	Map_quad2.v[3].tx=0;  Map_quad2.v[3].ty=330/330;

	Map_quad3.v[0].tx=0;  Map_quad3.v[0].ty=0; 
	Map_quad3.v[1].tx=440/440; Map_quad3.v[1].ty=0; 
	Map_quad3.v[2].tx=440/440; Map_quad3.v[2].ty=330/330;
	Map_quad3.v[3].tx=0;  Map_quad3.v[3].ty=330/330;

	TankP1_quad1.v[0].tx=0.0f/600.0;  TankP1_quad1.v[0].ty=0; 
	TankP1_quad1.v[1].tx=200.0f/600.0f;  TankP1_quad1.v[1].ty=0; 
	TankP1_quad1.v[2].tx=200.0f/600.0f;  TankP1_quad1.v[2].ty=200.0/200.0; 
	TankP1_quad1.v[3].tx=0.0f/600.0;  TankP1_quad1.v[3].ty=200.0/200.0; 

	TankP1_quad2.v[0].tx=200.0f/600.0f;  TankP1_quad2.v[0].ty=0; 
	TankP1_quad2.v[1].tx=400.0f/600.0f;  TankP1_quad2.v[1].ty=0; 
	TankP1_quad2.v[2].tx=400.0f/600.0f;  TankP1_quad2.v[2].ty=200.0/200.0; 
	TankP1_quad2.v[3].tx=200.0f/600.0f;  TankP1_quad2.v[3].ty=200.0/200.0; 

	TankP1_quad3.v[0].tx=400.0f/600.0f;  TankP1_quad3.v[0].ty=0;
	TankP1_quad3.v[1].tx=600.0f/600.0f;  TankP1_quad3.v[1].ty=0;
	TankP1_quad3.v[2].tx=600.0f/600.0f;  TankP1_quad3.v[2].ty=200.0f/200.0f; 
	TankP1_quad3.v[3].tx=400.0f/600.0f;  TankP1_quad3.v[3].ty=200.0f/200.0f; 

	TankP2_quad1.v[0].tx=0.0f/600.0;  TankP2_quad1.v[0].ty=0; 
	TankP2_quad1.v[1].tx=200.0f/600.0f;  TankP2_quad1.v[1].ty=0; 
	TankP2_quad1.v[2].tx=200.0f/600.0f;  TankP2_quad1.v[2].ty=200.0/200.0; 
	TankP2_quad1.v[3].tx=0.0f/600.0;  TankP2_quad1.v[3].ty=200.0/200.0; 

	TankP2_quad2.v[0].tx=200.0f/600.0f;  TankP2_quad2.v[0].ty=0; 
	TankP2_quad2.v[1].tx=400.0f/600.0f;  TankP2_quad2.v[1].ty=0; 
	TankP2_quad2.v[2].tx=400.0f/600.0f;  TankP2_quad2.v[2].ty=200.0/200.0; 
	TankP2_quad2.v[3].tx=200.0f/600.0f;  TankP2_quad2.v[3].ty=200.0/200.0; 

	TankP2_quad3.v[0].tx=400.0f/600.0f;  TankP2_quad3.v[0].ty=0;
	TankP2_quad3.v[1].tx=600.0f/600.0f;  TankP2_quad3.v[1].ty=0;
	TankP2_quad3.v[2].tx=600.0f/600.0f;  TankP2_quad3.v[2].ty=200.0f/200.0f; 
	TankP2_quad3.v[3].tx=400.0f/600.0f;  TankP2_quad3.v[3].ty=200.0f/200.0f; 
}

void FreeSelect()
{
	hge->Texture_Free(Map_quad1.tex);
	hge->Texture_Free(Map_quad2.tex);
	hge->Texture_Free(Map_quad3.tex);
	hge->Texture_Free(TankP1_quad1.tex);
	hge->Texture_Free(TankP1_quad2.tex);
	hge->Texture_Free(TankP1_quad3.tex);
	hge->Texture_Free(TankP2_quad1.tex);
	hge->Texture_Free(TankP2_quad2.tex);
	hge->Texture_Free(TankP2_quad3.tex);
	hge->Effect_Free(Selectsnd);
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);	//使用hgeCreate函数创建HGE接口，参数必须传递正确的HGE_VERSION,它是在hge.h中定义的

	hge->System_SetState(HGE_LOGFILE, "tank.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);//设置逻辑函数为FrameFunc函数
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);//设置绘制函数为RenderFunc函数
	hge->System_SetState(HGE_TITLE, "坦克大战");//设置窗口标题
	hge->System_SetState(HGE_WINDOWED, true);//设置使用窗口模式
	hge->System_SetState(HGE_SCREENWIDTH, 800);//设置窗口宽度
	hge->System_SetState(HGE_SCREENHEIGHT, 600);//设置窗口长度
	hge->System_SetState(HGE_SCREENBPP, 32); //设置颜色深度
	hge->System_SetState(HGE_SHOWSPLASH, false);
	hge->System_SetState(HGE_FPS,120);
	//hge->System_SetState(HGE_HWNDPARENT, hwnd);
	//hge->System_SetState(HGE_HIDEMOUSE, false); //HGE在子窗口模式下使用Windows系统鼠标指针

	if(hge->System_Initiate())//将上述的系统设置初始化,然后进行游戏初始化
	{
		Init();
		InitSelect();
		//gui[2]=new hgeGUI();

		//gui[2]->AddCtrl(new hgeGUIMenuItem(1,fnt,snd,400,200,0.0f,"Play"));

		//gui[2]->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);//设置键盘导航模式,上下循环导航
		//gui[2]->SetCursor(spr);//设置鼠标精灵
		//gui[2]->SetFocus(1);//设置键盘输入焦点,也就是默认在play上
		hge->Stream_Play(bgmusic,true,100);
		// Let's rock now!
		hge->System_Start();

		// Delete created objects and free loaded resources
		freeGame();
		FreeSelect();
	}

	// Clean up and shutdown
	hge->System_Shutdown();
	hge->Release();
	return 0;
}


