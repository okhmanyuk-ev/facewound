#include "enemy_zombie.h"

#define WIDTH 60
#define HEIGHT

#define MAXSTEPSIZE 32
#define DRAW_DEBUG_TEXT false
#define COLLISION enemy_base::pCEnemies->pcore->landscape.Collision

enemy_zombie::enemy_zombie(void)
{

}

enemy_zombie::~enemy_zombie(void)
{
	
}

void enemy_zombie::Do(void)
{
	if (!b_Activated) return;

	enemy_base::pCEnemies->NumEnemiesActivated++;

	if (m_nextdecision>0) 
	{
		m_nextdecision = m_nextdecision - 1*(enemy_base::pCEnemies->pcore->gfx.tick);

		if(m_nextdecision<=0 && !(dwFlags & AISTATE_COMPLETINGTASK))
		{
			DecideWhatToDo();
		}
	}

	if ((this->targetplayer!=NULL) && !(dwFlags & AISTATE_ATTACKING))
	{
		if (targetplayer->Collision(m_x+(m_facingscale*20),m_y))
		{
			InitAttack();
			return;
		}
	}
	HandleFalling();

	if (dwFlags & AISTATE_WALKINGLEFT || dwFlags & AISTATE_WALKINGRIGHT)  HandleWalking();
	if (dwFlags & AISTATE_ATTACKING)  HandleAttacking();

}

void enemy_zombie::Draw(void)
{
	if (m_x+256<enemy_base::pCEnemies->pcore->landscape.offset_x) return;
	if (m_x-256>enemy_base::pCEnemies->pcore->landscape.offset_x+(enemy_base::pCEnemies->pcore->gfx.m_d3dpp.BackBufferWidth/enemy_base::pCEnemies->pcore->landscape.m_zoom)) return;
	if (m_y+256<enemy_base::pCEnemies->pcore->landscape.offset_y) return;
	if (m_y-256>enemy_base::pCEnemies->pcore->landscape.offset_y+(enemy_base::pCEnemies->pcore->gfx.m_d3dpp.BackBufferHeight/enemy_base::pCEnemies->pcore->landscape.m_zoom)) return;
	
	enemy_base::pCEnemies->NumEnemiesDrawn++;
	b_Activated=true;

	float x,y;

	if (m_xvel>0) m_facingscale=1;
	else if (m_xvel<0) m_facingscale=-1;

	float targetarmangle = D3DX_PI/2;

	if (targetplayer!=NULL)
	{
		targetarmangle = atan2(targetplayer->m_fX-m_x,targetplayer->m_fY-(m_y+32));

		if (m_facingscale<0)
		{
			targetarmangle = targetarmangle - D3DX_PI;//D3DX_PI/2;
		}
	}

	if (armangle>D3DX_PI*2) armangle = armangle - (D3DX_PI*2);
	if (armangle<0) armangle = armangle + (D3DX_PI*2);

	if (targetarmangle>D3DX_PI*2) targetarmangle = targetarmangle - (D3DX_PI*2);
	if (targetarmangle<0) targetarmangle = targetarmangle + (D3DX_PI*2);
//	targetarmangle = D3DX_PI;

	if (armangle>targetarmangle) armangle=armangle- (0.0005*(enemy_base::pCEnemies->pcore->gfx.tick));
	else if (armangle<targetarmangle) armangle=armangle+ (0.0005*(enemy_base::pCEnemies->pcore->gfx.tick));



	//arm behind
	x = ((m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x)-13+(m_facingscale<0?4:0))  *(enemy_base::pCEnemies->pcore->landscape.m_zoom));
	y = (m_y-enemy_base::pCEnemies->pcore->landscape.offset_y+19+(m_facingscale<0?16:2))                              *(enemy_base::pCEnemies->pcore->landscape.m_zoom);
	enemy_base::pCEnemies->pcore->gfx.SpriteDraw(enemy_base::pCEnemies->pcore->enemies.t_zombie_arm,  x,y,  armangle,9*(enemy_base::pCEnemies->pcore->landscape.m_zoom),m_facingscale*6*(enemy_base::pCEnemies->pcore->landscape.m_zoom),  1*(enemy_base::pCEnemies->pcore->landscape.m_zoom),m_facingscale*(enemy_base::pCEnemies->pcore->landscape.m_zoom));


	// body
	x = ((m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x)-(WIDTH/2)+(m_facingscale==-1?64:0))  *(enemy_base::pCEnemies->pcore->landscape.m_zoom));
	y = (m_y-(enemy_base::pCEnemies->pcore->landscape.offset_y))  *(enemy_base::pCEnemies->pcore->landscape.m_zoom);
	enemy_base::pCEnemies->pcore->gfx.SpriteDraw(enemy_base::pCEnemies->pcore->enemies.t_zombie_body,  x,y,  0,0,0,  m_facingscale*(enemy_base::pCEnemies->pcore->landscape.m_zoom),1*(enemy_base::pCEnemies->pcore->landscape.m_zoom));



	//head
	x = ((m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x)-(WIDTH/2)+16+(m_facingscale==-1?34:0))  *(enemy_base::pCEnemies->pcore->landscape.m_zoom));
	y = (m_y-(enemy_base::pCEnemies->pcore->landscape.offset_y)-9)  *(enemy_base::pCEnemies->pcore->landscape.m_zoom);
	enemy_base::pCEnemies->pcore->gfx.SpriteDraw(enemy_base::pCEnemies->pcore->enemies.t_zombie_head,  x,y,  0,0,0,  m_facingscale*(enemy_base::pCEnemies->pcore->landscape.m_zoom),1*(enemy_base::pCEnemies->pcore->landscape.m_zoom));

