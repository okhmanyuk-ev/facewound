#include "enemy_rat.h"

#define PIXELS_TO_FEET 32
#define MAX_WALKING_SPEED m_maxwalkingspeed
#define NORMAL_ACCELERATION m_acceleration
#define WIDTH 32

#define MAXSTEPSIZE 0

#define DRAW_DEBUG_TEXT false
#define GENERICTHINKDELAY 1000

#define COLLISION enemy_base::pCEnemies->pcore->landscape.Collision

#define PCORE enemy_base::pCEnemies->pcore

enemy_rat::enemy_rat(void)
{
	m_x=0;
	m_y=0;
	m_yvel=0;
	pCEnemies=NULL;
	m_xvel=0;
	dwFlags=0;
	m_nextdecision=rand()%10;
	f_attacktimer = 1000+rand()%100;
	m_facingscale=-1;
	health=25;
	targetplayer=NULL;
	b_Activated=false;
	m_acceleration = 0.005f+((rand()%500)/10000.0f);;
	m_maxwalkingspeed = 0.15f+((rand()%1000)/10000.0f);
	m_scale = 0.7f+(rand()%300/1000.0f);
	this->UpdateMyRect();
}

enemy_rat::~enemy_rat(void)
{
	
}

void enemy_rat::Do(void)
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

	if (f_attacktimer<=0 && (dwFlags & AISTATE_WALKINGLEFT || dwFlags & AISTATE_WALKINGRIGHT) && !(dwFlags & AISTATE_ATTACKING) && targetplayer!=NULL)
	{
		dwFlags = dwFlags | AISTATE_ATTACKING;

		if (m_yvel==0 && fabs(targetplayer->m_fX-m_x)<300 && fabs(targetplayer->m_fY-m_y)<200)
		{	
			m_yvel= -0.25f-(rand()%1000/10000.0f);
			if (targetplayer->m_fX>m_x) m_xvel= 0.85f;
			else m_xvel= -0.85f;

			if (rand()%2==0)
			{
				PCORE->sound.PlaySnd(s_ratattack1,m_x,m_y,0.1f);
			}
			else
			{
				PCORE->sound.PlaySnd(s_ratattack2,m_x,m_y,0.1f);
			}
		}
	}


	HandleFalling();
	HandleWalking();
	HandleAttacking();

}

void enemy_rat::Draw(int layer)
{
	if (layer!=0) return;
	if (m_x+256<PCORE->landscape.offset_x) return;
	if (m_x-256>PCORE->landscape.offset_x+(PCORE->gfx.m_d3dpp.BackBufferWidth/PCORE->landscape.m_zoom)) return;
	if (m_y+256<PCORE->landscape.offset_y) return;
	if (m_y-256>PCORE->landscape.offset_y+(PCORE->gfx.m_d3dpp.BackBufferHeight/PCORE->landscape.m_zoom)) return;
	
	enemy_base::pCEnemies->NumEnemiesDrawn++;
	if (b_Activated==false)
	{
		m_xvel=((rand()%200)-100)/1000.0f;
		m_yvel= -0.01f;
	}
	b_Activated=true;
	PCORE->SetAlphaMode(false);

	if (m_xvel>0) m_facingscale=1;
	else if (m_xvel<0) m_facingscale=-1;

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

	PCORE->gfx.SpriteDraw(this->t_body,
					((this->m_x-(16.0f*m_facingscale*m_scale))-PCORE->landscape.offset_x)*PCORE->landscape.m_zoom,
					((this->m_y-PCORE->landscape.offset_y)+((1.0f-m_scale)*32.0f))*PCORE->landscape.m_zoom,  
					angle ,
					(m_facingscale<0?-16.0f:16.0f)*(PCORE->landscape.m_zoom)*m_scale, 
					22*(PCORE->landscape.m_zoom)*m_scale,
					m_facingscale*(PCORE->landscape.m_zoom)*m_scale,PCORE->landscape.m_zoom*m_scale);


	if (DRAW_DEBUG_TEXT)
	{
		PCORE->gfx.DrawTextEx( (m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x))*(enemy_base::pCEnemies->pcore->landscape.m_zoom),
		m_y-70-(PCORE->landscape.offset_y), 255,255,0,"decision:%i\nhealth:%i",(int)this->m_nextdecision, (int)this->health);
		PCORE->gfx.DrawTextEx( (m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x)-3)*(enemy_base::pCEnemies->pcore->landscape.m_zoom),
		 (m_y-(PCORE->landscape.offset_y)-3)*(PCORE->landscape.m_zoom), 255,255,0,"x");
	}
}

