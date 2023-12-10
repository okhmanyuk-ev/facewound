#include "enemy_sickzombie.h"

#define PIXELS_TO_FEET 64
#define MAX_WALKING_SPEED m_maxwalkingspeed
#define NORMAL_ACCELERATION m_acceleration
#define WIDTH 60

#define MAXSTEPSIZE 32

#define DRAW_DEBUG_TEXT false

#define GENERICTHINKDELAY 1500

#define COLLISION PCORE->landscape.Collision

#define PCORE enemy_base::pCEnemies->pcore

enemy_sickzombie::enemy_sickzombie(void)
{
	m_x=0;
	m_y=0;
	m_yvel=0;
	pCEnemies=NULL;
	m_xvel=0;
	dwFlags=0;
	m_nextdecision=20+rand()%3000;
	m_facingscale=1;
	health=100;
	targetplayer=NULL;
	b_Activated=false;
	m_acceleration = 0.0004f+((rand()%10000)/10000000.0f);
	m_maxwalkingspeed = 0.01f;
	t_head=NULL;
	m_blinkoffset= rand()%2000;
	this->m_attacktimer = 2000+rand()%3000;
}

enemy_sickzombie::~enemy_sickzombie(void)
{
	
}

void enemy_sickzombie::Do(void)
{
	if (!b_Activated) return;

	enemy_base::pCEnemies->NumEnemiesActivated++;

	if (m_nextdecision>0) 
	{
		m_nextdecision = m_nextdecision - 1*(PCORE->gfx.tick);
		if(m_nextdecision<=0 && !(dwFlags & AISTATE_COMPLETINGTASK))
		{
			DecideWhatToDo();
		}
	}

	if ((this->targetplayer!=NULL) && !(dwFlags & AISTATE_ATTACKING) && (m_attacktimer>5000))
	{
		if (fabs(targetplayer->m_fX-m_x)<800 && fabs(targetplayer->m_fY-m_y)<100)
		{
			InitAttack();
			return;
		}
	}
	else if ((this->targetplayer!=NULL))
	{
		if (fabs(targetplayer->m_fX-m_x)<800 && fabs(targetplayer->m_fY-m_y)<100)
		{
			m_attacktimer = m_attacktimer + PCORE->gfx.tick;
		}
	}
	HandleFalling();

	if (dwFlags & AISTATE_WALKINGLEFT || dwFlags & AISTATE_WALKINGRIGHT)  HandleWalking();

	if (dwFlags & AISTATE_ATTACKING)  HandleAttacking();

	UpdateMyRect();

}

void enemy_sickzombie::Draw(int layer)
{
	if (layer!=0) return;
	if (m_x+256<PCORE->landscape.offset_x) return;
	if (m_x-256>PCORE->landscape.offset_x+(PCORE->gfx.m_d3dpp.BackBufferWidth/PCORE->landscape.m_zoom)) return;
	if (m_y+256<PCORE->landscape.offset_y) return;
	if (m_y-256>PCORE->landscape.offset_y+(PCORE->gfx.m_d3dpp.BackBufferHeight/PCORE->landscape.m_zoom)) return;
	
	
	enemy_base::pCEnemies->NumEnemiesDrawn++;
	b_Activated=true;

	PCORE->SetAlphaMode(false);

	float x,y;

	if (m_xvel>0) m_facingscale=1;
	else if (m_xvel<0) m_facingscale=-1;

	// body
	x = ((m_x-(PCORE->landscape.offset_x)-(WIDTH/2)+(m_facingscale==-1?64:0))  *(PCORE->landscape.m_zoom));
	y = (m_y-(PCORE->landscape.offset_y))  *(PCORE->landscape.m_zoom);
	PCORE->gfx.SpriteDrawEx(t_zombie_body,  x,y,  0,0,0,  m_facingscale*(PCORE->landscape.m_zoom),1*(PCORE->landscape.m_zoom),
	((int)(m_x/2.0f)%8)*64,
	0,64,64,255,255,255,255);

	int i =0;
	if ((int)(PCORE->TimerA+m_blinkoffset)%1000>700) i=1;

	if (dwFlags & AISTATE_ATTACKING)
	{
		i=3;
		if ((int)PCORE->TimerA%200>100) i=2;
	}
	//head
	x = ((m_x-(PCORE->landscape.offset_x)-(WIDTH/2)+16+(m_facingscale==-1?24:10))  *(PCORE->landscape.m_zoom));
	y = (m_y-(PCORE->landscape.offset_y))  *(PCORE->landscape.m_zoom);
	PCORE->gfx.SpriteDrawEx(t_head,  x,y,  0,0,0,  m_facingscale*(PCORE->landscape.m_zoom),1*(PCORE->landscape.m_zoom),
		i*32,0,32,32,255,255,255,255);


	if (DRAW_DEBUG_TEXT)
	{
		PCORE->gfx.DrawTextEx( (m_x-(PCORE->landscape.offset_x))*(PCORE->landscape.m_zoom),y-70, 255,255,0,"decision:%i\nhealth:%i",(int)this->m_nextdecision, (int)this->health);
		PCORE->gfx.DrawTextEx( (m_x-(PCORE->landscape.offset_x)-3)*(PCORE->landscape.m_zoom),
		 (m_y-(PCORE->landscape.offset_y)-3)*(PCORE->landscape.m_zoom), 255,255,0,"x");
	}
}

