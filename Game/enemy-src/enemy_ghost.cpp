#include "enemy_ghost.h"

#define PIXELS_TO_FEET 32
#define MAX_WALKING_SPEED m_maxwalkingspeed
#define NORMAL_ACCELERATION m_acceleration
#define WIDTH 32

#define MAXSTEPSIZE 0

#define DRAW_DEBUG_TEXT false
#define GENERICTHINKDELAY 500

#define COLLISION enemy_base::pCEnemies->pcore->landscape.Collision

#define PCORE enemy_base::pCEnemies->pcore

enemy_ghost::enemy_ghost(void)
{
	m_x=0;
	m_y=0;
	m_yvel=0;
	pCEnemies=NULL;
	m_xvel=0;
	dwFlags=0;
	m_nextdecision=1+rand()%10;
	f_attacktimer = 1000+rand()%100;
	m_facingscale=-1;
	health=25;
	targetplayer=NULL;
	b_Activated=false;
	m_acceleration = 0.005f;
	m_maxwalkingspeed = 0.1f;
	m_scale = 1.0f;
	this->UpdateMyRect();
}

enemy_ghost::~enemy_ghost(void)
{
	
}

void enemy_ghost::Do(void)
{
	if (!b_Activated) return;

	if (f_attacktimer>0)
	{
		f_attacktimer = f_attacktimer - PCORE->gfx.tick;
	}

	enemy_base::pCEnemies->NumEnemiesActivated++;

	if (m_nextdecision>0) 
	{
		m_nextdecision = m_nextdecision - 1*(enemy_base::pCEnemies->pcore->gfx.tick);

		if(m_nextdecision<=0 && !(dwFlags & AISTATE_COMPLETINGTASK))
		{
			DecideWhatToDo();
		}
	}
	HandleFalling();
	HandleWalking();
	HandleAttacking();

}

void enemy_ghost::Draw(int layer)
{
	if (layer!=1) return;
	if (m_x+256<PCORE->landscape.offset_x) return;
	if (m_x-256>PCORE->landscape.offset_x+(PCORE->gfx.m_d3dpp.BackBufferWidth/PCORE->landscape.m_zoom)) return;
	if (m_y+256<PCORE->landscape.offset_y) return;
	if (m_y-256>PCORE->landscape.offset_y+(PCORE->gfx.m_d3dpp.BackBufferHeight/PCORE->landscape.m_zoom)) return;
	
	enemy_base::pCEnemies->NumEnemiesDrawn++;
	b_Activated=true;

	if (m_xvel>0) m_facingscale=1;
	else if (m_xvel<0) m_facingscale=-1;

	float angle=0;

	//angle = atan2(m_xvel,m_yvel)+(D3DX_PI/2);
	//if (m_facingscale>0) angle = angle - ((D3DX_PI));

	PCORE->SetAlphaMode(true);

	PCORE->gfx.SpriteDraw(this->t_body,
					((this->m_x-(32.0f*m_facingscale*m_scale))-PCORE->landscape.offset_x)*PCORE->landscape.m_zoom,
					((this->m_y-(32.0f*              m_scale))-PCORE->landscape.offset_y)*PCORE->landscape.m_zoom,  
					0,0,0,
					m_facingscale*(PCORE->landscape.m_zoom)*m_scale,PCORE->landscape.m_zoom*m_scale);

	if (DRAW_DEBUG_TEXT)
	{
		PCORE->gfx.DrawTextEx( (m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x))*(enemy_base::pCEnemies->pcore->landscape.m_zoom),
		m_y-70-(PCORE->landscape.offset_y), 255,255,0,"decision:%i\nhealth:%i",(int)this->m_nextdecision, (int)this->health);
		PCORE->gfx.DrawTextEx( (m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x)-3)*(enemy_base::pCEnemies->pcore->landscape.m_zoom),
		 (m_y-(PCORE->landscape.offset_y)-3)*(PCORE->landscape.m_zoom), 255,255,0,"x");
	}
}

bool enemy_ghost::LoadStuff(void)
{
	s_bite = PCORE->sound.LoadSound(2,"sound/enemies/ghost/bite.ogg");
	s_die = PCORE->sound.LoadSound(2,"sound/enemies/ghost/die.ogg");
		
	t_body = PCORE->textures.LoadTexture("textures/enemies/ghost/ghost1.bmp");

	return true;
}

void enemy_ghost::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void enemy_ghost::HandleFalling(void)
{
	if (targetplayer!=NULL)
	{
		m_yvel=m_yvel+(cos(aimangle)*NORMAL_ACCELERATION);
		if (fabs(m_yvel)>MAX_WALKING_SPEED) m_yvel=m_yvel-(cos(aimangle)*NORMAL_ACCELERATION);
	}

	m_y = m_y + (m_yvel*PCORE->gfx.tick);
	this->UpdateMyRect();
}

void enemy_ghost::HandleWalking(void)
{

	if (targetplayer!=NULL)
	{
		m_xvel= m_xvel+(sin(aimangle)*NORMAL_ACCELERATION);
		if (fabs(m_xvel)>MAX_WALKING_SPEED) m_xvel=m_xvel-(sin(aimangle)*NORMAL_ACCELERATION);

	}

	m_x = m_x + (m_xvel*PCORE->gfx.tick);
	this->UpdateMyRect();
}