bool enemy_rat::LoadStuff(void)
{
	s_bulletflesh1 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");
	s_bulletflesh2 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");
	s_bulletflesh3 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");
	
	s_zombiedie1 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/zombiedead.ogg");

	s_zombiepain1 = PCORE->sound.LoadSound(2,"sound/enemies/rat/squeak1.ogg");
	s_zombiepain2 = PCORE->sound.LoadSound(2,"sound/enemies/rat/squeak2.ogg");
	s_zombiepain3 = PCORE->sound.LoadSound(2,"sound/enemies/rat/squeak3.ogg");

	s_zombiespotted1 = PCORE->sound.LoadSound(2,"sound/enemies/rat/squeak4.ogg");
	s_zombiespotted2 = PCORE->sound.LoadSound(2,"sound/enemies/rat/squeak5.ogg");

	s_ratattack1 = PCORE->sound.LoadSound(2,"sound/enemies/rat/rat-attack2.ogg");
	s_ratattack2 = PCORE->sound.LoadSound(2,"sound/enemies/rat/rat-attack.ogg");

	
	t_body = PCORE->textures.LoadTexture("textures/enemies/rat/rat1.bmp");

	return true;
}

void enemy_rat::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void enemy_rat::HandleFalling(void)
{
	if (m_yvel==0) return;

	this->UpdateMyRect();
	GRECT trect;

	SetGRect(&trect,my_rect.left,my_rect.top+(m_yvel*(PCORE->gfx.tick)),my_rect.right,my_rect.bottom+(m_yvel*(PCORE->gfx.tick)));
	if (PCORE->landscape.Collision(&trect))
	{
		if (m_yvel>0)
		{
		m_yvel = 0;
		m_xvel=0;
		m_nextdecision = 100;
		dwFlags &= ~ AISTATE_WALKINGLEFT;
		dwFlags &= ~ AISTATE_WALKINGRIGHT;

		for (int i=0;i<32;i++)
		{
			SetGRect(&trect,my_rect.left,my_rect.top,my_rect.right,my_rect.bottom+i);
			if (PCORE->landscape.Collision(&trect))
			{
				m_y = m_y+i;
				dwFlags = dwFlags | AISTATE_STANDING;
				if (m_nextdecision<=0) m_nextdecision = GENERICTHINKDELAY;
				return;
			}
		}
		}
		else
		{
			m_yvel = 0.1f;
		}

	}
	else
	{
		dwFlags = dwFlags | AISTATE_FALLING;
		m_yvel = m_yvel+(0.001*PCORE->gfx.tick);
		if (m_yvel>1) m_yvel = 1;
	}

	// add y velocity
	if (m_yvel!=0)	m_y = m_y + (m_yvel*PCORE->gfx.tick);
	// if fell outa screen then kill
	if (m_y+32>(PCORE->landscape.height*32)) this->TakeDamage(100,3,m_x,m_y);
}

void enemy_rat::HandleWalking(void)
{
	if (m_yvel==0 || f_attacktimer<0)
	{
		if (dwFlags & AISTATE_WALKINGLEFT) m_xvel=m_xvel-NORMAL_ACCELERATION;
		else if (dwFlags & AISTATE_WALKINGRIGHT) m_xvel=m_xvel+NORMAL_ACCELERATION;
	}
	
	//if (m_yvel==0)
	//{
		if (m_xvel>MAX_WALKING_SPEED) m_xvel-=NORMAL_ACCELERATION;
		else if (m_xvel<-MAX_WALKING_SPEED) m_xvel+=NORMAL_ACCELERATION;
	//}

	float xtarget = m_x+((m_xvel>0?1:-1*(WIDTH/4)));

	this->UpdateMyRect();
	GRECT trect;
	SetGRect(&trect,my_rect.left+(m_xvel*(PCORE->gfx.tick)),my_rect.top,my_rect.right+(m_xvel*(PCORE->gfx.tick)),my_rect.bottom-1);

	// hit wall no way past
	if (PCORE->landscape.Collision(&trect))
	{
		m_xvel=0;
	}
	m_x = m_x + (m_xvel*(PCORE->gfx.tick)*0.5f);

	// can we fall
	if (m_xvel!=0 && m_yvel==0)// && (dwFlags & AISTATE_STANDING))
	{
		this->UpdateMyRect();
		SetGRect(&trect,my_rect.left,my_rect.top,my_rect.right,my_rect.bottom+1);
		if (!PCORE->landscape.Collision(&trect))
		{
			m_yvel = 0.1f;
		}
	}
}

