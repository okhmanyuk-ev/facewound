
#include "Weapons.h"
#include "Players.h"

#include "Core.h"

//#define MODELSIZE 0.5


// 0 = m16

void CWeapons::Init(CGarrysGraphics *gfx)
{
//	this->m_pGfx = gfx;

	// m16
	D3DXCreateTextureFromFile( pcore->gfx.m_pd3dDevice,"textures/p.bmp", &m_TextureWhiteP);
	D3DXCreateTextureFromFile( pcore->gfx.m_pd3dDevice,"textures/weapons/muzzle.bmp", &m_TextureMuzzle);
	D3DXCreateTextureFromFile( pcore->gfx.m_pd3dDevice,"textures/bright.bmp", &m_TextureBright);

	ChainsawIdle = pcore->sound.LoadSound( 5,"sound/weapons/chainsaw/idle.ogg");

	LoadWeapon(WEP_M16,						"m16.txt");
	LoadWeapon(WEP_BERETTA,					"berertta.txt");
	LoadWeapon(WEP_SHOTGUN,					"shotgun.txt");
	LoadWeapon(WEP_MINIGUN,					"minigun.txt");
	LoadWeapon(WEP_CROSSBOW,				"crossbow.txt");
	LoadWeapon(WEP_BOW,						"bow.txt");
	LoadWeapon(WEP_GRENADELAUNCHER,			"grenadelauncher.txt");
	LoadWeapon(WEP_MP5SD,					"mp5sd.txt");
	LoadWeapon(WEP_HANDGRENADE,				"handgrenade.txt");
	LoadWeapon(WEP_M60,						"m60.txt");
	LoadWeapon(WEP_LANDMINE,				"landmine.txt");
	LoadWeapon(WEP_SMAW,					"smaw.txt");
	LoadWeapon(WEP_AK47,					"ak47.txt");
	LoadWeapon(WEP_MAC10,					"mac10.txt");
	LoadWeapon(WEP_FLAMER,					"flamer.txt");
	LoadWeapon(WEP_DEAGLE,					"deagle.txt");
	LoadWeapon(WEP_PYTHON,					"python.txt");
	LoadWeapon(WEP_MOLOTOV,					"molotov.txt");
	LoadWeapon(WEP_CHAINSAW,				"chainsaw.txt");
	LoadWeapon(WEP_BAT,						"bat.txt");
	
	



	LoadWeapon(WEP_ARROWGUN,				"TEST-bow.txt");
	LoadWeapon(WEP_GRENADELAUNCHERSTRESS,	"TEST-grenadelauncher.txt");
}


void CWeapons::Draw(unsigned int gunnum, CPlayers* player, float x, float y, float zoom)//, float radians, float radcx, float radcy, float scalex, float scaley, unsigned int direction)
{

	this->Draw(gunnum,x,y,zoom,player->m_AimAngle,(player->m_Direction>0?1:-1), player->m_scale, player->m_timeSinceShot);

	return;
}

void CWeapons::DrawTracer(CPlayers* player, float a, float r, float g, float b)
{

}