void enemy_ghost::DecideWhatToDo(void)
{
	float dist, xdist, ydist;
	CPlayers* playerpointer;

	// randomly moving left or right for no real reason
	dist = PCORE->landscape.GetNearestPlayer((m_x+(m_facingscale*20)), m_y, &playerpointer, &xdist, &ydist);
		
	if (fabs(dist)<1500 && playerpointer!=NULL)
	{
		if (targetplayer!=playerpointer) // if this is teh first time we've spotted this player
		{
		}

		targetplayer = playerpointer;
		this->aimangle = atan2(targetplayer->m_fX-m_x,targetplayer->m_fY-m_y);
	}
	else
	{
		targetplayer = NULL;
	}

	
	m_nextdecision=GENERICTHINKDELAY;
}

bool enemy_ghost::BulletHit(float* x, float* y)
{
	if (!this->b_Activated) return false;

	if (*x<my_rect.left) return false;
	if (*x>my_rect.right) return false;
	if (*y<my_rect.top) return false;
	if (*y>my_rect.bottom) return false;
	return true;
}

bool enemy_ghost::BulletHitRect(GRECT* r)
{
	if (!this->b_Activated) return false;
	if (IntersectGRect(r,&my_rect)) return true;
	return false;
}
void enemy_ghost::TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet)
{
	LPParticle particle = NULL;

	if (bullet==NULL && !Splash) return;

	if (bullet!=NULL)
	if (bullet->type!=PARTICLE_FIREBITRISER && bullet->type!=PARTICLE_FIRE && !Splash && bullet->type!=PARTICLE_IMPACTGRENADE && bullet->type!=PARTICLE_MOLOTOV) 
	{
		particle = PCORE->landscape.AddParticle(bullet->type, x, y);
		if (particle!=NULL)
		{
			particle->xscale =  bullet->xscale;
			particle->yscale = bullet->yscale;
			particle->alpha = bullet->alpha;

			particle->xvel = (bullet->xvel*0.7f) + ((rand()%1000-500)/1000.0f);
			particle->yvel = (bullet->yvel*0.7f) + ((rand()%1000-500)/1000.0f);
		}
			
		return;
	}


	float randomangle = angle;
	float randomnumber;
	float beforehealth=health;

	health=health-damage;

	if (health<0 && beforehealth>=0)
	{
		PCORE->sound.PlaySnd(s_die, m_x, m_y,0.0f);

		if (bullet!=NULL)
		if (bullet->pplayer!=NULL)
		{
			bullet->pplayer->CountKill();
			bullet->pplayer->AddBulletTime(SKILL_BT_RAT);
		}

		PCORE->landscape.UnAttachAttachmentsMonster(this);
		for (int j=0;j<15;j++)
		{
			particle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_BLOODBLOBS, m_x+rand()%32, m_y+16+rand()%16,0,0,0,PCORE->landscape.t_BloodBlobs[rand()%NUM_BLOODBLOBS],0);
			if (particle!=NULL)
			{
				randomnumber = (rand()%1000)/1000.0f;
				particle->xvel = (randomnumber)/10;
				randomnumber = (rand()%1000)/3000.0f;
				particle->yvel = -(randomnumber);
				particle->xscale = 0.2f+(rand()%1000)/1000.0f;
				particle->yscale = 0.2f+(rand()%1000)/1000.0f;
			}
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

	if (bullet!=NULL)
		AttachParticle(bullet);

}

void enemy_ghost::HandleAttacking(void)
{
	if (f_attacktimer>0) return;
	if (targetplayer==NULL) return;

	if (IntersectGRect(&targetplayer->my_rect, &my_rect))
	{
		if (m_yvel!=0)
		{
			m_xvel= -m_xvel;
			m_yvel = -m_yvel;
		}
		InitAttack();
		return;
	}
}

void enemy_ghost::InitAttack(void)
{
	if (f_attacktimer>0) return;

	f_attacktimer  = 600+rand()%200;
	m_nextdecision = 100;

	targetplayer->TakeDamage(6,NULL);
	PCORE->landscape.GenericBloodSpurt(m_x,m_y+24,atan2(m_xvel,m_yvel),0.5f);
	PCORE->sound.PlaySnd(s_bite, m_x, m_y,0.0f);
}

void enemy_ghost::BulletHitSplash(float x, float y, float speed)
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

float enemy_ghost::GetX()
{
	return m_x;
}
float enemy_ghost::GetY()
{
	return m_y;
}
float enemy_ghost::GetFacing()
{
	return m_facingscale;
}

bool enemy_ghost::SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker)
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
			attacker->AddBulletTime(SKILL_BT_RAT);
		}
	}
	return true;
}
void enemy_ghost::AttachParticle(LPParticle particle)
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

void enemy_ghost::UpdateMyRect(void)
{
	my_rect.left = m_x-16;
	my_rect.right = m_x+16;
	my_rect.top = m_y-16;
	my_rect.bottom = m_y+16;
}

void enemy_ghost::DoFireDamage(float x, float y, float damage)
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
