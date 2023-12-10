
#include "Players.h"
#include "Weapons.h"
#include "Core.h"
#include "garryswrappers/garrysgraphicswrapper.h"


// bottom = y+38
// top = y-30

//#define MODELSIZE 0.5

//#define FEETPOS	38*m_scale
//#define HEADPOS 20*m_scale

#define PLAYERWIDTH 12.0f

#define GRAVITY 0.0017f*pcore->gravity
#define MAXFALLSPEED 900.0f
#define AIRSPEED 0.9f
#define AIRFRICTION 0.1f

CPlayers::CPlayers()
: m_LastMouthoff(0)
, OnLadder(false)
, bDead(false)
, dietimer(0)
{
	m_plutonium = 0;
	this->m_fX = 300.0f;
	m_health = 100.0f;
	this->m_fY = 300.0f;
	this->m_fWalkCount = 0.0f;
	this->m_fXVel = 0.0f;
	this->m_fYVel = 0.0f;
//	this->m_landscape = NULL;
//	this->m_pGfx = NULL;
	this->m_BodyTexture = NULL;
	this->m_HeadTexture = NULL;
	this->m_HitBoxTexture = NULL;
	this->m_AimAngle = 1.0f;
	this->m_iWeapon = 1;
	this->m_iState=PLAYERSTATE_FALLING;
	bSpawned=false;

	m_JumpStamina = 1;

	m_bShowHitboxes=false;

	m_scale = 1;

	//m_feetpos = 1;//40*m_scale;
	m_headpos = 20*m_scale;

	m_ShootCycle=0; // 0 ready to fire
	this->m_bEnabled = false;
	bDuck=false;

	AimLength=1;

	sprintf(Player_Name,"Player");

	for (int i=0;i<MAX_WEAPONS_HOLDING;i++)
	{
		WeaponCarrying[i] = -1;
	}

	weapons_inPump=false;

	FlyingPlane = true;

//	WeaponCarrying[0] = 1;
}

void CPlayers::Scale(float scale)
{

	m_fX = m_fX/m_scale;// * (scale/m_scale)) ;
	m_fY = m_fY/m_scale;// * (scale/m_scale));
	m_fX = m_fX*scale;// * (scale/m_scale)) ;
	m_fY = m_fY*scale;// * (scale/m_scale));

	m_fY =m_fY+1*(scale/m_scale);

	m_fY = (((int)((m_fY+m_feetpos)/((int)this->pcore->landscape.m_tilesize)))*((int)this->pcore->landscape.m_tilesize))-(m_feetpos+1);

	m_scale = scale;

	//m_feetpos = 35*m_scale;
	m_headpos = 20*m_scale;


}

void CPlayers::CreatePlayer()
{
	if (m_HitBoxTexture==NULL)
		D3DXCreateTextureFromFile( pcore->gfx.m_pd3dDevice,"textures/p.bmp", &m_HitBoxTexture);

	this->m_bEnabled = true;
	m_reloadTime=0;
	m_bReloading=false;
}

void CPlayers::SetXY(float x, float y)
{
	this->m_fX = x;
	this->m_fY = y;
}


void CPlayers::Draw()
{
	if (bUnducking) UnDuck();
	bUnducking=true;
	if (!bFired) bFireDown = false;

	if (!bSpawned) return;
	if (bDead) return;
	//if (pcore->doStartLevel) return;
	if (FlyingPlane) return MyPlane.Draw(this);

	pcore->SetAlphaMode(false);




	float x, y, radhead;

	x = m_fX-pcore->landscape.offset_x;
	y = m_fY-pcore->landscape.offset_y;

	float zoom = this->pcore->landscape.m_zoom;


	// head rotation
	radhead = m_AimAngle;	
	if (radhead<0) radhead=radhead+D3DX_PI;
	radhead = (radhead-(D3DX_PI/2))/1.5f;


	float duckoffset=0;

	if (bDuck) duckoffset=15;


	if (m_fYVel==0)
	{
		if (m_fXVel!=0)
		{
			

			bool under=false;
			if (m_fWalkCount<2) under = true;
			m_fWalkCount=m_fWalkCount+((m_fXVel<0?-m_fXVel:m_fXVel)*(0.03f*pcore->gfx.tick));
			if (under== true && m_fWalkCount>=2)
			{
				pcore->landscape.MakeExplosion(m_fX,m_fY+this->m_feetpos,10,20);

				if (bDuck)
					pcore->sound.PlaySnd(footstep2,m_fX,m_fY,0.0f,0.0f,0.0f,0.2f);
				else
					pcore->sound.PlaySnd(footstep2,m_fX,m_fY);
			}
			
		}
		else
		{
			m_fWalkCount = 0.9f;
		}
	}
	else
	{
		m_fWalkCount=3;
	}

	if (m_fWalkCount>=4)
	{
		pcore->landscape.MakeExplosion(m_fX,m_fY+this->m_feetpos,10,20);

		if (bDuck)
			pcore->sound.PlaySnd(footstep1,m_fX,m_fY,0.0f,0.0f,0.0f,0.2f);
		else
		pcore->sound.PlaySnd(footstep1,m_fX,m_fY);
		m_fWalkCount=0;
	}

	// only draw them if they're on the screen
	if (x>-32 &&
		y>-32)
	{

	
		// set facing direction
		if (m_AimAngle>0) m_Direction=1;else m_Direction=0;

		if (m_LegsTexture)
		{
			pcore->gfx.SpriteDrawEx(m_LegsTexture, (float)((x-15.5f)+(m_Direction==0?32.0f*m_scale:0)+(m_XShootShake/2))*zoom,
												(float)(y+12+(m_YShootShake/2)+(duckoffset/2))*zoom, 
												
												0,0,0,

												(m_Direction==0?-m_scale*this->pcore->landscape.m_zoom:m_scale*zoom)/2, 
												(m_scale*zoom)/2,

												(((int)m_fWalkCount%4)*64),
												(((int)m_fWalkCount/4)*64)+duckoffset,
												
												64,64-duckoffset,
												this->pcore->landscape.m_globalaplha,255,255,255);
		}

		if (m_BodyTexture)
		{
			pcore->gfx.SpriteDrawEx(m_BodyTexture, (float)((x-(15.5*m_scale))+(m_Direction==0?32*m_scale:0)+m_XShootShake)*zoom,
											  (float)(y-(15.5*m_scale)+m_YShootShake+duckoffset)*zoom,
											  0,0,0,
											  
											  (m_Direction==0?-m_scale*zoom:m_scale*zoom)/2, (m_scale*zoom)/2,
											  
											  0,0,64,64,
											  this->pcore->landscape.m_globalaplha,255,255,255
											  );
		}

		if (m_HeadTexture)
		{
			pcore->gfx.SpriteDrawEx(m_HeadTexture, (float)((x-(15.5*m_scale))+(m_Direction==0?32*m_scale:0)+m_XShootShake)*zoom,
											  (float)(y-32*m_scale+m_YShootShake+duckoffset)*zoom, 
											  
											  radhead, 
											  ((17*m_scale)-(m_Direction==0?32*m_scale:0))*zoom,
											  28*m_scale*zoom, 
											  
											  (m_Direction==0?-m_scale*zoom:m_scale*zoom)/2, (m_scale*zoom)/2,
											  
											  0,0,64,64,
											  this->pcore->landscape.m_globalaplha,255,255,255);

		}

		// weapon
		if (m_iWeapon==WEP_CHAINSAW)
		{
			pcore->weapons.PlayIdleSound(m_iWeapon, m_fX, m_fY);
		}
		pcore->weapons.Draw(m_iWeapon,this,x+m_XShootShake,y+m_YShootShake+duckoffset, zoom);
		



		if (m_ArmTexture && !pcore->weapons.m_GunAttributes[m_iWeapon].Melee)
		{
			pcore->gfx.SpriteDrawEx(m_ArmTexture, (x-(8*m_scale)+(m_Direction==0?(16*m_scale):0)+m_XShootShake)*zoom,
											 (y-2+m_YShootShake+duckoffset)*zoom,
											 
											 m_AimAngle,
											 ((8*m_scale)-(m_Direction==0?(16*m_scale):0))*zoom,
											 2*zoom,
											 
											 (m_Direction==0?-m_scale*zoom:m_scale*zoom)/2,(m_scale*zoom)/2,
											  0,0,16,64,
											 this->pcore->landscape.m_globalaplha,255,255,255);
		}
	}

//	pcore->gfx.DrawTextEx((x-25+(m_Direction==0?(16*m_scale):0)+m_XShootShake)*zoom,
//											 (y-80)*zoom,
//											 255,255,255,"%s\n%s",Player_Name, pcore->weapons.m_GunAttributes[this->m_iWeapon].name);

	DrawShoot();


}

