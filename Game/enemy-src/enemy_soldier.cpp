#include "enemy_solider.h"

#define PIXELS_TO_FEET 32
#define MAX_WALKING_SPEED m_maxwalkingspeed
#define NORMAL_ACCELERATION m_acceleration
#define WIDTH 32

#define AIMSPEED 0.002f

#define MAXSTEPSIZE 0

#define DRAW_DEBUG_TEXT false
#define GENERICTHINKDELAY 200

#define COLLISION enemy_base::pCEnemies->pcore->landscape.Collision

#define PCORE enemy_base::pCEnemies->pcore
#define TICK PCORE->gfx.tick

#define USEWEAPON WEP_BERETTA

enemy_soldier::enemy_soldier(void)
{
	m_x=0;
	m_y=0;
	m_yvel=1;
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
	fighttimer = rand()%30;
	animtimer = 0;
	this->UpdateMyRect();
	shootcount=0;
	s_die = NULL;

}

enemy_soldier::~enemy_soldier(void)
{
	
}

void enemy_soldier::Do(void)
{
	if (!b_Activated) return;
	enemy_base::pCEnemies->NumEnemiesActivated++;

	if (m_nextdecision>0) 
	{
		m_nextdecision = m_nextdecision - (PCORE->gfx.tick);
		if(m_nextdecision<=0)
		{ 
			DecideWhatToDo(); 
			m_nextdecision=GENERICTHINKDELAY;
		}
	}

	HandleFalling();
	HandleWalking();
	HandleAttacking();

}

void enemy_soldier::Draw(int layer)
{
	if (layer!=0) return;
	if (m_x+512<PCORE->landscape.offset_x) return;
	if (m_x-512>PCORE->landscape.offset_x+(PCORE->gfx.m_d3dpp.BackBufferWidth/PCORE->landscape.m_zoom)) return;
	if (m_y+512<PCORE->landscape.offset_y) return;
	if (m_y-256>PCORE->landscape.offset_y+(PCORE->gfx.m_d3dpp.BackBufferHeight/PCORE->landscape.m_zoom)) return;
	
	PCORE->SetAlphaMode(false);
	enemy_base::pCEnemies->NumEnemiesDrawn++;
	b_Activated=true;

	if (m_xvel>0)
	{
		if (m_facingscale!=1) this->aimangle = -aimangle;//targetangle;
		m_facingscale=1;

	}
	else if (m_xvel<0)
	{
		if (m_facingscale!=-1) this->aimangle = -aimangle;//targetangle;
		m_facingscale=-1;
	}

	PCORE->gfx.SpriteDrawEx(t_body,
					(this->m_x-PCORE->landscape.offset_x-(16.0f*m_facingscale))*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y-25.0f)*PCORE->landscape.m_zoom,  
					0,
					0, 
					0,
					m_facingscale*(PCORE->landscape.m_zoom),
					PCORE->landscape.m_zoom,
					(((int)animtimer/2)%2)*32,0,32,64,
					255,255,255,255);

	
	PCORE->gfx.SpriteDraw(t_head,
					(this->m_x-PCORE->landscape.offset_x-(16.0f*m_facingscale))*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y-32.0f)*PCORE->landscape.m_zoom,  
					aimangle+((D3DX_PI/2.0f)*-m_facingscale)+(sin((m_x/5.0f)+(fighttimer>0?fighttimer/50.0f:0))/8.0f),
					16.0f*PCORE->landscape.m_zoom*m_facingscale, 
					32.0f*PCORE->landscape.m_zoom,
					m_facingscale*(PCORE->landscape.m_zoom),
					PCORE->landscape.m_zoom);

	if (helmet)
	PCORE->gfx.SpriteDraw(t_hat,
					(this->m_x-PCORE->landscape.offset_x-(16.0f*m_facingscale))*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y-32.0f)*PCORE->landscape.m_zoom,  
					aimangle+((D3DX_PI/2.0f)*-m_facingscale)+(sin((m_x/5.0f)+(fighttimer>0?fighttimer/50.0f:0))/8.0f),
					16.0f*PCORE->landscape.m_zoom*m_facingscale, 
					32.0f*PCORE->landscape.m_zoom,
					m_facingscale*(PCORE->landscape.m_zoom),
					PCORE->landscape.m_zoom);

	//weapon.
	PCORE->weapons.Draw(USEWEAPON,
						m_x-(PCORE->landscape.offset_x),
						m_y-(PCORE->landscape.offset_y),
						PCORE->landscape.m_zoom,
						aimangle,
						m_facingscale,
						1);

	//arm
	PCORE->gfx.SpriteDraw(t_arm,
					(m_x-(-m_facingscale*8)-PCORE->landscape.offset_x)*PCORE->landscape.m_zoom,
					(m_y-PCORE->landscape.offset_y)*PCORE->landscape.m_zoom,  
					aimangle+(this->clip==0?(sin(PCORE->TimerA/70.0f)*0.2f):0),
					-m_facingscale*8.0f*PCORE->landscape.m_zoom, 
					5.0f*PCORE->landscape.m_zoom,

					-m_facingscale*(PCORE->landscape.m_zoom),
					PCORE->landscape.m_zoom);
	


	if (DRAW_DEBUG_TEXT)
	{
		PCORE->gfx.DrawTextEx( (m_x-(PCORE->landscape.offset_x))*(PCORE->landscape.m_zoom),
			m_y-70-(PCORE->landscape.offset_y), 255,255,0,"decision:%i\nhealth:%i\n:fighttimer %f",(int)this->m_nextdecision, (int)this->health, fighttimer);
		PCORE->gfx.DrawTextEx( (m_x-(PCORE->landscape.offset_x)-3)*(PCORE->landscape.m_zoom),
		 (m_y-(PCORE->landscape.offset_y)-3)*(PCORE->landscape.m_zoom), 255,255,0,"x");
	}
}

