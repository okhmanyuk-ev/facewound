#include "enemy_bonesy.h"

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

enemy_bonesy::enemy_bonesy(void)
{
	m_x=0;
	m_y=0;
	m_yvel=-2;
	pCEnemies=NULL;
	m_xvel=0;
	dwFlags=0;
	m_nextdecision=10;
	m_facingscale=-1;
	health=275;
	targetplayer=NULL;
	b_Activated=false;
	m_acceleration = 0.01f;
	m_maxwalkingspeed = 0.05f;
	bFollowing = false;
	fighttimer = 0;
	animtimer = 0;
	this->UpdateMyRect();
}

enemy_bonesy::~enemy_bonesy(void)
{
	
}

void enemy_bonesy::Do(void)
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

void enemy_bonesy::Draw(int layer)
{
	if (layer!=0) return;
	if (m_x+256<PCORE->landscape.offset_x) return;
	if (m_x-256>PCORE->landscape.offset_x+(PCORE->gfx.m_d3dpp.BackBufferWidth/PCORE->landscape.m_zoom)) return;
	if (m_y+256<PCORE->landscape.offset_y) return;
	if (m_y-256>PCORE->landscape.offset_y+(PCORE->gfx.m_d3dpp.BackBufferHeight/PCORE->landscape.m_zoom)) return;
	
	enemy_base::pCEnemies->NumEnemiesDrawn++;
	b_Activated=true;

	if (m_xvel>0) m_facingscale=1;
	else if (m_xvel<0) m_facingscale=-1;

	PCORE->SetAlphaMode(false);

	PCORE->gfx.SpriteDrawEx(t_body,
					(this->m_x-PCORE->landscape.offset_x-(32.0f*m_facingscale))*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y)*PCORE->landscape.m_zoom,  
					0,
					0, 
					0,
					m_facingscale*(PCORE->landscape.m_zoom),
					PCORE->landscape.m_zoom,
					(((int)animtimer/2)%3)*64,0,64,64,
					255,255,255,255);

	
	PCORE->gfx.SpriteDraw(t_head,
					(this->m_x-PCORE->landscape.offset_x-(10.0f*m_facingscale))*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y-20.0f)*PCORE->landscape.m_zoom,  
					sin((m_x/10.0f)+(fighttimer>0?fighttimer/50.0f:0))/2.0f,
					16.0f*PCORE->landscape.m_zoom*m_facingscale, 
					32.0f*PCORE->landscape.m_zoom,
					m_facingscale*(PCORE->landscape.m_zoom),
					PCORE->landscape.m_zoom);

	if (DRAW_DEBUG_TEXT)
	{
		PCORE->gfx.DrawTextEx( (m_x-(PCORE->landscape.offset_x))*(PCORE->landscape.m_zoom),
		m_y-70-(PCORE->landscape.offset_y), 255,255,0,"decision:%i\nhealth:%i",(int)this->m_nextdecision, (int)this->health);
		PCORE->gfx.DrawTextEx( (m_x-(PCORE->landscape.offset_x)-3)*(PCORE->landscape.m_zoom),
		 (m_y-(PCORE->landscape.offset_y)-3)*(PCORE->landscape.m_zoom), 255,255,0,"x");
	}
}

bool enemy_bonesy::LoadStuff(void)
{
	//s_bulletflesh1 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");

	t_body = PCORE->textures.LoadTexture("textures/enemies/bonesy/body.bmp");
	t_head = PCORE->textures.LoadTexture("textures/enemies/bonesy/head1.bmp");

	s_hit1 = PCORE->sound.LoadSound(4,"sound/enemies/bonesy/boneshot1.ogg");
	s_hit2 = PCORE->sound.LoadSound(4,"sound/enemies/bonesy/boneshot2.ogg");
	s_hit3 = PCORE->sound.LoadSound(4,"sound/enemies/bonesy/boneshot3.ogg");
	s_hit4 = PCORE->sound.LoadSound(4,"sound/enemies/bonesy/boneshot4.ogg");

	s_shoot1 = PCORE->sound.LoadSound(4,"sound/enemies/bonesy/throw1.ogg");
	s_shoot2 = PCORE->sound.LoadSound(4,"sound/enemies/bonesy/throw2.ogg");
	s_shoot3 = PCORE->sound.LoadSound(4,"sound/enemies/bonesy/throw3.ogg");

	return true;
}