void CPlayers::Jump()
{


	if (m_JumpStamina<1) return;
	if (m_iState==PLAYERSTATE_WALKING || OnLadder)
	{
		m_fYVel=0; 
		m_JumpStamina=10.0f;
	}
	else
	{
		m_JumpFPS = m_JumpFPS + pcore->gfx.tick;
		if (m_JumpFPS<40) return;
	}

	if (m_JumpStamina==10.0f)
	{
		if (bDuck)
			m_fYVel = m_fYVel - (0.2f);
		else 
			m_fYVel = m_fYVel - (0.4f);

		m_JumpFPS = 40.0f;
	}
	else
	{
		m_fYVel = m_fYVel - (m_JumpStamina*0.02f);
	}
	
	m_JumpStamina = m_JumpStamina*0.7f;
	m_iState = PLAYERSTATE_FALLING;
	OnLadder=false;
	m_JumpFPS = m_JumpFPS - 40.0f;
}

#define DUCKCLAUSE (bDuck?0.6:1)

void CPlayers::WalkLeft()
{
	GRECT trect;
	this->UpdateMyRect();
	CopyGRect(&trect, &my_rect);
	trect.left = trect.left-1;
	if (pcore->landscape.Collision(&trect))
	{
		return;
	}

	if (OnLadder)
	{
		m_fXVel = -0.1f;
		m_bMoving = true;
		return;
	}


	if (m_iState==PLAYERSTATE_WALKING)
	{
		m_fXVel = m_fXVel - (0.002*pcore->gfx.tick);
		m_bMoving = true;
	}
	else
	{
		m_fXVel = m_fXVel - (0.001*pcore->gfx.tick);
		m_bMoving = true;
	}
}

void CPlayers::WalkRight()
{
	GRECT trect;
	this->UpdateMyRect();
	CopyGRect(&trect, &my_rect);
	trect.right = trect.right+1;
	if (pcore->landscape.Collision(&trect))
	{
		return;
	}


	if (OnLadder)
	{
		m_fXVel = 0.1f;
		m_bMoving = true;
		return;
	}

	if (m_iState==PLAYERSTATE_WALKING)
	{
		m_fXVel = m_fXVel + (0.002*pcore->gfx.tick);
		m_bMoving = true;
	}
	else
	{
		m_fXVel = m_fXVel + 0.001*pcore->gfx.tick;
		m_bMoving = true;
	}
}

void CPlayers::Do()
{
	
	if (!bSpawned) return;
	if (pcore->doStartLevel) return;


	if (bDead)
	{
		dietimer = dietimer + pcore->gfx.RealTick*0.1;

		pcore->timemultiplier =  0.5 - dietimer/200.0f;

		if (dietimer>250) pcore->timemultiplier =  dietimer/500.0f;

		if (pcore->timemultiplier<0.02) pcore->timemultiplier=0.02;
		if (pcore->timemultiplier>1.0f) pcore->timemultiplier = 1.0f;

		if (pcore->landscape.ScreenFade.alpha>=255)
		{
			if (pcore->GlobalStats.Lives<=0)
			{
				pcore->GameOver();
			}
			else
			{
				pcore->RestartAfterDead();
			}
		}

		return;
	}

	if (bFired==false)
	{
		this->m_SpinUpTimer=0.0f;
	}
	bFired=false;
	DoMovements();

	if (m_LastMouthoff>0) m_LastMouthoff = m_LastMouthoff - pcore->gfx.tick;

	DoReload();

	if (m_JumpStamina<1.0f && (m_iState==PLAYERSTATE_WALKING || OnLadder)) 
	{
		m_JumpStamina = m_JumpStamina + (0.1*pcore->gfx.tick); // jump recover speed
	}

	m_timeSinceShot = m_timeSinceShot + 0.01*pcore->gfx.tick;
	pcore->ents.PlayerTrigger(this,&my_rect);

	

	DoPickups();
	DoBulletTime();
}





void CPlayers::LoadBodyTexture(LPCTSTR filename)
{
	m_BodyTexture = pcore->textures.LoadTexture((char*)filename);
}


void CPlayers::LoadHeadTexture(LPCTSTR filename)
{
	m_HeadTexture = pcore->textures.LoadTexture((char*)filename);
}

void CPlayers::LoadLegsTexture(LPCTSTR filename)
{
	m_LegsTexture = pcore->textures.LoadTexture((char*)filename);
}

void CPlayers::LoadArmTexture(LPCTSTR filename)
{
	m_ArmTexture = pcore->textures.LoadTexture((char*)filename);
}

void CPlayers::DrawShoot()
{
	if (m_timeSinceShot>0.4)
	{
		m_XShootShake = 0;
		m_YShootShake = 0;

	}

}

