#include "enemy_soldierturret.h"


#define WSCALE 0.6f
#define WOX (23*WSCALE*ZOOM)
#define WOY (45*facing*WSCALE*ZOOM)

#define PCORE enemy_base::pCEnemies->pcore
#define OFFSETX PCORE->landscape.offset_x
#define OFFSETY PCORE->landscape.offset_y
#define ZOOM PCORE->landscape.m_zoom
#define OX ((x-OFFSETX)*(ZOOM))
#define OY ((y-OFFSETY)*(ZOOM))

#define TICK PCORE->gfx.tick




enemy_soldierturret::enemy_soldierturret(void)
{
	health = 200.0f;
	aimangle = 0;
	targetplayer=NULL;
	aimspeed = 0.0005f;
	aimangle	= D3DX_PI/2;
	targetangle = D3DX_PI/2;
	thinkcount = rand()%900000/1000.0f;
	shoottimer=0;
	readytoshoot = false;
	clip=300;

	tarm=NULL;
	thead=NULL;
	tleg=NULL;
	tbody=NULL;
	that=NULL;
	WearingHat=true;


}

enemy_soldierturret::~enemy_soldierturret(void)
{
}

#define SOLDIERSIZE (0.8f)

void enemy_soldierturret::Draw(int layer)
{
	if (layer!=0) return;

	PCORE->SetAlphaMode(false);

	float handpiecex = (sin(aimangle)*10*facing*ZOOM);
	float handpiecey = (cos(aimangle)*10*facing*ZOOM);


	//angle between gun hand, and arm
	float armangle = atan2(handpiecex-(36*facing*ZOOM),(handpiecey)-(-5*ZOOM));

	if (facing>0)armangle = armangle+D3DX_PI;


	PCORE->gfx.SpriteDraw(PCORE->weapons.m_Texture[WEP_M60],(OX-WOX),(OY-WOY),aimangle,WOX,WOY,ZOOM*WSCALE,ZOOM*facing*WSCALE);

	//legs
	PCORE->gfx.SpriteDraw(tleg,OX-(23*facing*ZOOM),OY+(23*ZOOM),-((armangle-(D3DX_PI/2))/3),7*ZOOM*SOLDIERSIZE,0,ZOOM*-facing*SOLDIERSIZE,ZOOM*SOLDIERSIZE);
	PCORE->gfx.SpriteDraw(tleg,OX-(32*facing*ZOOM),OY+(23*ZOOM),((armangle-(D3DX_PI/2))/2),7*ZOOM*SOLDIERSIZE,0,ZOOM*-facing*SOLDIERSIZE,ZOOM*SOLDIERSIZE);

	// body
	PCORE->gfx.SpriteDraw(tbody,OX-(23*facing*ZOOM),OY-(1*ZOOM),-((armangle-(D3DX_PI/2))/6),0,0,ZOOM*-facing,ZOOM);


	//head
	PCORE->gfx.SpriteDraw(thead,OX-(26*facing*ZOOM),OY-(24*ZOOM),((targetangle-(D3DX_PI/2))+(aimangle-(D3DX_PI/2)))/2,16*ZOOM*-facing*SOLDIERSIZE,25*ZOOM*SOLDIERSIZE,ZOOM*-facing*SOLDIERSIZE,ZOOM*SOLDIERSIZE);

	//hat
	if (WearingHat)
	PCORE->gfx.SpriteDraw(that,OX-(26*facing*ZOOM),OY-(29*ZOOM),((targetangle-(D3DX_PI/2))+(aimangle-(D3DX_PI/2)))/2,16*ZOOM*-facing*SOLDIERSIZE,29*ZOOM*SOLDIERSIZE,ZOOM*-facing*SOLDIERSIZE,ZOOM*SOLDIERSIZE);

	//arm
	PCORE->gfx.SpriteDraw(tarm,OX-(handpiecex),OY-(handpiecey)-(5*ZOOM),armangle,SOLDIERSIZE*8*ZOOM,SOLDIERSIZE*8*ZOOM,SOLDIERSIZE*ZOOM,SOLDIERSIZE*ZOOM*-facing);


	//PCORE->gfx.SpriteDraw(tarm,OX,OY,0,6,6,ZOOM*WSCALE,ZOOM*facing*WSCALE);

//	PCORE->gfx.DrawTextEx(OX,OY,255,255,255,"X - soldierturret %f",aimangle);
	enemy_base::pCEnemies->NumEnemiesDrawn++;

	if (PCORE->gfx.InputKeyDown(DIK_C))
	PCORE->landscape.DrawRect(&rect_hit);

//	aimangle = aimangle + 0.005*TICK;
}


