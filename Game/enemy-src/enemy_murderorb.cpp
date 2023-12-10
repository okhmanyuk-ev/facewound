#include "enemy_murderorb.h"

#define PIXELS_TO_FEET 32
#define MAX_WALKING_SPEED m_maxwalkingspeed
#define NORMAL_ACCELERATION m_acceleration
#define WIDTH 32

#define MAXSTEPSIZE 0

#define DRAW_DEBUG_TEXT false
#define GENERICTHINKDELAY 1000

#define COLLISION enemy_base::pCEnemies->pcore->landscape.Collision

#define PCORE enemy_base::pCEnemies->pcore

enemy_murderorb::enemy_murderorb(void)
{
	m_x=0;
	m_y=0;
	pCEnemies=NULL;
	m_xvel=0;
	dwFlags=0;
	m_nextdecision=1+rand()%10;
	f_attacktimer = 1000+rand()%100;
	m_facingscale=-1;
	health=10;
	targetplayer=NULL;
	b_Activated=false;
	m_acceleration = 0.005f+((rand()%500)/10000.0f);;
	m_maxwalkingspeed = 0.15f+((rand()%1000)/10000.0f);
	m_scale = 0.5f;
	this->UpdateMyRect();
	this->iStatus=AI_LEFT;
}

enemy_murderorb::~enemy_murderorb(void)
{
	
}

void enemy_murderorb::Do(void)
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

void enemy_murderorb::DrawNormalMap()
{
	return;

	int alpha=m_scale*255;
	if (alpha>255) alpha = 255;
	if (alpha<0) alpha = 0;

	PCORE->gfx.SpriteDrawEx(this->t_normal,
					((this->m_x-(128.0f*m_scale))-PCORE->landscape.offset_x)*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y-(128.0f*m_scale))*PCORE->landscape.m_zoom,
					0,128*PCORE->landscape.m_zoom*m_scale,128*PCORE->landscape.m_zoom*m_scale,
					PCORE->landscape.m_zoom*m_scale,
					PCORE->landscape.m_zoom*m_scale,
					0,0,256,256,alpha,255,255,255);
}


void enemy_murderorb::Draw(int layer)
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

	if (m_yvel!=0)
	{
		angle = atan2(m_xvel,m_yvel)+(D3DX_PI/2);
		if (m_facingscale>0) angle = angle - ((D3DX_PI));
	}
	else
	{
		angle = 0;
	}

	PCORE->SetAlphaMode(false);
	PCORE->gfx.SpriteDrawEx(this->t_body,
					((this->m_x-(128.0f*m_scale))-PCORE->landscape.offset_x)*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y-(128.0f*m_scale))*PCORE->landscape.m_zoom,
					this->m_x*0.01f,128*PCORE->landscape.m_zoom*m_scale,128*PCORE->landscape.m_zoom*m_scale,
					PCORE->landscape.m_zoom*m_scale,
					PCORE->landscape.m_zoom*m_scale,
					0,0,256,256,255,255,255,255);

	PCORE->SetAlphaMode(true);
	PCORE->gfx.SpriteDrawEx(t_light,
					((this->m_x-(128.0f*m_scale))-PCORE->landscape.offset_x)*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y-(128.0f*m_scale))*PCORE->landscape.m_zoom,
					sin(this->m_x*0.01f)*0.01f,128*PCORE->landscape.m_zoom*m_scale,128*PCORE->landscape.m_zoom*m_scale,
					PCORE->landscape.m_zoom*m_scale,
					PCORE->landscape.m_zoom*m_scale,
					0,0,256,256,255,255,255,255);
	PCORE->SetAlphaMode(false);
	

	if (DRAW_DEBUG_TEXT)
	{
		PCORE->gfx.DrawTextEx( (m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x))*(enemy_base::pCEnemies->pcore->landscape.m_zoom),
		m_y-70-(PCORE->landscape.offset_y), 255,255,0,"decision:%i\nhealth:%i",(int)this->m_nextdecision, (int)this->health);
		PCORE->gfx.DrawTextEx( (m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x)-3)*(enemy_base::pCEnemies->pcore->landscape.m_zoom),
		 (m_y-(PCORE->landscape.offset_y)-3)*(PCORE->landscape.m_zoom), 255,255,0,"x");
	}
}

bool enemy_murderorb::LoadStuff(void)
{
	t_body = PCORE->textures.LoadTexture("textures/enemies/murderorb/body.tga");
	t_light = PCORE->textures.LoadTexture("textures/enemies/murderorb/lightmap.tga");
	t_normal = PCORE->textures.LoadTexture("textures/enemies/murderorb/normal.tga");

	

	s_boing[0] = PCORE->sound.LoadSound(2,"sound/enemies/murderorb/boing1.ogg");
	s_boing[1] = PCORE->sound.LoadSound(2,"sound/enemies/murderorb/boing2.ogg");
	s_boing[2] = PCORE->sound.LoadSound(2,"sound/enemies/murderorb/boing3.ogg");
	
	return true;
}

void enemy_murderorb::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void enemy_murderorb::HandleFalling(void)
{
	//return;

	this->UpdateMyRect();
	GRECT trect;

	m_yvel = m_yvel + (0.0002f*PCORE->gfx.tick)*(2-m_scale);

	// are we colliding?
	SetGRect(&trect,my_rect.left+1,my_rect.top+(m_yvel*(PCORE->gfx.tick)),my_rect.right-1,my_rect.bottom+(m_yvel*(PCORE->gfx.tick)));
	if (PCORE->landscape.Collision(&trect))
	{
		m_yvel = m_yvel*-0.99f;
		this->PlayBoingSound();
	}

	// add y velocity
	if (m_yvel!=0)	m_y = m_y + (m_yvel*PCORE->gfx.tick);
	// if fell outa screen then kill
	if (m_y+32>(PCORE->landscape.height*32)) this->TakeDamage(100,3,m_x,m_y);
}