void CPlayers::Shoot()
{
	bFired=true;
	if (bDead)
	{
		if (dietimer>50)
		{
			if (pcore->landscape.ScreenFade.alpha!=255)
			{
				pcore->landscape.ScreenFade.speed = 0.3f;
			}
		}
		
		return;
	}

	if (pcore->bPaused || pcore->doingLevelEnd || pcore->doStartLevel) return;

	float randomangle;

	// if we haven't got any bullets in teh gun - try to reload
	if (m_BulletsInClip[m_iWeapon]<=0 && pcore->weapons.m_GunAttributes[this->m_iWeapon].AmmoType!=AMMO_NONE)
	{
		Reload();
		return;
	}

	if ((pcore->weapons.m_GunAttributes[this->m_iWeapon].ShootType==ST_PISTOL ||
		pcore->weapons.m_GunAttributes[this->m_iWeapon].ShootType==ST_SHOTGUN) && 
		bIsPlayer)
	{
		if (bFireDown==true) return;
	}

	bFireDown=true;

	if (m_SpinUpTimer<this->pcore->weapons.m_GunAttributes[this->m_iWeapon].SpinupTime)
	{
		if (m_SpinUpTimer==0)
		{
			pcore->weapons.PlaySpinSound(this->m_iWeapon, this->m_fX, this->m_fY);
		}
		m_SpinUpTimer = m_SpinUpTimer + pcore->gfx.tick;
	}

	if (
		(m_timeSinceShot>this->pcore->weapons.m_GunAttributes[this->m_iWeapon].TimeBetweenShots
		&&
		m_SpinUpTimer>=this->pcore->weapons.m_GunAttributes[this->m_iWeapon].SpinupTime
		)
		|| !bIsPlayer)
	{
		if (bDuck) m_fY=m_fY+15;
		if (m_bReloading) return;//m_bReloading=false;

		if (this->weapons_inPump && this->pcore->weapons.m_GunAttributes[this->m_iWeapon].Pumper)
		{
			pcore->weapons.PlayPumpSound(m_fX,m_fY,this->m_iWeapon);
			weapons_inPump=false;
			m_timeSinceShot=0;
			// spawn brass (since that's what we're doing when we pump)
			pcore->weapons.SpawnShell(this);
			return;
		}

		if (this->pcore->weapons.m_GunAttributes[this->m_iWeapon].Pumper)
		{
			this->weapons_inPump=true;
		}

		if (bIsPlayer && m_iWeapon!=WEP_CHAINSAW)
		{
			pcore->LevelStats.ShotsFired++;
//			pcore->netcode.ShootWeapon(this->m_iWeapon,this->m_fX, this->m_fY, this->m_AimAngle);
		}

		pcore->weapons.PlayShootSound(this);

		

		//todo: dont shoot if the end of teh gun is in an object

		m_ShootCycle=1; // this is bollocks now

		if (this->pcore->weapons.m_GunAttributes[this->m_iWeapon].ShootType==ST_PISTOL ||
			this->pcore->weapons.m_GunAttributes[this->m_iWeapon].ShootType==ST_AUTOMATIC)
		{
			pcore->landscape.BulletShoot(this);
		}
		else if (this->pcore->weapons.m_GunAttributes[this->m_iWeapon].ShootType==ST_SHOTGUN)
		{
			// shoot x number of bullets
			for (unsigned int i=0;
				i<this->pcore->weapons.m_GunAttributes[this->m_iWeapon].Shotgun_BuckshotNumber;
				i++)
			{
				pcore->landscape.BulletShoot(this);
			}
		}

		if (bIsPlayer && pcore->weapons.m_GunAttributes[this->m_iWeapon].AmmoType!=AMMO_NONE)
		m_BulletsInClip[m_iWeapon]--;

		m_LastBulletHitPoint.x  = m_AimPoint.x;
		m_LastBulletHitPoint.y  = m_AimPoint.y;

		m_LastX = m_fX;
		m_LastY = m_fY;
		m_LastAimAngle = m_AimAngle;


		m_XBrightSparkScale = 10+rand()%20;
		m_YBrightSparkScale = 10+rand()%20;
		
		pcore->weapons.DrawMuzzle(this);

		if (!pcore->weapons.m_GunAttributes[this->m_iWeapon].Pumper && pcore->weapons.m_GunAttributes[this->m_iWeapon].ReloadDropClip!=1001)
			pcore->weapons.SpawnShell(this);

		m_timeSinceShot=0;
		
		if (m_BulletsInClip[m_iWeapon]<=0)
		{
			Reload();
		}

		if (bDuck) m_fY=m_fY-15;

		if (pcore->weapons.m_GunAttributes[this->m_iWeapon].ScreenShake>0)
		{
			pcore->landscape.ScreenShake(pcore->weapons.m_GunAttributes[this->m_iWeapon].ScreenShake*pcore->RecoilMultiplier,pcore->weapons.m_GunAttributes[this->m_iWeapon].ScreenShake*pcore->RecoilMultiplier);
		}

	}


}

void CPlayers::Spawn(bool firstspawn)
{
	bDead = false;
	inBulletTime = false;
	BulletTime = 100;

	float inx, iny;

	pcore->landscape.GetSpawnPoint(&inx,&iny);
	m_fX = inx;	m_fY = iny;
	pcore->landscape.FocusCameraOnPlayer();



	m_health = 100.0f;
	this->m_fXVel = 0.0f;
	this->m_fYVel = 0.0f;
	this->m_fWalkCount = 0.0f;
	m_JumpStamina = 1.0f;
	m_scale = 1;
	m_feetpos = 37*m_scale;
	m_headpos = 22*m_scale;
	m_ShootCycle=0;

	if (firstspawn)
	{
		this->m_iWeapon = 1;
		this->m_iState=PLAYERSTATE_FALLING;
		
		for (int i=0;i<NUM_WEAPONS;i++)
		{
			m_BulletsInClip[i] =  pcore->weapons.m_GunAttributes[i].ClipSize;
		}

		StripWeaponsAndAmmo();
		GiveWeapon(1); // spawn with a pistol
		GiveAmmo(AMMO_9MM, 250);
		m_plutonium = 0;
	}

	pcore->timemultiplier = 1.0f;
	this->MyPlane.StartNewLevel(this);

	bSpawned=true;
}

void CPlayers::UpdatePhysicalPresence(unsigned int num)
{
	/*
	dMatrix3 rot;
	dRSetIdentity(rot);

	dBodySetRotation(pcore->landscape.phyobjplayerp[num].body, rot);
	dBodySetPosition (pcore->landscape.phyobjplayerp[num].body, m_fX,m_fY,1);
	dBodySetLinearVel(pcore->landscape.phyobjplayerp[num].body, 0,0,0);
	dBodySetAngularVel(pcore->landscape.phyobjplayerp[num].body, 0,0,0);
	*/
}