void enemy_soldierturret::SetUp(float ix, float iy, float a, float b, char* string1, char* string2)
{
	x=ix;
	y=iy;
	if (a!=0) facing=1; else facing=-1;

	
	this->rect_hit.top = y-29;
	this->rect_hit.left = x+(facing>0?-40:32);
	this->rect_hit.right = rect_hit.left+8;
	this->rect_hit.bottom = rect_hit.top+75;
}

bool enemy_soldierturret::LoadStuff(void)
{
	//tgun = PCORE->textures.LoadTexture("textures/weapons/m60.bmp");
	tarm = PCORE->textures.LoadTexture("textures/enemies/turretsoldier/arm.bmp");
	tbody = PCORE->textures.LoadTexture("textures/enemies/turretsoldier/torso.bmp");
	thead = PCORE->textures.LoadTexture("textures/enemies/turretsoldier/head.bmp");

	int rnum = rand()%3;

	if (rnum==0)		that = PCORE->textures.LoadTexture("textures/enemies/turretsoldier/hat1.bmp");
	else if (rnum==1)	that = PCORE->textures.LoadTexture("textures/enemies/turretsoldier/hat2.bmp");
	else if (rnum==2)	that = PCORE->textures.LoadTexture("textures/enemies/turretsoldier/hat3.bmp");

	tleg = PCORE->textures.LoadTexture("textures/enemies/turretsoldier/leg.bmp");

	s_bulletflesh1 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");
	s_bulletflesh2 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");
	s_bulletflesh3 = PCORE->sound.LoadSound(2,"sound/bullethitflesh1.ogg");

	s_taunt1 = PCORE->sound.LoadSound(3,"sound/enemies/turretsoldier/sighted-1.ogg");
	s_taunt2 = PCORE->sound.LoadSound(3,"sound/enemies/turretsoldier/sighted-2.ogg");
	s_taunt3 = PCORE->sound.LoadSound(3,"sound/enemies/turretsoldier/sighted-3.ogg");
	s_taunt4 = PCORE->sound.LoadSound(3,"sound/enemies/turretsoldier/sighted-4.ogg");

	s_die1 = PCORE->sound.LoadSound(5,"sound/enemies/turretsoldier/death1.ogg");
	s_die2 = PCORE->sound.LoadSound(5,"sound/enemies/turretsoldier/death2.ogg");

	s_hat = PCORE->sound.LoadSound(7,"sound/enemies/turretsoldier/hat.ogg");

	return true;
}

void enemy_soldierturret::Do(void)
{
	float distx=0, disty=0;

	if (reloadtime>0) reloadtime = reloadtime - 0.01*TICK;

	if (readytoshoot)
	{
		Shoot();
	}

	thinkcount = thinkcount - 0.1*TICK;

	//PCORE->gui.WriteToConsole(0,0,0,0,"think: %f",thinkcount);

	if (fabs(targetangle-aimangle)<(0.01f)) 
		{
			aimangle=targetangle;
		}
	if (aimangle>targetangle)
		{
			aimangle=aimangle-(aimspeed*TICK);
		}
	else if (aimangle<targetangle) 
		{
			aimangle=aimangle+(aimspeed*TICK);
		}


	if (thinkcount<=0)
	{
		CPlayers* oldtargetplayer = targetplayer;

		PCORE->landscape.GetNearestPlayer(x,y,&targetplayer,&distx, &disty); 

		if (targetplayer!=NULL)
		{
				targetangle = atan2(x-targetplayer->m_fX,y-(targetplayer->m_fY+32));
			
				if (facing>0) targetangle = targetangle + D3DX_PI;

				if (targetangle>D3DX_PI-0.5f){ targetangle=D3DX_PI/2; readytoshoot = false;targetplayer=NULL; }
				else if (targetangle<0.5f) { targetangle=D3DX_PI/2; readytoshoot = false;targetplayer=NULL; }
				else if (fabs(distx)>1000) {targetangle=D3DX_PI/2; readytoshoot = false;targetplayer=NULL; }
				else if (fabs(disty)>fabs(distx)) {targetangle=D3DX_PI/2; readytoshoot = false;targetplayer=NULL; }
				// if we've spotted a new player - shout at the cunt
				if (oldtargetplayer!=targetplayer && targetplayer!=NULL)
				{
					unsigned int rn=rand()%4;


					if (rn==0)
						PCORE->sound.PlaySnd(s_taunt1,this->x,this->y);
					else if (rn==1)
						PCORE->sound.PlaySnd(s_taunt2,this->x,this->y);
					else if (rn==2)
						PCORE->sound.PlaySnd(s_taunt3,this->x,this->y);
					else if (rn==3)
						PCORE->sound.PlaySnd(s_taunt4,this->x,this->y);

					PCORE->gui.WriteToConsole(0,0,0,0,"rn: %i",rn);
				}
		}


		// if we're shooting, its the job of the shoot routine to determine when to stop.
		if (!readytoshoot && targetplayer!=NULL)
		{
			PCORE->gui.WriteToConsole(0,0,0,0,"facing: (%i) fabs(distx): %f",facing,fabs(distx));
			if (fabs(distx)<=100 || (facing>0 && distx<0) || (facing<0 && distx>0) )
			{
				readytoshoot = false;
			}
			else if (fabs(distx)<1000)
			{
				readytoshoot = false;
				//if (facing>0) targetangle = targetangle + D3DX_PI;
				if (targetangle>D3DX_PI-0.5f)
					{ targetangle=D3DX_PI/2; readytoshoot = false; }
				else
				if (targetangle<0.5f) 
					{targetangle=D3DX_PI/2;readytoshoot = false;}
				else
				if (fabs(targetangle-aimangle)<0.02)
				{
					readytoshoot = true;
					// attack!
				}

			}
			else
			readytoshoot = false;

		}

		thinkcount = 100;//+rand()%100;
	}

}