void enemy_bonesy::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void enemy_bonesy::HandleFalling(void)
{
	GRECT trect;

	UpdateMyRect();
	SetGRect(&trect,my_rect.left,my_rect.top,my_rect.right,my_rect.bottom+1);
	if (!PCORE->landscape.Collision(&trect))
	{
		m_yvel = m_yvel + 0.009*TICK;
	}
	else
	{
		if (PCORE->landscape.Collision(&my_rect))
		{
			for (int i=0;i<100;i++)
			{
				m_y = m_y-1;
				UpdateMyRect();
				if (!PCORE->landscape.Collision(&my_rect))
				{
					m_yvel = 0;
					break;
				}
			}
		}
		m_yvel = 0;
	}


	m_y = m_y + (m_yvel*TICK*0.3f);

}

void enemy_bonesy::HandleWalking(void)
{
	// bonesy is smart - he only walks when he sees a player he can fuck up
	if (targetplayer==NULL ||!bFollowing ||fighttimer>0) 
	{
		animtimer = 0;
		return;
	}

	if (targetplayer->m_fX<m_x) m_xvel = m_xvel - (m_acceleration*TICK);
	else						m_xvel = m_xvel + (m_acceleration*TICK);

	animtimer = animtimer - (m_acceleration*TICK);
	if (animtimer<0) animtimer = 10000;

	if (m_xvel>m_maxwalkingspeed) m_xvel = m_maxwalkingspeed;
	else if (m_xvel<-m_maxwalkingspeed) m_xvel = -m_maxwalkingspeed;

	GRECT trect;
	SetGRect(&trect,my_rect.left+(m_xvel*TICK),my_rect.top,my_rect.right+(m_xvel*TICK),my_rect.bottom);
	SetGRect(&my_rect,0,0,0,0);
	if (PCORE->landscape.Collision(&trect) || PCORE->enemies.BulletHitEnemyRect(&trect,NULL))
	{
		m_xvel=0;
		bFollowing = false;
		//todo step up
	}
	UpdateMyRect();

	m_x = m_x + (m_xvel*TICK);

}

void enemy_bonesy::DecideWhatToDo(void)
{
	if (m_yvel!=0) return; // can't do jack shit while falling

	float dist, xdist, ydist;
	CPlayers* playerpointer;

	// can we see a player
	dist = PCORE->landscape.GetNearestPlayer((m_x+(m_facingscale*20)), m_y, &playerpointer, &xdist, &ydist);
		
	if (fabs(ydist)<200 && fabs(xdist)<1000)
	{
		if (fabs(dist)>100)
		{
			bFollowing = true;
		}
		else
		{
			bFollowing = false;
		}
		targetplayer = playerpointer;
		// if close enough to fight then fight
		if (fabs(dist)<800 && fighttimer<-1000)
		{
			fighttimer = 350.0f;
			int rsound = rand()%3;

			if (rsound==0)
				PCORE->sound.PlaySnd(s_shoot1, m_x, m_y);
			else if (rsound==1)
				PCORE->sound.PlaySnd(s_shoot2, m_x, m_y);
			else 
				PCORE->sound.PlaySnd(s_shoot3, m_x, m_y);
		}
	}
	else
	{
		targetplayer=NULL;
		bFollowing = false;
	}

}

bool enemy_bonesy::BulletHit(float* x, float* y)
{
	if (!this->b_Activated) return false;
	if (*x<my_rect.left) return false;
	if (*x>my_rect.right) return false;
	if (*y<my_rect.top) return false;
	if (*y>my_rect.bottom) return false;
	return true;
}