// when reload key is pressed
void CPlayers::Reload(void)
{
	if (m_timeSinceShot<=this->pcore->weapons.m_GunAttributes[this->m_iWeapon].TimeBetweenShots) return; // make them wait a shoot cycle
	if (m_bReloading) return; // we're already reloading
	if (m_SpareAmmo[(pcore->weapons.m_GunAttributes[this->m_iWeapon].AmmoType)]<=0) return; // no ammo!
	if (m_BulletsInClip[m_iWeapon]>=pcore->weapons.m_GunAttributes[this->m_iWeapon].ClipSize) return; // full!

	// spawns the clip particle
//	if (m_BulletsInClip[m_iWeapon]==1)
	{
		if (pcore->weapons.m_GunAttributes[this->m_iWeapon].ReloadDropClip!=0 && 
			pcore->weapons.m_GunAttributes[this->m_iWeapon].ReloadDropClip!=1001)
		{
			this->pcore->landscape.AddParticle(pcore->weapons.m_GunAttributes[this->m_iWeapon].ReloadDropClip,m_fX+(sin(m_AimAngle)*(this->pcore->weapons.m_GunAttributes[this->m_iWeapon].MuzzleTip/2)),
									m_fY+(cos(m_AimAngle)*(this->pcore->weapons.m_GunAttributes[this->m_iWeapon].MuzzleTip/2)));
		}

		if (pcore->weapons.m_GunAttributes[this->m_iWeapon].ReloadDropClip==1001)
		{
			LPParticle ppointer;


			for (int i=0;i<pcore->weapons.m_GunAttributes[this->m_iWeapon].ClipSize;i++)
			{
					ppointer = pcore->landscape.AddParticle(pcore->weapons.m_GunAttributes[m_iWeapon].shelltype, 
						m_fX+(sin(m_LastAimAngle)*(pcore->weapons.m_GunAttributes[m_iWeapon].MuzzleTip/2)),
						m_fY+(cos(m_LastAimAngle)*(pcore->weapons.m_GunAttributes[m_iWeapon].MuzzleTip/2)));

					if (ppointer!=NULL)
					{
						ppointer->xvel = fabs(ppointer->xvel/2.0f)*(this->m_Direction==0?1.0f:-1.0f);
						ppointer->yvel = ppointer->yvel/2.0f;
					}
			}
		}
	}

	
	
	m_reloadTime = pcore->weapons.m_GunAttributes[this->m_iWeapon].ReloadTime;
	m_bReloading = true;

	// shotgun reloads bullet by bullet
	if (pcore->weapons.m_GunAttributes[this->m_iWeapon].AmmoType!=AMMO_SHOTGUN)
		pcore->weapons.PlayReloadSound(this);
}

//if we're doing a reload, reload
void CPlayers::DoReload(void)
{

	if (!m_bReloading) return;

	m_reloadTime = m_reloadTime - (0.01*pcore->gfx.tick);

	if (m_reloadTime<0)
	{
		m_bReloading = false;
		m_reloadTime = 0;


		// if space left in clip is smaller than a reload size
		if ((pcore->weapons.m_GunAttributes[this->m_iWeapon].ClipSize-m_BulletsInClip[m_iWeapon])<pcore->weapons.m_GunAttributes[this->m_iWeapon].ReloadSize)
		{
			int bulletstoadd=0;
			// bullet add equals space left
			bulletstoadd = pcore->weapons.m_GunAttributes[this->m_iWeapon].ClipSize-m_BulletsInClip[m_iWeapon];
			// if we're trying to put in more bullets than we have, cap it
			if (bulletstoadd>m_SpareAmmo[pcore->weapons.m_GunAttributes[m_iWeapon].AmmoType])
					bulletstoadd = m_SpareAmmo[pcore->weapons.m_GunAttributes[m_iWeapon].AmmoType];

			m_BulletsInClip[m_iWeapon] += bulletstoadd;
			m_SpareAmmo[pcore->weapons.m_GunAttributes[m_iWeapon].AmmoType] = m_SpareAmmo[pcore->weapons.m_GunAttributes[m_iWeapon].AmmoType] - bulletstoadd;

			if (pcore->weapons.m_GunAttributes[this->m_iWeapon].AmmoType==AMMO_SHOTGUN)
				pcore->weapons.PlayReloadSound(this);

		}
		// if we have less than a clip, put the remainder in
		else if (m_SpareAmmo[pcore->weapons.m_GunAttributes[m_iWeapon].AmmoType] < pcore->weapons.m_GunAttributes[this->m_iWeapon].ReloadSize)
		{
			m_BulletsInClip[m_iWeapon] += m_SpareAmmo[pcore->weapons.m_GunAttributes[m_iWeapon].AmmoType];
			m_SpareAmmo[pcore->weapons.m_GunAttributes[m_iWeapon].AmmoType] = 0;

			if (pcore->weapons.m_GunAttributes[this->m_iWeapon].AmmoType==AMMO_SHOTGUN)
				pcore->weapons.PlayReloadSound(this);
		}
		else
		{
			if (pcore->weapons.m_GunAttributes[this->m_iWeapon].AmmoType==AMMO_SHOTGUN)
				pcore->weapons.PlayReloadSound(this);

			m_BulletsInClip[m_iWeapon] += pcore->weapons.m_GunAttributes[this->m_iWeapon].ReloadSize;
			m_SpareAmmo[pcore->weapons.m_GunAttributes[m_iWeapon].AmmoType] = m_SpareAmmo[pcore->weapons.m_GunAttributes[m_iWeapon].AmmoType] - pcore->weapons.m_GunAttributes[this->m_iWeapon].ReloadSize;
		}

		// if we are adding bullets in chunks
 		if (m_BulletsInClip[m_iWeapon]<pcore->weapons.m_GunAttributes[this->m_iWeapon].ClipSize)
		{
			Reload();
		}

	}

	
}    

void CPlayers::ChangeWeapon(int wepid)
{
	this->m_iWeapon = wepid;
//	pcore->netcode.UpdateMyWeapon(wepid);
}

void CPlayers::Free(void)
{
	if (this->m_BodyTexture!=NULL) this->m_BodyTexture->Release();
	if (this->m_ArmTexture!=NULL) this->m_ArmTexture->Release();
	if (this->m_HeadTexture!=NULL) this->m_HeadTexture->Release();
	if (this->m_LegsTexture!=NULL) this->m_LegsTexture->Release();
	if (this->m_HitBoxTexture!=NULL) this->m_HitBoxTexture->Release();


	this->m_bEnabled = false;

}