void CWeapons::DrawMuzzle(CPlayers* player)
{
	if (player->m_iWeapon==WEP_BOW ||
		player->m_iWeapon==WEP_CROSSBOW ||
		player->m_iWeapon==WEP_HANDGRENADE ||
		player->m_iWeapon==WEP_GRENADELAUNCHER ||
		player->m_iWeapon==WEP_SMAW ||
		player->m_iWeapon==WEP_FLAMER ||
		player->m_iWeapon==WEP_LANDMINE ||
		player->m_iWeapon==WEP_MOLOTOV ||
		player->m_iWeapon==WEP_CHAINSAW) 
		return;

	if (pcore->weapons.m_GunAttributes[player->m_iWeapon].Melee) return;

	

	LPParticle ppointer;

	// huge gun spark
	if (rand()%4==0 && player->m_iWeapon!=WEP_MP5SD)
	{
		pcore->landscape.AddParticle(PARTICLE_GUNSPARK, 
		player->m_fX+(sin(player->m_LastAimAngle)*(m_GunAttributes[player->m_iWeapon].MuzzleTip)),
		player->m_fY+(cos(player->m_LastAimAngle)*(m_GunAttributes[player->m_iWeapon].MuzzleTip)));
		
		pcore->landscape.AddParticle(PARTICLE_GUNSPARK, 
		player->m_fX+(sin(player->m_LastAimAngle)*(m_GunAttributes[player->m_iWeapon].MuzzleTip)),
		player->m_fY+(cos(player->m_LastAimAngle)*(m_GunAttributes[player->m_iWeapon].MuzzleTip)));
	}

	float rfloat=(2.1f+(rand()%1000/1000.0f));
	float origrfloat= rfloat;
	float angle;

	int smoketimes = 1;

	if (player->m_iWeapon==WEP_SHOTGUN)
	{
		smoketimes = 9;
	}

	for (int i=0;i<smoketimes;i++)
	{
		if (i>0) rfloat = 0.1f+((origrfloat/smoketimes)*i);
		ppointer = pcore->landscape.AddParticle(PARTICLE_GUNSMOKE, player->m_fX+(sin(player->m_LastAimAngle)*(this->pcore->weapons.m_GunAttributes[player->m_iWeapon].MuzzleTip)),player->m_fY+(cos(player->m_LastAimAngle)*(player->pcore->weapons.m_GunAttributes[player->m_iWeapon].MuzzleTip)));
		if (ppointer!=NULL)
		{
			angle = player->m_AimAngle + (rand()%1000/2000.0f);
			ppointer->xvel = sin(angle)*0.5f*rfloat*(this->pcore->weapons.m_GunAttributes[player->m_iWeapon].MuzzleTip/80.0f);
			ppointer->yvel = cos(angle)*0.5f*rfloat*(this->pcore->weapons.m_GunAttributes[player->m_iWeapon].MuzzleTip/80.0f);
		}
	}

	
	ppointer = pcore->landscape.AddParticle(PARTICLE_MUZZLEFLASH, player->m_fX+(sin(player->m_LastAimAngle)*(pcore->weapons.m_GunAttributes[player->m_iWeapon].MuzzleTip)),player->m_fY+(cos(player->m_LastAimAngle)*(pcore->weapons.m_GunAttributes[player->m_iWeapon].MuzzleTip)));
	if (ppointer!=NULL)
	{
		ppointer->rot = player->m_LastAimAngle;
	}
	
	// for 3 way muzzle flash
	if (player->m_iWeapon==WEP_MINIGUN || player->m_iWeapon==WEP_M60)
	{
		ppointer = pcore->landscape.AddParticle(PARTICLE_MUZZLEFLASH, player->m_fX+(sin(player->m_LastAimAngle)*(pcore->weapons.m_GunAttributes[player->m_iWeapon].MuzzleTip)),player->m_fY+(cos(player->m_LastAimAngle)*(pcore->weapons.m_GunAttributes[player->m_iWeapon].MuzzleTip)));
		if (ppointer!=NULL)
		{
			ppointer->xscale = ppointer->xscale *0.5;
			ppointer->yscale = ppointer->yscale *0.5;
			ppointer->rot = player->m_LastAimAngle-0.8;
		}
		ppointer = pcore->landscape.AddParticle(PARTICLE_MUZZLEFLASH, player->m_fX+(sin(player->m_LastAimAngle)*(pcore->weapons.m_GunAttributes[player->m_iWeapon].MuzzleTip)),player->m_fY+(cos(player->m_LastAimAngle)*(pcore->weapons.m_GunAttributes[player->m_iWeapon].MuzzleTip)));
		if (ppointer!=NULL)
		{
			ppointer->xscale = ppointer->xscale *0.5;
			ppointer->yscale = ppointer->yscale *0.5;
			ppointer->rot = player->m_LastAimAngle+0.8;
		}
	}
}

