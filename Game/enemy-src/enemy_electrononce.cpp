#include "enemy_electrononce.h"

#define PIXELS_TO_FEET 32
#define MAX_WALKING_SPEED m_maxwalkingspeed
#define NORMAL_ACCELERATION m_acceleration
#define WIDTH 32

#define MAXSTEPSIZE 0

#define DRAW_DEBUG_TEXT false
#define GENERICTHINKDELAY 1000

#define COLLISION enemy_base::pCEnemies->pcore->landscape.Collision

#define PCORE enemy_base::pCEnemies->pcore

enemy_electrononce::enemy_electrononce(void)
{
	m_x=0;
	m_y=0;
	pCEnemies=NULL;
	m_xvel=0;
	dwFlags=0;
	m_nextdecision=1+rand()%10;
	f_attacktimer = 1000+rand()%100;
	m_facingscale=-1;
	health=2000;
	targetplayer=NULL;
	b_Activated=false;
	m_acceleration = 0.005f+((rand()%500)/10000.0f);;
	m_maxwalkingspeed = 0.15f+((rand()%1000)/10000.0f);
	m_scale = 1.0f;
	this->UpdateMyRect();
	this->iStatus=AI_LEFT;
}

enemy_electrononce::~enemy_electrononce(void)
{
	
}

void enemy_electrononce::Do(void)
{
	if (!b_Activated) return;
	enemy_base::pCEnemies->NumEnemiesActivated++;


	if (m_nextdecision>0) 
	{
		m_nextdecision = m_nextdecision - 1*(enemy_base::pCEnemies->pcore->gfx.tick);

		if (m_nextdecision<=0)
		{
			DecideWhatToDo();
		}
	}

	HandleFalling();
	HandleWalking();
	HandleAttacking();

}

void enemy_electrononce::DrawNormalMap()
{
}


void enemy_electrononce::Draw(int layer)
{
	
	if (layer!=0) return;
	if (m_x+256<PCORE->landscape.offset_x) return;
	if (m_x-256>PCORE->landscape.offset_x+(PCORE->gfx.m_d3dpp.BackBufferWidth/PCORE->landscape.m_zoom)) return;
	if (m_y+256<PCORE->landscape.offset_y) return;
	if (m_y-256>PCORE->landscape.offset_y+(PCORE->gfx.m_d3dpp.BackBufferHeight/PCORE->landscape.m_zoom)) return;
	
	enemy_base::pCEnemies->NumEnemiesDrawn++;
	if (b_Activated==false)
	{
		//m_xvel = ((rand()%1000)-500)/1000.0f;
	}
	b_Activated=true;

	

	m_facingscale=1;

	float angle=0;

	angle = angle + m_xvel;

	PCORE->SetAlphaMode(false);
	PCORE->gfx.SpriteDrawEx(this->t_body,
					((this->m_x-(64.0f*m_scale))-PCORE->landscape.offset_x)*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y-(128.0f*m_scale))*PCORE->landscape.m_zoom,
					angle,
					64*PCORE->landscape.m_zoom*m_scale,
					(128-31)*PCORE->landscape.m_zoom*m_scale,
					PCORE->landscape.m_zoom*m_scale,
					PCORE->landscape.m_zoom*m_scale,
					0,0,128,128,255,255,255,255);
	

	if (DRAW_DEBUG_TEXT)
	{
		PCORE->gfx.DrawTextEx( (m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x))*(enemy_base::pCEnemies->pcore->landscape.m_zoom),
		m_y-70-(PCORE->landscape.offset_y), 255,255,0,"decision:%i\nhealth:%i",(int)this->m_nextdecision, (int)this->health);
		PCORE->gfx.DrawTextEx( (m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x)-3)*(enemy_base::pCEnemies->pcore->landscape.m_zoom),
		 (m_y-(PCORE->landscape.offset_y)-3)*(PCORE->landscape.m_zoom), 255,255,0,"x");
	}
}

bool enemy_electrononce::LoadStuff(void)
{
	t_body = PCORE->textures.LoadTexture("textures/enemies/electrononce/body.tga");

	return true;
}