void enemy_soldierturret::Shoot(void)
{
	if (!readytoshoot) return;

	if (targetplayer==NULL) 
	{
		readytoshoot=false;
		return;
	}
	if (reloadtime>0)
		return;

	shoottimer = shoottimer - 0.01*TICK;

	if (shoottimer<=0)
	{

		float sx = x + sin(aimangle+(facing>0?0:D3DX_PI))*PCORE->weapons.m_GunAttributes[WEP_M60].MuzzleTip;
		float sy = y + cos(aimangle+(facing>0?0:D3DX_PI))*PCORE->weapons.m_GunAttributes[WEP_M60].MuzzleTip;

		LPParticle prt = PCORE->landscape.BulletShoot(WEP_MINIGUN,sx,sy,aimangle+(facing>0?0:D3DX_PI),1);
		if (prt!=NULL)
		{
			prt->damage = prt->damage*0.50;
		}

		PCORE->weapons.DrawMuzzle(sx,sy,aimangle+(facing>0?0:D3DX_PI), WEP_M60);
		PCORE->weapons.PlayShootSound(sx,sy, WEP_M60);

		// recoil
		aimangle=aimangle+(0.1*facing*(rand()%1000/1000.0f));
		if (fabs(targetangle-aimangle)>0.3)
		{
			readytoshoot = false;
		}

		clip--;

		if (clip<=0) 
		{
			PCORE->weapons.PlayReloadSound(sx,sy, WEP_M60);
			reloadtime = 100;
			clip = 100;
		}
		
		shoottimer = PCORE->weapons.m_GunAttributes[WEP_M60].TimeBetweenShots;//*2.0f;

	}
}

bool enemy_soldierturret::BulletHit(float* ix, float* iy)
{
	if (*ix<rect_hit.left) return false;
	if (*ix>rect_hit.right) return false;
	if (*iy<rect_hit.top) return false;
	if (*iy>rect_hit.bottom) return false;

	return true;
}


#define RANDMULTIPLIER (rand()%10000/10000.0f)

bool enemy_soldierturret::BulletHitRect(GRECT* r)
{
	if (r->bottom < rect_hit.top || rect_hit.bottom < r->top) return false;    
	if (r->right < rect_hit.left || rect_hit.right < r->left) return false;
	return true;
}