void CPlayers::LoadPlayerModel(char* model)
{
	char path[255];
	char defaultpath[255];
	char usepath[255];
	char filename[255];

	sprintf(path,"players\\%s\\",model);
	sprintf(defaultpath,"players\\player\\");

	sprintf(filename,"%sbody.bmp", path);
	if (fopen(filename,"r")==NULL) sprintf(filename,"%sbody.bmp", defaultpath);
	LoadBodyTexture(filename);

	sprintf(filename,"%shead.bmp", path);
	if (fopen(filename,"r")==NULL) sprintf(filename,"%shead.bmp", defaultpath);
	LoadHeadTexture(filename);

	sprintf(filename,"%slegs.bmp", path);
	if (fopen(filename,"r")==NULL) sprintf(filename,"%slegs.bmp", defaultpath);
	LoadLegsTexture(filename);

	sprintf(filename,"%sarm.bmp", path);
	if (fopen(filename,"r")==NULL) sprintf(filename,"%sarm.bmp", defaultpath);
	LoadArmTexture(filename);


	// load custom sounds
	sprintf(filename,"%sfootstep1.ogg", path);
	if (fopen(filename,"r")==NULL) sprintf(filename,"%sfootstep1.ogg", defaultpath);
	footstep1 = pcore->sound.LoadSound(3,filename);

	sprintf(filename,"%sfootstep2.ogg", path);
	if (fopen(filename,"r")==NULL) sprintf(filename,"%sfootstep2.ogg", defaultpath);
	footstep2 = pcore->sound.LoadSound(3,filename);

	s_ladder1 = pcore->sound.LoadSound(3,"sound/ladder1.ogg");
	s_ladder2 = pcore->sound.LoadSound(3,"sound/ladder2.ogg");

	m_NumMouthOffs=0;
	sprintf(filename,"%smouthoff%i.ogg", path, m_NumMouthOffs+1);

	while (fopen(filename,"r")!=NULL && m_NumMouthOffs<10)
	{
		MouthOff[m_NumMouthOffs] = pcore->sound.LoadSound(8,filename);
		m_NumMouthOffs++;
		sprintf(filename,"%smouthoff%i.ogg", path, m_NumMouthOffs+1);
	}

	// death
	sprintf(filename,"%sdeath%i.ogg", path, 1);
	if (fopen(filename,"r")==NULL) sprintf(usepath, defaultpath);
	else sprintf(usepath, path);

	m_NumDeathSounds=0;
	sprintf(filename,"%sdeath%i.ogg", usepath, m_NumDeathSounds+1);

	while (fopen(filename,"r")!=NULL && m_NumDeathSounds<10)
	{
		DeathSounds[m_NumDeathSounds] = pcore->sound.LoadSound(6,filename);
		m_NumDeathSounds++;
		sprintf(filename,"%sdeath%i.ogg", usepath, m_NumDeathSounds+1);
	}

	// pain
	sprintf(filename,"%spain%i.ogg", path, 1);
	if (fopen(filename,"r")==NULL) sprintf(usepath, defaultpath);
	else sprintf(usepath, path);

	m_NumPainSounds=0;
	sprintf(filename,"%spain%i.ogg", usepath, m_NumPainSounds+1);

	while (fopen(filename,"r")!=NULL && m_NumPainSounds<10)
	{
		PainSounds[m_NumPainSounds] = pcore->sound.LoadSound(4,filename);
		m_NumPainSounds++;
		sprintf(filename,"%spain%i.ogg", usepath, m_NumPainSounds+1);
	}

	MyPlane.Init(this, model);

}


bool CPlayers::TakeDamage(float damage, CPlayers* player)
{
	if (bDead) return false;
	if (this->FlyingPlane) return MyPlane.TakeDamage(this, damage, player);

	//player 4 future (scores)


	if (&pcore->players[pcore->MyPlayerID].player == this)
	{
		pcore->gui.TookDamage();
	}

	
	DoPainSound();

	m_fXVel = m_fXVel*0.7;
	m_health = m_health - damage;

	if ((int)m_health<=0)
	{
		
		//this->Spawn();
		this->Die(true);
		return true;
	}
	
	return false;
}

void CPlayers::SetPlayerName(char* name)
{
	sprintf(Player_Name, "%s", name);
}



void CPlayers::ClimbUpLadder(void)
{
	if (FlyingPlane) return MyPlane.Accelerate(this);

	// if not on a ladder then no!
	if (!pcore->landscape.IsLadder(m_fX,m_fY))
	{
		return;
	}

	// if the movement will put us off the ladder - stay at the top
	if (!pcore->landscape.IsLadder(m_fX,m_fY+(-0.3f*pcore->gfx.tick)))
	{
		m_fYVel = 0;
		return;
	}

	m_iState=PLAYERSTATE_FALLING;
	this->m_fYVel = -0.3;
	OnLadder = true;
}

void CPlayers::ClimbDownLadder(void)
{
	if (!pcore->landscape.IsLadder(m_fX,m_fY))
	{
		return;
	}

	m_iState=PLAYERSTATE_FALLING;
	this->m_fYVel = 0.3;
	OnLadder = true;
}

bool CPlayers::FootCollision(bool fix, float x, float y)
{
	exit(0); // nothing should be calling this
	return false;
}

#define DUCKOFFSET (bDuck?33:0)

bool CPlayers::Collision(float x, float y)
{
	if (bDead) return false;

	if (x>m_fX-5 &&
		x<m_fX+5 &&
		y>(m_fY-30+DUCKOFFSET) &&
		y<m_fY+40)
	{
		return true;
	}

	return false;
}

bool CPlayers::Collision(GRECT* grct)
{
	if (bDead) return false;

	return (IntersectGRect(grct,&my_rect));
}
void CPlayers::DoMouthOff(void)
{
	if (m_LastMouthoff>0) return;
	
	m_LastMouthoff = pcore->s_MouthOffDelay;

	if (m_NumMouthOffs>0)
		pcore->sound.PlaySnd(MouthOff[rand()%m_NumMouthOffs],m_fX,m_fY);
}

void CPlayers::DoDeathSound(void)
{

	if (m_NumDeathSounds>0)
		pcore->sound.PlaySnd(DeathSounds[rand()%m_NumDeathSounds],m_fX,m_fY);
}

void CPlayers::DoPainSound(void)
{

	if (m_NumPainSounds>0)
		pcore->sound.PlaySnd(PainSounds[rand()%m_NumPainSounds],m_fX,m_fY);
}

void CPlayers::ChangeModel(char* modelname)
{
	// todo - free old model crap
	LoadPlayerModel(modelname);

	sprintf(Player_Model, modelname);
}

