#include "enemy_zombie.h"

#define PIXELS_TO_FEET 64
#define MAX_WALKING_SPEED m_maxwalkingspeed
#define NORMAL_ACCELERATION m_acceleration
#define WIDTH 60

#define MAXSTEPSIZE 32

#define DRAW_DEBUG_TEXT false

#define GENERICTHINKDELAY 1500

#define COLLISION PCORE->landscape.Collision

#define PCORE enemy_base::pCEnemies->pcore

enemy_zombie::enemy_zombie(void)
{
	
	m_x=0;
	m_y=0;
	m_yvel=0;
	pCEnemies=NULL;
	m_xvel=0;
	dwFlags=0;
	m_nextdecision=20+((rand()%30000)/2000);
	m_facingscale=1;
	health=100;
	targetplayer=NULL;
	armangle=0;
	armangle = D3DX_PI/2;
	b_Activated=false;
	m_acceleration = 0.0004f+((rand()%10000)/10000000.0f);
	m_maxwalkingspeed = 0.01f+((rand()%1000)/10000.0f);
	t_head=NULL;
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
		m_nextdecision = m_nextdecision - 1*(PCORE->gfx.tick);
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

	UpdateMyRect();

}

void enemy_zombie::Draw(int layer)
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

	if (armangle>targetarmangle) armangle=armangle- (0.0005*(PCORE->gfx.tick));
	else if (armangle<targetarmangle) armangle=armangle+ (0.0005*(PCORE->gfx.tick));



	//arm behind
	x = ((m_x-(PCORE->landscape.offset_x)-13+(m_facingscale<0?4:0))  *(PCORE->landscape.m_zoom));
	y = (m_y-PCORE->landscape.offset_y+19+(m_facingscale<0?16:2))                              *(PCORE->landscape.m_zoom);
	PCORE->gfx.SpriteDraw(t_zombie_arm,  x,y,  armangle,9*(PCORE->landscape.m_zoom),m_facingscale*6*(PCORE->landscape.m_zoom),  1*(PCORE->landscape.m_zoom),m_facingscale*(PCORE->landscape.m_zoom));


	// body
	x = ((m_x-(PCORE->landscape.offset_x)-(WIDTH/2)+(m_facingscale==-1?64:0))  *(PCORE->landscape.m_zoom));
	y = (m_y-(PCORE->landscape.offset_y))  *(PCORE->landscape.m_zoom);
	
	
	PCORE->gfx.SpriteDrawEx(t_zombie_body,  x,y,  0,0,0,  m_facingscale*(PCORE->landscape.m_zoom),1*(PCORE->landscape.m_zoom),
	((int)(m_x/7.0f)%8)*64,0,64,64,255,255,255,255);

	//head
	x = ((m_x-(PCORE->landscape.offset_x)-(WIDTH/2)+16+(m_facingscale==-1?34:0))  *(PCORE->landscape.m_zoom));
	y = (m_y-(PCORE->landscape.offset_y)-9)  *(PCORE->landscape.m_zoom);
	PCORE->gfx.SpriteDraw(t_head,  x,y,  0,0,0,  m_facingscale*(PCORE->landscape.m_zoom),1*(PCORE->landscape.m_zoom));

	//arm
	x = ((m_x-(PCORE->landscape.offset_x)-10+(m_facingscale<0?4:0))  *(PCORE->landscape.m_zoom));
	y = (m_y-PCORE->landscape.offset_y+19+(m_facingscale<0?14:0))                              *(PCORE->landscape.m_zoom);
	PCORE->gfx.SpriteDraw(t_zombie_arm,  x,y,  armangle,9*(PCORE->landscape.m_zoom),m_facingscale*6*(PCORE->landscape.m_zoom),  1*(PCORE->landscape.m_zoom),m_facingscale*(PCORE->landscape.m_zoom));


	

	if (DRAW_DEBUG_TEXT)
	{
		PCORE->gfx.DrawTextEx( (m_x-(PCORE->landscape.offset_x))*(PCORE->landscape.m_zoom),y-70, 255,255,0,"decision:%i\nhealth:%i",(int)this->m_nextdecision, (int)this->health);
		PCORE->gfx.DrawTextEx( (m_x-(PCORE->landscape.offset_x)-3)*(PCORE->landscape.m_zoom),
		 (m_y-(PCORE->landscape.offset_y)-3)*(PCORE->landscape.m_zoom), 255,255,0,"x");
	}
}

