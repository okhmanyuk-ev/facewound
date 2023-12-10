#include "enemies.h"

#include "Landscape.h"
#include "Players.h"
#include "garryswrappers/GarrysHelperFunctions.h"
#include "Weapons.h"
#include "Entitys.h"

#include "Core.h"

#include "enemy_zombie.h"
//#include "enemy_bird.h"
#include "enemy_soldierturret.h"
#include "enemy-src/enemy_rat.h"
#include "enemy-src/enemy_mine.h"
#include "enemy-src/enemy_bonesy.h"
#include "enemy-src/enemy_statue.h"
#include "enemy-src/enemy_ghost.h"
#include "enemy-src/enemy_sickzombie.h"
#include "enemy-src/enemy_solider.h"
#include "enemy-src/enemy_solider_m16.h"
#include "enemy-src/enemy_solider_smaw.h"
#include "enemy-src/enemy_jawfish.h"
#include "enemy-src/enemy_hardrock.h"
#include "enemy-src/enemy_murderorb.h"
#include "enemy-src/enemy_electrononce.h"

CEnemies::CEnemies()
{
	enemy[0]=NULL;
	pcore=NULL;
	NumEnemies=0;
	FirstEnemy=NULL;
}

CEnemies::~CEnemies(void)
{

}

void CEnemies::DrawEnemies(int layer)
{
	NumEnemiesDrawn=0;
	
	LPEnemy tempp=FirstEnemy;
	while (tempp!=NULL)
	{
		if (layer==10)
		{
			tempp->pointer->DrawNormalMap();
		}
		else
		{
			tempp->pointer->Draw(layer);
		}
		tempp=tempp->Next;
	} 

	//pcore->gfx.DrawTextEx(10,200,255,255,0,"num enemies: %i\ndrawn: %i\nactive: %i", NumEnemies, NumEnemiesDrawn, NumEnemiesActivated);
}

void CEnemies::DoEnemies(void)
{
	NumEnemiesActivated=0;
	LPEnemy tempp=FirstEnemy;
	LPEnemy delme=NULL;
	while (tempp!=NULL)
	{
		if (tempp->pointer->health<0)
		{
			delme = tempp;
		}
		else
		{
			tempp->pointer->Do();

			if (pcore->landscape.WaterLine>0)
			{
				if ((tempp->pointer->my_rect.top-16) > pcore->landscape.WaterLine)
				{
					pcore->landscape.WaterLargeObjectPlunk(tempp->pointer->GetX());
					pcore->sound.PlaySnd((rand()%2==0?pcore->landscape.s_waterplonk1:pcore->landscape.s_waterplonk2),tempp->pointer->GetX(), pcore->landscape.WaterLine,0.3f);

					if (tempp->type==ENT_E_HARDROCK)
					{
						pcore->landscape.WaterLargeObjectPlunk(tempp->pointer->GetX()-20.0f);
						pcore->landscape.WaterLargeObjectPlunk(tempp->pointer->GetX()+20.0f);
					}

					delme = tempp;
				}
			}

		}
		tempp=tempp->Next;

		if (delme!=NULL)
		{
			this->RemoveEnemy(delme);
			delme=NULL;
		}
	} 
}

LPEnemy CEnemies::AddEnemy(unsigned int type, float x, float y, float a, float b, char* str1, char* str2)
{
	LPEnemy pen = new sEnemies();

	if (type==ENEMY_ZOMBIE)						pen->pointer = new enemy_zombie();
	else if (type==ENEMY_SOLDIERTURRET)			pen->pointer = new enemy_soldierturret();
	else if (type==ENT_E_RAT)			pen->pointer = new enemy_rat();
	else if (type==ENT_E_MINE)			pen->pointer = new enemy_mine();
	else if (type==ENT_E_BONESY)			pen->pointer = new enemy_bonesy();
	else if (type==ENT_E_STATUE)			pen->pointer = new enemy_statue();
	else if (type==ENT_E_GHOST)			pen->pointer = new enemy_ghost();
	else if (type==ENT_E_SICKZOMBIE)			pen->pointer = new enemy_sickzombie();
	else if (type==ENT_E_PISTOLSOLDIER)			pen->pointer = new enemy_soldier();
	else if (type==ENT_E_M16SOLDIER)			pen->pointer = new enemy_soldier_m16();
	else if (type==ENT_E_SMAWSOLDIER)			pen->pointer = new enemy_soldier_smaw();
	else if (type==ENT_E_JAWFISH)			pen->pointer = new enemy_jawfish();
	else if (type==ENT_E_HARDROCK)			pen->pointer = new enemy_hardrock();
	else if (type==ENT_E_MURDERORB)			pen->pointer = new enemy_murderorb();
	else if (type==ENT_E_ELECTRONONCE)			pen->pointer = new enemy_electrononce();


	else 

	{
		delete pen;
		return NULL;
	}

	
	pen->pointer->pCEnemies = this;
	pen->pointer->LoadStuff();

	pen->type = type;
	pen->pointer->SetXY(x,y);
	pen->pointer->SetUp(x, y, a, b, str1, str2);

	pen->Next = NULL; // we're on the end

	if (FirstEnemy==NULL) 
	{
		FirstEnemy = pen;
	}
	else 
	{
		// find the last one so we can add ourselved to the end
		LPEnemy tempp=FirstEnemy;
		while (tempp->Next!=NULL)
		{
			tempp=tempp->Next;
		} 
		tempp->Next = pen;	
	}

	NumEnemies++;

	return pen;
}