bool CPlayers::SplashDamage(float x, float y, float size, float damage, CPlayers* attacker)
{
	if (x<m_fX-size) return false;
	if (x>m_fX+size) return false;
	if (y-20<m_fY-size) return false;
	if (y-20>m_fY+size) return false;

	float distance = sqrtf( (m_fX-x)*(m_fX-x) + ((m_fY+20)-y)*((m_fY+20)-y));
	float angle = atan2((m_fX-x),(m_fY+20)-y);

	damage = damage * 0.4f; // skill

	/*
	for (float dis=0;dis<distance;dis=dis+10)
	{
		pcore->landscape.AddParticle(0,x+sin(atan2(m_fX-x, m_fY-y))*dis,y+cos(atan2(m_fX-x, m_fY-y))*dis);
	}
	*/
	if (distance>size) return false;

	if (((size-distance)/size)*damage>1)
	{
		for (int i=0;i<10;i++)
		{
			pcore->landscape.GenericBloodSpurt(m_fX+rand()%8,m_fY+rand()%64, angle, (rand()%1000/1300.0f)+0.5f);
		}

		TakeDamage((((size-distance)/size)*damage), attacker);
	}

	return true;
}
void CPlayers::Duck(void)
{
	if (pcore->bPaused || pcore->doingLevelEnd || pcore->doStartLevel) return;

	bUnducking=false;
	bDuck=true;
}

void CPlayers::UnDuck(void)
{
	if (!bDuck) return;

	bDuck=false;
}

void CPlayers::Die(bool Gib)
{
	LPParticle pt;
	float radhead;	
	float rnum;

	this->DoDeathSound();
	pcore->sound.PlayStream(pcore->landscape.music_playerdie);

	
	bDead = true;

	dietimer = 0;

	
	radhead = m_AimAngle;	
	if (radhead<0) radhead=radhead+D3DX_PI;
	radhead = (radhead-(D3DX_PI/2))/1.5f;

	// HEAD
	pt = pcore->landscape.AddParticle(PARTICLE_GENERIC_64X64, (m_fX), (m_fY-16.0f+(this->bDuck?16:0)));

	if (pt!=NULL)
	{
		rnum = (rand()%2000/1000.0f)-1.0f;
		pt->textureoverride = m_HeadTexture;
		pt->xscale = 0.5f*(m_Direction==0?-1:1);
		pt->yscale = 0.5f;
		pt->xvel = m_fXVel+(rnum*0.2f);;
		pt->yvel = m_fYVel+(rnum*0.2f);;
		pt->rot = radhead;
	}

	// ARMS
	for (int i=0;i<2;i++)
	{
		pt = pcore->landscape.AddParticle(PARTICLE_GENERIC_16X64, m_fX+(i*5), (m_fY+(this->bDuck?16:0)));

		if (pt!=NULL)
		{
			rnum = (rand()%2000/1000.0f)-1.0f;
			pt->textureoverride = m_ArmTexture;
			pt->xscale = 0.5f*(m_Direction==0?-1:1);
			pt->yscale = 0.5f;
			pt->xvel = m_fXVel+(rnum*0.2f);
			pt->yvel = m_fYVel+(rnum*0.2f);
			//pt->rotvel = 0;
			pt->rot = m_AimAngle;
		}
	}

	// BODY
	pt = pcore->landscape.AddParticle(PARTICLE_GENERIC_64X64, 
	(m_fX), 
	(m_fY+(this->bDuck?16:0)));

	if (pt!=NULL)
	{
		rnum = (rand()%2000/1000.0f)-1.0f;
		pt->textureoverride = m_BodyTexture;
		pt->xscale = 0.5f*(m_Direction==0?-1:1);
		pt->yscale = 0.5f;
		pt->xvel = m_fXVel+(rnum*0.1f);
		pt->yvel = m_fYVel+(rnum*0.1f);
		pt->rot = 0;
	}

	for (int i=0;i<30;i++)
	{
		pcore->landscape.GenericBloodSpurt(m_fX+rand()%8,m_fY-32+rand()%64, rand()%10000/1000.0f, 1, this);
	}

	pcore->DoDieRoutine();
}

void CPlayers::DoPickups(void)
{
	LPEntity p=NULL;
	p = pcore->ents.CollidePickup(this->my_rect);

	if (p!=NULL)
	{
		if (p->type==ENT_PLUTONIUM) this->CollectPlutonium();
		pcore->ents.RemoveEnt(p);
	}
}

bool CPlayers::GiveWeapon(unsigned int weapon)
{
	// if we're already carrying the weapon then add teh ammo from the weapon
	for (int i=0;i<MAX_WEAPONS_HOLDING;i++)
	{
		if (WeaponCarrying[i]==weapon)
		{
			GiveAmmo(pcore->weapons.m_GunAttributes[weapon].AmmoType, pcore->weapons.m_GunAttributes[weapon].ClipSize);
			return true;
		}
	}

	for (int i=0;i<MAX_WEAPONS_HOLDING;i++)
	{
		if (WeaponCarrying[i]==-1)
		{
			WeaponCarrying[i] = weapon;
			this->m_iWeapon = weapon;
		//	this->m_SpareAmmo[pcore->weapons.m_GunAttributes[weapon].AmmoType] = this->m_SpareAmmo[pcore->weapons.m_GunAttributes[weapon].AmmoType]  + pcore->weapons.m_GunAttributes[weapon].ClipSize;
			return true;
		}
	}

	return false;
}

void CPlayers::GiveAmmo(unsigned int type, unsigned int amount)
{
	this->m_SpareAmmo[type] = this->m_SpareAmmo[type]  + amount;
}


void CPlayers::StripWeaponsAndAmmo(void)
{
	for (int i=0;i<MAX_WEAPONS_HOLDING;i++)
	{
		WeaponCarrying[i]=-1;
	}

	for (int i=0;i<NUM_TYPESOFAMMO;i++)
	{
		m_SpareAmmo[i] = 0;
	}
	

}

void CPlayers::CollectPlutonium(int amount)
{
	m_plutonium = m_plutonium + amount;
	pcore->LevelStats.Plutonium++; 
	pcore->gui.TookPlutonium();
	
	// sound gets more high pitched the more you have collected
	pcore->sound.PlaySnd(pcore->props.s_pickup_plutonium,m_fX,m_fY,0.00f,0.0f,
		0.5f+(m_plutonium%100/200.0f));

	if (m_plutonium%100==0)
		pcore->sound.PlaySnd(pcore->s_Credit,0.0f,0.0f,0.00f,-5.0f,1.0f);
}

bool CPlayers::SpendMoney(int amount)
{
	amount = amount*100; // 1 = 100

	if (m_plutonium>=amount)
	{
		m_plutonium = m_plutonium - amount;
		return true;
	}
	return false;
}

bool CPlayers::CanSpendMoney(int amount)
{
	amount = amount*100; // 1 = 100

	if (m_plutonium>=amount)	return true;
	return false;
}

