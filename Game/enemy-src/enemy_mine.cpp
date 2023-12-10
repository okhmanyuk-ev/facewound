#include "enemy_mine.h"

#define PIXELS_TO_FEET 32
#define MAX_WALKING_SPEED m_maxwalkingspeed
#define NORMAL_ACCELERATION m_acceleration
#define WIDTH 32

#define MAXSTEPSIZE 0

#define DRAW_DEBUG_TEXT false
#define GENERICTHINKDELAY 200

#define COLLISION enemy_base::pCEnemies->pcore->landscape.Collision

#define PCORE enemy_base::pCEnemies->pcore

enemy_mine::enemy_mine(void)
{
	m_x=0;
	m_y=0;
	m_yvel=0;
	pCEnemies=NULL;
	m_nextdecision=100+rand()%100;
	health=1;
	targetplayer=NULL;
	b_Activated=false;
	m_facingscale = 1;
	m_initcountdown=0;
	m_countdown=0;
	this->UpdateMyRect();
}

enemy_mine::~enemy_mine(void)
{
	
}

void enemy_mine::Do(void)
{
	if (!b_Activated) return;
	enemy_base::pCEnemies->NumEnemiesActivated++;

	if (m_initcountdown>0)
	{
		m_initcountdown = m_initcountdown - PCORE->gfx.tick;
		if (m_initcountdown<=0)
		{
			PCORE->sound.PlaySnd(s_init,m_x,m_y,0);
			PCORE->landscape.AddParticle(PARTICLE_LANDMINERED,m_x,m_y+24);
		}
	}
	else
	if (m_countdown>0)
	{
		m_countdown = m_countdown - PCORE->gfx.tick;
		if (m_countdown<=0)
		{
			doDetonate();
		}
	}

	if (m_nextdecision>0) 
	{
		m_nextdecision = m_nextdecision - 1*(PCORE->gfx.tick);

		if(m_nextdecision<=0)
		{
			DecideWhatToDo();
		}
	}
	HandleFalling();
	HandleWalking();
	HandleAttacking();

}

void enemy_mine::Draw(int layer)
{
	if (layer!=0) return;
	if (m_x+256<PCORE->landscape.offset_x) return;
	if (m_x-256>PCORE->landscape.offset_x+(PCORE->gfx.m_d3dpp.BackBufferWidth/PCORE->landscape.m_zoom)) return;
	if (m_y+256<PCORE->landscape.offset_y) return;
	if (m_y-256>PCORE->landscape.offset_y+(PCORE->gfx.m_d3dpp.BackBufferHeight/PCORE->landscape.m_zoom)) return;
	
	enemy_base::pCEnemies->NumEnemiesDrawn++;

	// just been activated
	if (b_Activated==false)
	{
		//m_yvel= -0.01f;
	}

	b_Activated=true;
	PCORE->SetAlphaMode(false);

	PCORE->gfx.SpriteDraw(this->t_body,
					((this->m_x-16.0f)-PCORE->landscape.offset_x)*PCORE->landscape.m_zoom,
					(this->m_y-PCORE->landscape.offset_y)*PCORE->landscape.m_zoom,  
					0,0,0,
					PCORE->landscape.m_zoom,PCORE->landscape.m_zoom);


	if (DRAW_DEBUG_TEXT)
	{
		PCORE->gfx.DrawTextEx( (m_x-(PCORE->landscape.offset_x))*(PCORE->landscape.m_zoom),
		m_y-70-(PCORE->landscape.offset_y), 255,255,0,"decision:%i\nhealth:%i",(int)this->m_nextdecision, (int)this->health);
		PCORE->gfx.DrawTextEx( (m_x-(PCORE->landscape.offset_x)-3)*(PCORE->landscape.m_zoom),
		 (m_y-(PCORE->landscape.offset_y)-3)*(PCORE->landscape.m_zoom), 255,255,0,"x");
	}
}

bool enemy_mine::LoadStuff(void)
{
	s_abouttoblow = PCORE->sound.LoadSound(8,"sound/enemies/landmine/pre.ogg");
	s_init = PCORE->sound.LoadSound(8,"sound/enemies/landmine/armed.ogg");
	t_body = PCORE->textures.LoadTexture("textures/enemies/mine/mine.bmp");

	m_countdown=0;

	if (PCORE->b_LoadingMap)	m_initcountdown=0;
	else	m_initcountdown=3000;
	//PCORE->gui.WriteToConsole(0,0,0,255,"mine initcountdown: %i",(int)m_initcountdown);
	return true;
}

void enemy_mine::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void enemy_mine::HandleFalling(void)
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
			m_nextdecision = 100;
			m_initcountdown= 3000;
			

			for (int i=0;i<32;i++)
			{
				SetGRect(&trect,my_rect.left,my_rect.top,my_rect.right,my_rect.bottom+i);
				if (PCORE->landscape.Collision(&trect))
				{
					m_y = m_y+i;
					if (m_nextdecision<=0) m_nextdecision = GENERICTHINKDELAY;
					PCORE->landscape.AddParticle(PARTICLE_LANDMINEGREEN,m_x,m_y+24);
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
		m_yvel = m_yvel+0.01;
		if (m_yvel>1) m_yvel = 1;
	}

	// add y velocity
	if (m_yvel!=0)	m_y += (float)(m_yvel*(PCORE->gfx.tick));
	// if fell outa screen then kill
	if (m_y+32>(PCORE->landscape.height*32)) this->TakeDamage(100,3,m_x,m_y);
}

