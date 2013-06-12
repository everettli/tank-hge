#include "TankSprite.h"

TankSprite::TankSprite()
{
	iTank_Blood = iTank_RTBlood = 0;
	fTank_Speed = 0;
	vTank_Pos.x = vTank_Pos.y = 0;
	vTank_Direction.x = vTank_Direction.y = 0;
	rect1 = new hgeRect(0,0,0,0);
	rect2 = new hgeRect(0,0,0,0);
	rect3 = new hgeRect(0,0,0,0);
	iBloodPercent = 100;
	rot = 0;
}

TankSprite::TankSprite(const TankSprite & tankSprite)
{
	iTank_ID = tankSprite.iTank_ID;
	sTank_Name = tankSprite.sTank_Name;
	iTank_Blood = this->iTank_RTBlood = tankSprite.iTank_Blood;
	fTank_Speed = tankSprite.fTank_Speed;
	animtex = tankSprite.animtex;
	nframes = tankSprite.nframes;
	bulletManager = tankSprite.bulletManager;
	rect1 = new hgeRect(0,0,0,0);
	rect2 = new hgeRect(0,0,0,0);
	rect3 = new hgeRect(0,0,0,0);
	iBloodPercent = tankSprite.iBloodPercent;
	rot = 0;
	validKeys = tankSprite.validKeys;

	HGE * hge = hgeCreate(HGE_VERSION);
	tank_tex_width = hge->Texture_GetWidth(animtex) / nframes;
	tank_tex_height = hge->Texture_GetHeight(animtex);
	hge->Release();
	/*�����ڴ�ռ�*/
	fnt=new hgeFont("Font/font1.fnt");
	pAnim = new hgeAnimation(animtex,nframes,24,0,0,tank_tex_width,tank_tex_height);
	pAnim->SetHotSpot(tank_tex_width/2,tank_tex_height/2);
}

void TankSprite::loadResource(int iTank_ID, std::string sTank_Name, int iTank_Blood, float fTank_Speed, HTEXTURE animtex,int nframes)
{
	this->iTank_ID = iTank_ID;
	this->sTank_Name = sTank_Name;
	this->iTank_Blood = this->iTank_RTBlood = iTank_Blood;
	this->fTank_Speed = fTank_Speed;
	this->animtex = animtex;
	this->nframes = nframes;
	//��ʼ��̹�˱���
	initTank();
}

void TankSprite::initTank()
{
	HGE * hge = hgeCreate(HGE_VERSION);
	tank_tex_width = hge->Texture_GetWidth(animtex) / nframes;
	tank_tex_height = hge->Texture_GetHeight(animtex);
	hge->Release();
	pAnim = new hgeAnimation(animtex,nframes,24,0,0,tank_tex_width,tank_tex_height);
	pAnim->SetHotSpot(tank_tex_width/2,tank_tex_height/2);
	fnt=new hgeFont("Font/font1.fnt");
	//����Ĭ����Ӧ���а���
	unlockKey();
}

void TankSprite::setBulletManager(Bullet bullet, int amount)
{
	bulletManager.initBulletManager(bullet,amount);
}

void TankSprite::setStartPosAndDirection(float vTank_PosX,float vTank_PosY,int vTank_DirectionX,int vTank_DirectionY)
{
	this->vTank_Pos.x = vTank_PosX;
	this->vTank_Pos.y = vTank_PosY;
	this->vTank_Direction.x = vTank_DirectionX;
	this->vTank_Direction.y = vTank_DirectionY;
}

hgeRect * TankSprite::getTankRect()
{
	pAnim->GetBoundingBox(vTank_Pos.x,vTank_Pos.y,rect1);
	return rect1;
}
void TankSprite::update(float fDetalTime,KeyStruct* pKeys,TankSprite* pEnemy,vector<ObstacleSprite>* pObstacles)
{
	//����λ��
	vTank_Pos.x += vTank_Direction.x * fDetalTime * fTank_Speed;
	vTank_Pos.y += vTank_Direction.y * fDetalTime * fTank_Speed;
	
	//��ײ���
	collisionWithTank(pEnemy);		//�������ڵ���̹�˵���ײ���
	collisionWithObstacles(pObstacles);//�������ϰ������ײ���

	//������
	keyHandle(pKeys);

	//��ת
	directionToRotation();

	//���и���
	bulletManager.update(fDetalTime,pObstacles);
	//����
	pAnim->Update(fDetalTime);
}

void TankSprite::render()
{
	//����̹��
	pAnim->RenderEx(vTank_Pos.x,vTank_Pos.y,rot);
	//�����ӵ�
	bulletManager.render();
	//����Ѫ������ʣ���ڵ�
	fnt->SetScale(0.5);
	if(strcmp(player.c_str(),"P1") == 0)
	{
		fnt->SetColor(0xF5555500);
	}
	else
	{
		fnt->SetColor(0xFFFF5500);
	}
	fnt->printf(vTank_Pos.x,vTank_Pos.y + tank_tex_height/2,HGETEXT_CENTER,"%s\nBlood:%d%%\nBullet:%d/%d\n",player.c_str(),iBloodPercent,bulletManager.getTankSPBulletAmount(),bulletManager.getTankBulletAmount());
	//fnt->printf(200, 250, HGETEXT_LEFT, "valid UP:%d\nvalid DOWN:%d\nvalid LEFT:%d\nvaild RIGHT:%d\n",validKeys.up,validKeys.down,validKeys,left,validKeys.right);
	
}