// return true if collision after push
bool CPlayers::Push(float x, float y)
{
	m_fY += y;

	UpdateMyRect();
	return false;
/*
	if (pcore->landscape.Collision(&my_rect))
	{
		// if there's collision with head and feet - we're being squished
		GRECT trect;
		bool squish=false;
		SetGRect(&trect,my_rect.left,my_rect.top,my_rect.right,my_rect.top+5);
		if (pcore->landscape.Collision(&trect))
		{
			SetGRect(&trect,my_rect.left,my_rect.bottom-5,my_rect.right,my_rect.bottom);
			if (pcore->landscape.Collision(&trect))
			{
				squish=true;
			}

		}
	//	m_fX -= x;
		m_fY -= y;
		UpdateMyRect();
		if (squish) TakeDamage(100,NULL);
		// todo: add hurt if the push causes collision - and stop the elevator(?)
		return true;
	}

	return false;    

	*/
}

void CPlayers::UpdateMyRect(void)
{
	if (this->FlyingPlane) return MyPlane.UpdateMyRect(this);
	my_rect.left = m_fX-12;
	my_rect.right = m_fX+12;
	my_rect.top = m_fY-this->m_headpos;
	my_rect.bottom = m_fY+this->m_feetpos;
}

void CPlayers::DoMovements(void)
{
	UpdateMyRect();
	if (FlyingPlane) return MyPlane.Movement(this);

	double movx, movy;
	GRECT trect, target_rect, movespan_rect, my_feetrect;

	// GRAVITY
	if (m_iState==PLAYERSTATE_FALLING && !OnLadder)
	{
		m_fYVel = m_fYVel + (GRAVITY*pcore->gfx.tick*AIRSPEED); 
		if (m_fYVel>MAXFALLSPEED) m_fYVel=MAXFALLSPEED; 
	}

	// WALKING DRAG
	if (m_iState==PLAYERSTATE_WALKING)
	{
		if (!m_bMoving)
		{
			if (m_fXVel>0)
			{
				m_fXVel = m_fXVel - (pcore->gfx.tick*0.005f);
				if (!m_bMoving && m_fXVel<0)  m_fXVel=0;
			}
			else if (m_fXVel<0) 
			{
				m_fXVel = m_fXVel + (pcore->gfx.tick*0.005f);
				if (!m_bMoving && m_fXVel>0)  m_fXVel=0;
			}
		}
	}
	else
	{
		if (!m_bMoving)
		{
			if (m_fXVel>0)
			{
				m_fXVel = m_fXVel - (0.00333*pcore->gfx.tick*AIRFRICTION);
				if (!m_bMoving && m_fXVel<0)  m_fXVel=0;
			}
			else if (m_fXVel<0) 
			{
				m_fXVel = m_fXVel + (0.00333*pcore->gfx.tick*AIRFRICTION);
				if (!m_bMoving && m_fXVel>0)  m_fXVel=0;
			}	
		}
	}

	// MAX WALK SPEEDS
	if (bDuck && m_iState==PLAYERSTATE_WALKING)
	{
		if (m_fXVel>0.1f) m_fXVel = 0.1f;
		else if (m_fXVel<-0.1f) m_fXVel = -0.1f;
	}
	else
	{
		if (m_fXVel>0.3f) m_fXVel = 0.3f;
		else if (m_fXVel<-0.3f) m_fXVel = -0.3f;
	}

	if (pcore->landscape.WaterLine>0 &&
	this->my_rect.bottom>pcore->landscape.WaterLine &&
	m_fXVel!=0.0f)
	{
		if (fmod(pcore->TimerA,50)>=10 && fmod(pcore->TimerAOld,50)<10)
		{
			LPParticle par;
			for (int i=0;i<4;i++)
			{
				par = pcore->landscape.AddParticle(PARTICLE_WATERSPLASHMIST,m_fX+(rand()%32-16),pcore->landscape.WaterLine);
				if (par!=NULL)
				{
					par->xscale = par->xscale*(0.2+(rand()%100/200.0f));
					par->yscale = par->xscale;
					par->xvel = m_fXVel*0.5;
				}
			}
		}
	}

	movx = m_fXVel*pcore->gfx.tick;
	movy = m_fYVel*pcore->gfx.tick*AIRSPEED;

	
	// if we can fall - fall
	if (m_iState==PLAYERSTATE_WALKING)
	{
		this->UpdateMyRect();
		SetGRect(&trect, my_rect.left, my_rect.top+1, my_rect.right,my_rect.bottom+1); 
		if (!pcore->landscape.Collision(&trect))
		{
			m_iState=PLAYERSTATE_FALLING;
		}
		else
		{
			m_fYVel=0;
			movy=0;
		}
	}

	// walking left/right
	if (movx!=0)
	{
		this->UpdateMyRect();
		SetGRect(&target_rect, my_rect.left+movx, my_rect.top, my_rect.right+movx,my_rect.bottom); 
		UnionGRect(&movespan_rect,&my_rect,&target_rect);
		if (pcore->landscape.Collision(&movespan_rect))
		{
			if (m_iState==PLAYERSTATE_WALKING)
			{
				for (int yup=1;yup<34;yup++) // step up
				{
					SetGRect(&target_rect, my_rect.left+movx, my_rect.top-yup, my_rect.right+movx,my_rect.bottom-yup); 
					if (!pcore->landscape.Collision(&target_rect))
					{
						movy = -yup;
						break;
					}
				}
				if (yup>32) 
				{
					m_fXVel=0;
					movx=0;
				}
			}
			else
			{
				m_fXVel=0;
				movx=0;
			}
		}
	}


	if (OnLadder)
	{
		DoLadderStuff();
		SetGRect(&target_rect, my_rect.left+movx, my_rect.top+movy, my_rect.right+movx,my_rect.bottom+movy); 
		UnionGRect(&movespan_rect,&target_rect,&target_rect);
		if (pcore->landscape.Collision(&movespan_rect))
		{
			m_fYVel=0;
			movy=0;
		}
	}
	else
	if (m_fYVel!=0) // falling or jumping
	{
		this->UpdateMyRect();
		if (m_fYVel>0)  // falling
		{
			SetGRect(&my_feetrect, my_rect.left, (my_rect.bottom-10), my_rect.right,my_rect.bottom); 
			SetGRect(&target_rect, my_rect.left+movx, (my_rect.bottom-10)+movy, my_rect.right+movx,my_rect.bottom+movy); 
			UnionGRect(&movespan_rect,&my_feetrect,&target_rect);
			if (pcore->landscape.Collision(&movespan_rect))
			{
				
				m_fYVel = 0;
				for (int i=0;i<fabs(movy)+1;i++)
				{
					SetGRect(&target_rect, my_rect.left+movx, my_rect.top+i, my_rect.right+movx, my_rect.bottom+i); 
					UnionGRect(&movespan_rect,&my_rect,&target_rect);
					if (pcore->landscape.Collision(&movespan_rect))
					{
						movy = i-1;
						m_iState=PLAYERSTATE_WALKING;//FALLING
					//	pcore->gui.WriteToConsole(0,0,0,0,"Land took %i steps",i);
						break;
					}
				}
				pcore->landscape.MakeExplosion(m_fX,m_fY+movy+this->m_feetpos+5,20,30);
			}
		}
		else // jumping
		{
			SetGRect(&target_rect, my_rect.left+movx, my_rect.top+movy, my_rect.right+movx,my_rect.bottom+movy); 
			UnionGRect(&movespan_rect,&target_rect,&target_rect);
			if (pcore->landscape.Collision(&movespan_rect)) 
			{
				m_fYVel=m_fYVel*0.5f;
				movy=0;
			}
		}
	}

	bool AboveWater=true;

	if (pcore->landscape.WaterLine>0 &&
	    this->my_rect.bottom>pcore->landscape.WaterLine)
		AboveWater=false;

	m_fX = (m_fX+movx);
	m_fY = (m_fY+movy);
	int steps=0;

	if (pcore->landscape.Collision(&my_rect))
	for (int i=0;i<100;i++)
		{
			steps++;
			UpdateMyRect();
			SetGRect(&target_rect, 
						my_rect.left, 
						my_rect.bottom-10.0f, 
						my_rect.right,
						my_rect.bottom); 

			if (!pcore->landscape.Collision(&target_rect))
			{
				m_iState=PLAYERSTATE_WALKING;
				break;
			}

			m_fY--; m_fYVel=0;
		}
	steps=00;
	if (pcore->landscape.Collision(&my_rect))
	for (int i=0;i<100;i++)
		{
			steps++;
			UpdateMyRect();
			SetGRect(&target_rect, 
						my_rect.left, 
						my_rect.top, 
						my_rect.right,
						my_rect.top+10.0f); 

			if (!pcore->landscape.Collision(&target_rect))
			{
				m_iState=PLAYERSTATE_WALKING;
				break;
			}

			m_fY++; m_fYVel=0;
		}

	UpdateMyRect();

	// their feet are now under water
	if (pcore->landscape.WaterLine>0 &&
	    this->my_rect.bottom>pcore->landscape.WaterLine &&
		AboveWater)
	{
		pcore->landscape.WaterLargeObjectPlunk(m_fX);
		pcore->sound.PlaySnd((rand()%2==0?pcore->landscape.s_waterplonk1:pcore->landscape.s_waterplonk2),m_fX, pcore->landscape.WaterLine,0.3f);
	}

	

	if (pcore->landscape.WaterLine>0 &&
	    (this->my_rect.top-16)>pcore->landscape.WaterLine)
	{
		this->TakeDamage(1000,NULL);
	}

	if (this->my_rect.bottom+10>=(pcore->landscape.height*32))
	{
		this->TakeDamage(1000,NULL);
	}

	if (pcore->landscape.Collision(&my_rect))
		this->TakeDamage(1000,NULL);
	
	m_bMoving = false;
}