void enemy_rat::DecideWhatToDo(void)
{
	if (m_yvel!=0) return;
	// are there any players close to me?
	float dist, xdist, ydist;
	CPlayers* playerpointer;

	dwFlags &= ~AISTATE_ATTACKING; // stop attacking 

	// randomly moving left or right for no real reason
	if (rand()%10==0)
	{
		if (rand()%2==0)
		{
			dwFlags= dwFlags | AISTATE_WALKINGLEFT;
			dwFlags &= ~ AISTATE_WALKINGRIGHT;
		}
		else
		{
			dwFlags= dwFlags | AISTATE_WALKINGRIGHT;
			dwFlags &= ~ AISTATE_WALKINGLEFT;
		}
	}
	else
	{
		dist = PCORE->landscape.GetNearestPlayer((m_x+(m_facingscale*20)), m_y, &playerpointer, &xdist, &ydist);
		
		if (fabs(ydist)<400)
		{
			if (targetplayer!=playerpointer) // if this is teh first time we've spotted this player
			{
				if (rand()%20==1)
				{
					if (rand()%2==0)
						PCORE->sound.PlaySnd(s_zombiespotted1,m_x,m_y);
					else
						PCORE->sound.PlaySnd(s_zombiespotted2,m_x,m_y);
				}
			}
			targetplayer = playerpointer;

			if (xdist<0 || rand()%80==1)
			{
				dwFlags= dwFlags | AISTATE_WALKINGLEFT;
				dwFlags&= ~ AISTATE_WALKINGRIGHT;
			}
			
			if (xdist>0|| rand()%80==1)
			{
				dwFlags= dwFlags | AISTATE_WALKINGRIGHT;
				dwFlags&= ~ AISTATE_WALKINGLEFT;
			}

		}

	}

	
	m_nextdecision=GENERICTHINKDELAY;
}

bool enemy_rat::BulletHit(float* x, float* y)
{
	if (!this->b_Activated) return false;

	if (*x<my_rect.left) return false;
	if (*x>my_rect.right) return false;
	if (*y<my_rect.top) return false;
	if (*y>my_rect.bottom) return false;
	return true;
}

bool enemy_rat::BulletHitRect(GRECT* r)
{
	if (!this->b_Activated) return false;
	if (IntersectGRect(r,&my_rect)) return true;
	return false;
}
void enemy_rat::TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet)
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
				bullet->pplayer->AddBulletTime(SKILL_BT_RAT);
			}

		PCORE->landscape.UnAttachAttachmentsMonster(this);
		PCORE->sound.PlaySnd(s_zombiedie1,m_x,m_y);
		particle = PCORE->landscape.AddParticle(PARTICLE_GENERIC_32X32, m_x, m_y+16);
		if (particle!=NULL)
		{

			particle->textureoverride = PCORE->textures.LoadTexture("textures/enemies/rat/deadrat1.bmp");
			if (bullet!=NULL)
			{
				randomnumber = 0.3f+(rand()%1000)/10000.0f;
				particle->xvel = bullet->xvel*randomnumber;
				particle->yvel = bullet->yvel*randomnumber;
			}
			else
			{
				randomnumber = (rand()%1000)/1000.0f;
				particle->xvel = (randomnumber)/10;
				randomnumber = (rand()%1000)/3000.0f;
				particle->yvel = -(randomnumber);
			}
			particle->xscale = m_scale;
			particle->yscale = m_scale;
		}
	
		// loverly loverly blood
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

			for (int i=0;i<5;i++)
			{
				float randx = m_x+rand()%32;
				float randy = m_y+16+rand()%16;

				particle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_BLOODMIST, randx, randy);
				if (particle!=NULL)
					{
						randomangle = angle;
						randomnumber = (((rand()%10000))/10000.0f);
						particle->xvel = 0.0f;
						particle->yvel = cos(randomangle)*((0.1*randomnumber));
				
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

	if (!Splash)
		enemy_base::pCEnemies->pcore->landscape.GenericBloodSpurt(x, y, angle, 1.0f);
	else
		BulletHitSplash(x, y, 10);

	if (bullet!=NULL)
		AttachParticle(bullet);

	if (rand()%3==0) PCORE->sound.PlaySnd(s_bulletflesh1,m_x,m_y);
	else if (rand()%2==0) PCORE->sound.PlaySnd(s_bulletflesh2,m_x,m_y);
	else PCORE->sound.PlaySnd(s_bulletflesh3,m_x,m_y);


}

void enemy_rat::HandleAttacking(void)
{
	if (f_attacktimer>0) return;
	if (targetplayer==NULL) return;

	if (IntersectGRect(&targetplayer->my_rect, &my_rect))
	{
		if (m_yvel!=0)
		{
			m_xvel= -m_xvel;
			m_yvel = m_yvel - 0.2f;
		}
		dwFlags &= ~AISTATE_ATTACKING;
		InitAttack();
		return;
	}
}

void enemy_rat::InitAttack(void)
{
	if (f_attacktimer>0) return;

	f_attacktimer  = 1500;
	m_nextdecision = 100;

	targetplayer->TakeDamage(5,NULL);
	PCORE->landscape.GenericBloodSpurt(m_x,m_y+24,atan2(m_xvel,m_yvel),0.5f);
}

void enemy_rat::BulletHitSplash(float x, float y, float speed)
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

float enemy_rat::GetX()
{
	return m_x;
}
float enemy_rat::GetY()
{
	return m_y;
}
float enemy_rat::GetFacing()
{
	return m_facingscale;
}

bool enemy_rat::SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker)
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
void enemy_rat::AttachParticle(LPParticle particle)
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

void enemy_rat::UpdateMyRect(void)
{
	my_rect.left = m_x-8;
	my_rect.right = m_x+8;
	my_rect.top = m_y+16;
	my_rect.bottom = m_y+32;
}

void enemy_rat::DoFireDamage(float x, float y, float damage)
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