bool enemy_sickzombie::LoadStuff(void)
{
	unsigned int i = rand()%3;

	//if (i==0)
		
	t_head = PCORE->textures.LoadTexture("textures/enemies/sickzombie/head1.bmp");
//	else if (i==1)t_head = PCORE->textures.LoadTexture("textures/enemies/sickzombie/head1.bmp");
//	else if (i==2)t_head = PCORE->textures.LoadTexture("textures/enemies/sickzombie/head2.bmp");
	
	t_zombie_body = PCORE->textures.LoadTexture("textures/enemies/sickzombie/body1.bmp");

	s_bulletflesh1 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");
	s_bulletflesh2 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");
	s_bulletflesh3 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");
	s_zombiedie1 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/zombiedead.ogg");

	s_sick1 = PCORE->sound.LoadSound(6,"sound/enemies/sickzombie/sick01.ogg");
	s_sick2 = PCORE->sound.LoadSound(6,"sound/enemies/sickzombie/sick02.ogg");
	s_sick3 = PCORE->sound.LoadSound(6,"sound/enemies/sickzombie/sick03.ogg");
	s_sick4 = PCORE->sound.LoadSound(6,"sound/enemies/sickzombie/sick04.ogg");

	s_zombiepain1 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/pain1.ogg");
	s_zombiepain2 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/pain2.ogg");
	s_zombiepain3 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/pain3.ogg");

	s_zombiespotted1 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/sighted1.ogg");
	s_zombiespotted2 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/sighted2.ogg");
	


	return true;
}

void enemy_sickzombie::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void enemy_sickzombie::HandleFalling(void)
{
	GRECT trect;

	// if we're standing - don't do anything.
	if ((dwFlags & AISTATE_STANDING)) return;

	UpdateMyRect();
	SetGRect(&trect,my_rect.left,my_rect.top,my_rect.right,my_rect.bottom+1+(float)(m_yvel*(PCORE->gfx.tick)));

	// if falling and feet hit floor
	if (PCORE->landscape.Collision(&trect))
	{
		dwFlags &= ~AISTATE_FALLING;
		m_y = m_y + (float)(m_yvel*(PCORE->gfx.tick));
		float top = my_rect.top;

		for (int i=0;i<1000;i++)
		{
			m_y = m_y-1;
			UpdateMyRect();
			SetGRect(&trect,my_rect.left, top,my_rect.right,my_rect.bottom);

			if (!PCORE->landscape.Collision(&trect))
			{
				UpdateMyRect();
				m_yvel = 0;
				dwFlags = dwFlags | AISTATE_STANDING;
				return;
			}
		}

	}
	else
	{
		dwFlags = dwFlags | AISTATE_FALLING;
		m_yvel = m_yvel+(0.001*(PCORE->gfx.tick));
	}

	if (m_yvel!=0)	m_y += (float)(m_yvel*(PCORE->gfx.tick));

	//Blow up if fell out of screen
	if (my_rect.bottom>(PCORE->landscape.height*32)) this->TakeDamage(100,3,m_x,m_y);
}

