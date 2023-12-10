#include "enemy_statue.h"

#define PIXELS_TO_FEET 32
#define MAX_WALKING_SPEED m_maxwalkingspeed
#define NORMAL_ACCELERATION m_acceleration
#define WIDTH 32

#define MAXSTEPSIZE 0

#define DRAW_DEBUG_TEXT false
#define GENERICTHINKDELAY 1000

#define COLLISION enemy_base::pCEnemies->pcore->landscape.Collision
#define PCORE enemy_base::pCEnemies->pcore
#define TICK PCORE->gfx.tick

enemy_statue::enemy_statue(void)
{
	m_x=0;
	m_y=0;
	m_yvel=-2;
	pCEnemies=NULL;
	m_xvel=0;
	m_nextdecision=10;
	health=475;
	targetplayer=NULL;
	b_Activated=false;
	fighttimer = 0;
	animtimer = 0;
	this->UpdateMyRect();
}

enemy_statue::~enemy_statue(void)
{
	
}

void enemy_statue::Do(void)
{
	if (!b_Activated) return;
	enemy_base::pCEnemies->NumEnemiesActivated++;

	if (m_nextdecision>0) 
	{
		m_nextdecision = m_nextdecision - (PCORE->gfx.tick);
		if(m_nextdecision<=0){ DecideWhatToDo(); m_nextdecision=GENERICTHINKDELAY;}
	}

	HandleFalling();
	HandleWalking();
	HandleAttacking();

}

void enemy_statue::Draw(int layer)
{
	if (layer!=0) return;
	if (m_x+512<PCORE->landscape.offset_x) return;
	if (m_x-512>PCORE->landscape.offset_x+(PCORE->gfx.m_d3dpp.BackBufferWidth/PCORE->landscape.m_zoom)) return;
	if (m_y+512<PCORE->landscape.offset_y) return;
	if (m_y-512>PCORE->landscape.offset_y+(PCORE->gfx.m_d3dpp.BackBufferHeight/PCORE->landscape.m_zoom)) return;
	
	enemy_base::pCEnemies->NumEnemiesDrawn++;
	b_Activated=true;
	PCORE->SetAlphaMode(false);

	PCORE->gfx.SpriteDrawEx(t_body,
					(this->m_x-PCORE->landscape.offset_x-(64.0f*0.6))*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y-(118.0f*0.6))*PCORE->landscape.m_zoom,  
					0,
					0, 
					0,
					PCORE->landscape.m_zoom*0.6,
					PCORE->landscape.m_zoom*0.6,
					0,0,0,0,
					255,255,255,255);

	if (fighttimer>0 && fighttimer<1000)
	{
		PCORE->SetAlphaMode(true);

		for (int i=-1;i<2;i=i+2)
		PCORE->gfx.SpriteDrawEx(PCORE->landscape.ParticleTypes[PARTICLE_EYELASERLIGHTS].particletexture,
					((this->m_x+(9.0f*i)-(20.0f*1.7))-PCORE->landscape.offset_x)*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y-(115.0f*0.6))*PCORE->landscape.m_zoom,  
					0,
					0, 
					0,
					PCORE->landscape.m_zoom*2,
					PCORE->landscape.m_zoom*2,
					0,0,0,0,
					(1000.0f-fighttimer)*0.255f,255,255,255);

		for (int i=-1;i<2;i=i+2)
		PCORE->gfx.SpriteDrawEx(PCORE->landscape.ParticleTypes[PARTICLE_EYELASERLIGHTS].particletexture,
					((this->m_x+(9.0f*i)-(20.0f*0.95))-PCORE->landscape.offset_x)*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y-(85.0f*0.6))*PCORE->landscape.m_zoom,  
					0,
					0, 
					0,
					PCORE->landscape.m_zoom,
					PCORE->landscape.m_zoom,
					0,0,0,0,
					(1000.0f-fighttimer)*0.255f,255,255,255);
	}
	
	if (DRAW_DEBUG_TEXT)
	{
		PCORE->gfx.DrawTextEx( (m_x-(PCORE->landscape.offset_x))*(PCORE->landscape.m_zoom),
		m_y-70-(PCORE->landscape.offset_y), 255,255,0,"decision:%i\nhealth:%i",(int)this->m_nextdecision, (int)this->health);
		PCORE->gfx.DrawTextEx( (m_x-(PCORE->landscape.offset_x)-3)*(PCORE->landscape.m_zoom),
		 (m_y-(PCORE->landscape.offset_y)-3)*(PCORE->landscape.m_zoom), 255,255,0,"x");
	}
}