void enemy_mine::HandleWalking(void)
{
	this->UpdateMyRect();
	GRECT trect;
	// can we fall
	if (m_yvel==0)
	{
		SetGRect(&trect,my_rect.left,my_rect.top,my_rect.right,my_rect.bottom+1);
		if (!PCORE->landscape.Collision(&trect))
		{
			m_yvel = 0.1f;
		}
	}
}

void enemy_mine::DecideWhatToDo(void)
{
	m_nextdecision=GENERICTHINKDELAY;

	if (m_countdown>0) return; // explosion already started
	if (m_yvel!=0) return;
	if (m_initcountdown>0) return;

	// are there any players close to me?
	float dist, xdist, ydist;
	CPlayers* playerpointer;
	LPEnemy enemypointer;

	// randomly moving left or right for no real reason
	dist = PCORE->landscape.GetNearestPlayer((m_x), m_y+32, &playerpointer, &xdist, &ydist);
		
	if (fabs(dist)<80)
	{
		targetplayer = playerpointer;
		this->TakeDamage(100,3,m_x,m_y+24);
	}
	else
	{
		dist = PCORE->enemies.GetNearestEnemy((m_x), m_y+32, &enemypointer, &xdist, &ydist, (LPEnemy)this, ENT_E_MINE);
		if (fabs(dist)<80)
		{
			targetplayer = playerpointer;
			this->TakeDamage(100,3,m_x,m_y+24);
		}
	}
	
	
}

bool enemy_mine::BulletHit(float* x, float* y)
{
	if (*x<my_rect.left) return false;
	if (*x>my_rect.right) return false;
	if (*y<my_rect.top) return false;
	if (*y>my_rect.bottom) return false;
	return true;
}

bool enemy_mine::BulletHitRect(GRECT* r)
{
	if (!this->b_Activated) return false;
	if (IntersectGRect(r,&my_rect)) return true;

	return false;
}
void enemy_mine::TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet)
{
	if (health<=0) return;

	//PCORE->gui.WriteToConsole(0,0,0,255,"mine tookdamage: %i",(int)damage);

	if (m_initcountdown<=0 && m_countdown<=0)
	{
		PCORE->sound.PlaySnd(this->s_abouttoblow,m_x,m_y,0);
		PCORE->landscape.AddParticle(PARTICLE_LANDMINERED,m_x,m_y+24);
	}

	LPParticle particle = NULL;
	float randomangle = angle;
	float beforehealth=health;

	if (m_countdown<=0 && m_initcountdown<=0)
	{
		m_countdown = 500.0f+(rand()%150);
	}

	if (!Splash)
	{
		float angle =0;
		if (bullet!=NULL) angle= atan2(-bullet->xvel, -bullet->yvel);

		float randomangle;
		float ranspeed;
		for (int i=0;i<10;i++)
		{
			particle = PCORE->landscape.AddParticle(PARTICLE_RICHOCHETSPARK,x,y-2);
			if (particle!=NULL)
			{
				if (bullet!=NULL)
				{
					randomangle = angle+(((rand()%2000)-1000)/3000.0f);
					ranspeed = 0.2f+(((rand()%1000))/2000.0f);
					particle->xvel = sin(randomangle)*ranspeed;
					particle->yvel = cos(randomangle)*ranspeed;
				}
				else
				{
					particle->xvel = particle->xvel*0.5f;
					particle->yvel = particle->yvel*0.5f;
					if (particle->yvel>0) particle->yvel = -particle->yvel;
				}

			}
		}
	}
}

void enemy_mine::HandleAttacking(void)
{

}

void enemy_mine::InitAttack(void)
{
}

void enemy_mine::BulletHitSplash(float x, float y, float speed)
{
}

float enemy_mine::GetX()
{
	return m_x;
}
float enemy_mine::GetY()
{
	return m_y;
}
float enemy_mine::GetFacing()
{
	return m_facingscale;
}

bool enemy_mine::SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker)
{
	if (x<this->m_x-size) return false;
	if (x>this->m_x+size) return false;
	if (y-20<this->m_y-size) return false;
	if (y-20>this->m_y+size) return false;

	float distance = sqrtf( (m_x-x)*(m_x-x) + (m_y-y-20)*(m_y-y-20));

	if (distance>size) return false;

	TakeDamage(((size-distance)/size)*damage, atan2(m_x-x, m_y-y-20), x, y, true);

	return true;
}
void enemy_mine::AttachParticle(LPParticle particle)
{
}

void enemy_mine::UpdateMyRect(void)
{
	my_rect.left = m_x-5;
	my_rect.right = m_x+5;
	my_rect.top = m_y+27;
	my_rect.bottom = m_y+32;
}

void enemy_mine::doDetonate(void)
{
	if (health<=0) return;

	//PCORE->gui.WriteToConsole(0,0,0,255,"mine detonate!: %i",(int)health);

	health=-1;
	PCORE->landscape.DoSplashDamage(m_x+16,m_y+32,300,350,NULL);
	PCORE->landscape.DrawExplosion(m_x,m_y+29,50);
	// blow up
	float angle =0;

	

	for (int i=0;i<7;i++)
	{
		PCORE->landscape.AddParticle(PARTICLE_LANDMINEDUST,m_x+(rand()%160)-80,m_y-(i*35));
	}

	LPParticle particle = NULL;

	for (int i=0;i<60;i++)
	{
		particle = PCORE->landscape.AddParticle(PARTICLE_RICHOCHETSPARK, m_x, m_y+29);
		if (particle!=NULL)
		{
			particle->xvel = particle->xvel*0.5f;
			particle->yvel = particle->yvel*2.5f;
			if (particle->yvel>0) particle->yvel = -particle->yvel;
		}
	}
}

