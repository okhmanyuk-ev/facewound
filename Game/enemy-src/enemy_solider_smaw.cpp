#include "enemy_solider_smaw.h"

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

#define USEWEAPON WEP_SMAW

enemy_soldier_smaw::enemy_soldier_smaw(void)
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
	fighttimer = rand()%20;
	animtimer = 0;
	this->UpdateMyRect();
	shootcount=0;
	s_die = NULL;

}

enemy_soldier_smaw::~enemy_soldier_smaw(void)
{
	
}

void enemy_soldier_smaw::Draw(int layer)
{
	if (layer!=0) return;
	if (m_x+256<PCORE->landscape.offset_x) return;
	if (m_x-256>PCORE->landscape.offset_x+(PCORE->gfx.m_d3dpp.BackBufferWidth/PCORE->landscape.m_zoom)) return;
	if (m_y+256<PCORE->landscape.offset_y) return;
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

void enemy_soldier_smaw::HandleAttacking(void)
{
	if (reloadtimer>0 && fighttimer<=0)
	{
		reloadtimer = reloadtimer -  (0.01*PCORE->gfx.tick);
		if (reloadtimer<=0)
		{
			clip = PCORE->weapons.m_GunAttributes[USEWEAPON].ClipSize;
			fighttimer = 10.0f;
		}
	}

	if (targetplayer==NULL) 
	{
		targetangle = -D3DX_PI/4;
		return;
	}

	if (fighttimer<=0 || reloadtimer <=0)
	{
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
	}


	if (fighttimer>0)
	{
		fighttimer = fighttimer -  (0.01*PCORE->gfx.tick);
	}
	else if (aimangle==targetangle && clip>0)
	{
		clip = clip - 1;
		fighttimer = 10.0f;
		
		reloadtimer = PCORE->weapons.m_GunAttributes[USEWEAPON].ReloadTime*SKILL_RELOADMULTIPLIER;

		float sx = m_x + sin(aimangle)*PCORE->weapons.m_GunAttributes[USEWEAPON].MuzzleTip;
		float sy = m_y + cos(aimangle)*PCORE->weapons.m_GunAttributes[USEWEAPON].MuzzleTip;

		PCORE->landscape.BulletShoot(USEWEAPON,sx,sy,aimangle+(shootcount*0.03f*m_facingscale),1, true);
		PCORE->weapons.PlayShootSound(m_x,m_y,USEWEAPON);
		PCORE->weapons.DrawMuzzle(sx,sy,aimangle,USEWEAPON);
	}
	else if (reloadtimer<=0)
	{
		PCORE->weapons.PlayReloadSound(m_x,m_y,USEWEAPON);
	}
}


void enemy_soldier_smaw::StartupVariables(void)
{
	aimangle = -D3DX_PI/5;
	this->clip = PCORE->weapons.m_GunAttributes[USEWEAPON].ClipSize;
	reloadtimer=0;
	fighttimer = (rand()%10000/1000.0f);
}