void enemy_soldierturret::TakeDamage(float damage, float angle, float x, float y, bool Splash, LPParticle bullet=NULL)
{
	//damage = damage*100;

	float oldhealth= health;
	float rnum=1;
	health=health-damage;
	LPParticle tp=NULL;

	// shoot off helmet
	if ((y<this->y-16|| Splash || health<=0) && WearingHat) 
	{

		tp = PCORE->landscape.AddParticle(PARTICLE_GENERIC_32X16,12,12);
		if (tp!=NULL)
		{
			//PCORE->sound.PlaySnd(s_hat,x,y);

			tp->x = this->x-(36*facing);
			tp->y = this->y-(29)+(8*SOLDIERSIZE);
			tp->rot = ((targetangle-(D3DX_PI/2))+(aimangle-(D3DX_PI/2)))/2;
			tp->textureoverride = that;
			tp->xvel = sin(angle)*(damage/40);
			tp->yvel = cos(angle)*(damage/40);
			tp->xscale = -facing*SOLDIERSIZE;
			tp->yscale = -facing*SOLDIERSIZE;
		}


		PCORE->sound.PlaySnd(s_hat,x,y,0.2f);

		WearingHat=false;
	}


	// sounds of bullet hitting flesh
	unsigned int rinum=rand()%3;
	if (rinum==0) PCORE->sound.PlaySnd(s_bulletflesh1,x,y,0.4);
	else if (rinum==1) PCORE->sound.PlaySnd(s_bulletflesh2,x,y,0.4);
	else if (rinum==2) PCORE->sound.PlaySnd(s_bulletflesh3,x,y,0.4);




	// die
	if (health<=0 && oldhealth>0)
	{
		if (bullet!=NULL)
		{
			if (bullet->pplayer!=NULL)
			{
				bullet->pplayer->CountKill();
				bullet->pplayer->AddBulletTime(SKILL_BT_TURRETSOLIDER);
			}
		}
		if (rand()%2==0)
			PCORE->sound.PlaySnd(s_die1,x,y);
		else
			PCORE->sound.PlaySnd(s_die2,x,y);

		float handpiecex = (sin(aimangle)*10*facing*ZOOM);
		float handpiecey = (cos(aimangle)*10*facing*ZOOM);
		float armangle = atan2(handpiecex-(36*facing*ZOOM),(handpiecey)-(-5*ZOOM));
		if (facing>0)armangle = armangle+D3DX_PI;


		// head
		tp = PCORE->landscape.AddParticle(PARTICLE_GENERIC_32X32,12,12);
		if (tp!=NULL)
		{
			tp->x = this->x-(34*facing);
			tp->y = this->y-(17);
			tp->rot = ((targetangle-(D3DX_PI/2))+(aimangle-(D3DX_PI/2)))/2;
			tp->textureoverride = thead;
			rnum = RANDMULTIPLIER;
			tp->xvel = sin(angle)*(damage/70)*rnum;
			tp->yvel = cos(angle)*(damage/70)*rnum;
			tp->xscale = -facing*SOLDIERSIZE;
			tp->yscale = SOLDIERSIZE;
		}
		// leg
		tp = PCORE->landscape.AddParticle(PARTICLE_GENERIC_16X32,12,12);
		if (tp!=NULL)
		{
			// 23 32
			tp->x = this->x-((23+8.5)*facing);
			tp->y = this->y+(23)+16;
			tp->rot = ((armangle-(D3DX_PI/2))/5);
			tp->textureoverride = tleg;
			rnum = RANDMULTIPLIER;
			tp->xvel = sin(angle)*(damage/90)*rnum;
			tp->yvel = cos(angle)*(damage/90)*rnum;
			tp->xscale = -facing*SOLDIERSIZE;
			tp->yscale = SOLDIERSIZE;
		}
		// leg2
		tp = PCORE->landscape.AddParticle(PARTICLE_GENERIC_16X32,12,12);
		if (tp!=NULL)
		{
			// 23 32
			tp->x = this->x-((32+8.5f)*facing);
			tp->y = this->y+23+16;
			tp->rot = -((armangle-(D3DX_PI/2))/5);
			tp->textureoverride = tleg;
			rnum = RANDMULTIPLIER;
			tp->xvel = sin(angle)*(damage/90)*rnum;
			tp->yvel = cos(angle)*(damage/90)*rnum;
			tp->xscale = -facing*SOLDIERSIZE;
			tp->yscale = SOLDIERSIZE;
		}

		// loads of blood spluters
		for (int i=0;i<10;i++)
		{
			PCORE->landscape.GenericBloodSpurt(rect_hit.left+rand()%abs(rect_hit.left-rect_hit.right), rect_hit.top+rand()%(abs(rect_hit.top-rect_hit.bottom)), rand()%10000/1000.0f, 0.8f);
		}
		//die
		PCORE->landscape.UnAttachAttachmentsMonster(this);
	}

	if (!Splash)
	{
		PCORE->landscape.GenericBloodSpurt(x, y, angle, 1.0f);
	}
	else
	{
		for (int i=0;i<5;i++)
		{
			PCORE->landscape.GenericBloodSpurt(rect_hit.left+rand()%abs(rect_hit.left-rect_hit.right), rect_hit.top+rand()%(abs(rect_hit.top-rect_hit.bottom)), angle, 0.5f);
		}
	}

}

bool enemy_soldierturret::SplashDamageEnemy(float x, float y, float size, float damage, CPlayers* attacker)
{
	if (x<this->x-size) return false;
	if (x>this->x+size) return false;
	if (y-20<this->y-size) return false;
	if (y-20>this->y+size) return false;

	float distance = sqrtf( (this->x-x)*(this->x-x) + (this->y-y-20)*(this->y-y-20));

/*	
	for (float dis=0;dis<distance;dis=dis+10)
	{
		enemy_base::pCEnemies->pcore->landscape.AddParticle(0,x+sin(atan2(this->x-x, this->y-(y-20)))*dis,y+cos(atan2(this->x-x, this->y-(y-20)))*dis);
	}
*/

	if (distance>size) return false;

	if (this->health>=0)
	{
		TakeDamage(((size-distance)/size)*damage, atan2(this->x-x, this->y-y-20), x, y, true);

		if (this->health<0 && attacker!=NULL)
		{
			attacker->CountKill();
			attacker->AddBulletTime(SKILL_BT_TURRETSOLIDER);
		}
	}	
	
	return true;
}