//	enemy_base::pCEnemies->pcore->gfx.DrawTextEx(x-30,y-30,255,255,0,"Heath: %i\narmangle: %f\ntanbgle: %f", (int)health, armangle, targetarmangle);

	//arm
	x = ((m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x)-10+(m_facingscale<0?4:0))  *(enemy_base::pCEnemies->pcore->landscape.m_zoom));
	y = (m_y-enemy_base::pCEnemies->pcore->landscape.offset_y+19+(m_facingscale<0?14:0))                              *(enemy_base::pCEnemies->pcore->landscape.m_zoom);
	enemy_base::pCEnemies->pcore->gfx.SpriteDraw(enemy_base::pCEnemies->pcore->enemies.t_zombie_arm,  x,y,  armangle,9*(enemy_base::pCEnemies->pcore->landscape.m_zoom),m_facingscale*6*(enemy_base::pCEnemies->pcore->landscape.m_zoom),  1*(enemy_base::pCEnemies->pcore->landscape.m_zoom),m_facingscale*(enemy_base::pCEnemies->pcore->landscape.m_zoom));


	

	if (DRAW_DEBUG_TEXT)
	enemy_base::pCEnemies->pcore->gfx.DrawTextEx( (m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x))*(enemy_base::pCEnemies->pcore->landscape.m_zoom),y-70, 255,255,0,"decision:%i\nhealth:%i",(int)this->m_nextdecision, (int)this->health);

	enemy_base::pCEnemies->pcore->gfx.DrawTextEx( (m_x-(enemy_base::pCEnemies->pcore->landscape.offset_x)-3)*(enemy_base::pCEnemies->pcore->landscape.m_zoom),
		 (m_y-(enemy_base::pCEnemies->pcore->landscape.offset_y)-3)*(enemy_base::pCEnemies->pcore->landscape.m_zoom), 255,255,0,"x");
}

bool enemy_zombie::LoadStuff(void)
{
	

	return true;
}