void CWeapons::DrawMuzzle(float x, float y, float aimangle, unsigned int weapon)
{
	LPParticle ppointer;

	if (weapon==WEP_SMAW)
	{
		doRPGLaunchEffect(x-(sin(aimangle)*45),y-(cos(aimangle)*45),aimangle);
	}

	
	

	// huge gun spark
	pcore->landscape.AddParticle(7, x, y);
	pcore->landscape.AddParticle(7, x, y);

	ppointer = pcore->landscape.AddParticle(PARTICLE_GUNSMOKE, x,y);
	if (ppointer!=NULL)
	{
		ppointer->xvel = sin(aimangle)*0.02;
		ppointer->yvel = cos(aimangle)*0.02;
	}

	ppointer = pcore->landscape.AddParticle(PARTICLE_MUZZLEFLASH, x,y);
	if (ppointer!=NULL)
	{
		ppointer->rot = aimangle;
	}
	

	// for 3 way muzzle flash
	if (weapon==WEP_MINIGUN || weapon==WEP_M60)
	{
		ppointer = pcore->landscape.AddParticle(PARTICLE_MUZZLEFLASH, x,y);
		if (ppointer!=NULL)
		{
			ppointer->xscale = ppointer->xscale *0.5;
			ppointer->yscale = ppointer->yscale *0.5;
			ppointer->rot = aimangle-0.8;
		}
		ppointer = pcore->landscape.AddParticle(PARTICLE_MUZZLEFLASH, x,y);
		if (ppointer!=NULL)
		{
			ppointer->xscale = ppointer->xscale *0.5;
			ppointer->yscale = ppointer->yscale *0.5;
			ppointer->rot = aimangle+0.8;
		}
	}
}

void CWeapons::PlayShootSound(CPlayers* player)
{
	PlayShootSound(player->m_fX,player->m_fY,player->m_iWeapon);
}

void CWeapons::PlayShootSound(float x, float y, unsigned int wep)
{
	float flux=0.15f;

	if (wep==WEP_SHOTGUN ||
		wep==WEP_BERETTA ||
		wep==WEP_CHAINSAW)
		flux=0.0f;

	if (wep==WEP_CHAINSAW && LastShotChannel != 0)
	{
		flux=0.1f;

		if (pcore->sound.IsPlaying(LastShotChannel))
		{
			if (rand()%10!=1)
			{
				return;
			}
			pcore->sound.StopSound(LastShotChannel);
			LastShotChannel = 0;
		}
	}

	LastShotChannel = pcore->sound.PlaySnd(pcore->weapons.m_GunAttributes[wep].ShootSound,x,y,flux);

	if (wep!=WEP_CHAINSAW)
	{
		LastShotChannel = 0;
	}

}

void CWeapons::PlayPumpSound(float x, float y, unsigned int wep)
{
	float flux=0.0f;

	pcore->sound.PlaySnd(pcore->weapons.m_GunAttributes[wep].PumpSound,x,y,flux);
}
void CWeapons::PlayReloadSound(CPlayers* player)
{
	PlayReloadSound(player->m_fX,player->m_fY,player->m_iWeapon);
}
void CWeapons::PlayReloadSound(float x, float y, unsigned int wep)
{
	// to do replace this with the generic sound class
	//this->m_reloadsound[wep].Play(0,pcore->XPosOnScreen(x), pcore->timemultiplier);
//	pcore->sound.PlaySnd(this->m_GunAttributes[wep].ReloadSoundID,x,y,0.0f);
	if (this->m_GunAttributes[wep].ReloadSound!=NULL)
	{
		pcore->sound.PlaySnd(this->m_GunAttributes[wep].ReloadSound, x,y);
	}
}


void CWeapons::SpawnShell(CPlayers* player)
{
	if (m_GunAttributes[player->m_iWeapon].shelltype==0) return;

	// if shell spawns in landscape, fuck it off
	if (pcore->landscape.Collision(player->m_fX+(sin(player->m_LastAimAngle)*(m_GunAttributes[player->m_iWeapon].MuzzleTip/2)),player->m_fY+(cos(player->m_LastAimAngle)*(m_GunAttributes[player->m_iWeapon].MuzzleTip/2)))) return;

	float randomangle;

	LPParticle ppointer = pcore->landscape.AddParticle(m_GunAttributes[player->m_iWeapon].shelltype, player->m_fX+(sin(player->m_LastAimAngle)*(m_GunAttributes[player->m_iWeapon].MuzzleTip/2)),player->m_fY+(cos(player->m_LastAimAngle)*(m_GunAttributes[player->m_iWeapon].MuzzleTip/2)));

	if (ppointer!=NULL)
	{
		randomangle = (rand()%10000/10000.0f);
		randomangle = randomangle/2+0.2f;
		ppointer->xvel = (player->m_LastAimAngle<0?-1:1)*(sin(player->m_LastAimAngle+(D3DX_PI/2)+(randomangle*(player->m_LastAimAngle<0?-1:1)))*(((rand()%10000/10000.0f)/2)+0.3));//(rand()%1000/2000.0f);
		ppointer->yvel = cos(fabs(player->m_LastAimAngle)+(D3DX_PI/2)+randomangle)*(((rand()%10000/10000.0f)/2)+0.3);//-(rand()%1000/2000.0f);
	}
}