bool enemy_statue::LoadStuff(void)
{
	t_body = PCORE->textures.LoadTexture("textures/enemies/statue/head.tga");
	s_charge = PCORE->sound.LoadSound(4,"sound/enemies/statue/charge.ogg");
	s_shoot = PCORE->sound.LoadSound(4,"sound/enemies/statue/shoot.ogg");

	return true;
}

void enemy_statue::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void enemy_statue::HandleFalling(void)
{
	UpdateMyRect();
}

void enemy_statue::HandleWalking(void)
{
	UpdateMyRect();
}

void enemy_statue::DecideWhatToDo(void)
{
	float dist, xdist, ydist;
	CPlayers* playerpointer;

	// can we see a player
	dist = PCORE->landscape.GetNearestPlayer(m_x, m_y-200, &playerpointer, &xdist, &ydist);
		
	if (fabs(dist)<900)
	{
		targetplayer = playerpointer;
		// if close enough to fight then fight
		if (fighttimer<-1500)
		{
			fighttimer = 1000.0f;
		}
	}
	else
	{
		targetplayer=NULL;
	}

}

bool enemy_statue::BulletHit(float* x, float* y)
{
	if (!this->b_Activated) return false;
	if (*x<my_rect.left) return false;
	if (*x>my_rect.right) return false;
	if (*y<my_rect.top) return false;
	if (*y>my_rect.bottom) return false;
	return true;
}