void enemy_sickzombie::HandleWalking(void)
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

	float xmov = m_xvel*(PCORE->gfx.tick);
	bool collision = false;

	GRECT trect;
	UpdateMyRect();
	SetGRect(&trect,my_rect.left+xmov,my_rect.top,my_rect.right+xmov,my_rect.bottom);

	if (PCORE->landscape.Collision(&trect))
	{
		collision=true;

		// if there's a step we can step up on
		SetGRect(&trect,my_rect.left+xmov,my_rect.bottom-32,my_rect.right+xmov,my_rect.bottom);
		if (PCORE->landscape.Collision(&trect))
		{
			float stepup=0;

			for (int i=1;i<=MAXSTEPSIZE;i++)
			{
				SetGRect(&trect,my_rect.left+xmov,my_rect.top-i,my_rect.right+xmov,my_rect.bottom-i);
				if (!PCORE->landscape.Collision(&trect))
				{
					stepup = i;
					collision=false;
					break;
				}
			}

			m_y=m_y-stepup;
			UpdateMyRect();
		}

	}

	if (collision)
	{
		m_xvel=0;
	}

	m_x = m_x + m_xvel*(PCORE->gfx.tick);

	// can we fall in this new position?
	if (dwFlags & AISTATE_STANDING)
	{
		GRECT trect;
		UpdateMyRect();
		SetGRect(&trect,my_rect.left,my_rect.top,my_rect.right,my_rect.bottom+2);
		if (!PCORE->landscape.Collision(&trect))
		{
			dwFlags = dwFlags | AISTATE_FALLING;
			dwFlags &= ~AISTATE_STANDING;
		}
	}

}

void enemy_sickzombie::DecideWhatToDo(void)
{
	// are there any players close to me?

	float dist, xdist, ydist;
	CPlayers* playerpointer;

	dwFlags &= ~AISTATE_ATTACKING; // stop attacking 

	if (rand()%10==0)
	{
		// chances of moving left/right - IF NOT BLOCKED
		if (rand()%2==0)
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
		dist = PCORE->landscape.GetNearestPlayer((m_x+(m_facingscale*20)), m_y, &playerpointer, &xdist, &ydist);

		if (fabs(ydist)<400)
		{
			if (targetplayer!=playerpointer)
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

bool enemy_sickzombie::BulletHit(float* x, float* y)
{
	if (!this->b_Activated) return false;

	if (*x<my_rect.left) return false;
	if (*x>my_rect.right) return false;
	if (*y<my_rect.top) return false;
	if (*y>my_rect.bottom) return false;

	return true;
}

bool enemy_sickzombie::BulletHitRect(GRECT* r)
{
	if (!this->b_Activated) return false;

	if (IntersectGRect(r,&my_rect)) return true;

	return false;
}
void enemy_sickzombie::TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet)
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
				bullet->pplayer->AddBulletTime(SKILL_BT_ZOMBIE);
			}
		}

		PCORE->landscape.UnAttachAttachmentsMonster(this);
		PCORE->sound.PlaySnd(s_zombiedie1,m_x,m_y);

		x = (m_x-(WIDTH/2)+16);
		y = (m_y-9);

		float rnum;
		LPParticle p = PCORE->landscape.AddParticle(PARTICLE_SICKBULLET,m_x, m_y+rand()%40);
		
		for (int i=0;i<5;i++)
		{
			if (p!=NULL)
			{
				p->xscale = 0.4f;
				p->yscale = 0.4f;
				rnum = -1.0f+rand()%2000/1000.0f;
				p->xvel = rnum*0.2f;
				rnum = rand()%1000/1000.0f*-0.9;
				p->yvel = rnum*0.3f;
				p->damage = SKILL_ZOMBIE_SICK_DAMAGE;
			}
		}

		for (int i=0;i<10;i++)
		{
			p = PCORE->landscape.AddParticle(PARTICLE_SICK, m_x, m_y+rand()%40);
			if (p!=NULL)
			{
				p->xscale = p->xscale*3.0f;
				p->yscale = p->yscale*3.0f;
				rnum = -1.0f+rand()%2000/1000.0f;
				p->xvel = rnum*0.1f;
				rnum = rand()%1000/1000.0f*-0.9;
				p->yvel = rnum*0.4f;
			}
		}

		// loverly loverly blood
		for (int j=0;j<35;j++)
		{
			
			particle = PCORE->landscape.AddParticle(PARTICLE_BLOODBLOBS, m_x+rand()%25-12, m_y+rand()%48-24,0,0,0,PCORE->landscape.t_BloodBlobs[rand()%NUM_BLOODBLOBS],0);
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

				particle = PCORE->landscape.AddParticle(PARTICLE_BLOODMIST, randx, randy);
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
			if (rand()%50==0)
			{
				unsigned int rnum=rand()%3;

				if (rnum==0) PCORE->sound.PlaySnd(s_zombiepain1,m_x,m_y);
				else if (rnum==1) PCORE->sound.PlaySnd(s_zombiepain2,m_x,m_y);
				else PCORE->sound.PlaySnd(s_zombiepain2,m_x,m_y);
			}
			return;
		}
	}

	if (!Splash)
		PCORE->landscape.GenericBloodSpurt(x, y, angle, 1.0f);
	else
		BulletHitSplash(x, y, 10);

	if (bullet!=NULL)
		AttachParticle(bullet);

	if (rand()%3==0) PCORE->sound.PlaySnd(s_bulletflesh1,m_x,m_y);
	else if (rand()%2==0) PCORE->sound.PlaySnd(s_bulletflesh2,m_x,m_y);
	else PCORE->sound.PlaySnd(s_bulletflesh3,m_x,m_y);

	if (rand()%10==0)
	{
		unsigned int rnum=rand()%3;

		if (rnum==0) PCORE->sound.PlaySnd(s_zombiepain1,m_x,m_y);
		else if (rnum==1) PCORE->sound.PlaySnd(s_zombiepain2,m_x,m_y);
		else PCORE->sound.PlaySnd(s_zombiepain2,m_x,m_y);
	}

}

