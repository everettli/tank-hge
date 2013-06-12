#include "Bullet.h"

Bullet::Bullet()
{
	vBullet_Pos.x=0;
	vBullet_Pos.y=0;
	vBullet_direction.x=0;
	vBullet_direction.y=0;
	isExplosed = false;
	rect1 = new hgeRect(0,0,0,0);
	rect2 = new hgeRect(0,0,0,0);
}

//拷贝构造函数
Bullet::Bullet(const Bullet & bullet)
{
	//复制常规成员
	iBullet_ID = bullet.iBullet_ID;
	sBullet_Name = bullet.sBullet_Name;
	vBullet_Pos = bullet.vBullet_Pos;
	vBullet_direction = bullet.vBullet_direction;
	iBullet_Bounce_Amount = bullet.iBullet_Bounce_Amount;
	iBullet_SPBounce_Amount = bullet.iBullet_SPBounce_Amount;
	fBullet_Speed = bullet.fBullet_Speed;
	iBullet_Power = bullet.iBullet_Power;
	isExplosed = bullet.isExplosed;
	isFired = bullet.isFired;
	tex = bullet.tex;
	explosnd = bullet.explosnd;
	firesnd = bullet.firesnd;
	bullet_tex_width =bullet.bullet_tex_width;
	bullet_tex_height = bullet.bullet_tex_height;
	exploparfilename = bullet.exploparfilename;
	moveparfilename = bullet.moveparfilename;
	/*分配内存空间*/
	sprite = new hgeSprite(0,0,0,0,0);
	explopar = new hgeParticleSystem(exploparfilename,sprite);
	movepar = new hgeParticleSystem(moveparfilename,sprite);
	rect1 = new hgeRect(0,0,0,0);
	rect2 = new hgeRect(0,0,0,0);
	fnt=new hgeFont("Font/font1.fnt");
	//复制指针指向的内容
	memcpy(sprite,bullet.sprite,sizeof(hgeSprite));
	memcpy(explopar,bullet.explopar,sizeof(hgeParticleSystem));
	memcpy(movepar,bullet.movepar,sizeof(hgeParticleSystem));
	memcpy(rect1,bullet.rect1,sizeof(hgeRect));
	memcpy(rect2,bullet.rect2,sizeof(hgeRect));
}


void Bullet::loadResource(int iBullet_ID, string sBullet_Name, int iBullet_Bounce_Amount, int iBullet_Power,float fBullet_Speed, HTEXTURE tex, HEFFECT explosnd, HEFFECT firesnd,char* exploparfilename, char * moveparfilename)
{
	this->iBullet_ID = iBullet_ID;
	this->sBullet_Name = sBullet_Name;
	this->iBullet_SPBounce_Amount = this->iBullet_Bounce_Amount = iBullet_Bounce_Amount;
	this->iBullet_Power = iBullet_Power;
	this->fBullet_Speed = fBullet_Speed;
	this->tex = tex;
	this->explosnd = explosnd;
	this->firesnd = firesnd;
	this->exploparfilename = exploparfilename;
	this->moveparfilename = moveparfilename;
	//初始化有关变量
	initBulllet();
}

void Bullet::initBulllet()
{
	HGE * hge = hgeCreate(HGE_VERSION);
	bullet_tex_width = hge->Texture_GetWidth(tex);
	bullet_tex_height = hge->Texture_GetHeight(tex);
	hge->Release();
	//精灵初始化
	sprite = new hgeSprite(tex,0,0,bullet_tex_width,bullet_tex_height);
	sprite->SetHotSpot(bullet_tex_width/2,bullet_tex_height/2);
	//轨迹粒子
	movepar = new hgeParticleSystem(moveparfilename,sprite);
	//爆炸粒子
	explopar = new hgeParticleSystem(exploparfilename,sprite);

	fnt=new hgeFont("Font/font1.fnt");

}

void Bullet::setStartPos(vertexPos vBullet_Pos)
{
	this->vBullet_Pos = vBullet_Pos;
}

void Bullet::setDirections(VertexDirec vBullet_direction)
{
	this->vBullet_direction = vBullet_direction;
}

//与地图障碍物碰撞检测
bool Bullet::collisionWithObstacles(vector<ObstacleSprite>* pObstacles)
{
	if(getIsFired())
	{
		for(vector<ObstacleSprite>::iterator it = pObstacles->begin(); it != pObstacles->end(); it++)
		{
			/*TODO添加与障碍物碰撞检测的代码*/
			if(collisionWithRect(this->getBulletRect(),it->getSpriteUnit()->GetBoundingBox(it->getObstaclePos().x,it->getObstaclePos().y,rect2)))
			{
				if(isBulletSPBounceAmountOver())
				{
					bulletBoomb();
				}
				else
				{
					reserveDirection(it);
				}
			}
		}
	}
	return true;
}

bool Bullet::collisionWithRect(hgeRect * rect1,hgeRect * rect2)
{
	return rect1->Intersect(rect2);
}