bool enemy_zombie::LoadStuff(void)
{
	unsigned int i = rand()%3;

	if (i==0)t_head = PCORE->textures.LoadTexture("textures/enemies/zombie/head.bmp");
	else if (i==1)t_head = PCORE->textures.LoadTexture("textures/enemies/zombie/head1.bmp");
	else if (i==2)t_head = PCORE->textures.LoadTexture("textures/enemies/zombie/head2.bmp");
	
	s_bulletflesh1 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");
	s_bulletflesh2 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");
	s_bulletflesh3 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");
	s_zombiedie1 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/zombiedead.ogg");

	s_zombiepain1 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/pain1.ogg");
	s_zombiepain2 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/pain2.ogg");
	s_zombiepain3 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/pain3.ogg");

	s_zombiespotted1 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/sighted1.ogg");
	s_zombiespotted2 = PCORE->sound.LoadSound(2,"sound/enemies/zombie/sighted2.ogg");
	
	t_zombie_body = PCORE->textures.LoadTexture("textures/enemies/zombie/body1.bmp");
	t_zombie_arm = PCORE->textures.LoadTexture("textures/enemies/zombie/arm.bmp");

	return true;
}

void enemy_zombie::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void enemy_zombie::HandleFalling(void)
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

void enemy_zombie::DecideWhatToDo(void)
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

bool enemy_zombie::BulletHit(float* x, float* y)
{
	if (!this->b_Activated) return false;

	if (*x<my_rect.left) return false;
	if (*x>my_rect.right) return false;
	if (*y<my_rect.top) return false;
	if (*y>my_rect.bottom) return false;

	return true;
}

bool enemy_zombie::BulletHitRect(GRECT* r)
{
	if (!this->b_Activated) return false;

	if (IntersectGRect(r,&my_rect)) return true;

	return false;
}
void enemy_zombie::TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet)
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


		particle = PCORE->landscape.AddParticle(PARTICLE_ZOMBIE_HEAD, x+16.5, y+16.5);
		

		if (particle!=NULL)
		{
			particle->xvel = sin(angle)*(damage/40);//bullet->xvel/20;
			particle->yvel = cos(angle)*(damage/40);//bullet->yvel/20;
			particle->rot = 0;
			particle->xscale = m_facingscale;
			particle->textureoverride = t_head;//PCORE->textures.LoadTexture("textures/enemies/zombie/head1.bmp");
		}

		for (int i=0;i<2;i++)
		{
			x = (m_x)-10+(m_facingscale<0?4:0);
			y = (m_y+19+(m_facingscale<0?14:0));
			particle = PCORE->landscape.AddParticle(PARTICLE_ZOMBIE_ARM, x, y-(i*2));

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
	{
		PCORE->landscape.GenericBloodSpurt(x, y, angle, 1.0f);
	}
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

void enemy_zombie::HandleAttacking(void)
{
	if (m_nextdecision<700) return;

	if (this->m_facingscale<0)
	{
		this->armangle = this->armangle + 0.03* PCORE->gfx.tick;
	//	if (this->armangle>1) this->armangle = -2;
	}
	else
	{
		this->armangle = this->armangle - 0.03* PCORE->gfx.tick;
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

	LPParticle particle = PCORE->landscape.AddParticle(PARTICLE_ZOMBIEATTACK,m_x+(m_facingscale*20),m_y);
	if (particle!=NULL)
	{
		particle->xvel = (m_facingscale*0.5);
		particle->yvel = 0.1;
		particle->damage = 5;
	}
	particle = PCORE->landscape.AddParticle(PARTICLE_ZOMBIEATTACK,m_x+(m_facingscale*20),m_y+10);
	if (particle!=NULL)
	{
		particle->xvel = (m_facingscale*0.4);
		particle->yvel = 0.2;
		particle->damage = 5;
	}
	particle = PCORE->landscape.AddParticle(PARTICLE_ZOMBIEATTACK,m_x+(m_facingscale*20),m_y+19);
	if (particle!=NULL)
	{
		particle->xvel = (m_facingscale*0.3);
		particle->yvel = 0.2;
		particle->damage = 5;
	}
}

void enemy_zombie::BulletHitSplash(float x, float y, float speed)
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
void enemy_zombie::AttachParticle(LPParticle particle)
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

void enemy_zombie::DoFireDamage(float x, float y, float damage)
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


void enemy_zombie::UpdateMyRect(void)
{
	my_rect.left = m_x-8;
	my_rect.right = m_x+8;
	my_rect.top = m_y+1;
	my_rect.bottom = m_y+64;
}