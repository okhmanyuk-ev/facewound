
#include "Players.h"
#include "Weapons.h"
#include "Core.h"
#include "garryswrappers/garrysgraphicswrapper.h"
#include "flyingmachine.h"

#define SQUARED(f) (f*f)

#define DISTANCE_BETWEEN_2_POINTS(x1, x2, y1, y2) (sqrtf(SQUARED(x1-x2) + (SQUARED(y1-y2))))
#define VECTOR_LENGTH(x1,y1) (sqrtf(SQUARED(x1) + (SQUARED(y1))))


#define PC  p->pcore
#define TX  PC->textures
#define GF  PC->gfx
#define LD  PC->landscape
#define ZM  LD.m_zoom
#define TO_SCX(f) ((f)-LD.offset_x)
#define TO_SCY(f) ((f)-LD.offset_y)

#define SCALE 1.1f
#define MAX_VELOCITY 50.0f

void CFlyingMachine::Init(CPlayers* p, char* model)
{
	char filename[500];
	char path[255];
	char defaultpath[255];

	sprintf(path,"players/%s/",model);
	sprintf(defaultpath,"players/player/");

	sprintf(filename,"%sflyingmachine.bmp", path);
	if (fopen(filename,"r")==NULL) 
		sprintf(filename,"%sflyingmachine.bmp", defaultpath);
	t_main = TX.LoadTexture(filename);

	sprintf(filename,"%sflyingengine.bmp", path);
	if (fopen(filename,"r")==NULL) 
		sprintf(filename,"%sflyingengine.bmp", defaultpath);
	t_afterburner = TX.LoadTexture(filename);
	

	SmokeTimer = 0;
	Engine = false;
}





void CFlyingMachine::Draw(CPlayers* p)
{
	int FacingMP = 1;

	if (sin(HeadingAngle)<0) FacingMP = -1;

	if (Engine)
	{
	PC->SetAlphaMode(true);
	for (int i=0;i<2;i++)
		GF.SpriteDrawEx(t_afterburner, 
					TO_SCX(((p->m_fX-(170*SCALE))))*ZM,
				    TO_SCY(((p->m_fY-(13.0f*SCALE*FacingMP))))*ZM,
					HeadingAngle-(0.5f*D3DX_PI),
					(170.0f*ZM*SCALE),
					13.0f*ZM*SCALE*FacingMP,

					-(ZM*SCALE*(Velocity/MAX_VELOCITY)*(0.5f+rand()%1000/1000.0f)),
					ZM*SCALE*FacingMP*0.6f,

					0,0,64,32,
					rand()%100,255,255,255);

	GF.SpriteDrawEx(t_afterburner, 
					TO_SCX(((p->m_fX-(170*SCALE))))*ZM,
				    TO_SCY(((p->m_fY-(13.0f*SCALE*FacingMP))))*ZM,
					HeadingAngle-(0.5f*D3DX_PI),
					(170.0f*ZM*SCALE),
					13.0f*ZM*SCALE*FacingMP,

					-(ZM*SCALE*(Velocity/MAX_VELOCITY)*(0.5f+rand()%1000/1000.0f))*2,
					ZM*SCALE*FacingMP*0.6f,

					0,32,64,32,
					rand()%100,255,255,255);
	}

	PC->SetAlphaMode(false);
	GF.SpriteDraw(t_main, 
					TO_SCX(((p->m_fX-(178*SCALE))))*ZM,
				    TO_SCY(((p->m_fY-(220.0f*SCALE*FacingMP))))*ZM,
					HeadingAngle-(0.5f*D3DX_PI),
					178.0f*ZM*SCALE,220.0f*ZM*SCALE*FacingMP,
					ZM*SCALE,ZM*SCALE*FacingMP);



	//LD.DrawRect(&p->my_rect);
}