bool enemy_soldier::LoadStuff(void)
{
	//s_bulletflesh1 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");

	t_body = PCORE->textures.LoadTexture("textures/enemies/soldier/body.bmp");
	t_head = PCORE->textures.LoadTexture("textures/enemies/soldier/head.bmp");
	t_hat = PCORE->textures.LoadTexture("textures/enemies/soldier/hat.bmp");
	t_arm = PCORE->textures.LoadTexture("textures/enemies/soldier/arm.bmp");


	aimangle = -D3DX_PI/5;
	this->clip = PCORE->weapons.m_GunAttributes[USEWEAPON].ClipSize;
	reloadtimer=0;

	this->StartupVariables();



	return true;
}

void enemy_soldier::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void enemy_soldier::HandleFalling(void)
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

void enemy_soldier::HandleWalking(void)
{
	if (this->reloadtimer>0) return;
	// bonesy is smart - he only walks when he sees a player he can fuck up
	if (targetplayer==NULL ||!bFollowing) 
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

void enemy_soldier::DecideWhatToDo(void)
{
	if (m_yvel!=0) return; // can't do jack shit while falling

	float dist, xdist, ydist;
	CPlayers* playerpointer;

	// can we see a player
	dist = PCORE->landscape.GetNearestPlayer(m_x, m_y, &playerpointer, &xdist, &ydist);
	
	if (fabs(ydist)<800 && fabs(xdist)<1000 && playerpointer!=NULL)
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
		targetangle = atan2(targetplayer->m_fX-(m_x),targetplayer->m_fY-m_y);
		if (clip<=0)
		{
			targetangle = (D3DX_PI/5)*m_facingscale;
		}
	}
	else
	{
		targetplayer=NULL;
		bFollowing = false;
	}

}

bool enemy_soldier::BulletHit(float* x, float* y)
{
	if (!this->b_Activated) return false;
	if (*x<my_rect.left) return false;
	if (*x>my_rect.right) return false;
	if (*y<my_rect.top) return false;
	if (*y>my_rect.bottom) return false;
	return true;
}