bool enemy_bonesy::BulletHitRect(GRECT* r)
{
	if (!this->b_Activated) return false;
	if (IntersectGRect(r,&my_rect)) return true;
	return false;
}
void enemy_bonesy::TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet)
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

		for (int i=0;i<12;i++)
		{
			particle = PCORE->landscape.AddParticle(PARTICLE_BULLETBONEBOUNCE, m_x, m_y-16+(rand()%60));
			if (particle!=NULL)
			{
					randomnumber = ((rand()%4000)/10000.0f)-0.2f;
					particle->xvel = randomnumber;
					randomnumber = ((rand()%2000)/5000.0f)-0.3f;
					particle->yvel = randomnumber;
					randomnumber = ((rand()%1000)/10000.0f)-0.05f;
					particle->rotvel = randomnumber;
			}
		}
	
		if (bullet!=NULL)
		{
			for (int i=0;i<3;i++)
			{
				particle = PCORE->landscape.AddParticle(PARTICLE_BULLETBONEBOUNCE, bullet->x, bullet->y);
				if (particle!=NULL)
				{
						randomnumber = 0.1f+((rand()%1000)/20000.0f);
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
			int rsound = rand()%4;

			if (rsound==0)
				PCORE->sound.PlaySnd(s_hit1, m_x, m_y);
			else if (rsound==1)
				PCORE->sound.PlaySnd(s_hit2, m_x, m_y);
			else if (rsound==2)
				PCORE->sound.PlaySnd(s_hit3, m_x, m_y);
			else 
				PCORE->sound.PlaySnd(s_hit4, m_x, m_y);
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
		for (int j=0;j<4;j++)
		{
			particle = PCORE->landscape.AddParticle(PARTICLE_BONEDUST, x, y);
			if (particle!=NULL)
			{
				particle->xscale = j*0.6f;
			}
		}

		if (rand()%2==0)
		for (int j=0;j<1;j++)
		{
			particle = PCORE->landscape.AddParticle(PARTICLE_BONECHUNK, x, y);
			if (particle!=NULL)
			{
				randomangle = angle + (((rand()%2000)-1000)/70000.0f);
				randomnumber = 1.9f+((rand()%500)/1000.0f);
				particle->xvel = sin(randomangle)*(randomnumber);
				particle->yvel = cos(randomangle)*(randomnumber);
				particle->SpawnDistanceReset = 4.0f;
				particle->SpawnParticle = PARTICLE_BONEDUST;
			}
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
	else
		BulletHitSplash(x, y, 10);

	if (bullet!=NULL)
		AttachParticle(bullet);

	//if (rand()%3==0) PCORE->sound.PlaySnd(s_bulletflesh1,m_x,m_y);
	//else if (rand()%2==0) PCORE->sound.PlaySnd(s_bulletflesh2,m_x,m_y);
	//else PCORE->sound.PlaySnd(s_bulletflesh3,m_x,m_y);


}

void enemy_bonesy::HandleAttacking(void)
{
	if (targetplayer==NULL) return;

	float oldft=fighttimer;
	fighttimer = fighttimer -  (PCORE->gfx.tick);

	if (oldft>0 && fighttimer<=0) // shoot bullet
	{
		LPParticle p = PCORE->landscape.AddParticle(PARTICLE_BULLETBONE,m_x,m_y+16.0f);

		if (p!=NULL)
		{
			p->pplayer = NULL;
			float angle = atan2(targetplayer->m_fX-(m_x),targetplayer->m_fY-(m_y+12.0f));
			p->xvel = sin(angle)*0.5f;
			p->yvel = cos(angle)*0.5f;
			p->damage = BONESY_BONEDAMAGE;
		}
		
	}
}

void enemy_bonesy::BulletHitSplash(float x, float y, float speed)
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

float enemy_bonesy::GetX()
{
	return m_x;
}
float enemy_bonesy::GetY()
{
	return m_y;
}
float enemy_bonesy::GetFacing()
{
	return m_facingscale;
}

bool enemy_bonesy::SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker)
{
	if (x<this->m_x-size) return false;
	if (x>this->m_x+size) return false;
	if (y-20<this->m_y-size) return false;
	if (y-20>this->m_y+size) return false;

	float distance = sqrtf( (m_x-x)*(m_x-x) + (m_y-y-20)*(m_y-y-20));

	if (distance>size) return false;

	if (this->health>=0)
	{
		TakeDamage(((size-distance)/size)*damage, atan2(m_x-x, m_y-y-20), x, y, true);

		if (this->health<0 && attacker!=NULL)
		{
			
			attacker->CountKill();
			attacker->AddBulletTime(SKILL_BT_BONESY);
			
		}
	}
	return true;
}
void enemy_bonesy::AttachParticle(LPParticle particle)
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
			dparticle->attached_x = (particle->x - this->m_x)* this->m_facingscale ;// ;
			dparticle->attached_y = particle->y - this->m_y;
			dparticle->attached_rot = atan2(sin(particle->rot),cos(particle->rot)* this->m_facingscale)+ (((rand()%20000/10000.f)-1)*0.1f);
			dparticle->IsMoving = false;
		}
	}
}

void enemy_bonesy::UpdateMyRect(void)
{
	my_rect.left = m_x-4;
	my_rect.right = m_x+4;
	my_rect.top = m_y-16;
	my_rect.bottom = m_y+64;
}

void enemy_bonesy::DoFireDamage(float x, float y, float damage)
{
	LPParticle dparticle=NULL;

	dparticle = PCORE->landscape.AddParticle(PARTICLE_FIREBITATTACHED,x,y);		
	
	if (dparticle!=NULL)
		{
			dparticle->AttachedTo = ATTACHED_TO_MONSTER;
			dparticle->pmonster = this;
			dparticle->attached_x = (x - this->m_x)* this->m_facingscale ;// ;
			dparticle->attached_y = y - this->m_y;
			dparticle->IsMoving = false;
			dparticle->SpawnParticle = PARTICLE_FIREBITRISERATTACHED;
			dparticle->SpawnTimerReset = 20;
			dparticle->SpawnDistance = 100;
			dparticle->damage = FIRE_DAMAGE;
		}

}