bool TankSprite::collisionWithTank(TankSprite* pEnemy)
{
	//��з�̹�˵��ڵ�����ײ���
	pEnemy->bulletManager.getTankBulletAmount();
	for(int i = 0;i < pEnemy->bulletManager.getTankBulletAmount(); i++)
	{
		if(pEnemy->bulletManager.bullets[i].getIsFired())
		{
			/*bullet�뱾̹�˵���ײ���*/
			if(collisionWithRect(this->getTankRect(),pEnemy->bulletManager.bullets[i].getBulletRect()))
			{
				pEnemy->bulletManager.bullets[i].bulletBoomb();
				decreaseBlood(pEnemy->bulletManager.bullets[i].getBullet_Power());
			}	
		}
	}
	//��з�̹�˵���ײ���
	if(collisionWithRect(this->getTankRect(),pEnemy->getTankRect()))
	{
		//if���ϣ�������ײ�淨���ϵ��ٶ�
		//��ȡ����ײ��������ĵ�����
		//ע�⣬����ê������еĲ�������ת��ê�������
		int surface = getCollisionSuface(this->getTankPos(),pEnemy->getTankPos());
		//������ײ�淨���ϵ�����
		lockKey(surface);
	}
	
	return true;
}

bool TankSprite::collisionWithObstacles(vector<ObstacleSprite>* pObstacles)
{
	for(vector<ObstacleSprite>::iterator it =pObstacles->begin(); it != pObstacles->end(); it++)
	{
		/*TODO������ϰ�����ײ���Ĵ���*/
		if(collisionWithRect(this->getTankRect(),it->getSpriteUnit()->GetBoundingBox(it->getObstaclePos().x,it->getObstaclePos().y,rect2)))
		{
			//��ȡ����ײ��������ĵ�����
			VertexPos vSelfHotSpot,vOtherHotSpot;
			//ע�⣬����ê������еĲ�������ת��ê�������
			vSelfHotSpot.x = vTank_Pos.x;
			vSelfHotSpot.y = vTank_Pos.y;
			vOtherHotSpot.x = it->getObstaclePos().x + it->getSpriteUnit()->GetWidth()/2;
			vOtherHotSpot.y = it->getObstaclePos().y + it->getSpriteUnit()->GetHeight()/2;
			int surface = getCollisionSuface(vSelfHotSpot,vOtherHotSpot);
			//������ײ�淨���ϵ�����
			lockKey(surface);
		}
	}
			
	return true;
}

bool TankSprite::collisionWithRect(hgeRect * rect1,hgeRect * rect2)
{
	return rect1->Intersect(rect2);
}

void TankSprite::decreaseBlood(int bulletPower)
{
	iTank_RTBlood -= bulletPower;
	iBloodPercent = (iTank_RTBlood *100 /iTank_Blood);
}
void TankSprite::keyHandle(KeyStruct* pKeys)
{
	//���˵�����Ӧ�İ���
	(validKeys.up == 0)?(pKeys->up = 0):NULL;
	(validKeys.down == 0)?(pKeys->down = 0):NULL;
	(validKeys.right == 0)?(pKeys->right = 0):NULL;
	(validKeys.left == 0)?(pKeys->left = 0):NULL;
	(validKeys.isFire == false)?(pKeys->isFire = false):NULL;
	//��Ӧ����
	if(pKeys->isFire)
	{
		//��ȡ��ȷ���ӵ�����λ�úͷ��䷽��
		//�����ӵ�
		bulletManager.shoot(getAccurateBulletOutPos(),rotationToDirection());
		pKeys->isFire = false;
	}
	vTank_Direction.x = pKeys->right - pKeys->left;
	vTank_Direction.y = pKeys->down - pKeys->up;
	//��������ͣ�˶�����
	if((abs(vTank_Direction.x) + abs(vTank_Direction.y)) > 0)
	{
		if(!pAnim->IsPlaying()) 
			pAnim->Play(); 
	}else
	{
		if(pAnim->IsPlaying())
			pAnim->Stop();
	}

	//��ⰴ���෴����ʱ��������
	if((vTank_Direction.x >= 0 && validKeys.left == 0) || (vTank_Direction.x <= 0 && validKeys.right == 0) ||  (vTank_Direction.y <= 0 && validKeys.down == 0) || (vTank_Direction.y >= 0 && validKeys.up == 0))
	{
		unlockKey();
	}
}
void TankSprite::directionToRotation()
{
	if(vTank_Direction.x == -1)
	{
		if(vTank_Direction.y == -1)
			rot = 7*M_PI_4;
		if(vTank_Direction.y == 0)
			rot = 6*M_PI_4;
		if(vTank_Direction.y == 1)
			rot = 5*M_PI_4;
	}
	else if(vTank_Direction.x == 0)
	{
		if(vTank_Direction.y == -1)
			rot = 0;
		if(vTank_Direction.y == 1)
			rot = 4*M_PI_4;
	}
	else if(vTank_Direction.x == 1)
	{
		if(vTank_Direction.y == -1)
			rot = 1*M_PI_4;
		if(vTank_Direction.y == 0)
			rot = 2*M_PI_4;
		if(vTank_Direction.y == 1)
			rot = 3*M_PI_4;
	}
}