void enemy_electrononce::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void enemy_electrononce::HandleFalling(void)
{
	if (targetplayer!=NULL)
	{
		if (fabs(targetplayer->m_fY-200-m_y)<50)
		{
			m_yvel = m_yvel *0.98f;
		}
		else
		{
			if ((targetplayer->m_fY-200-m_y)<0)
				this->m_yvel += -0.01f;
			else if ((targetplayer->m_fY-200-m_y)>0)
				this->m_yvel += 0.01f;
		}
	}

	if (m_yvel>0.22) m_yvel=0.22;
	if (m_yvel<-0.22) m_yvel=-0.22;

	this->UpdateMyRect();
	GRECT trect;

	//m_yvel = m_yvel + (0.0002f*PCORE->gfx.tick)*(2-m_scale);

	// are we colliding?
	SetGRect(&trect,my_rect.left+1,my_rect.top+(m_yvel*(PCORE->gfx.tick)),my_rect.right-1,my_rect.bottom+(m_yvel*(PCORE->gfx.tick)));
	if (PCORE->landscape.Collision(&trect))
	{
		m_yvel = m_yvel*-0.99f;
	}

	// add y velocity
	if (m_yvel!=0)	m_y = m_y + (m_yvel*PCORE->gfx.tick);
	// if fell outa screen then kill
	if (m_y+32>(PCORE->landscape.height*32)) this->TakeDamage(100,3,m_x,m_y);
}

void enemy_electrononce::HandleWalking(void)
{
	if (targetplayer!=NULL)
	{
		if (fabs(targetplayer->m_fX-m_x)<100)
		{
			m_xvel = m_xvel *0.98f;
		}
		else
		{
			if ((targetplayer->m_fX-m_x)<0)
				this->m_xvel += -0.01f;
			else if ((targetplayer->m_fX-m_x)>0)
				this->m_xvel += 0.01f;
		}
	}

	if (m_xvel>0.32) m_xvel=0.32;
	if (m_xvel<-0.32) m_xvel=-0.32;


	this->UpdateMyRect();
	GRECT crect;
	SetGRect(&crect,my_rect.left+(m_xvel*PCORE->gfx.tick),my_rect.top,my_rect.right+(m_xvel*PCORE->gfx.tick),my_rect.bottom-1);
	if (PCORE->landscape.Collision(&crect))
	{
		m_xvel = -m_xvel;
	}

	if (m_xvel!=0)
	{
		m_x = m_x + (m_xvel*PCORE->gfx.tick);

		// moved - can we fall in this new position?
		if (m_yvel==0)
		{
			this->UpdateMyRect();
			GRECT trect;
			SetGRect(&trect,my_rect.left+3,my_rect.top+2,my_rect.right-3,my_rect.bottom+2);
			if (!PCORE->landscape.Collision(&trect))
			{
				m_yvel = 0.01f;

			}
			else
			{

			}
		}
		else
		{

		}
	
	}

	

}

void enemy_electrononce::DecideWhatToDo(void)
{
	if (iStatus!=AI_WAIT) return;

	float dist, xdist, ydist;

	dist = PCORE->landscape.GetNearestPlayer((m_x+(m_facingscale*20)), m_y, &this->targetplayer, &xdist, &ydist);
	
	m_nextdecision=GENERICTHINKDELAY;
}

bool enemy_electrononce::BulletHit(float* x, float* y)
{
	if (!this->b_Activated) return false;
	if (*x<my_rect.left) return false;
	if (*x>my_rect.right) return false;
	if (*y<my_rect.top) return false;
	if (*y>my_rect.bottom) return false;
	return true;
}

bool enemy_electrononce::BulletHitRect(GRECT* r)
{
	if (!this->b_Activated) return false;
	if (IntersectGRect(r,&my_rect)) return true;
	return false;
}
void enemy_electrononce::TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet)
{
	LPParticle particle = NULL;
	float randomangle = angle;
	float randomnumber;
	float beforehealth=health;

	health=health-damage;




	if (health<0 && beforehealth>=0)
	{
		if (bullet!=NULL)
		if (bullet->pplayer!=NULL)
		{
			bullet->pplayer->CountKill();
			bullet->pplayer->AddBulletTime(SKILL_BT_HARDROCK);
		}

		PCORE->landscape.MakeExplosion(m_x,m_y,100,50);
		PCORE->landscape.DrawExplosion(m_x,m_y,100);


		PCORE->landscape.UnAttachAttachmentsMonster(this);
	}

	if (bullet!=NULL)
	{
		if (bullet->type==PARTICLE_FIREBITRISER || bullet->type==PARTICLE_FIRE)
		{	
			if ((bullet->type==PARTICLE_FIRE && rand()%2==0) || (bullet->type==PARTICLE_FIREBITRISER && rand()%10==0))
				this->DoFireDamage(x, y, damage);
			return;
		}
		else
		{
			this->m_xvel += bullet->xvel*0.35f;
			this->m_yvel += bullet->yvel*0.35f;
		}
	}

	if (!Splash)
	{
		PCORE->landscape.AddParticle(PARTICLE_SPARKBALL, x,y);
		for (int i=0;i<15;i++)
		PCORE->landscape.AddSpark(x,y,-angle+((rand()%1000-500)/300.0f),0.2+rand()%1000/1200.0f);
	}

	if (bullet!=NULL)
		AttachParticle(bullet);



}