int CWeapons::NameToID(char* wname)
{
	for (int i=0; i<NUMWEAPONS; i++)
	{
		if (strcmp(wname, m_GunAttributes[i].name)==0)
			return i;
	}

	return -1;
}

void CWeapons::LoadWeapon(unsigned int weapon_num, char* filename)
{
	char realfilename[500];
	char string[300]; 

	sprintf(realfilename,"%s/data/weapons/%s",pcore->modfolder,filename);
	if (!FileExists(realfilename))
		sprintf(realfilename,"%s/data/weapons/%s",DEFAULT_MOD_FOLDER,filename);
	if (!FileExists(realfilename))
	{
		pcore->gfx.WriteToLog("Couldn't find weapon definitions '%s'", filename);
		sprintf(string,"Can't continue - missing weapon definition file '%s'",filename);
		MessageBox(NULL,string,"Error - can't continue",MB_ICONERROR);
		pcore->ExitGame();
	}

	FILE* f;

	f = fopen(realfilename,"r");
	char item[100];
	char value[100];
	char buffer[200];

	while (fgets (string , 300 , f)!=NULL)
	{
		sscanf(string,"%s %[^\n]",&item, &value);

		if (strcmp(item, "name")==0)
			sprintf(m_GunAttributes[weapon_num].name, value);

		if (strcmp(item, "poshname")==0)
			sprintf(m_GunAttributes[weapon_num].poshname, value);

		if (strcmp(item, "slot")==0)
			m_GunAttributes[weapon_num].slot = atoi(value);

		if (strcmp(item, "melee")==0)
			m_GunAttributes[weapon_num].Melee = true;

		if (strcmp(item, "weapontexture")==0)
		{
			sprintf(buffer,"textures/weapons/%s",value);
			m_Texture[weapon_num] = pcore->textures.LoadTexture(buffer);
		}

		if (strcmp(item, "guitexture")==0)
		{
			sprintf(buffer,"textures/gui/dropdown/%s",value);
			m_GuiTexture[weapon_num] = pcore->textures.LoadTexture(buffer);
		}

		if (strcmp(item, "shoottype")==0)
		{
			if (strcmp(value, "automatic")==0) m_GunAttributes[weapon_num].ShootType = ST_AUTOMATIC;
			if (strcmp(value, "pistol")==0) m_GunAttributes[weapon_num].ShootType = ST_PISTOL;
			if (strcmp(value, "shotgun")==0) m_GunAttributes[weapon_num].ShootType = ST_SHOTGUN;
		}

		if (strcmp(item, "carryoffset")==0)
			m_GunAttributes[weapon_num].CarryOffset= atof(value);

		if (strcmp(item, "spawndelay")==0)
			m_GunAttributes[weapon_num].spawndelay= atof(value);

		if (strcmp(item, "animframes")==0)
			m_GunAttributes[weapon_num].frames = atoi(value);

		if (strcmp(item, "animspeed")==0)
			m_GunAttributes[weapon_num].animspeed = atof(value);

		

		if (strcmp(item, "heightoffset")==0)
			m_GunAttributes[weapon_num].HeightOffset= atof(value);
		
		if (strcmp(item, "muzzletip")==0)
			m_GunAttributes[weapon_num].MuzzleTip = atof(value);

		if (strcmp(item, "timebetweenshots")==0)
			m_GunAttributes[weapon_num].TimeBetweenShots = atof(value);

		if (strcmp(item, "bulletspeed")==0)
			m_GunAttributes[weapon_num].BulletSpeed = atof(value);

		if (strcmp(item, "bulletspeedrandom")==0)
			m_GunAttributes[weapon_num].BulletSpeedRandomise = atof(value);

		if (strcmp(item, "clipsize")==0)
			m_GunAttributes[weapon_num].ClipSize = atoi(value);

		if (strcmp(item, "reloadsize")==0)
			m_GunAttributes[weapon_num].ReloadSize = atoi(value);

		if (strcmp(item, "reloadtime")==0)
			m_GunAttributes[weapon_num].ReloadTime = atof(value);

		if (strcmp(item, "ammotype")==0)
		{
			if (strcmp(value, "rifle")==0) m_GunAttributes[weapon_num].AmmoType = AMMO_RIFLE;
			if (strcmp(value, "9mm")==0) m_GunAttributes[weapon_num].AmmoType = AMMO_9MM;
			if (strcmp(value, "shotgun")==0) m_GunAttributes[weapon_num].AmmoType = AMMO_SHOTGUN;
			if (strcmp(value, "chain")==0) m_GunAttributes[weapon_num].AmmoType = AMMO_CHAIN;
			if (strcmp(value, "arrows")==0) m_GunAttributes[weapon_num].AmmoType = AMMO_ARROWS;
			if (strcmp(value, "grenades")==0) m_GunAttributes[weapon_num].AmmoType = AMMO_GRENADES;
			if (strcmp(value, "landmines")==0) m_GunAttributes[weapon_num].AmmoType = AMMO_LANDMINES;
			if (strcmp(value, "petrol")==0) m_GunAttributes[weapon_num].AmmoType = AMMO_PETROL;
			if (strcmp(value, "molotovs")==0) m_GunAttributes[weapon_num].AmmoType = AMMO_MOLOTOV;	
			if (strcmp(value, "none")==0) m_GunAttributes[weapon_num].AmmoType = AMMO_NONE;	

			
		}

		if (strcmp(item, "aimcone")==0)
			m_GunAttributes[weapon_num].AimCone = atof(value);

		if (strcmp(item, "reloaddropclip")==0)
			m_GunAttributes[weapon_num].ReloadDropClip = atoi(value);

		if (strcmp(item, "damage")==0)
			m_GunAttributes[weapon_num].damage = atof(value);

		if (strcmp(item, "damagefade")==0)
			m_GunAttributes[weapon_num].damagefade = atof(value);

		if (strcmp(item, "shelltype")==0)
		{
			if (strcmp(value, "rifleshell")==0) m_GunAttributes[weapon_num].shelltype = PARTICLE_RIFLESHELL;
			if (strcmp(value, "shell")==0) m_GunAttributes[weapon_num].shelltype = PARTICLE_SHELL;
			if (strcmp(value, "shotgunshell")==0) m_GunAttributes[weapon_num].shelltype = PARTICLE_SHOTGUNSHELL;
			if (strcmp(value, "chainshell")==0) m_GunAttributes[weapon_num].shelltype = PARTICLE_CHAINSHELL;
		
		}


		if (strcmp(item, "spintimer")==0)
			m_GunAttributes[weapon_num].SpinupTime = atof(value);	
		
		if (strcmp(item, "spinsound")==0)
			m_GunAttributes[weapon_num].SpinSound = pcore->sound.LoadSound(7,value);

		if (strcmp(item, "shootsound")==0)
			m_GunAttributes[weapon_num].ShootSound = pcore->sound.LoadSound(7,value);

		if (strcmp(item, "pumpsound")==0)
		{
			m_GunAttributes[weapon_num].Pumper = true;
			m_GunAttributes[weapon_num].PumpSound = pcore->sound.LoadSound(7,value);
		}


		

		if (strcmp(item, "reloadsound")==0)
			m_GunAttributes[weapon_num].ReloadSound = pcore->sound.LoadSound(3,value);

		if (strcmp(item, "screenshake")==0)
			m_GunAttributes[weapon_num].ScreenShake = atof(value);	

		if (strcmp(item, "numbullets")==0)
			m_GunAttributes[weapon_num].Shotgun_BuckshotNumber = atoi(value);

		if (strcmp(item, "bulletparticle")==0)
		{
			if (strcmp(value, "buckshot")==0) m_GunAttributes[weapon_num].BulletParticle = PARTICLE_BUCKSHOT;
			if (strcmp(value, "arrow")==0) m_GunAttributes[weapon_num].BulletParticle = PARTICLE_ARROW;
			if (strcmp(value, "grenade")==0) m_GunAttributes[weapon_num].BulletParticle = PARTICLE_HANDGRENADE;
			if (strcmp(value, "impactgrenade")==0) m_GunAttributes[weapon_num].BulletParticle = PARTICLE_IMPACTGRENADE;
			if (strcmp(value, "landmine")==0) m_GunAttributes[weapon_num].BulletParticle = PARTICLE_LANDMINE;
			if (strcmp(value, "rpg")==0) m_GunAttributes[weapon_num].BulletParticle = PARTICLE_RPG;
			if (strcmp(value, "fire")==0) m_GunAttributes[weapon_num].BulletParticle = PARTICLE_FIRE;
			if (strcmp(value, "molotov")==0) m_GunAttributes[weapon_num].BulletParticle = PARTICLE_MOLOTOV;
			if (strcmp(value, "static")==0) m_GunAttributes[weapon_num].BulletParticle = PARTICLE_STATICBULLET;
	
		}

		if (strcmp(item, "width")==0)
			m_GunAttributes[weapon_num].Width = atof(value);
		
	}

}