void enemy_zombie::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void enemy_zombie::HandleFalling(void)
{
	// if we're standing
	if ((dwFlags & AISTATE_STANDING))
	{
		enemy_base::pCEnemies->pcore->landscape.Collision(m_x,m_y+PIXELS_TO_FEET+1);
	}
	else
	if (enemy_base::pCEnemies->pcore->landscape.Collision(m_x,m_y+PIXELS_TO_FEET+(m_yvel*(enemy_base::pCEnemies->pcore->gfx.tick))))
	{
		dwFlags &= ~AISTATE_FALLING;
		m_yvel = 0;

		for (int i=0;i<32;i++)
		{
			if (enemy_base::pCEnemies->pcore->landscape.Collision(m_x,m_y+PIXELS_TO_FEET+i))
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
		dwFlags = dwFlags | AISTATE_FALLING;
		m_yvel = m_yvel+0.03;
		if (m_yvel>0.3) m_yvel = 0.3;
	}

	// add y velocity
	if (m_yvel!=0)	m_y += (float)(m_yvel*(enemy_base::pCEnemies->pcore->gfx.tick));

	if (m_y+PIXELS_TO_FEET>(enemy_base::pCEnemies->pcore->landscape.height*32)) this->TakeDamage(100,3,m_x,m_y);
}

void enemy_zombie::HandleWalking(void)
{
	if (dwFlags & AISTATE_STANDING)
	{
		if (dwFlags & AISTATE_WALKINGLEFT) m_xvel=m_xvel-NORMAL_ACCELERATION;
		else
		if (dwFlags & AISTATE_WALKINGRIGHT) m_xvel=m_xvel+NORMAL_ACCELERATION;
	}

	if (m_xvel>MAX_WALKING_SPEED) m_xvel-=NORMAL_ACCELERATION;
	else
	if (m_xvel<-MAX_WALKING_SPEED) m_xvel+=NORMAL_ACCELERATION;


	float xtarget = m_x+((m_xvel>0?1:-1)*(WIDTH/4));
	bool collision = false;

	// hit face, no way past.
	if (COLLISION(xtarget,m_y) || COLLISION(xtarget,m_y+16) || COLLISION(xtarget,m_y+PIXELS_TO_FEET-MAXSTEPSIZE-1))
	{
		collision=true;
	}
	// if hitting foot
	else if (COLLISION(xtarget,m_y+PIXELS_TO_FEET-1))
	{
		float stepup=0;
		// find out how high the step is
		for (int i=1;i<=MAXSTEPSIZE;i++)
		{
			if (!COLLISION(xtarget, m_y+PIXELS_TO_FEET-i-1))
			{
				stepup = i;
				break;
			}
		}

		// are we hitting our head if we step up
		if (!COLLISION(xtarget,m_y-stepup) && !COLLISION(m_x,m_y-stepup))
		{
			m_y=m_y-stepup;
			m_xvel = m_xvel * 0.5;
		}
		else
			collision=true;
	}

	/*
	if (COLLISION(m_x+((m_xvel>0?1:-1)*(WIDTH/4)),m_y+(PIXELS_TO_FEET/2)+16) ||
		COLLISION(m_x+((m_xvel>0?1:-1)*(WIDTH/4)),m_y+16))
	{
		// if we are can't step up
		if (
			enemy_base::pCEnemies->pcore->landscape.Collision(m_x+((m_xvel>0?1:-1)*(WIDTH/4)),m_y+16) ||
			enemy_base::pCEnemies->pcore->landscape.Collision(m_x,m_y-16) ||
			enemy_base::pCEnemies->pcore->landscape.Collision(m_x+((m_xvel>0?1:-1)*(WIDTH/4)),m_y-32) ||
			
			// no floor underneath us (to step up from)
			!enemy_base::pCEnemies->pcore->landscape.Collision(m_x,m_y+PIXELS_TO_FEET+1)
			
			)
		{
			if (dwFlags & AISTATE_WALKINGLEFT) 
			{
				dwFlags &= ~AISTATE_WALKINGLEFT;
				if (rand()%5==0) dwFlags = dwFlags | AISTATE_WALKINGRIGHT; // stop wall hugging
			}
			else
			if (dwFlags & AISTATE_WALKINGRIGHT)
			{
				dwFlags &= ~AISTATE_WALKINGRIGHT;
				if (rand()%5==0) dwFlags = dwFlags | AISTATE_WALKINGLEFT; // stop wall hugging
			}
			if (dwFlags & AISTATE_COMPLETINGTASK) dwFlags &= ~AISTATE_COMPLETINGTASK;
			//dwFlags = dwFlags | AISTATE_STANDING;
			m_nextdecision=GENERICTHINKDELAY;
			m_xvel=0;
			return;
		}
		else
		{
			float stepup=0;
			// how far do we step!??!?!?!?!?!
			for (int i=0;i<=32;i++)
			{
				if (!enemy_base::pCEnemies->pcore->landscape.Collision(m_x+((m_xvel>0?1:-1)*(WIDTH/4)), 
																	   m_y+PIXELS_TO_FEET-i))
				{
					stepup = i;
					break;
				}
			}


		m_y = m_y-stepup;

		//				break;
		//	m_y=m_y-32;
			//m_xvel = m_xvel/2;
		}
	}
	*/

	if (collision)
	{
		m_xvel=0;
	}

	m_x = m_x + m_xvel*(enemy_base::pCEnemies->pcore->gfx.tick);

	// can we fall in this new position?
	if (dwFlags & AISTATE_STANDING)
	if (!enemy_base::pCEnemies->pcore->landscape.Collision(m_x+(WIDTH/4)+1,m_y+PIXELS_TO_FEET+1) &&
		!enemy_base::pCEnemies->pcore->landscape.Collision(m_x,m_y+PIXELS_TO_FEET+1) &&
		!enemy_base::pCEnemies->pcore->landscape.Collision(m_x-(WIDTH/4)-1,m_y+PIXELS_TO_FEET+1))
	{
		dwFlags = dwFlags | AISTATE_FALLING;
		dwFlags &= ~AISTATE_STANDING;
	}
}

void enemy_zombie::DecideWhatToDo(void)
{
	// are there any players close to me?

	float dist, xdist, ydist;
	CPlayers* playerpointer;

	dwFlags &= ~AISTATE_ATTACKING; // stop attacking 

	// chances of moaning
	if (rand()%50==0)
	{
		if (rand()%2==0)
			enemy_base::pCEnemies->pcore->sound.PlaySnd(S_ZOMBIE_IDLE1,m_x,m_y);
		else
			enemy_base::pCEnemies->pcore->sound.PlaySnd(S_ZOMBIE_IDLE1,m_x,m_y);
	}

	if (rand()%10==0)
	{
		// chances of moving left/right - IF NOT BLOCKED
		if (rand()%2==0 && !enemy_base::pCEnemies->pcore->landscape.Collision(m_x-30,m_y+PIXELS_TO_FEET/2))
		{
			dwFlags= dwFlags | AISTATE_WALKINGLEFT;
		}
		else
		{
			dwFlags= dwFlags | AISTATE_WALKINGRIGHT;
		}
	}
	else
	{
		dist = enemy_base::pCEnemies->pcore->landscape.GetNearestPlayer((m_x+(m_facingscale*20)), m_y, &playerpointer, &xdist, &ydist);

	//	enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_SPARKBALL,playerpointer->m_fX,playerpointer->m_fY);

		if (fabs(ydist)<400)
		{
			targetplayer = playerpointer;

			if (xdist<0 || rand()%40==1)
			{
				dwFlags= dwFlags | AISTATE_WALKINGLEFT;
				dwFlags&= ~ AISTATE_WALKINGRIGHT;
			}
			
			if (xdist>0|| rand()%40==1)
			{
				dwFlags= dwFlags | AISTATE_WALKINGRIGHT;
				dwFlags&= ~ AISTATE_WALKINGLEFT;
			}

		}

	}

	
	m_nextdecision=GENERICTHINKDELAY;
}

#define HITBOXWIDTH 20
#define HITBOXYOFFSET 26
#define HITBOXHEIGHT 64

bool enemy_zombie::BulletHit(float* x, float* y)
{
	if (*x<this->m_x-(HITBOXWIDTH/2))
		return false;
	if (*x>this->m_x+(HITBOXWIDTH/2))
		return false;

	if (*y<this->m_y-(HITBOXHEIGHT/2)+HITBOXYOFFSET)
		return false;
	if (*y>this->m_y+(HITBOXHEIGHT/2)+HITBOXYOFFSET)
		return false;


	return true;
}

bool enemy_zombie::BulletHitRect(RECT* r)
{
	if (m_x+(HITBOXWIDTH/2) < r->left) return false;
	if (m_x-(HITBOXWIDTH/2) > r->right) return false;
	if (m_y-(HITBOXHEIGHT/2)+HITBOXYOFFSET > r->bottom) return false;
	if (m_y+(HITBOXHEIGHT/2)+HITBOXYOFFSET < r->top) return false;

	return true;
}
void enemy_zombie::TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet)
{
	LPParticle particle = NULL;
	float randomangle = angle;
	float randomnumber;
	float beforehealth=health;

	health=health-damage;

	if (rand()%3==0)
		enemy_base::pCEnemies->pcore->sound.PlaySnd(S_BULLETHIT_FLESH1,m_x,m_y);
	else if (rand()%2==0)
		enemy_base::pCEnemies->pcore->sound.PlaySnd(S_BULLETHIT_FLESH2,m_x,m_y);
	else
		enemy_base::pCEnemies->pcore->sound.PlaySnd(S_BULLETHIT_FLESH3,m_x,m_y);

	if (health<0 && beforehealth>=0)
	{

		enemy_base::pCEnemies->pcore->landscape.UnAttachAttachmentsMonster(this);

		enemy_base::pCEnemies->pcore->sound.PlaySnd(S_ZOMBIE_DIE,m_x,m_y);

		x = (m_x-(WIDTH/2)+16);
		y = (m_y-9);


		particle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_ZOMBIE_HEAD, x+16.5, y+16.5);
		

		if (particle!=NULL)
		{
			particle->xvel = sin(angle)*(damage/40);//bullet->xvel/20;
			particle->yvel = cos(angle)*(damage/40);//bullet->yvel/20;
			particle->rot = 0;
			particle->xscale = m_facingscale;
		}

		for (int i=0;i<2;i++)
		{
			x = (m_x)-10+(m_facingscale<0?4:0);
			y = (m_y+19+(m_facingscale<0?14:0));
			particle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_ZOMBIE_ARM, x, y-(i*2));

			if (particle!=NULL)
			{
				particle->yscale = m_facingscale;
				particle->rot = armangle;
				
				particle->xvel = sin(angle)*((health*-1)/85)+((rand()%1000/2000.0f)-0.25f);//bullet->xvel/20;
				particle->yvel = cos(angle)*((health*-1)/50);//bullet->yvel/20;

				if (rand()%20==1) particle->xvel = -particle->xvel;
				if (rand()%20==1) particle->yvel = -1;

			}

		}


		// loverly loverly blood
		for (int j=0;j<35;j++)
		{
			
			particle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_BLOODBLOBS, m_x+rand()%25-12, m_y+rand()%48-24);
			if (particle!=NULL)
			{
				randomnumber = (rand()%1000)/1000.0f;
				particle->xvel = (randomnumber)/5;
				randomnumber = (rand()%100)/200.0f;
				particle->yvel = -(randomnumber);
				particle->xscale = 0.2f+(rand()%1000)/1000.0f;
				particle->yscale = 0.2f+(rand()%1000)/1000.0f;
			}
			
		}

			for (int i=0;i<10;i++)
			{
				float randx = m_x+rand()%5;
				float randy = m_y+rand()%60;

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

	if (!Splash)
		BulletHit(x, y, angle, 10);
	else
		BulletHitSplash(x, y, 10);

	if (bullet!=NULL)
		AttachParticle(bullet);

}