void CFlyingMachine::Movement(CPlayers* p)
{
	Engine=false;
	HeadingAngle = atan2((sin(HeadingAngle)*6+sin(p->m_AimAngle))/7.0f,
						 (cos(HeadingAngle)*6+cos(p->m_AimAngle))/7.0f);

	Velocity = (VECTOR_LENGTH(VelocityVector.x, VelocityVector.y));
	Velocity = fabs(Velocity);
	if (Velocity>MAX_VELOCITY)  Velocity = MAX_VELOCITY;

	float xch = ((VelocityVector.x*0.9)+(Velocity*sin(HeadingAngle)*0.1)) - VelocityVector.x;
	float ych = ((VelocityVector.y*0.9)+(Velocity*cos(HeadingAngle)*0.1)) - VelocityVector.y;


	VelocityVector.x += (xch);
	VelocityVector.y += (ych);

	p->m_fX += VelocityVector.x*GF.tick*0.1f;
	p->m_fY += VelocityVector.y*GF.tick*0.1f;

	// gravity
	VelocityVector.y = VelocityVector.y + 0.001f*GF.tick;

	return;
/*

	float CosVel = cos(HeadingAngle);

	if (CosVel>0) // descending
	{
		Velocity += Gravity*CosVel*0.003f*GF.tick;
		//Velocity += Gravity*0.003f*GF.tick;
	}
	else if (CosVel<0) // ascending
	{
		Velocity += CosVel*CosVel*-0.01f*GF.tick;
	}

	Gravity = Gravity + ((MAX_VELOCITY/2.0f)-fabs(Velocity))*0.0006f*GF.tick;
	if (Gravity<0) Gravity = 0;
	if (Gravity>MAX_VELOCITY) Gravity = MAX_VELOCITY;
	p->m_fY += Gravity;

	//drag
	if (Velocity>0) Velocity -= 0.001*GF.tick;
	if (Velocity<0) Velocity += 0.001*GF.tick;


	if (Velocity>MAX_VELOCITY) Velocity = MAX_VELOCITY;
	if (Velocity<-MAX_VELOCITY) Velocity = -MAX_VELOCITY;
	//if (Velocity<0) Velocity = 0;


	p->m_fX += sin(HeadingAngle)*Velocity*GF.tick*0.1f;
	p->m_fY += cos(HeadingAngle)*Velocity*GF.tick*0.1f;
	p->m_fXVel = sin(HeadingAngle)*Velocity*0.1f;
	p->m_fYVel = cos(HeadingAngle)*Velocity*0.1f;

	p->UpdateMyRect();
	if (LD.Collision(&p->my_rect))
	{
		p->TakeDamage(300,NULL);
	}


	if (&PC->players[PC->MyPlayerID].player == p)
	{
		LD.SetZoom(1.0f-(Velocity/MAX_VELOCITY)*0.5f);
	}

*/
	
}

void CFlyingMachine::Accelerate(CPlayers* p)
{
	if (Velocity<MAX_VELOCITY/2)
	{
		VelocityVector.x = VelocityVector.x + sin(HeadingAngle)*(0.006f*GF.tick);
		VelocityVector.y = VelocityVector.y + cos(HeadingAngle)*(0.006f*GF.tick);
	}

	SmokeTimer = SmokeTimer + GF.tick;

	if (SmokeTimer>25)
	{
		LPParticle lp = LD.AddParticle(PARTICLE_EXPLOSIONBITSMOKE,
					   p->m_fX - sin(HeadingAngle)*230,
					   p->m_fY - cos(HeadingAngle)*230,
					   0,
					   1.0f+(rand()%1000/1000.0f)*3.0f,
					   0,0,0);

		if (lp!=NULL)
		{
			lp->xvel = VelocityVector.x*-0.01f;
			lp->yvel = VelocityVector.y*-0.01f;
		}

		SmokeTimer = fmod(SmokeTimer,30);
	}

	Engine = true;

}

void CFlyingMachine::StartNewLevel(CPlayers* p)
{
	Gravity = 0;
	VelocityVector.x = 0;
	VelocityVector.y = 0;
	SmokeTimer = 0;
}

bool CFlyingMachine::TakeDamage(CPlayers* p, float damage, CPlayers* player)
{
	if (&PC->players[PC->MyPlayerID].player == p)
	{
		PC->gui.TookDamage();
	}
	
	p->DoPainSound();
	p->m_health = p->m_health - damage;

	if ((int)p->m_health<=0)
	{
		p->Die(false);
		LD.MakeExplosion(p->m_fX,p->m_fY, 100, 100);
		LD.DrawExplosion(p->m_fX,p->m_fY, 100);
		return true;
	}
	
	return false;
}

void CFlyingMachine::UpdateMyRect(CPlayers* p)
{
	p->my_rect.left = p->m_fX-50;
	p->my_rect.right = p->m_fX+50;
	p->my_rect.top = p->m_fY-20;
	p->my_rect.bottom = p->m_fY+5;
}