void CWeapons::doRPGLaunchEffect(float x, float y, float angle)
{
	float rangle;
	float speed;
	LPParticle p=NULL;

	angle = angle + D3DX_PI;
	x = x + (sin(angle)*20);
	y = y + (cos(angle)*20);


	for (int i=0;i<5;i++)
	{
		p = pcore->landscape.AddParticle(PARTICLE_RPG_TRAIL,x+(rand()%30)-15,y+(rand()%30)-15);

		if (p!=NULL)
		{
			p->xscale = 1.0f + ((rand()%100)/100.0f);
			rangle = angle + (((rand()%2000)-1000)/2000.0f);
			speed = ((rand()%1000)/50000.0f);
			p->xvel = sin(rangle)*speed;
			p->yvel = cos(rangle)*speed;
		}
	}
	
	for (int i=0;i<20;i++)
	{
		p = pcore->landscape.AddParticle(PARTICLE_RICHOCHETSPARK,x,y);

		if (p!=NULL)
		{
			rangle = angle + (((rand()%2000)-1000)/2000.0f);
			speed = 0.1f+((rand()%2000)/3000.0f);
			p->xvel = sin(rangle)*speed;
			p->yvel = cos(rangle)*speed;
		}
	}

}

void CWeapons::PlayIdleSound(int wep, float x, float y)
{
	if (pcore->sound.IsPlaying(ChainsawIdleChannel)) 
	{
		this->pcore->sound.UpdateSound(ChainsawIdleChannel, x, y);
	}
	else
	{
		ChainsawIdleChannel = pcore->sound.PlaySnd(ChainsawIdle,x,y,0.0f,0.0f);
	}
}

