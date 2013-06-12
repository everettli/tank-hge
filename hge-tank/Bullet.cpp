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

//�������캯��
Bullet::Bullet(const Bullet & bullet)
{
	//���Ƴ����Ա
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
	/*�����ڴ�ռ�*/
	sprite = new hgeSprite(0,0,0,0,0);
	explopar = new hgeParticleSystem(exploparfilename,sprite);
	movepar = new hgeParticleSystem(moveparfilename,sprite);
	rect1 = new hgeRect(0,0,0,0);
	rect2 = new hgeRect(0,0,0,0);
	fnt=new hgeFont("Font/font1.fnt");
	//����ָ��ָ�������
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
	//��ʼ���йر���
	initBulllet();
}

void Bullet::initBulllet()
{
	HGE * hge = hgeCreate(HGE_VERSION);
	bullet_tex_width = hge->Texture_GetWidth(tex);
	bullet_tex_height = hge->Texture_GetHeight(tex);
	hge->Release();
	//�����ʼ��
	sprite = new hgeSprite(tex,0,0,bullet_tex_width,bullet_tex_height);
	sprite->SetHotSpot(bullet_tex_width/2,bullet_tex_height/2);
	//�켣����
	movepar = new hgeParticleSystem(moveparfilename,sprite);
	//��ը����
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

//���ͼ�ϰ�����ײ���
bool Bullet::collisionWithObstacles(vector<ObstacleSprite>* pObstacles)
{
	if(getIsFired())
	{
		for(vector<ObstacleSprite>::iterator it = pObstacles->begin(); it != pObstacles->end(); it++)
		{
			/*TODO������ϰ�����ײ���Ĵ���*/
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
	//��ȡ����ײ��������ĵ�����
	VertexPos vSelfHotSpot,vOtherHotSpot;
	vSelfHotSpot.x = vBullet_Pos.x/* + bullet_tex_width/2*/;
	vSelfHotSpot.y = vBullet_Pos.y/* + bullet_tex_height/2*/;
	vOtherHotSpot.x = obstaclesprite->getObstaclePos().x + obstaclesprite->getSpriteUnit()->GetWidth()/2;
	vOtherHotSpot.y = obstaclesprite->getObstaclePos().y + obstaclesprite->getSpriteUnit()->GetHeight()/2;
	int surface = getCollisionSuface(vSelfHotSpot,vOtherHotSpot);
	/*TODO��ת�ط��߷�����ٶ�*/
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
	//����ʣ����ײ����
	decreaseSPBounceAmount();
}

void Bullet::shot(vertexPos vBullet_Pos,VertexDirec vBullet_direction)
{
	//�����ӵ�����ʱλ�ü�����
	setStartPos(vBullet_Pos);
	setDirections(vBullet_direction);
	//���ŷ��������
	HGE * hge = hgeCreate(HGE_VERSION);
	hge->Effect_Play(firesnd);
	hge->Release();
	//��������Ч��
	movepar->MoveTo(vBullet_Pos.x,vBullet_Pos.y);
	movepar->Fire();
	isFired = true;
}

void Bullet::update(float fDetalTime,vector<ObstacleSprite>* pObstacles)
{
	if(isFired)
	{
		//�����ӵ���λ��
		vBullet_Pos.x += vBullet_direction.x * fDetalTime * fBullet_Speed;
		vBullet_Pos.y += vBullet_direction.y * fDetalTime * fBullet_Speed;

		//�ӵ����ϰ������ײ���
		collisionWithObstacles(pObstacles);
		//ά������Ч��
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
		/*TODO���ƴ���*/
		sprite->Render(vBullet_Pos.x,vBullet_Pos.y);
		
		/*��������*/
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

//�ӵ���ը
void Bullet::bulletBoomb()
{
	//��ӱ�ը����Ч������
	explopar->FireAt(vBullet_Pos.x,vBullet_Pos.y);
	//ֹͣ���ͷ��ƶ�����
	movepar->Stop(true);

	//����ӵ���ը����Ч������
	HGE * hge = hgeCreate(HGE_VERSION);
	hge->Effect_Play(explosnd);
	hge->Release();
	//�ָ�����ײ�����Լ�״̬
	iBullet_SPBounce_Amount = iBullet_Bounce_Amount;
	isFired = false;
}


//���ٿ���ײ����
void Bullet::decreaseSPBounceAmount()
{
	if(iBullet_SPBounce_Amount > 0)
	{
		iBullet_SPBounce_Amount--;
	}
}

//������ײ�����Ƿ��þ�
bool Bullet::isBulletSPBounceAmountOver()
{
	return (iBullet_SPBounce_Amount <= 0);
}


int Bullet::getCollisionSuface(VertexPos vSelfHotSpot, VertexPos vOtherHotSpot)
{
	//б��Ϊ-1�ĶԽ��ߺ�����׼ʽ(����ϵ������Ļ����ϵ)
	float resultFunc1 = vSelfHotSpot.x - vOtherHotSpot.x - vOtherHotSpot.y + vSelfHotSpot.y;
	//б��Ϊ-1�ĶԽ��ߺ�����׼ʽ
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