void enemy_zombie::HandleAttacking(void)
{
	if (m_nextdecision<700) return;

	if (this->m_facingscale<0)
	{
		this->armangle = this->armangle + 0.03* enemy_base::pCEnemies->pcore->gfx.tick;
	//	if (this->armangle>1) this->armangle = -2;
	}
	else
	{
		this->armangle = this->armangle - 0.03* enemy_base::pCEnemies->pcore->gfx.tick;
	//	if (this->armangle<1) this->armangle = 4;
	}



}

void enemy_zombie::InitAttack(void)
{
	dwFlags &= ~AISTATE_WALKINGLEFT;
	dwFlags &= ~AISTATE_WALKINGRIGHT;
	dwFlags = dwFlags | AISTATE_ATTACKING;
	this->m_nextdecision = 1000;
	armangle=0.0f;

	LPParticle particle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_ZOMBIEATTACK,m_x+(m_facingscale*20),m_y);
	if (particle!=NULL)
	{
		particle->xvel = (m_facingscale*0.5);
		particle->yvel = 0.1;
		particle->damage = 5;
	}
	particle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_ZOMBIEATTACK,m_x+(m_facingscale*20),m_y+10);
	if (particle!=NULL)
	{
		particle->xvel = (m_facingscale*0.4);
		particle->yvel = 0.2;
		particle->damage = 5;
	}
	particle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_ZOMBIEATTACK,m_x+(m_facingscale*20),m_y+19);
	if (particle!=NULL)
	{
		particle->xvel = (m_facingscale*0.3);
		particle->yvel = 0.2;
		particle->damage = 5;
	}
}