LPEnemy CEnemies::PreviousNME(LPEnemy index)
{
	LPEnemy temp=FirstEnemy;
	if(index==FirstEnemy) return FirstEnemy;
	while(temp->Next != index)
		temp=temp->Next;

	return temp;
}

void CEnemies::RemoveEnemy(LPEnemy delme)
{
	LPEnemy tempp=FirstEnemy;

	if (delme==this->FirstEnemy)
	{
		FirstEnemy = delme->Next;

		NumEnemies--;
		delete delme;
	}
	else if  (delme==this->LastEnemy)
	{
		LastEnemy = PreviousNME(delme);
		LastEnemy->Next = NULL;

		NumEnemies--;
		delete delme;
	}
	else
	{
		PreviousNME(delme)->Next = delme->Next;

		delete delme;
		NumEnemies--;
	}
}

void CEnemies::RemoveAll()
{
	LPEnemy tempp=FirstEnemy;
	LPEnemy del;
	while (tempp!=NULL)
	{
		del = tempp;
		tempp = tempp->Next;
		delete del;
	} 

	FirstEnemy=NULL;
	NumEnemies=0;
}

bool CEnemies::BulletHitEnemy(bool DoDamage, float x, float y, LPParticle bullet)
{
	LPEnemy tempp=FirstEnemy;
	while (tempp!=NULL)
	{
		if (tempp->pointer->BulletHit(&x, &y))
		{
			if (bullet==NULL) return true;

			float ox=bullet->x;
			float oy=bullet->y;

			bullet->x = x;
			bullet->y = y;

			if (DoDamage)
			{
				tempp->pointer->TakeDamage(bullet->damage, atan2(bullet->xvel, bullet->yvel), bullet->x, bullet->y, false, bullet);
				
			}

			bullet->x = ox;
			bullet->y = oy;

			return true;
		}
		tempp=tempp->Next;
	} 

	return false;
}

void CEnemies::Init(void)
{

}


bool CEnemies::BulletHitEnemyRect(GRECT* rectangle, LPParticle bullet)
{
	LPEnemy tempp=FirstEnemy;
	while (tempp!=NULL)
	{
		if (tempp->pointer->BulletHitRect(rectangle))
		{
			return true;
		}
		tempp=tempp->Next;
	} 

	return false;
}

bool CEnemies::SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker)
{
	bool retval=false;

	LPEnemy tempp=FirstEnemy;
	while (tempp!=NULL)
	{
		if (tempp->pointer->SplashDamageEnemy(x, y, size, damage, attacker))
		{
			retval= true;
		}
		tempp=tempp->Next;
	} 

	return retval;
}

float CEnemies::GetNearestEnemy(float x, float y, LPEnemy* enemy, float* xdist, float* ydist, LPEnemy notthisenemy, int nottype)
{
	float realdistance=9999;
	float xdistance=9999;
	float ydistance=9999;
	*enemy=NULL;
	float tsqr=0;

	NumEnemiesActivated=0;
	LPEnemy tempp=FirstEnemy;
	LPEnemy delme=NULL;

	while (tempp!=NULL)
	{
		if (tempp->pointer->health>=0)
		if (tempp!=notthisenemy)
		if (tempp->pointer!=(enemy_base*)notthisenemy)
		if (tempp->type!=nottype)
		{
			tsqr = sqrtf(SQUARED(tempp->pointer->GetX()-x)+SQUARED(tempp->pointer->GetY()-y));

			if (tsqr<realdistance || realdistance==0)
			{
				realdistance = tsqr;
				xdistance = tempp->pointer->GetX() - x;
				ydistance = tempp->pointer->GetY() - y;
				*enemy = tempp;
			}
		}

		tempp=tempp->Next;
	} 

	
	*xdist = xdistance;
	*ydist = ydistance;
	return realdistance;
}