bool enemy_soldier::BulletHitRect(GRECT* r)
{
	if (!this->b_Activated) return false;
	if (IntersectGRect(r,&my_rect)) return true;
	return false;
}
void enemy_soldier::TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet)
{
	LPParticle particle = NULL;
	float randomangle = angle;
	float randomnumber;
	float beforehealth=health;

	health=health-damage;

	if (y<m_y-10 || (health<0 && beforehealth>=0 && helmet))
	{
		if (helmet)
		{
			helmet=false;
			LPParticle lp=NULL;
			lp = PCORE->landscape.AddParticle(PARTICLE_GENERIC_32X32,m_x-16.0f,m_y-32.0f);
			if (lp!=NULL)
			{
				if (bullet!=NULL)
				{
					lp->textureoverride = this->t_hat;
					lp->xvel = bullet->xvel*0.1f;
					lp->yvel = bullet->yvel*0.1f;
					lp->rot = aimangle+((D3DX_PI/2.0f)*-m_facingscale)+(sin((m_x/5.0f)+(fighttimer>0?fighttimer/50.0f:0))/8.0f);
					lp->xscale = m_facingscale;
					
					for (int j=0;j<5;j++)
					{
						particle = PCORE->landscape.AddParticle(PARTICLE_RICHOCHETSPARK, x, y);
						if (particle!=NULL)
						{
							randomangle = atan2(bullet->xvel, bullet->yvel) + (((rand()%2000)-1000)/3000.0f);
							randomnumber = 0.1f+((rand()%500)/1000.0f);
							particle->xvel = sin(randomangle)*(randomnumber);
							particle->yvel = cos(randomangle)*(randomnumber);
						}
					}
				}
			}
		}
	}

	if (health<0 && beforehealth>=0)
	{
		if (bullet!=NULL)
		{
			if (bullet->pplayer!=NULL)
			{
				bullet->pplayer->CountKill();
				bullet->pplayer->AddBulletTime(SKILL_BT_SOLDIER);
			}
		}

		PCORE->landscape.UnAttachAttachmentsMonster(this);
		PCORE->sound.PlaySnd(s_die,m_x,m_y);

		LPParticle tp = PCORE->landscape.AddParticle(PARTICLE_GENERIC_32X32,12,12);
		if (tp!=NULL)
		{
			
			tp->x = (this->m_x-(16.0f*-m_facingscale));
			tp->y = this->m_y-(17);
			tp->rot = aimangle+((D3DX_PI/2.0f)*-m_facingscale)+(sin((m_x/5.0f)+(fighttimer>0?fighttimer/50.0f:0))/8.0f);
			tp->textureoverride = this->t_head;
			float rnum = (rand()%10000/10000.0f);
			tp->xvel = sin(angle)*(damage/70)*rnum;
			tp->yvel = cos(angle)*(damage/70)*rnum;
			tp->xscale = m_facingscale;
			tp->yscale = 1;
		}


		for (int i=0;i<10;i++)
			{
				float randx = m_x-16+rand()%32;
				float randy = m_y-10+rand()%60;

				particle = PCORE->landscape.AddParticle(PARTICLE_BLOODMIST, randx, randy);
				if (particle!=NULL)
					{
						randomangle = angle;
						randomnumber = (((rand()%10000))/10000.0f);
						particle->xvel = 0.0f;
						particle->yvel = cos(randomangle)*((0.1*randomnumber));
				
					}

			}
		for (int i=0;i<20;i++)
			{
				float randx = m_x-16+rand()%32;
				float randy = m_y-10+rand()%60;
				PCORE->landscape.GenericBloodSpurt(randx, randy,(rand()%10000)/1000.0f,0.5f+((rand()%1000)/1000.0f));
			}

	}

	if (bullet!=NULL)
	{
		if (bullet->type!=PARTICLE_FIREBITRISER && bullet->type!=PARTICLE_FIRE)
		{

			PCORE->landscape.GenericBloodSpurt(m_x,m_y,atan2(bullet->xvel, bullet->yvel), 1.2f);
			if (rand()%5==0)
			{
				PCORE->landscape.GenericBloodSpurt(m_x,m_y,atan2(bullet->xvel, bullet->yvel)+D3DX_PI, 1.5f);
			}
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

	}
	else
	{
		BulletHitSplash(x, y, 10);
	}

	if (bullet!=NULL)
		AttachParticle(bullet);

	//if (rand()%3==0) PCORE->sound.PlaySnd(s_bulletflesh1,m_x,m_y);
	//else if (rand()%2==0) PCORE->sound.PlaySnd(s_bulletflesh2,m_x,m_y);
	//else PCORE->sound.PlaySnd(s_bulletflesh3,m_x,m_y);


}



void enemy_soldier::HandleAttacking(void)
{
	if (reloadtimer>0)
	{
		reloadtimer = reloadtimer -  (0.01*PCORE->gfx.tick);
		if (reloadtimer<=0)
		{
			clip = PCORE->weapons.m_GunAttributes[USEWEAPON].ClipSize;
		}
	}

	if (targetplayer==NULL) 
	{
		targetangle = -D3DX_PI/4;
		return;
	}

	if (targetangle>aimangle)
	{
		aimangle = aimangle + (AIMSPEED*PCORE->gfx.tick);
		if (aimangle>targetangle) (aimangle=targetangle);
	}
	else if (targetangle<aimangle)
	{
		aimangle = aimangle - (AIMSPEED*PCORE->gfx.tick);
		if (aimangle<targetangle) aimangle=targetangle;
	}


	if (clip<=0 && reloadtimer<=0)
	{
		PCORE->weapons.PlayReloadSound(m_x,m_y,USEWEAPON);
		reloadtimer = PCORE->weapons.m_GunAttributes[USEWEAPON].ReloadTime*SKILL_RELOADMULTIPLIER;
		fighttimer = 5;

	}
	else if (aimangle==targetangle && clip>0)
	{
		

		if (fighttimer>0)
		{
			fighttimer = fighttimer -  (0.01*PCORE->gfx.tick);
		}
		else
		{
			clip = clip - 1;

			
		
			fighttimer = PCORE->weapons.m_GunAttributes[USEWEAPON].TimeBetweenShots+0.2f+((rand()%8000)/1000.0f);
			

			float sx = m_x + sin(aimangle)*PCORE->weapons.m_GunAttributes[USEWEAPON].MuzzleTip;
			float sy = m_y + cos(aimangle)*PCORE->weapons.m_GunAttributes[USEWEAPON].MuzzleTip;

			PCORE->landscape.BulletShoot(USEWEAPON,sx,sy,aimangle+(shootcount*0.03f*m_facingscale),1, true);
			PCORE->weapons.PlayShootSound(m_x,m_y,USEWEAPON);
			PCORE->weapons.DrawMuzzle(sx,sy,aimangle,USEWEAPON);
		}
	}
}

void enemy_soldier::BulletHitSplash(float x, float y, float speed)
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
			dparticle = PCORE->landscape.AddParticle(PARTICLE_BLOODBLOBS, randx, randy);
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

float enemy_soldier::GetX()
{
	return m_x;
}
float enemy_soldier::GetY()
{
	return m_y;
}
float enemy_soldier::GetFacing()
{
	return m_facingscale;
}

bool enemy_soldier::SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker)
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
void enemy_soldier::AttachParticle(LPParticle particle)
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

void enemy_soldier::UpdateMyRect(void)
{
	my_rect.left = m_x-4;
	my_rect.right = m_x+4;
	my_rect.top = m_y-20;
	my_rect.bottom = m_y+38;
}

void enemy_soldier::DoFireDamage(float x, float y, float damage)
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
/*
void enemy_soldier::StartupVariables(void)
{

}
*/