void enemy_zombie::BulletHit(float x, float y, float angle, float speed)
{
	LPParticle dparticle;

	float randomnumber = (((rand()%1000)-500)/1000.0f);
	float randomnumber2 = (((rand()%1000)-500)/1000.0f);
	float ultrandomangle = angle;//+(0.2*randomnumber);
	float randomangle;

	for (int i=0;i<3;i++)
	{
		randomnumber = (((rand()%1000)-500)/1000.0f);
		ultrandomangle = angle+(0.3*randomnumber);
		randomnumber = (((rand()%10000))/10000.0f);
		randomnumber2 = (((rand()%10000))/10000.0f);
		randomnumber = (((rand()%10000))/10000.0f);

		randomangle = ultrandomangle;//+(0.05*randomnumber);
		for (int j=0;j<6;j++)
		{
			dparticle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_BLOODBLOBS, x, y);
			if (dparticle!=NULL)
			{
				dparticle->xvel = (sin(randomangle)*(0.08+(randomnumber*0.6))) +  (sin(randomangle)* (j*0.016));
				dparticle->yvel = (cos(randomangle)*(0.08+(randomnumber*0.6))) + (cos(randomangle)* (j*0.016));
			}
		}
	}

	dparticle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_BLOODMIST, x+((rand()%10)-5), y+((rand()%10)-5));
	if (dparticle!=NULL)
		{
			randomangle = angle;
			randomnumber = (((rand()%10000))/10000.0f);
			dparticle->xvel = sin(randomangle)*((0.01*randomnumber));
			if (enemy_base::pCEnemies->pcore->landscape.Collision(dparticle->x+dparticle->xvel,dparticle->y)) dparticle->xvel = -dparticle->xvel;
			dparticle->yvel = cos(randomangle)*((0.01*randomnumber));
			if (enemy_base::pCEnemies->pcore->landscape.Collision(dparticle->x,dparticle->y+dparticle->yvel)) dparticle->yvel = -dparticle->yvel;
			
		}

	for (int j=0;j<5;j++)
	{
		dparticle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_BLOODBLOBS, x+rand()%20-10, y+rand()%20-10);
		if (dparticle!=NULL)
		{
			dparticle->xvel = (rand()%1000/1000.0f-0.5f)/3;
			dparticle->yvel = (rand()%1000/1000.0f-0.9f)/3;
		}
	}

}