void enemy_murderorb::HandleWalking(void)
{
/*
	if (iStatus==AI_LEFT)
	{
		m_xvel = -0.2f;
	}
	else if (iStatus==AI_RIGHT)
	{
		m_xvel = 0.2f;
	}
	else
	{
		m_xvel = 0.0f;
	}
*/
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

void enemy_murderorb::DecideWhatToDo(void)
{
	if (iStatus!=AI_WAIT) return;

	float dist, xdist, ydist;

	dist = PCORE->landscape.GetNearestPlayer((m_x+(m_facingscale*20)), m_y, &this->targetplayer, &xdist, &ydist);
	
	m_nextdecision=GENERICTHINKDELAY;
}

bool enemy_murderorb::BulletHit(float* x, float* y)
{
	if (!this->b_Activated) return false;
	if (*x<my_rect.left) return false;
	if (*x>my_rect.right) return false;
	if (*y<my_rect.top) return false;
	if (*y>my_rect.bottom) return false;
	return true;
}

bool enemy_murderorb::BulletHitRect(GRECT* r)
{
	if (!this->b_Activated) return false;
	if (IntersectGRect(r,&my_rect)) return true;
	return false;
}
void enemy_murderorb::TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet)
{
	LPParticle particle = NULL;
	float randomangle = angle;
	float randomnumber;
	float beforehealth=health;

	health=health-damage;




	if (health<0 && beforehealth>=0)
	{
		float attackangle = 0.0f;
		if (bullet!=NULL)
		if (bullet->pplayer!=NULL)
		{
			bullet->pplayer->CountKill();
			bullet->pplayer->AddBulletTime(SKILL_BT_HARDROCK);
			attackangle = atan2(bullet->xvel,bullet->yvel);
			bullet->pplayer->CollectPlutonium(1);
		}

		PCORE->landscape.UnAttachAttachmentsMonster(this);

	
		if (m_scale>0.1f)
		for (int i=-1;i<(m_scale*7.0f);i++)
		{
			PCORE->enemies.AddEnemy(ENT_E_MURDERORB,m_x,m_y,m_scale*0.5f*256.0f,attackangle,NULL,NULL);
		}
	}

	if (bullet!=NULL)
	{
		if (bullet->type==PARTICLE_FIREBITRISER || bullet->type==PARTICLE_FIRE)
		{	
			if ((bullet->type==PARTICLE_FIRE && rand()%2==0) || (bullet->type==PARTICLE_FIREBITRISER && rand()%10==0))
				this->DoFireDamage(x, y, damage);
			return;
		}
	}

	if (!Splash)
	{
		PCORE->landscape.AddStoneDust(x,y,2);
	}

	if (bullet!=NULL)
		AttachParticle(bullet);



}

void enemy_murderorb::HandleAttacking(void)
{
	float xdist, ydist;

	f_attacktimer = f_attacktimer - PCORE->gfx.tick;
	if (f_attacktimer>0) return;

	PCORE->landscape.GetNearestPlayer(m_x, m_y, &targetplayer, &xdist, &ydist);

	if (targetplayer==NULL) return;

	if (IntersectGRect(&targetplayer->my_rect, &my_rect))
	{
	//	m_yvel=0;
		f_attacktimer = 1500;
		m_nextdecision = 100;
		targetplayer->TakeDamage(20.0f*this->m_scale,NULL);
		PCORE->landscape.GenericBloodSpurt(targetplayer->m_fX,targetplayer->m_fY,atan2(m_xvel,m_yvel),0.5f);
		targetplayer->m_fXVel = m_xvel*5;
		targetplayer->m_fYVel = -0.3f;
		targetplayer->m_fY--;
		return;
	}
}

void enemy_murderorb::BulletHitSplash(float x, float y, float speed)
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

float enemy_murderorb::GetX()
{
	return m_x;
}
float enemy_murderorb::GetY()
{
	return m_y;
}
float enemy_murderorb::GetFacing()
{
	return m_facingscale;
}

bool enemy_murderorb::SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker)
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
void enemy_murderorb::AttachParticle(LPParticle particle)
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

void enemy_murderorb::UpdateMyRect(void)
{
	my_rect.left = m_x-(128*m_scale);
	my_rect.right = m_x+(128*m_scale);
	my_rect.top = m_y-(128*m_scale);
	my_rect.bottom = m_y+(128*m_scale);
}

void enemy_murderorb::DoFireDamage(float x, float y, float damage)
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

void enemy_murderorb::SetUp(float ix, float iy, float a, float b, char* string1, char* string2)
{
	if (a==0) m_scale = 0.25f;
	else this->m_scale = a/256.0f;

	health = m_scale*128.0f;
	m_yvel = cos(b)*(rand()%1000/1000.0f)*0.6f;
	m_xvel = sin(b)*(rand()%1000/1000.0f)*0.6f;

}
	
void enemy_murderorb::PlayBoingSound(void)
{
	PCORE->sound.PlaySnd(s_boing[rand()%NUM_BOINGS],m_x,m_y,0.2f);
}