void enemy_sickzombie::HandleAttacking(void)
{
	m_attacktimer = m_attacktimer + (1.0f*PCORE->gfx.tick);
	m_sicktimer = m_sicktimer + (1.0f*PCORE->gfx.tick);

	PCORE->sound.UpdateSound(attackchannel,m_x,m_y);

	if (m_attacktimer<2500)
	{
		if (m_sicktimer>30.0f)
		{
			m_sicktimer = m_sicktimer - 30.0f;
			float rnum;
			float tnum=1.0f;

			if (m_attacktimer>2000)
			{
				tnum = (1000-(m_attacktimer-2000))/1000.0f;
			}

			// The bullet does the damage, the rest is for decoration
			LPParticle p = PCORE->landscape.AddParticle(PARTICLE_SICKBULLET,m_x+(15*this->m_facingscale), m_y+20);
			if (p!=NULL)
			{
				rnum = 0.75f+ rand()%1000/4000.0f;
				p->xvel = 0.5*this->m_facingscale*rnum*tnum;
				rnum = 0.75f+ rand()%1000/4000.0f;
				p->yvel = -0.2*rnum*tnum;
				p->damage = SKILL_ZOMBIE_SICK_DAMAGE;
			}

			for (int i=0;i<5;i++)
			{
				p = PCORE->landscape.AddParticle(PARTICLE_SICK,m_x+(15*this->m_facingscale), m_y+20);
				if (p!=NULL)
				{
					rnum = 0.75f+ rand()%1000/4000.0f;
					p->xvel = 0.5*this->m_facingscale*rnum*tnum;
					rnum = 0.75f+ rand()%1000/4000.0f;
					p->yvel = -0.2*rnum*tnum;
				}
			}

		}
	}
	else
	{
		float tnum = ((500-(m_attacktimer-2500))/500.0f)*0.1;

		if (m_sicktimer>50.0f)
		{		
			m_sicktimer = m_sicktimer - 50.0f;
			LPParticle p = PCORE->landscape.AddParticle(PARTICLE_BLOODBLOBS,m_x+(15*this->m_facingscale), m_y+20);
			if (p!=NULL)
			{
				p->xvel = 0.5*this->m_facingscale*tnum;
				p->yvel = -0.2*tnum;
			}
			
		}
	}




	if (m_attacktimer>3000)
	{
		dwFlags &= ~AISTATE_ATTACKING;
		m_attacktimer = rand()%300;
		
	}
}

void enemy_sickzombie::InitAttack(void)
{
	dwFlags &= ~AISTATE_WALKINGLEFT;
	dwFlags &= ~AISTATE_WALKINGRIGHT;
	dwFlags = dwFlags | AISTATE_ATTACKING;
	this->m_nextdecision = 5000+rand()%2000;
	this->m_attacktimer = 0;
	m_sicktimer = 0;

	int r = rand()%4;

	if (r==0)
	attackchannel = PCORE->sound.PlaySnd(s_sick1,m_x,m_y,0.0f);
	else if (r==1)
	attackchannel = PCORE->sound.PlaySnd(s_sick2,m_x,m_y,0.0f);
	else if (r==2)
	attackchannel = PCORE->sound.PlaySnd(s_sick3,m_x,m_y,0.0f);
	else if (r==3)
	attackchannel = PCORE->sound.PlaySnd(s_sick4,m_x,m_y,0.0f);

}