void enemy_electrononce::HandleAttacking(void)
{
	float dist, xdist, ydist;

	f_attacktimer = f_attacktimer - PCORE->gfx.tick;
	if (f_attacktimer>0) return;

	dist = PCORE->landscape.GetNearestPlayer(m_x, m_y, &targetplayer, &xdist, &ydist);

	if (targetplayer==NULL) return;

	if (dist<300)
	{
	//	PCORE->landscape.AddParticle(PARTICLE_GUNSPARK,m_x,m_y-(31*0.6f));


		PCORE->landscape.Lightning(m_x,m_y-(31*0.6f),targetplayer->m_fX+rand()%100-50.0f,targetplayer->m_fY+rand()%100-50.0f,24,0.9f, 0.1f, false,0);
		PCORE->landscape.Lightning(m_x,m_y-(31*0.6f),targetplayer->m_fX+rand()%100-50.0f,targetplayer->m_fY+rand()%100-50.0f,12,0.4f, 0.3f, false, 0);
		PCORE->landscape.Lightning(m_x,m_y-(31*0.6f),targetplayer->m_fX,targetplayer->m_fY,8,0.9f, 0.6f, true,2);
		targetplayer->TakeDamage(0.3f,NULL);
		targetplayer->ElectricDamage();

	//	for (int i=0;i<30;i++)
	//	PCORE->landscape.AddSpark(targetplayer->m_fX,targetplayer->m_fY,(rand()%5000)/1000.0f,0.1f+(rand()%200)/400.0f);


	}

	f_attacktimer = 100;//30+rand()%500;

}

void enemy_electrononce::BulletHitSplash(float x, float y, float speed)
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

float enemy_electrononce::GetX()
{
	return m_x;
}
float enemy_electrononce::GetY()
{
	return m_y;
}
float enemy_electrononce::GetFacing()
{
	return m_facingscale;
}

bool enemy_electrononce::SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker)
{
	if (x<this->m_x-size) return false;
	if (x>this->m_x+size) return false;
	if (y-20<this->m_y-size) return false;
	if (y-20>this->m_y+size) return false;
	if (!this->b_Activated) return false;

	float distance = sqrtf( (m_x-x)*(m_x-x) + (m_y-y-20)*(m_y-y-20));

	if (distance>size) return false;

	if (this->health>=0)
	{
		TakeDamage(((size-distance)/size)*damage, atan2(m_x-x, m_y-y-20), x, y, true);

		if (this->health<0 && attacker!=NULL)
		{
			attacker->CountKill();
			attacker->AddBulletTime(SKILL_BT_HARDROCK);
		}
	}
	return true;
}
void enemy_electrononce::AttachParticle(LPParticle particle)
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
		//	dparticle->AttachedTo = ATTACHED_TO_MONSTER;
		//	dparticle->pmonster = this;
			dparticle->attached_x = (particle->x - this->m_x)* this->m_facingscale ;// ;
			dparticle->attached_y = particle->y - this->m_y;
			dparticle->attached_rot = atan2(sin(particle->rot),cos(particle->rot)* this->m_facingscale)+ (((rand()%20000/10000.f)-1)*0.1f);
			dparticle->xvel = particle->xvel*-(rand()%1000/1000.0f);
			dparticle->yvel = particle->yvel*-(rand()%1000/1000.0f);
			dparticle->rotvel = rand()%1000/10000.0f;
			dparticle->IsMoving = true;
		}
	}
}

void enemy_electrononce::UpdateMyRect(void)
{
	my_rect.left = m_x-(57*m_scale);
	my_rect.right = m_x+(57*m_scale);
	my_rect.top = m_y-(120*m_scale);
	my_rect.bottom = m_y-(38*m_scale);
}

void enemy_electrononce::DoFireDamage(float x, float y, float damage)
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

void enemy_electrononce::SetUp(float ix, float iy, float a, float b, char* string1, char* string2)
{

}