bool enemy_statue::BulletHitRect(GRECT* r)
{
	if (!this->b_Activated) return false;
	if (IntersectGRect(r,&my_rect)) return true;
	return false;
}
void enemy_statue::TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet)
{
	LPParticle particle = NULL;
	float randomangle = angle;
	float randomnumber;
	float beforehealth=health;

	health=health-damage;

	if (health<0 && beforehealth>=0)
	{
		if (bullet!=NULL)
		{
			if (bullet->pplayer!=NULL)
			{
				bullet->pplayer->CountKill();
				bullet->pplayer->AddBulletTime(SKILL_BT_BONESY);
			}
		}

		PCORE->landscape.UnAttachAttachmentsMonster(this);
//		PCORE->sound.PlaySnd(s_zombiedie1,m_x,m_y);

		for (int i=0;i<30;i++)
		{
			particle = PCORE->landscape.AddParticle(PARTICLE_GENERIC_32X32, my_rect.left+(rand()%(int)(my_rect.right-my_rect.left)), my_rect.top+(rand()%(int)(my_rect.bottom-my_rect.top)));
			if (particle!=NULL)
			{
					particle->xscale = 0.5f + ((rand()%1000)/5000.0f);
					particle->yscale = particle->xscale;
					if (i%3==0)
						particle->textureoverride = PCORE->textures.LoadTexture("textures/particles/gib_statue.bmp");
					else if (i%3==1)
						particle->textureoverride = PCORE->textures.LoadTexture("textures/particles/gib_statue2.bmp");
					else 
						particle->textureoverride = PCORE->textures.LoadTexture("textures/particles/gib_statue3.bmp");

					randomnumber = ((rand()%2000)/10000.0f)-0.1f;
					particle->xvel = randomnumber/1.0f;
					randomnumber = ((rand()%1000)/1000.0f);
					particle->yvel = -randomnumber/10.0f;
					randomnumber = ((rand()%1000)/10000.0f)-0.05f;
					particle->rotvel = randomnumber/100.0f;
					randomnumber = ((rand()%1000)/1000.0f);
					particle->rot = randomnumber;
			}
		}
	
		if (bullet!=NULL)
		{
			for (int i=0;i<10;i++)
			{
				particle = PCORE->landscape.AddParticle(PARTICLE_GENERIC_32X32, bullet->x-8+(rand()%16), bullet->y-8+(rand()%16));
				if (particle!=NULL)
				{
						particle->xscale = 0.5f + ((rand()%1000)/5000.0f);
						particle->yscale = particle->xscale;
						if (i%3==0)
							particle->textureoverride = PCORE->textures.LoadTexture("textures/particles/gib_statue.bmp");
						else if (i%3==1)
							particle->textureoverride = PCORE->textures.LoadTexture("textures/particles/gib_statue2.bmp");
						else 
							particle->textureoverride = PCORE->textures.LoadTexture("textures/particles/gib_statue3.bmp");

						randomnumber = 0.01f+((rand()%1000)/10000.0f);
						particle->xvel = bullet->xvel*randomnumber;
						particle->yvel = bullet->yvel*randomnumber;
						randomnumber = (rand()%1000)/10000.0f;
						particle->rotvel = particle->rotvel*randomnumber;
				}
			}
		}
	}

	if (bullet!=NULL)
	{
		if (bullet->type!=PARTICLE_FIREBITRISER && bullet->type!=PARTICLE_FIRE)
		{

		}
		else
		{	
			if ((bullet->type==PARTICLE_FIRE && rand()%2==0) || (bullet->type==PARTICLE_FIREBITRISER && rand()%10==0))
				this->DoFireDamage(x, y, damage);
			return;
		}
	}

	if (!Splash)
	{
		particle = PCORE->landscape.AddParticle(PARTICLE_BONEDUST, x, y);
		if (particle!=NULL)
		{
			particle->xscale = 0.7f+rand()%1000/1000.0f;
			particle->alpha = 100.0f;
		}

		if (rand()%2==0)
		for (int j=0;j<10;j++)
		{
			particle = PCORE->landscape.AddParticle(PARTICLE_RICHOCHETSPARK, x, y);
			if (particle!=NULL)
			{
				randomangle = angle + (((rand()%2000)-1000)/3000.0f);
				randomnumber = 0.1f+((rand()%500)/1000.0f);
				particle->xvel = sin(randomangle)*(randomnumber);
				particle->yvel = cos(randomangle)*(randomnumber);
			}
		}
	}

	if (bullet!=NULL)
		AttachParticle(bullet);
}

void enemy_statue::HandleAttacking(void)
{
	if (targetplayer==NULL) return;

	float oldft=fighttimer;
	fighttimer = fighttimer -  (PCORE->gfx.tick);



	if (oldft>0 && fighttimer<=0) // shoot bullet
	{

		PCORE->sound.PlaySnd(s_shoot, m_x, m_y-230,0.0f);

		for (int i=-1;i<2;i=i+2)
		{
			LPParticle p = PCORE->landscape.AddParticle(PARTICLE_EYELASERS,m_x+(11*i),m_y-(64.0f*0.6f));

			if (p!=NULL)
			{
				p->pplayer = NULL;
				float angle = atan2(targetplayer->m_fX-(m_x+(7*i)),targetplayer->m_fY-(m_y-(64.0f*0.6f)));
				angle = angle + (((rand()%1000)/20000.0f)*i);
				p->xvel = sin(angle)*1.3f;
				p->yvel = cos(angle)*1.3f;
				p->damage = BONESY_BONEDAMAGE;
			}
		}
		
	}
}