void CPlayers::DoLadderStuff(void)
{
	m_fYVel=0;
	m_fXVel=0;

	// we've fallen off the ladder
	if (!pcore->landscape.IsLadder(m_fX,m_fY))
	{
		OnLadder = false;
		m_fYVel=0;
	}
	else
	{
		// if the next step up/down is on ladder, move them up.
		if (!pcore->landscape.IsLadder(m_fX,m_fY))
		{
			m_fYVel=0;
		}
		GRECT temp_rect;

		m_fLadderStepCount = m_fLadderStepCount +(m_fYVel*pcore->gfx.tick*0.1);

		if (fabs(m_fLadderStepCount)>4)
		{
			if (rand()%2==0)
				pcore->sound.PlaySnd(s_ladder1,m_fX, m_fY,0.1);
			else
				pcore->sound.PlaySnd(s_ladder2,m_fX, m_fY,0.1);

			m_fLadderStepCount=0;
		}

	}
}

bool CPlayers::TakeFireDamage(LPParticle p)
{
	if (bDead) return false;

	if (&pcore->players[pcore->MyPlayerID].player == this)
	{
		pcore->gui.TookDamage();
	}
	
	m_health = m_health - p->damage;

	if ((int)m_health<=0)
	{
		
		//this->Spawn();
		this->Die(true);
		return true;
	}

	return false;
}

void CPlayers::CountKill(void)
{
	pcore->LevelStats.Enemies++;
}

void CPlayers::DoBulletTimeToggle(void)
{
	if (inBulletTime || pcore->bShowBuyMenu)	inBulletTime=false;
	else if (BulletTime>0)	inBulletTime = true;
}

void CPlayers::DoBulletTime(void)
{
	if (pcore->bShowBuyMenu)
	{
		inBulletTime = false;
		pcore->timemultiplier = 1;
	}


	float slowestspeed = 0.1f;
	if (inBulletTime)
	{
		if (pcore->timemultiplier>slowestspeed)
		{
			pcore->timemultiplier = pcore->timemultiplier - (0.002f*pcore->gfx.tick);
			if (pcore->timemultiplier<slowestspeed) pcore->timemultiplier = slowestspeed;
		}
		BulletTime = BulletTime - (0.005f*pcore->gfx.RealTick);
		if (BulletTime<=0)
		{
			BulletTime=0;
			inBulletTime=false;
		}
	}
	else
	{
		if (pcore->timemultiplier<1)
		{
			pcore->timemultiplier = pcore->timemultiplier + (0.002f*pcore->gfx.tick);
			if (pcore->timemultiplier>1) pcore->timemultiplier = 1;
		}
	}
}

void CPlayers::AddBulletTime(float amount)
{
	BulletTime = BulletTime + amount;
	if (BulletTime>100) BulletTime = 100;
}

void CPlayers::ElectricDamage(void)
{
	float backx = (my_rect.right-my_rect.left)*0.5f;
	float backy = (my_rect.bottom-my_rect.top)*0.5f;
	
	for (int i=0;i<10;i++)
	pcore->landscape.Lightning(my_rect.left+(rand()%(int)(my_rect.right-my_rect.left))*2-backx,
							   my_rect.top+(rand()%(int)(my_rect.bottom-my_rect.top))*2-backy,
							   my_rect.left+(rand()%(int)(my_rect.right-my_rect.left))*2-backx,
							   my_rect.top+(rand()%(int)(my_rect.bottom-my_rect.top))*2-backy,
							   5,
							   0.9f, 
							   0.3f, 
							   true,
							   4);
}