void enemy_zombie::BulletHitSplash(float x, float y, float speed)
{
	LPParticle dparticle;

	float randomnumber;
	float randomnumber2;
	float ultrandomangle;
	float randomangle;
	float randx, randy;
	float angle;

	for (int i=0;i<10;i++)
	{
		randx = m_x+rand()%5;
		randy = m_y+rand()%60;

		angle = atan2(randx-x,randy-y);

		
		randomnumber = (((rand()%100000))/100000.0f);
		randomangle = angle+(0.1*randomnumber);
		randomnumber = (((rand()%100000))/100000.0f);
		for (int j=0;j<6;j++)
		{
			dparticle = enemy_base::pCEnemies->pcore->landscape.AddParticle(PARTICLE_BLOODBLOBS, randx, randy);
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

float enemy_zombie::GetX()
{
	return m_x;
}
float enemy_zombie::GetY()
{
	return m_y;
}
float enemy_zombie::GetFacing()
{
	return m_facingscale;
}

bool enemy_zombie::SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker)
{
	if (x<this->m_x-size) return false;
	if (x>this->m_x+size) return false;
	if (y-20<this->m_y-size) return false;
	if (y-20>this->m_y+size) return false;

	float distance = sqrtf( (m_x-x)*(m_x-x) + (m_y-y-20)*(m_y-y-20));

	/*
	for (float dis=0;dis<distance;dis=dis+10)
	{
		enemy_base::pCEnemies->pcore->landscape.AddParticle(0,x+sin(atan2(m_x-x, m_y-(y-20)))*dis,y+cos(atan2(m_x-x, m_y-(y-20)))*dis);
	}
	*/

	if (distance>size) return false;

	TakeDamage(((size-distance)/size)*damage, atan2(m_x-x, m_y-y-20), x, y, true);

	return true;
}
void enemy_zombie::AttachParticle(LPParticle particle)
{
	LPParticle dparticle=NULL;

	if (health>0)
	if (particle->type==PARTICLE_ARROW)
	{
		float randdepth= (rand()%10000/1000.f);

		particle->x = particle->x - (sin(atan2(particle->xvel,particle->yvel)) *(randdepth-0));
		particle->y = particle->y - (cos(atan2(particle->xvel,particle->yvel)) *(randdepth-0));
		enemy_base::pCEnemies->pcore->sound.PlaySnd(S_BHIT_ARROW,particle->x,particle->y,0.5000000f);

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