void enemy_statue::BulletHitSplash(float x, float y, float speed)
{
	LPParticle dparticle;

	float randomnumber;
	float randomangle;
	float randx, randy;
	float angle;

	for (int i=0;i<5;i++)
	{
		randx = m_x+rand()%32-16;
		randy = m_y+rand()%16;
		angle = atan2(randx-x,randy-y);
		randomnumber = (((rand()%100000))/100000.0f);
		randomangle = angle+(0.1*randomnumber);
		randomnumber = (((rand()%100000))/100000.0f);
		for (int j=0;j<6;j++)
		{
			dparticle = PCORE->landscape.AddParticle(PARTICLE_BLOODBLOBS, randx, randy,0,0,0,PCORE->landscape.t_BloodBlobs[rand()%NUM_BLOODBLOBS],0);
			if (dparticle!=NULL)
			{
				dparticle->xvel = (sin(randomangle)*0.7*randomnumber);
				dparticle->yvel = (cos(randomangle)*0.7*randomnumber);
			}
		}
	
		randomnumber = (((rand()%100000))/100000.0f);

		dparticle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_BLOODMIST, randx, randy);
		if (dparticle!=NULL)
			{
				randomangle = angle;
				randomnumber = (((rand()%10000))/10000.0f);
				dparticle->xvel = sin(randomangle)*((0.1*randomnumber));
				dparticle->yvel = cos(randomangle)*((0.1*randomnumber));
		
			}

	}


}

float enemy_statue::GetX()
{
	return m_x;
}
float enemy_statue::GetY()
{
	return m_y;
}
float enemy_statue::GetFacing()
{
	return 1;
}

bool enemy_statue::SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker)
{
	if (x<this->m_x-size) return false;
	if (x>this->m_x+size) return false;
	if (y-20<(this->m_y-230)-size) return false;
	if (y-20>(this->m_y-230)+size) return false;

	float distance = sqrtf( (m_x-x)*(m_x-x) + ((m_y-230)-y-20)*((m_y-230)-y-20));

	if (distance>size) return false;

	if (this->health>=0)
	{
		TakeDamage(((size-distance)/size)*damage, atan2(m_x-x, (m_y-230)-y-20), x, y, true);

		if (this->health<0 && attacker!=NULL)
		{
			
			attacker->CountKill();
			attacker->AddBulletTime(SKILL_BT_BONESY);
			
		}
	}
	return true;
}
void enemy_statue::AttachParticle(LPParticle particle)
{
	LPParticle dparticle=NULL;

	if (health>0)
	if (particle->type==PARTICLE_ARROW)
	{
		float randdepth= (rand()%10000/1000.f);

		particle->x = particle->x - (sin(atan2(particle->xvel,particle->yvel)) *(randdepth-0));
		particle->y = particle->y - (cos(atan2(particle->xvel,particle->yvel)) *(randdepth-0));

		dparticle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_ARROWDEAD,particle->x,particle->y);		if (dparticle!=NULL)
		{
			dparticle->AttachedTo = ATTACHED_TO_MONSTER;
			dparticle->pmonster = this;
			dparticle->attached_x = (particle->x - this->m_x);
			dparticle->attached_y = particle->y - this->m_y;
			dparticle->attached_rot = atan2(sin(particle->rot),cos(particle->rot))+ (((rand()%20000/10000.f)-1)*0.1f);
			dparticle->IsMoving = false;
		}
	}
}

void enemy_statue::UpdateMyRect(void)
{
	my_rect.left = m_x-25;
	my_rect.right = m_x+25;
	my_rect.top = m_y-(108*0.6f);
	my_rect.bottom = m_y-(10*0.6f);
}

void enemy_statue::DoFireDamage(float x, float y, float damage)
{
	LPParticle dparticle=NULL;

	dparticle = PCORE->landscape.AddParticle(PARTICLE_FIREBITATTACHED,x,y);		
	
	if (dparticle!=NULL)
		{
			dparticle->AttachedTo = ATTACHED_TO_MONSTER;
			dparticle->pmonster = this;
			dparticle->attached_x = (x - this->m_x);
			dparticle->attached_y = y - this->m_y;
			dparticle->IsMoving = false;
			dparticle->SpawnParticle = PARTICLE_FIREBITRISERATTACHED;
			dparticle->SpawnTimerReset = 20;
			dparticle->SpawnDistance = 100;
			dparticle->damage = FIRE_DAMAGE;
		}

}