void CWeapons::PlaySpinSound(int wep, float x, float y)
{
	pcore->sound.PlaySnd(pcore->weapons.m_GunAttributes[wep].SpinSound,x,y);
}

//direction should be either -1 or 1
void CWeapons::Draw(unsigned int gunnum, float x, float y, float zoom, float angle, int direction, float scale, float shoottimer)
{
	if (gunnum==WEP_CHAINSAW)
	{
		x=x + (((rand()%100)-50)/70.0f);
		y=y + (((rand()%100)-50)/70.0f);
		angle = angle + ((rand()%1000-500)/20000.0f);
	}

	if (m_GunAttributes[gunnum].CarryOffset!=0)
	{
		x = x + (sin(angle)*m_GunAttributes[gunnum].CarryOffset);
		y = y + (cos(angle)*m_GunAttributes[gunnum].CarryOffset);
	}

	if (m_GunAttributes[gunnum].HeightOffset!=0)
	{
		x = x - (sin(angle+(D3DX_PI/2))*m_GunAttributes[gunnum].HeightOffset*(direction));
		y = y - (cos(angle+(D3DX_PI/2))*m_GunAttributes[gunnum].HeightOffset*(direction));
	}

	float offx=0,offy=0;

	if (m_GunAttributes[gunnum].frames>0)
	{
		int animoffs = (int)(shoottimer*m_GunAttributes[gunnum].animspeed);
		if (animoffs<m_GunAttributes[gunnum].frames)
		{
			offy = animoffs*m_GunAttributes[gunnum].Width;
		}
	}


	// gun
	pcore->gfx.SpriteDrawEx(m_Texture[gunnum],
					(x-(11)+(direction==-1?22:0))*zoom,
					y*zoom,

					angle,
					(11-(direction==-1?22:0))*zoom,
					zoom,

					(direction*zoom)/2,(zoom)/2,
					
					 offx,offy,m_GunAttributes[gunnum].Width,m_GunAttributes[gunnum].Height,
					pcore->landscape.m_globalaplha,255,255,255

					);
}