void Bullet::reserveDirection(vector<ObstacleSprite>::iterator obstaclesprite)
{
	//获取两个撞击物的中心点坐标
	VertexPos vSelfHotSpot,vOtherHotSpot;
	vSelfHotSpot.x = vBullet_Pos.x/* + bullet_tex_width/2*/;
	vSelfHotSpot.y = vBullet_Pos.y/* + bullet_tex_height/2*/;
	vOtherHotSpot.x = obstaclesprite->getObstaclePos().x + obstaclesprite->getSpriteUnit()->GetWidth()/2;
	vOtherHotSpot.y = obstaclesprite->getObstaclePos().y + obstaclesprite->getSpriteUnit()->GetHeight()/2;
	int surface = getCollisionSuface(vSelfHotSpot,vOtherHotSpot);
	/*TODO反转沿法线方向的速度*/
	if(surface == DOWN)
	{
		(vBullet_direction.y > 0) ? vBullet_direction.y = (-vBullet_direction.y) : NULL;
	}
	if(surface == LEFT)
	{
		(vBullet_direction.x < 0) ? vBullet_direction.x = (-vBullet_direction.x) : NULL;
	}
	if(surface == UP)
	{
		(vBullet_direction.y < 0) ? vBullet_direction.y = (-vBullet_direction.y) : NULL;
	}
	if(surface == RIGHT)
	{
		(vBullet_direction.x > 0) ? vBullet_direction.x = (-vBullet_direction.x) : NULL;
	}
	//减少剩余碰撞次数
	decreaseSPBounceAmount();
}

void Bullet::shot(vertexPos vBullet_Pos,VertexDirec vBullet_direction)
{
	//设置子弹发射时位置及方向
	setStartPos(vBullet_Pos);
	setDirections(vBullet_direction);
	//播放发射的声音
	HGE * hge = hgeCreate(HGE_VERSION);
	hge->Effect_Play(firesnd);
	hge->Release();
	//播放粒子效果
	movepar->MoveTo(vBullet_Pos.x,vBullet_Pos.y);
	movepar->Fire();
	isFired = true;
}

void Bullet::update(float fDetalTime,vector<ObstacleSprite>* pObstacles)
{
	if(isFired)
	{
		//更新子弹的位置
		vBullet_Pos.x += vBullet_direction.x * fDetalTime * fBullet_Speed;
		vBullet_Pos.y += vBullet_direction.y * fDetalTime * fBullet_Speed;

		//子弹与障碍物的碰撞检测
		collisionWithObstacles(pObstacles);
		//维护粒子效果
		/*TODO*/
		movepar->MoveTo(vBullet_Pos.x,vBullet_Pos.y);
		movepar->Update(fDetalTime);
	}
	
	explopar->Update(fDetalTime);
	
}

void Bullet::render()
{
	if(isFired)
	{
		/*TODO绘制代码*/
		sprite->Render(vBullet_Pos.x,vBullet_Pos.y);
		
		/*绘制粒子*/
		movepar->Render();
	}
	explopar->Render();
}

hgeRect * Bullet::getBulletRect()
{
	sprite->GetBoundingBox(vBullet_Pos.x,vBullet_Pos.y,rect1);
	hgeRect * rect = new hgeRect(rect1->x1 + OFFSET_VALUE,rect1->y1 +OFFSET_VALUE,rect1->x2 -OFFSET_VALUE,rect1->y2-OFFSET_VALUE);
	return rect;
}

//子弹爆炸
void Bullet::bulletBoomb()
{
	//添加爆炸粒子效果代码
	explopar->FireAt(vBullet_Pos.x,vBullet_Pos.y);
	//停止并释放移动粒子
	movepar->Stop(true);

	//添加子弹爆炸声音效果代码
	HGE * hge = hgeCreate(HGE_VERSION);
	hge->Effect_Play(explosnd);
	hge->Release();
	//恢复可碰撞次数以及状态
	iBullet_SPBounce_Amount = iBullet_Bounce_Amount;
	isFired = false;
}


//减少可碰撞次数
void Bullet::decreaseSPBounceAmount()
{
	if(iBullet_SPBounce_Amount > 0)
	{
		iBullet_SPBounce_Amount--;
	}
}

//检测可碰撞次数是否用尽
bool Bullet::isBulletSPBounceAmountOver()
{
	return (iBullet_SPBounce_Amount <= 0);
}


int Bullet::getCollisionSuface(VertexPos vSelfHotSpot, VertexPos vOtherHotSpot)
{
	//斜率为-1的对角线函数标准式(坐标系参照屏幕坐标系)
	float resultFunc1 = vSelfHotSpot.x - vOtherHotSpot.x - vOtherHotSpot.y + vSelfHotSpot.y;
	//斜率为-1的对角线函数标准式
	float resultFunc2 = vSelfHotSpot.x - vOtherHotSpot.x + vOtherHotSpot.y - vSelfHotSpot.y;
	if( resultFunc1 <= 0 && resultFunc2 > 0)
	{
		return DOWN;
	}
	if(resultFunc1 > 0 && resultFunc2 >= 0)
	{
		return LEFT;
	}
	if(resultFunc1 >= 0 && resultFunc2 < 0)
	{
		return UP;
	}
	if(resultFunc1 < 0 && resultFunc2 <= 0)
	{
		return RIGHT;
	}
	return 0;
}