void enemy_sickzombie::BulletHitSplash(float x, float y, float speed)
{
	LPParticle dparticle;

	float randomnumber;
//	float randomnumber2;
//	float ultrandomangle;
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
			dparticle = PCORE->landscape.AddParticle(PARTICLE_BLOODBLOBS, randx, randy,0,0,0,PCORE->landscape.t_BloodBlobs[rand()%NUM_BLOODBLOBS],0);
			if (dparticle!=NULL)
			{
				dparticle->xvel = (sin(randomangle)*0.7*randomnumber);
				dparticle->yvel = (cos(randomangle)*0.7*randomnumber);
			}
		}
	
		randomnumber = (((rand()%100000))/100000.0f);

		dparticle = PCORE->landscape.AddParticle(PARTICLE_BLOODMIST, randx, randy);
		if (dparticle!=NULL)
			{
				randomangle = angle;
				randomnumber = (((rand()%10000))/10000.0f);
				dparticle->xvel = sin(randomangle)*((0.1*randomnumber));
				dparticle->yvel = cos(randomangle)*((0.1*randomnumber));
		
			}

	}


}

float enemy_sickzombie::GetX()
{
	return m_x;
}
float enemy_sickzombie::GetY()
{
	return m_y;
}
float enemy_sickzombie::GetFacing()
{
	return m_facingscale;
}

bool enemy_sickzombie::SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker)
{
	if (x<this->m_x-size) return false;
	if (x>this->m_x+size) return false;
	if (y-20<this->m_y-size) return false;
	if (y-20>this->m_y+size) return false;

	float distance = sqrtf( (m_x-x)*(m_x-x) + (m_y-y-20)*(m_y-y-20));

	/*
	for (float dis=0;dis<distance;dis=dis+10)
	{
		PCORE->landscape.AddParticle(0,x+sin(atan2(m_x-x, m_y-(y-20)))*dis,y+cos(atan2(m_x-x, m_y-(y-20)))*dis);
	}
	*/

	if (distance>size) return false;

	if (this->health>=0)
	{

		TakeDamage(((size-distance)/size)*damage, atan2(m_x-x, m_y-y-20), x, y, true);

		if (this->health<0 && attacker!=NULL)
		{
			attacker->CountKill();
			attacker->AddBulletTime(SKILL_BT_ZOMBIE);
		}

	}
	return true;
}
void enemy_sickzombie::AttachParticle(LPParticle particle)
{
	if (health<=0) return;

	LPParticle dparticle=NULL;
	if (particle->type==PARTICLE_ARROW)
	{
		float randdepth= (rand()%10000/1000.f);

		particle->x = particle->x - (sin(atan2(particle->xvel,particle->yvel)) *(randdepth-0));
		particle->y = particle->y - (cos(atan2(particle->xvel,particle->yvel)) *(randdepth-0));

		dparticle = PCORE->landscape.AddParticle(PARTICLE_ARROWDEAD,particle->x,particle->y);		if (dparticle!=NULL)
		{
			dparticle->AttachedTo = ATTACHED_TO_MONSTER;
			dparticle->pmonster = this;
			dparticle->attached_x = (particle->x - this->m_x)* this->m_facingscale ;// ;
			dparticle->attached_y = particle->y - this->m_y;
			dparticle->attached_rot = atan2(sin(particle->rot),cos(particle->rot)* this->m_facingscale)+ (((rand()%20000/10000.f)-1)*0.1f);
			dparticle->IsMoving = false;
		}
	}
	else if (particle->type==PARTICLE_FIRE)
	{
		float randdepth= (rand()%10000/1000.f);

		particle->x = particle->x - (sin(atan2(particle->xvel,particle->yvel)) *(randdepth-0));
		particle->y = particle->y - (cos(atan2(particle->xvel,particle->yvel)) *(randdepth-0));

		dparticle = PCORE->landscape.AddParticle(PARTICLE_ARROWDEAD,particle->x,particle->y);		if (dparticle!=NULL)
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

void enemy_sickzombie::DoFireDamage(float x, float y, float damage)
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
			dparticle->SpawnTimerReset = 10;
			dparticle->SpawnDistance = rand()%20;
			dparticle->damage = FIRE_DAMAGE;
		}

}


void enemy_sickzombie::UpdateMyRect(void)
{
	my_rect.left = m_x-4;
	my_rect.right = m_x+4;
	my_rect.top = m_y;
	my_rect.bottom = m_y+62;
}