VertexDirec TankSprite::rotationToDirection()
{
	VertexDirec temp;
	if(rot == 0)
	{
		temp.x = 0;
		temp.y = -1;
	}
	if(rot == 1*M_PI_4)
	{
		temp.x =1;
		temp.y = -1;
	}
	if(rot == 2*M_PI_4)
	{
		temp.x =1;
		temp.y = 0;
	}
	if(rot == 3*M_PI_4)
	{
		temp.x =1;
		temp.y = 1;
	}
	if(rot == 4*M_PI_4)
	{
		temp.x =0;
		temp.y = 1;
	}
	if(rot == 5*M_PI_4)
	{
		temp.x =-1;
		temp.y = 1;
	}
	if(rot == 6*M_PI_4)
	{
		temp.x =-1;
		temp.y = 0;
	}
	if(rot == 7*M_PI_4)
	{
		temp.x = -1;
		temp.y = -1;
	}
	return temp;
}

//��ȡ��ȷ���ӵ�����λ��
VertexPos TankSprite::getAccurateBulletOutPos()
{
	VertexPos tempVertex;
	if(rot == 0)
	{
		tempVertex.x = vTank_Pos.x;
		tempVertex.y = vTank_Pos.y - tank_tex_height/2;
	}
	else if(rot == 1*M_PI_4)
	{
		tempVertex.x = vTank_Pos.x + tank_tex_height/2/SQRT2;
		tempVertex.y = vTank_Pos.y - tank_tex_height/2/SQRT2;
	}
	else if(rot == 2*M_PI_4)
	{
		tempVertex.x = vTank_Pos.x + tank_tex_width/2;
		tempVertex.y = vTank_Pos.y;
	}
	else if(rot == 3*M_PI_4)
	{
		tempVertex.x = vTank_Pos.x + tank_tex_height/2/SQRT2;
		tempVertex.y = vTank_Pos.y + tank_tex_height/2/SQRT2;
	}
	else if(rot == 4*M_PI_4)
	{
		tempVertex.x = vTank_Pos.x;
		tempVertex.y = vTank_Pos.y + tank_tex_height/2;
	}
	else if(rot == 5*M_PI_4)
	{
		tempVertex.x = vTank_Pos.x - tank_tex_height/2/SQRT2;
		tempVertex.y = vTank_Pos.y + tank_tex_height/2/SQRT2;
	}
	else if(rot == 6*M_PI_4)
	{
		tempVertex.x = vTank_Pos.x - tank_tex_width/2;
		tempVertex.y = vTank_Pos.y;
	}
	else if(rot == 7*M_PI_4)
	{
		tempVertex.x = vTank_Pos.x - tank_tex_height/2/SQRT2;
		tempVertex.y = vTank_Pos.y - tank_tex_height/2/SQRT2;
	}
	return tempVertex;
}
int TankSprite::getCollisionSuface(VertexPos vSelfHotSpot, VertexPos vOtherHotSpot)
{
	//б��Ϊ-1�ĶԽ��ߺ�����׼ʽ(����ϵ������Ļ����ϵ)
	float resultFunc1 = vSelfHotSpot.x - vOtherHotSpot.x - vOtherHotSpot.y + vSelfHotSpot.y;
	//б��Ϊ-1�ĶԽ��ߺ�����׼ʽ
	float resultFunc2 = vSelfHotSpot.x - vOtherHotSpot.x + vOtherHotSpot.y - vSelfHotSpot.y;
	if( resultFunc1 < 0 && resultFunc2 >= 0)
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
	if(resultFunc1 <= 0 && resultFunc2 < 0)
	{
		return RIGHT;
	}
	return 0;
}

//���Բ�����ָ�������ϵİ���
void TankSprite::lockKey(int direction)
{
	(direction == UP)?(validKeys.up = 0):NULL;
	(direction == DOWN)?(validKeys.down = 0):NULL;
	(direction == RIGHT)?(validKeys.right = 0):NULL;
	(direction == LEFT)?(validKeys.left = 0):NULL;
}
//�����ָ����а����Ŀɰ���
void TankSprite::unlockKey()
{
	//�ָ����а����Ŀɰ���
	validKeys.up = 1;
	validKeys.down = 1;
	validKeys.left = 1;
	validKeys.right = 1;
	validKeys.isFire = true;
}

void TankSprite::setPlayer(std::string playername)
{
	this->player = playername;
}
