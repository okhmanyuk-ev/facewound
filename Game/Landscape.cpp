
#include "Landscape.h"

#include "Core.h"

#include "Players.h"

#include "garryswrappers/GarrysHelperFunctions.h"
#include "Weapons.h"
#include "Entitys.h"

#include "enemies.h"


#include "euler_angle/EulerAngles.c"



void CLandscape::Initialize(CGarrysGraphics *p_Gfx)
{
	spawnx=0;
	spawny=0;
	m_tilesize = 32;
	sprintf(m_NextLevel, "");
	music=NULL;
	enumWeatherMode = WEATHER_OFF;
	music = NULL;
	music_playerdie = NULL;
	music_levelcomplete = NULL;

	srand((unsigned int)timeGetTime());
//	this->p_mGfx = p_Gfx;
	char filename[100];
	ptiles = NULL;
	ptilesover = NULL;

	int i;

	offset_x = 0;
	offset_y = 0;

	LoadSky("grey");
	#include "particle-definitions.h"

	m_zoom=1;
	m_zoom_real=1;
	m_ParticleCount=0;
	m_pFirstParticle=NULL;
	m_pLastParticle=NULL;

	WaterEffect_Speed = 0.4f;
	WaterEffect_Size = 0.3f;

	s_9mmbounce1		= pcore->sound.LoadSound(1,"sound/particles/9mm_shell.ogg");
	s_arrowhit1			= pcore->sound.LoadSound(1,"sound/particles/arrowhit.ogg");
	s_buckshotbounce1	= pcore->sound.LoadSound(1,"sound/particles/shotgun_shell.ogg");
	s_chaingunbounce1	= pcore->sound.LoadSound(1,"sound/particles/minigun_shell.ogg");
	s_clipbounce1		= pcore->sound.LoadSound(1,"sound/particles/magazine1.ogg");
	s_clipbounce2		= pcore->sound.LoadSound(1,"sound/particles/magazine2.ogg");
	s_grenadebounce1	= pcore->sound.LoadSound(1,"sound/particles/grenade-bounce.ogg");
	s_ricochet1			= pcore->sound.LoadSound(1,"sound/particles/richo-1.ogg");
	s_ricochet2			= pcore->sound.LoadSound(1,"sound/particles/richo-2.ogg");
	s_ricochet3			= pcore->sound.LoadSound(1,"sound/particles/richo-3.ogg");
	s_ricochet4			= pcore->sound.LoadSound(1,"sound/particles/richo-4.ogg");
	s_riflebounce1		= pcore->sound.LoadSound(1,"sound/particles/m16_shell.ogg");
	s_explosion			= pcore->sound.LoadSound(9,"sound/weapons/explosion1.ogg");

	s_fire1				= pcore->sound.LoadSound(9,"sound/env/fireloop1.ogg");
	s_fire2				= pcore->sound.LoadSound(9,"sound/env/fireloop2.ogg");
	s_fire3				= pcore->sound.LoadSound(9,"sound/env/fireloop3.ogg");
	s_fire4				= pcore->sound.LoadSound(9,"sound/env/fireloop4.ogg");

	s_molotov			= pcore->sound.LoadSound(9,"sound/weapons/molotov.ogg");
	s_bonehit			= pcore->sound.LoadSound(9,"sound/particles/bonebounce.ogg");


	s_waterbullet1		= pcore->sound.LoadSound(9,"sound/env/waterbullet1.ogg");
	s_waterbullet2		= pcore->sound.LoadSound(9,"sound/env/waterbullet2.ogg");
	s_waterplonk1		= pcore->sound.LoadSound(9,"sound/env/waterplonk1.ogg");
	s_waterplonk2		= pcore->sound.LoadSound(9,"sound/env/waterplonk2.ogg");
	s_watershell1		= pcore->sound.LoadSound(9,"sound/env/watershell1.ogg");
	s_watershell2		= pcore->sound.LoadSound(9,"sound/env/watershell2.ogg");

	t_water				=  pcore->textures.LoadTexture("textures/props/water.tga");
	t_normalwater		=  pcore->textures.LoadTexture("textures/props/normal/water.tga");

	t_BloodDrips[0]		= pcore->textures.LoadTexture("textures/particles/blooddecal1.tga");
	t_BloodDrips[1]		= pcore->textures.LoadTexture("textures/particles/blooddecal2.tga");
	t_BloodDrips[2]		= pcore->textures.LoadTexture("textures/particles/blooddecal3.tga");
	t_BloodDrips[3]		= pcore->textures.LoadTexture("textures/particles/blooddecal4.tga");
	t_BloodDrips[4]		= pcore->textures.LoadTexture("textures/particles/blooddecal5.tga");
	t_BloodDrips[5]		= pcore->textures.LoadTexture("textures/particles/blooddecal6.tga");

	t_BloodBlobs[0] = pcore->textures.LoadTexture("textures/particles/bloodblob1.tga");
	t_BloodBlobs[1] = pcore->textures.LoadTexture("textures/particles/bloodblob2.tga");
	t_BloodBlobs[2] = pcore->textures.LoadTexture("textures/particles/bloodblob3.tga");



}

void CLandscape::SetZoom(float scale)
{
	if (scale<0.42) scale=0.42;

	m_zoom_real = scale;

	m_zoom = m_zoom_real*pcore->ScreenZoom;
	this->SetWindowSize();
}

int CLandscape::Map2D(int x, int y)
{
	return (x + (y*width));
}

void CLandscape::LoadMap(const char *inf)
{
	HiddenAreas=0;
	spawnx=0;
	spawny=0;
	enumWeatherMode = WEATHER_OFF;

	this->WaterLineOriginal = 0;
	this->WaterLine = 0;
	this->WaterLineVelocity = 0;

	pcore->Loading(26);
	pcore->sound.CloseStream(music);
	pcore->sound.CloseStream(music_playerdie);
	pcore->sound.CloseStream(music_levelcomplete);

	pcore->Loading(27);
	music_levelcomplete = NULL;
	music_playerdie = NULL;
	music = NULL;
	pcore->StartTimer();
	pcore->b_LoadingMap = true;
	

	pcore->Loading(28);
	// queue up the new music
	music_levelcomplete = pcore->sound.LoadStream("sound/music/levelcomplete.ogg",true);
	music_playerdie = pcore->sound.LoadStream("sound/music/playerdie.ogg",false);
	pcore->Loading(33);

	char filename[500];

	if (pcore->Effect!=NULL)
	{
		pcore->Effect->Release();
		pcore->Effect=NULL;
	}

	if (pcore->GlobalEffect!=NULL)
	{
		pcore->GlobalEffect->Release();
		pcore->GlobalEffect=NULL;
	}

	sprintf(filename,"%s/maps/%s",pcore->modfolder,inf);
	if (!FileExists(filename))
	{
		pcore->gfx.WriteToLog("Map not found in mod folder '%s'<br>", filename);
		sprintf(filename, "%s/maps/%s", DEFAULT_MOD_FOLDER,inf);
	}

	


	pcore->Loading(34);
	// couldn't open map
	if (fopen(filename,"r")==NULL) 
	{
		pcore->gui.WriteToConsole(1,255,0,0,"Couldn't open map : ",filename);
		pcore->gfx.WriteToLog("FATAL ERROR: COULDN'T LOAD MAP '%s'!<br>",filename);
		if (ptiles!=NULL)
		{
			pcore->b_LoadingMap = false;
			return;
		}
		else 
		{
			MessageBox(NULL,"COULDN'T LOAD INITIAL MAP!","Fuck.",0);
			exit(0);
		}
	}

	sprintf(m_currentlevel,inf);
	this->LoadLevelShader();

	pcore->gfx.WriteToLog("<br><br><b>Loading map: %s</b><br>", filename);
	pcore->ents.RemoveAll();
	pcore->props.RemoveAll();
	pcore->enemies.RemoveAll();
	pcore->landscape.RemoveAllParticle();

	pcore->Loading(38);

	char buffer[500];

	if (ptiles!=NULL)
	{
		delete[] ptiles;
		ptiles=NULL;
	}

	if (ptilesover!=NULL)
	{
		delete[] ptilesover;
		ptilesover=NULL;
	}

	if (ptilesunder!=NULL)
	{
		delete[] ptilesunder;
		ptilesunder=NULL;
	}

	pcore->OutputTimer("Remove old stuff");
	pcore->StartTimer();

	gzFile  file;

	file = gzopen(filename,"r+b");

	pcore->Loading(42);

	// load width and height
	gzread(file,&width,sizeof(unsigned int));
	gzread(file,&height,sizeof(unsigned int));
	pcore->gfx.WriteToLog("Width: %i Height %i<br>", width, height);
	
	width=width+1;
	height=height+1;
	
	pcore->OutputTimer("Read array size");
	pcore->StartTimer();

	pcore->Loading(43);

	// make array and clear it
	ptiles = new unsigned int[(width*height)];
	ptilesover = new unsigned int[(width*height)];
	ptilesunder = new unsigned int[(width*height)];
	
	for (int i=0;i<(width*height);i++)
	{
		ptiles[i] = 2;
		ptilesover[i] = 2;
	}

	pcore->OutputTimer("Create new arrays");
	pcore->StartTimer();
	
	pcore->Loading(45);

	gzread(file,&ptiles[0],sizeof(unsigned int)*width*height);
	gzread(file,&ptilesover[0],sizeof(unsigned int)*width*height);
	gzread(file,&ptilesunder[0],sizeof(unsigned int)*width*height);

	pcore->OutputTimer("Fill new arrays");
	pcore->StartTimer();

	//load entitys
	unsigned int numents;
	LPEntity pent, tent=NULL;
	
	gzread(file, &numents,sizeof(unsigned int));
	pcore->gfx.WriteToLog("Number of entities: %i<br>", numents);

	pcore->Loading(48);

	for (i=0;i<numents;i++)
	{
		gzread(file, &LoadEnt,sizeof(LoadEnt));

		pent = ents->AddEntity();
	
		if (LoadEnt.type==ENT_SECRETAREA)
			HiddenAreas++;

	//	if (LoadEnt.type==2) this->AddPhysicalObject(LoadEnt.x,LoadEnt.y,LoadEnt.a,LoadEnt.b,LoadEnt.c,LoadEnt.stringa, LoadEnt.d, LoadEnt.e, LoadEnt.f);
		
		if (LoadEnt.type==4 && LoadEnt.a>4 && LoadEnt.b>4) // prop
		{
			sProps* prop =  pcore->props.AddProp();
			prop->x = LoadEnt.x;
			prop->y = LoadEnt.y;
			if (strcmp(LoadEnt.stringa,"")!=0)
			{
				sprintf(buffer, "textures/props/%s",LoadEnt.stringa);
				prop->texture = pcore->textures.LoadTexture(buffer);
			}
			else
			{
				prop->texture=NULL;
			}
			prop->width = LoadEnt.a;
			prop->height = LoadEnt.b;
			prop->canbedestroyed = (LoadEnt.c==-1);
			prop->explosion = LoadEnt.d;
			prop->health = LoadEnt.c;
			prop->particle = (int)LoadEnt.e;
			prop->particle_num = (int)LoadEnt.f;
			prop->blendmode = (int)LoadEnt.g;
			sprintf(prop->name, LoadEnt.name);       // name
			sprintf(prop->trigger, LoadEnt.stringb); // trigger


			if (prop->canbedestroyed)
				prop->type = PTYPE_PROP; // normal prop
			else
				prop->type = PTYPE_DESTROYABLE; // normal prop

		//	pcore->gfx.WriteToLog("added prop (%s) using blendmode (%i)<br>",LoadEnt.stringa, prop->blendmode);
			pcore->props.UpdateProp(prop);
		}

		if (LoadEnt.type==10) // mover
		{
			sProps* prop =  pcore->props.AddProp();
			prop->x = LoadEnt.x;
			prop->y = LoadEnt.y;
			sprintf(buffer, "textures/props/%s",LoadEnt.stringa);
			sprintf(prop->trigger, LoadEnt.stringb);
			prop->texture = pcore->textures.LoadTexture(buffer);
			prop->width = LoadEnt.a;
			prop->height = LoadEnt.b;
			prop->canbedestroyed = false;
			prop->health = 0;
			prop->blendmode = (int)LoadEnt.g;
			//pcore->gfx.WriteToLog("added slider (%s) using blendmode (%i)<br>",LoadEnt.stringa, prop->blendmode);
			prop->xvel = 0;
			prop->yvel = LoadEnt.d;
			prop->xoff = LoadEnt.e; // # seconds before return
			prop->yoff = LoadEnt.c;
			prop->type = PTYPE_MOVER;
			pcore->props.UpdateProp(prop);
			sprintf(prop->name, LoadEnt.name);
			prop=NULL;
		}

		if (LoadEnt.type==8) // pickup
		{
			sProps* prop =  pcore->props.AddProp();
			prop->ispickup = true;
			prop->pickuptype = (int)LoadEnt.c;
			prop->x = LoadEnt.x;
			prop->y = LoadEnt.y;
			sprintf(buffer, "textures/props/%s",LoadEnt.stringa);
			prop->texture = pcore->textures.LoadTexture(buffer);
			prop->width = LoadEnt.a;
			prop->height = LoadEnt.b;
			sprintf(prop->trigger, LoadEnt.stringb);
			prop->canbedestroyed = false;
			prop->explosion = 0;
			prop->health = 0;
			prop->particle = 0;
			prop->particle_num = 0;
			prop->blendmode = 0;
			prop->type = PTYPE_PICKUP;
			sprintf(prop->name, LoadEnt.name);
			//pcore->gfx.WriteToLog("added pickup (%s) <br>",LoadEnt.stringa, prop->blendmode);
			pcore->props.UpdateProp(prop);

		}

		if (LoadEnt.type==9) // trigger
		{
			pent->c = LoadEnt.c;
		}

		// Water line
		if (LoadEnt.type==ENT_WATERLINE)
		{
			this->WaterLine = LoadEnt.y;
			this->WaterLineOriginal = this->WaterLine;
			this->WaterLineVelocity = LoadEnt.a;
			if (strcmp(LoadEnt.stringa,"")!=0)
			{
				t_water		=  pcore->textures.LoadTexture(LoadEnt.stringa);
			}
		}

	//	pcore->gui.WriteToConsole(5,255,255,0,"Adding Entity (%i %i %i)",LoadEnt.type,(int)LoadEnt.x,(int)LoadEnt.y);
		pent->type = LoadEnt.type;
		pent->x = LoadEnt.x;
		pent->y = LoadEnt.y;
		pent->Width = LoadEnt.a;
		pent->Height = LoadEnt.b;
		pent->c = LoadEnt.c;
		pent->d = LoadEnt.d;
		pent->e = LoadEnt.e;
		pent->f = LoadEnt.f;
		pent->g = LoadEnt.g;
		pent->h = LoadEnt.h;
		pent->rect.top = LoadEnt.y;
		pent->rect.bottom = LoadEnt.y+LoadEnt.b;
		pent->rect.left = LoadEnt.x;
		pent->rect.right = LoadEnt.x+LoadEnt.a;
		sprintf(pent->name, LoadEnt.name);
		sprintf(pent->trigger, LoadEnt.stringb);
		sprintf(pent->String1,LoadEnt.stringa);

		if (pent->type==ENT_PARTICLESPAWNER)
		{
			if ((int)pent->Width==0) pent->Width=1;
			if ((int)pent->Height==0) pent->Height=1;
			if ((int)pent->g==0) pent->g=1;
			if ((int)pent->h!=0) pent->inuse=false;
			else pent->inuse=true;
		}
		else if (pent->type==ENT_TRIGGER_ONNOENEMIES)
		{
			pent->inuse=true;
			sprintf(pent->trigger, pent->String1); // trigger
		}


		// build a level end entity
		if (LoadEnt.type==ENT_LEVELEND)
		{
			tent = NULL;
			tent = ents->AddEntity();
			if (tent!=NULL)
			{
				tent->inuse = true;
				tent->type = ENT_TRIGGER;
				tent->x = pent->x - 8.0f;
				tent->y = pent->y - 8.0f;
				tent->Width = 16;
				tent->Height = 16;
				sprintf(tent->trigger,pent->name);
				tent->rect.top = tent->y;
				tent->rect.bottom = tent->y+tent->Width;
				tent->rect.left = tent->x;
				tent->rect.right = tent->x+tent->Height;
			}
			tent = NULL;
			tent = ents->AddEntity();
			if (tent!=NULL)
			{
				tent->inuse=true;
				tent->type = ENT_PROP;
				tent->x = pent->x - 128.0f;
				tent->y = pent->y - 128.0f;
				tent->Width = 2;
				tent->Height = 2;
				sprintf(tent->String1,"exit.bmp");
				tent->rect.top = tent->y;
				tent->rect.bottom = tent->y+tent->Width;
				tent->rect.left = tent->x;
				tent->rect.right = tent->x+tent->Height;
			}
			tent = NULL;
			tent = ents->AddEntity();
			if (tent!=NULL)
			{
				tent->inuse=true;
				tent->type = ENT_PARTICLESPAWNER;
				tent->x = pent->x - 40.0f;
				tent->y = pent->y - 40.0f;
				tent->Width = 80;
				tent->Height = 80;
				tent->c = 16;
				tent->d = 50;
				tent->e = 50;
				tent->f = 0;
				tent->g = 2;
				tent->h = 0;
				tent->rect.top = tent->y;
				tent->rect.bottom = tent->y+tent->Width;
				tent->rect.left = tent->x;
				tent->rect.right = tent->x+tent->Height;
			}
		}

	}

	pcore->Loading(49);

	pcore->ents.ProcessEntitys();

	pcore->Loading(50);

	pcore->OutputTimer("Load entitys/props");
	pcore->StartTimer();
	
	// load texture names
	sprintf(textures[0].path,"t1.bmp");
	sprintf(texturesunder[0].path,"u1.bmp");
	sprintf(texturesover[0].path,"o1.bmp");

	for (int i=0;i<NUM_LANDCAPETEXTURES;i++)
	{
		gzread(file, textures[i].path,sizeof(char)*255);
	}
	for (int i=0;i<NUM_LANDCAPETEXTURES;i++)
	{
		gzread(file, texturesover[i].path,sizeof(char)*255);
	}
	for (int i=0;i<NUM_LANDCAPETEXTURES;i++)
	{
		gzread(file, texturesunder[i].path,sizeof(char)*255);
	}

	pcore->Loading(53);

	pcore->OutputTimer("Get texture names");
	pcore->StartTimer();

	gzread(file, &m_MapProperties, sizeof(MapProperties));

	gzclose(file);

	m_MaxXTile = width;
	m_MaxYTile = height;

	pcore->gui.WriteToConsole(5,0,0,255,"");
	pcore->gui.WriteToConsole(5,0,0,255,"======Map Loaded======");
	pcore->gui.WriteToConsole(5,0,0,255,"Title: %s", m_MapProperties.maptitle);
	pcore->gui.WriteToConsole(5,0,0,255,"Author: %s", m_MapProperties.author);
	pcore->gui.WriteToConsole(5,0,0,255,"Music: %s", m_MapProperties.music);
	pcore->gui.WriteToConsole(5,0,0,255,"Gravity: %f", m_MapProperties.gravity);
	pcore->gui.WriteToConsole(5,0,0,255,"SkyName: %s", m_MapProperties.skyname);
	pcore->gui.WriteToConsole(5,0,0,255,"SkySpeed: %f", m_MapProperties.skyspeed);
	pcore->gui.WriteToConsole(5,0,0,255,"");

	pcore->gfx.WriteToLog("Done loading map..<br>");


	pcore->gfx.WriteToLog("Setting Gravity Multiplier (%f)<br>", m_MapProperties.gravity);
	pcore->gravity = m_MapProperties.gravity;

	pcore->gfx.WriteToLog("loading Sky textures (%s)<br>", m_MapProperties.skyname);
	this->LoadSky(m_MapProperties.skyname);
	
	
	pcore->OutputTimer("write stuff to console");
	pcore->StartTimer();

	pcore->gfx.WriteToLog("Loading Map Textures<br>");
	this->LoadMapTextures(true);

	pcore->Loading(55);

	pcore->OutputTimer("loading map textures");
	pcore->StartTimer();
	


	pcore->gfx.WriteToLog("Start Level Spawners<br>");
	pcore->ents.StartLevelSpawners();

	pcore->Loading(58);

	pcore->OutputTimer("level spawners");
	pcore->StartTimer();

	pcore->players[pcore->MyPlayerID].player.Spawn(false);

	pcore->timemultiplier = 1;
	pcore->gui.ShowConsole = false;
	FocusCameraOnPlayer();

	
	pcore->OutputTimer("player spawn");
	pcore->StartTimer();

	

	pcore->b_LoadingMap = false;

	pcore->Loading(60);

	pcore->gfx.WriteToLog("Finished Map Load!<br>");

	return;
}

void CLandscape::Clear(unsigned int i)
{
	for (int x=0;x<(int)(width*height);x++)
	{
		ptiles[x] = 0;
		ptiles[x] = 0;
	}
}

bool CLandscape::BulletShoot(CPlayers* player)
{
	if (player->m_iWeapon==WEP_SMAW)
	{
		pcore->weapons.doRPGLaunchEffect(player->m_fX,player->m_fY,player->m_AimAngle);
	}


	
	
	LPParticle particle = BulletShoot(player->m_iWeapon,
				player->m_fX+sin(player->m_AimAngle)*pcore->weapons.m_GunAttributes[player->m_iWeapon].MuzzleTip,
				player->m_fY+cos(player->m_AimAngle)*pcore->weapons.m_GunAttributes[player->m_iWeapon].MuzzleTip,
				player->m_AimAngle,
				player->m_iWeapon==WEP_BOW?player->AimLength:1);

	if (particle!=NULL)
	{
		particle->pplayer = player;

		// trace bullet down gunshaft
		float oldx=particle->x, 
			  oldy=particle->y, 
			  oldxvel=particle->xvel, 
			  oldyvel=particle->yvel,
			  oldtick=pcore->gfx.tick;
		bool remove=false;

		if (particle->type!=PARTICLE_HANDGRENADE && 
			particle->type!=PARTICLE_STATICBULLET && 
			particle->type!=PARTICLE_FIRE) // grenades need to bounce
		{
			particle->x = player->m_fX;
			particle->y = player->m_fY;

			particle->xvel = oldx-player->m_fX;
			particle->yvel = oldy-player->m_fY;


			pcore->gfx.tick = 1.0f;
			
			this->DoBulletParticle(particle,&remove,false);

			pcore->gfx.tick = oldtick;
		}

		if (remove)
		{
			ParticleRemoveExplosive(particle);
			this->RemoveParticle(particle);
			particle=NULL;
			return false;
		}
		else
		{
			particle->x = oldx;
			particle->y = oldy;

			particle->xvel = oldxvel;
			particle->yvel = oldyvel;
//-0.4714;
//1.0000;1.6709;0.0000;0.0000;0.0000;0

			pcore->cdemo.AddItem(NCTYPE_PLAYERSHOOT,oldx,oldy,oldxvel, oldyvel, (float)player->m_iWeapon, player->m_AimAngle,0,0,0,0,0);
		}



		if (particle!=NULL)
		{
			if (player->m_iWeapon==WEP_HANDGRENADE || player->m_iWeapon==WEP_MOLOTOV)
			{
				particle->xvel = particle->xvel + (player->m_fXVel*0.5f);
				particle->yvel = particle->yvel + (player->m_fYVel*0.5f);

				if (player->m_iWeapon==WEP_MOLOTOV)
				{
					particle->SpawnDistanceReset = 6.0f;
					particle->SpawnDistance = 6.0f;
					particle->SpawnParticle = PARTICLE_MOLOTOVTAIL;
				}
			}
		}
	}

	return true;
}

LPParticle CLandscape::BulletShoot(int guntype, float x, float y, float angle, float speedmultiplier, bool enemy)
{
	if (pcore->weapons.m_GunAttributes[guntype].BulletParticle==PARTICLE_LANDMINE)
	{
		pcore->enemies.AddEnemy(ENT_E_LANDMINE,x,y,0,0,"","");
		return NULL;
	}


	if (pcore->weapons.m_GunAttributes[guntype].BulletParticle==PARTICLE_STATICBULLET)
	{
		return DoChainsawStyleDamage(guntype, angle, x, y, pcore->weapons.m_GunAttributes[guntype].MuzzleTip, pcore->weapons.m_GunAttributes[guntype].damage);
	}

	float xm = sin(angle);
	float ym = cos(angle);
	float ranangle=angle;

	// create bullet
	LPParticle particle = this->AddParticle(pcore->weapons.m_GunAttributes[guntype].BulletParticle,x,y);

	if (particle!=NULL)
	{
		float speedrandomiser = ((float)((int)(rand()%2000)-1000)   /1000.0f);
		
		// if we have an aim cone - change the aim angles
		if (pcore->weapons.m_GunAttributes[guntype].AimCone>0)
		{
			ranangle = angle + (pcore->weapons.m_GunAttributes[guntype].AimCone*  ((float)((int)(rand()%2000)-1000)   /1000.0f));
			xm = sin(ranangle);
			ym = cos(ranangle);
		}

		particle->rot = ranangle+(D3DX_PI/2);

		particle->xvel = xm*((pcore->weapons.m_GunAttributes[guntype].BulletSpeed)+(pcore->weapons.m_GunAttributes[guntype].BulletSpeedRandomise*speedrandomiser))*(speedmultiplier)*(enemy?SKILL_BULLETSPEED:1.0f);
		particle->yvel = ym*((pcore->weapons.m_GunAttributes[guntype].BulletSpeed)+(pcore->weapons.m_GunAttributes[guntype].BulletSpeedRandomise*speedrandomiser))*(speedmultiplier)*(enemy?SKILL_BULLETSPEED:1.0f);

		particle->damage = pcore->weapons.m_GunAttributes[guntype].damage*(enemy?SKILL_BULLETDAMAGE:1.0f);
		particle->weapon = guntype;
		particle->time = fabs(pcore->weapons.m_GunAttributes[guntype].spawndelay)*-1.0f;

		if (particle->type==PARTICLE_FIRE)
		{
			
			LPParticle nparticle = AddParticle(PARTICLE_FIRE2,x,y);
			if (nparticle!=NULL)
			{
				nparticle->rot = particle->rot;
				nparticle->xvel = particle->xvel*1.1f;
				nparticle->yvel = particle->yvel*1.1f;
				nparticle->x = nparticle->x - (sin(angle)*25);
				nparticle->y = nparticle->y - (cos(angle)*25);
			}
			
			nparticle = AddParticle(PARTICLE_HEATSHIMMER,x,y-10);
			if (nparticle!=NULL)
			{
				nparticle->rot = particle->rot;
				nparticle->xvel = particle->xvel*0.8f;
				nparticle->yvel = particle->yvel*1.1f;
				nparticle->x = nparticle->x - (sin(angle)*25);
				nparticle->y = nparticle->y - (cos(angle)*25);
			}
			
		}
		else
		if (particle->type==PARTICLE_RPG)
		{
			particle->SpawnParticle = PARTICLE_RPG_TRAIL;
			particle->SpawnDistanceReset = 13.0f;
			particle->SpawnDistance = 0;
		}
		else
		{
			if (pcore->timemultiplier!=1.0f)
			{
				particle->SpawnParticle = PARTICLE_SONIC;
				particle->SpawnDistanceReset = 10.0f;
				particle->SpawnDistance = -100;
			}
			
		}
	}

	return particle;

}

void CLandscape::DrawUnder()
{	
	pcore->SetAlphaMode(false);

	int tile=0;
	int posx,posy;
	float fmodx, fmody;
	
	for (int x=0 ; x<(window_width/(m_tilesize*m_zoom))+1; x++)
	{
		for (int y=0 ; y<(window_height/(m_tilesize*m_zoom))+1 ; y++)
		{
			// the position on the tile map
			posx = x+(offset_x/m_tilesize);
			posy = y+(offset_y/m_tilesize);

			if (posx>=0 && posx<width && posy>=0 && posy<height)
			{
				tile = ptilesunder[Map2D(posx,posy)];// tiles[posx][posy];
				if (tile>0)
				{
					fmodx = fmod(offset_x,m_tilesize)*m_zoom;
					fmody = fmod(offset_y,m_tilesize)*m_zoom;

					pcore->gfx.SpriteDrawEx(texturesunder[(tile-1)/49].texture,
						(x*m_tilesize*m_zoom)-fmodx,
						(y*m_tilesize*m_zoom)-fmody,
						0,0,0,
						m_zoom,m_zoom,
						((tile-1)%7)*34+1,
						(((tile-1)/7)%7)*34+1,
						32,32,
						m_globalaplha,255,255,255);
				}

			}
		}
	}
}

void CLandscape::Draw()
{	
	pcore->SetAlphaMode(false);
	int tile=0;
	int posx,posy;
	float fmodx, fmody;
	
	for (int x=0 ; x<(window_width/(m_tilesize*m_zoom))+1; x++)
	{
		for (int y=0 ; y<(window_height/(m_tilesize*m_zoom))+1 ; y++)
		{

			// the position on the tile map
			posx = x+(offset_x/m_tilesize);
			posy = y+(offset_y/m_tilesize);

			if (posx>=0 && posx<width && posy>=0 && posy<height)
			{
				tile = ptiles[Map2D(posx,posy)];// tiles[posx][posy];
				if (tile>0)
				{
					fmodx = fmod(offset_x,m_tilesize)*m_zoom;
					fmody = fmod(offset_y,m_tilesize)*m_zoom;

					pcore->gfx.SpriteDrawEx(textures[(tile-1)/49].texture,
						(x*m_tilesize*m_zoom)-fmodx,
						(y*m_tilesize*m_zoom)-fmody,
						0,0,0,
						m_zoom,m_zoom,
						((tile-1)%7)*34+1,
						(((tile-1)/7)%7)*34+1,
						32,32,
						m_globalaplha,255,255,255);
						
				}

				tile = ptilesover[Map2D(posx,posy)];
				if (tile>0)
				{
					fmodx = fmod(offset_x,m_tilesize)*m_zoom;
					fmody = fmod(offset_y,m_tilesize)*m_zoom;

					pcore->gfx.SpriteDrawEx(texturesover[(tile-1)/49].texture,
						(x*m_tilesize*m_zoom)-fmodx,
						(y*m_tilesize*m_zoom)-fmody,
						0,0,0,
						m_zoom,m_zoom,
						((tile-1)%7)*34+1,
						(((tile-1)/7)%7)*34+1,
						32,32,
						m_globalaplha,255,255,255);
						
				}
			}


		}
	}

	

//	pcore->gfx.DrawTextEx(30,10,255,255,255,"w: %i h: %i  tw: %i th: %i zoom:%f width: %i",window_width,window_height,(int)window_widthtiles,(int)window_heighttiles,m_zoom, width);

	//pcore->gfx.DrawTextEx(10, 300,0,255,0,"particles: %i\ndrawn: %i",m_ParticleCount, m_ParticleDrawnCount);



}

void CLandscape::SetXYoffsets(float x, float y)
{
	targetoffset_x = x;
	targetoffset_y = y;

	if (targetoffset_x>((width*m_tilesize)-((window_widthtiles*m_tilesize)/m_zoom))) 
		targetoffset_x=(width*m_tilesize)-((window_widthtiles*m_tilesize)/m_zoom);
	if (targetoffset_y>((height*m_tilesize)-((window_heighttiles*m_tilesize)/m_zoom))) 
		targetoffset_y=(height*m_tilesize)-((window_heighttiles*m_tilesize)/m_zoom);

	if (targetoffset_x<0) targetoffset_x=0;
	if (targetoffset_y<0) targetoffset_y=0;

	if (offset_x>((width*m_tilesize)-((window_widthtiles*m_tilesize)/m_zoom))) 
		offset_x=(width*m_tilesize)-((window_widthtiles*m_tilesize)/m_zoom);
	if (offset_y>((height*m_tilesize)-((window_heighttiles*m_tilesize)/m_zoom))) 
		offset_y=(height*m_tilesize)-((window_heighttiles*m_tilesize)/m_zoom);

	if (offset_x<0) offset_x=0;
	if (offset_y<0) offset_y=0;
}

void CLandscape::SetXYoffsetsForce(float x, float y)
{
	offset_x = x;
	offset_y = y;
	targetoffset_x = x;
	targetoffset_y = y;

	if (offset_x>((width*m_tilesize)-((window_widthtiles*m_tilesize)/m_zoom))) 
		offset_x=(width*m_tilesize)-((window_widthtiles*m_tilesize)/m_zoom);
	if (offset_y>((height*m_tilesize)-((window_heighttiles*m_tilesize)/m_zoom))) 
		offset_y=(height*m_tilesize)-((window_heighttiles*m_tilesize)/m_zoom);

	if (offset_x<0) offset_x=0;
	if (offset_y<0) offset_y=0;
}

void CLandscape::SetWindowSize()
{
	if (m_tilesize==0) m_tilesize=32;

	if (pcore!=NULL)
	{
		window_width = this->pcore->gfx.m_d3dpp.BackBufferWidth;
		window_height = this->pcore->gfx.m_d3dpp.BackBufferHeight;
		window_widthtiles = ((window_width/m_tilesize));
		window_heighttiles = ((window_height/m_tilesize));
	}
}

void CLandscape::DrawSky()
{

	float sc = ((window_width/4.0f)/256);
	float czoom = m_zoom;

	if (czoom<1) czoom=1;

	if (window_height>window_width) sc = ((window_height/3.0f)/256);

	if (window_height/3>256*sc) sc = ((window_height/3.0f)/256);

	if (sc<1) sc=1;
	int x,y;

	//players[0].m_fX-((gfx.m_d3dpp.BackBufferWidth/2)/landscape.m_zoom

	float offset = fmod((((this->offset_x*0.2)+m_skyx)),256*sc)*czoom;
	int iadd = (int)((((((this->offset_x*0.2)+m_skyx))*czoom)/(256*sc*czoom)))%4;
	int i=0;

	m_skyx	=	m_skyx	+	((0.01*(this->pcore->gfx.tick)*czoom)*m_MapProperties.skyspeed);

	if (m_skyx>sc*4*256) m_skyx=0;

//	pcore->gfx.Clear(255,255,255);
	for (y=-2;y<1;y++)
	{

		for (x=-2;x<3;x++)
		{

			pcore->gfx.SpriteDrawEx(skymap[((x+2+iadd)%4 +(4*(y+2)))%16],
						
						(((window_width/2)*czoom)  + (x*sc*256*czoom))-offset,
						((window_height/2) + (y*sc*256*czoom))+128*sc,
						
						

						0,0,0,
						
						(sc*czoom),(sc*czoom),
						0,0,256,256,
						m_globalaplha,255,255,255
						
						
						
						);



	if (x!=2) i++;

		}
	}

	i=0;

	//pcore->gfx.DrawTextEx(60,60,255,255,255,"sc:%f offset:%i m_skyx:%f",sc,iadd,m_skyx);


}


LPParticle CLandscape::AddParticle(int type, float x, float y)
{
	return AddParticle(type, x, y, 0,0,0,NULL,0);
}

LPParticle CLandscape::AddParticle(int type, float x, float y, float angle, float scalex, float scaley, LPDIRECT3DTEXTURE9 texture, float rotvel)
{
	if ((type==PARTICLE_FIREBIT || type==PARTICLE_FIREBITATTACHED) && !pcore->bProcessingLevelStart)
	{
		int rnum=rand()%6;
		if      (rnum==0) pcore->sound.PlaySnd(s_fire1,x,y);
		else if (rnum==1) pcore->sound.PlaySnd(s_fire2,x,y);
		else if (rnum==2) pcore->sound.PlaySnd(s_fire3,x,y);
		else if (rnum==3) pcore->sound.PlaySnd(s_fire4,x,y);
	}

	float randomiser;

	if (x<=0 || y<=0) return NULL;


	//if (Collision(x,y)) return NULL;

	// create new particle
	LPParticle pointer = new ParticleList();


	randomiser = ( ((rand()%2000)-1000)/1000.0f );
	

	pointer->x = x;
	pointer->y = y;

	pointer->time = 0;

	pointer->type = type;

	randomiser = ( ((rand()%2000)-1000)/1000.0f );
	pointer->alpha = ParticleTypes[type].alphainit + (ParticleTypes[type].alphainitRndm*randomiser);

	randomiser = ( ((rand()%2000)-1000)/1000.0f );
	pointer->alphavel = ParticleTypes[type].alphaovertime + (ParticleTypes[type].alphaovertimeRndm*randomiser);
	
	randomiser = ( ((rand()%2000)-1000)/1000.0f );
	pointer->xscale = ParticleTypes[type].scalexinit + (ParticleTypes[type].scalexinitRndm*randomiser);

	randomiser = ( ((rand()%2000)-1000)/1000.0f );
	pointer->xscalevel = ParticleTypes[type].scalexovertime + (ParticleTypes[type].scalexovertimeRndm*randomiser);

	randomiser = ( ((rand()%2000)-1000)/1000.0f );
	pointer->yscale = ParticleTypes[type].scaleyinit + (ParticleTypes[type].scaleyinitRndm*randomiser);

	randomiser = ( ((rand()%2000)-1000)/1000.0f );
	pointer->yscalevel = ParticleTypes[type].scaleyovertime + (ParticleTypes[type].scaleyovertimeRndm*randomiser);
	
	pointer->keepaspect = ParticleTypes[type].keepaspect;

	randomiser = ( ((rand()%2000)-1000)/1000.0f );
	pointer->xvel = ParticleTypes[type].xvel + (ParticleTypes[type].xvelRndm*randomiser);

	randomiser = ( ((rand()%2000)-1000)/1000.0f );
	pointer->yvel = ParticleTypes[type].yvel + (ParticleTypes[type].yvelRndm*randomiser);

	randomiser = ( ((rand()%2000)-1000)/1000.0f );
	pointer->rot = ParticleTypes[type].rotinit + (ParticleTypes[type].rotinitRndm*randomiser);

	randomiser = ( ((rand()%2000)-1000)/1000.0f );
	pointer->rotvel = ParticleTypes[type].rotvel + (ParticleTypes[type].rotvelRndm*randomiser);
	
	if (ParticleTypes[type].SpawnDelayRndm!=0 || ParticleTypes[type].SpawnDelay!=0)
	{
		randomiser = ( ((rand()%2000)-1000)/1000.0f );
		pointer->time = fabs(ParticleTypes[type].SpawnDelay + (ParticleTypes[type].SpawnDelayRndm*randomiser))*-1.0f;
	}
	
	
	


	if (ParticleTypes[type].gui)
	{
		pointer->xscale = pointer->xscale * pcore->ScreenZoom;
		pointer->yscale = pointer->yscale * pcore->ScreenZoom;
	}

	if (angle!=0) pointer->rot = angle;
	if (scalex!=0) pointer->xscale = scalex;
	if (scaley!=0) pointer->yscale = scaley;
	if (rotvel!=0) pointer->rotvel = rotvel;
	if (texture!=NULL) pointer->textureoverride = texture;

//	pcore->gfx.WriteToLog("setting pointers<br>");

	// if there was a particle before this one, set the next to reference us
	if (m_pLastParticle!=NULL) m_pLastParticle->Next = pointer;
	// if there wasnt, that means we're first.
	else {m_pFirstParticle = pointer;}

	// set us as the last one in the list
	m_pLastParticle = pointer;

	// increase count
	m_ParticleCount++;

	return m_pLastParticle;

}

void CLandscape::RemoveParticle( LPParticle pointer)
{
//	pcore->gfx.WriteToLog("removing particle<br>");
	LPParticle delme=NULL;

	if (pointer==m_pFirstParticle)
	{
	//	pcore->gfx.WriteToLog("1st<br>");
		if (m_pLastParticle==m_pFirstParticle) m_pLastParticle=NULL;
		m_pFirstParticle = pointer->Next;

		delme = pointer;
		delete delme;
		m_ParticleCount--;
	}
	else if  (pointer==m_pLastParticle)
	{
	//	pcore->gfx.WriteToLog("last<br>");
		m_pLastParticle = PreviousParticle(pointer);
		PreviousParticle(pointer)->Next = NULL;

		delme = pointer;
		delete delme;
		m_ParticleCount--;
	}
	else
	{
	//	pcore->gfx.WriteToLog("middle one<br>");
		PreviousParticle(pointer)->Next = pointer->Next;

		delme = pointer;
		delete delme;
		m_ParticleCount--;
	}
}

void CLandscape::RemoveAllParticle()
{
	LPParticle p=m_pFirstParticle, delme=NULL;

	while (p!=NULL)
	{
		delme = p;
		p = p->Next;
		delete delme;
	}

	m_pFirstParticle = NULL;
	m_pLastParticle = NULL;
	m_ParticleCount = 0;
}

LPParticle CLandscape::PreviousParticle(LPParticle index)
{
	LPParticle temp=m_pFirstParticle;

 if(index==m_pFirstParticle) return m_pFirstParticle;
  
 while(temp->Next != index)
 { 
	 temp=temp->Next;
 }

 return temp;
}


void CLandscape::ShutDown()
{
	LPParticle pointer=m_pFirstParticle;
	LPParticle nextpointer=NULL;

	int count=0;
	while (pointer!=NULL)
	{
		nextpointer = pointer->Next;
		RemoveParticle(pointer);
		pointer=nextpointer;
		count++;
	}

	pcore->gfx.WriteToLog("released <b>%i</b> sprites<br>",count);
}


void CLandscape::FreeTextures()
{
	
	int i;

	for (i=0;i<NUM_LANDCAPETEXTURES;i++)
	{
		if (this->textures[i].texture!=NULL)
		{
			this->textures[i].texture->Release();
			this->textures[i].texture=NULL;
			this->pcore->gfx.WriteToLog("<b>Freed Texture:</b> Landscape %i (%s)<br>",i,this->textures[i].path);
		}
	}

	for (i=0;i<NUM_PARTICLETYPES;i++)
	{
		if (this->ParticleTypes[i].particletexture!=NULL)
		{
			this->ParticleTypes[i].particletexture->Release();
			this->ParticleTypes[i].particletexture=NULL;
			this->pcore->gfx.WriteToLog("<b>Freed Texture:</b> Particle %i<br>",i);
		}
	}

	for (i=0;i<16;i++)
	{
		if (this->skymap[i]!=NULL)
		{
			this->skymap[i]->Release();
			this->skymap[i]=NULL;
			this->pcore->gfx.WriteToLog("<b>Freed Texture:</b> skymap %i<br>",i);
		}
	}
	
	

}

void CLandscape::DrawOverlay()
{
	int tile=0;
	int posx,posy;
	float fmodx, fmody;

	for (int x=0 ; x<width ; x++)
	{
		for (int y=0 ; y<height ; y++)
		{

			// the position on the tile map
			posx = x+(offset_x/m_tilesize);
			posy = y+(offset_y/m_tilesize);

			if (posx>=0 && posx<width && posy>=0 && posy<height)
			{
				tile = ptilesover[Map2D(posx,posy)];// tiles[posx][posy];

				if (tile>0)
				{
					fmodx = fmod(offset_x,m_tilesize)*m_zoom;
					fmody = fmod(offset_y,m_tilesize)*m_zoom;

					pcore->gfx.SpriteDrawEx(texturesover[0].texture,
						
						(x*m_tilesize*m_zoom)-fmodx,
						(y*m_tilesize*m_zoom)-fmody,
						
						0,0,0,

						m_zoom,m_zoom,

						1 + (((tile-1)%7)*2)            +  ((int)( (((int)tile-1)%7) *32)%256),

						1 + (((int)(tile-1)/7)*2)    +		((int)((tile-1)/7)*32  ),
						32,
						32,

						255,255,255,255
						);
						
				}
			
			}


		}
	}
}

void CLandscape::GetSpawnPoint(float *x, float *y)
{
	if (spawnx!=0 && spawny!=0)
	{
		*x = spawnx;
		*y = spawny;
		return;
	}
	POINT p[20];
	int i=0;
	LPEntity pent = ents->FirstEnt;
	
	while (pent!=NULL)
	{
		if (pent->type==ENT_SPAWN)
		{
			p[i].x = pent->x;
			p[i].y = pent->y;
			i++;
		}

		pent = pent->Next;
	}

	i = rand()%i;

	*x = p[i].x;
	*y = p[i].y;
	spawnx = *x;
	spawny = *y;

}
/*
static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
	const int N = 5;
	dContact contacts[N];
	CLandscape* lspt;
	lspt = (CLandscape*)data;

	// get the contacts up to a maximum of N contacts
	const int n = dCollide(o1, o2, N, &contacts[0].geom, sizeof(dContact));

	if (dGeomGetClass(o2)==dRayClass)
	{
		CPlayers* plypt;
		plypt = (CPlayers*) dGeomGetData(o2);
		if ((plypt->m_LastAimHitLength>contacts[0].geom.depth || plypt->m_LastAimHitLength==-1) && contacts[0].geom.depth>0)
			{
				plypt->m_LastAimHitLength=contacts[0].geom.depth;
				plypt->m_LastBulletHitPoint.x = contacts[0].geom.pos[0];
				plypt->m_LastBulletHitPoint.y = contacts[0].geom.pos[1];
//				plypt->m_lastHitGeom = o1;
			}

	}
	else
	{
		for(int i = 0; i < n; ++i)
		{
			sPhysicsObject* objectp=NULL;
			objectp = (sPhysicsObject*)dGeomGetData(o1);
			if (objectp==NULL) objectp = (sPhysicsObject*)dGeomGetData(o2);
			if (objectp==NULL) return;

			if (objectp->material==MAT_HEAVYDULL)
			{
				contacts[i].surface.mode = dContactMu2 | dContactApprox1 ;
				contacts[i].surface.mu = 0.7;
				contacts[i].surface.mu2 = 0.7;
			}
			else if (objectp->material==MAT_BOUNCY)
			{
				contacts[i].surface.mode = dContactApprox1 | dContactBounce ;
				contacts[i].surface.mu = 0.2;
				contacts[i].surface.bounce = 0.8;
				contacts[i].surface.bounce_vel = 0.01;
			}

				
			dJointID c = dJointCreateContact(lspt->ode_world, lspt->ode_contactgroup, &contacts[i]);
			dJointAttach(c, dGeomGetBody(contacts[i].geom.g1), dGeomGetBody(contacts[i].geom.g2));
		}

	}

	
}
*/
void CLandscape::DrawPhysicsObjects()
{
	/*
	float stepsize = pcore->gfx.tick/300.0f;

	if (stepsize>0)
	{
		RECT objbounds;
		dReal aabb[6];
		dContact contacts;

		dSpaceCollide (ode_space,this,&nearCallback);
		dWorldStepFast1 (ode_world,stepsize,5);
	//	dWorldStep (ode_world,stepsize);
		dJointGroupEmpty (ode_contactgroup);
	}

	for (int i=0;i<NUM_PHYSOBJ;i++)
	{
		if (phyobjp[i].inuse==true)
		{
				
			const dReal *dr = dBodyGetPosition(phyobjp[i].body);
			const dReal *R2 = dBodyGetRotation (phyobjp[i].body);

			const dReal     *rot = dBodyGetAngularVel (phyobjp[i].body);
			const dReal     *quat_ptr;
			dReal           quat[4],
							quat_len;


			quat_ptr = dBodyGetQuaternion (phyobjp[i].body);

			dReal eul;
			Quat quatl;

			quatl.x = quat_ptr[0];
			quatl.y = quat_ptr[1];
			quatl.z = quat_ptr[2];
			quatl.w = quat_ptr[3];
	
			EulerAngles angles = Eul_FromQuat(quatl,0);

			pcore->gfx.SpriteDraw(phyobjp[i].texture,
								(dr[0]-offset_x-(phyobjp[i].width*phyobjp[i].scalex/2))*m_zoom,(dr[1]-offset_y-(phyobjp[i].height*phyobjp[i].scaley/2))*m_zoom,
								
								angles.x,


									(phyobjp[i].width*phyobjp[i].scalex/2)*m_zoom,
									
									(phyobjp[i].height*phyobjp[i].scaley/2)*m_zoom,

									// scalex
									(phyobjp[i].width/32)*m_zoom*phyobjp[i].scalex,
									// scaley
									(phyobjp[i].height/32)*m_zoom*phyobjp[i].scaley

										
										
										);
		}
	}
	*/
}

void CLandscape::AddPlayerPhysObj(int num)
{
	/*
	dMass m;
	dMassSetBox (&m,1,20,64,32);

	phyobjplayerp[num].inuse=true;
	phyobjplayerp[num].body = dBodyCreate (ode_world);

	dBodySetMass(phyobjplayerp[num].body,&m);

	phyobjplayerp[num].geom = dCreateBox (ode_space,20,64,32);
	dGeomSetBody(phyobjplayerp[num].geom , phyobjplayerp[num].body );
	phyobjplayerp[num].joint = dJointCreatePlane2D (ode_world, 0);
	dJointAttach (phyobjplayerp[num].joint, phyobjplayerp[num].body, 0);
	dBodySetAngularVel(phyobjplayerp[num].body,0,0,0);
	dBodySetLinearVel(phyobjplayerp[num].body,0,0,0);
	*/
}

void CLandscape::AddPhysicalObject(float x, float y, float objwidth, float objheight, unsigned int type, LPSTR texturefilename, unsigned int material, float scalex, float scaley)
{
	/*
	unsigned int onbnum=0;

	for (int i=0;i<NUM_PHYSOBJ;i++)
	{
		if (phyobjp[i].inuse==false)
		{
			onbnum = i;
			break;
		}
	}


	phyobjp[onbnum].type = type;
	phyobjp[onbnum].inuse = true;
	phyobjp[onbnum].width = objwidth;
	phyobjp[onbnum].height = objheight;
	phyobjp[onbnum].material = material;
	phyobjp[onbnum].scalex = scalex;
	phyobjp[onbnum].scaley = scaley;

	// spheres have to be round.
	if (phyobjp[onbnum].type==OBJ_SPHERE) 
	{
		phyobjp[onbnum].height = phyobjp[onbnum].width;
		phyobjp[onbnum].scaley = scalex;
	}

	HRESULT hr;

	hr = D3DXCreateTextureFromFile( pcore->gfx.m_pd3dDevice,texturefilename, &phyobjp[onbnum].texture);
	if (hr!=D3D_OK) { MessageBox(NULL,"Failed to load texture for object..","error",MB_ICONEXCLAMATION); exit(1);}

	


	dMass m;
	dMassSetBox (&m,phyobjp[onbnum].mass,objwidth*phyobjp[onbnum].scalex,objheight*phyobjp[onbnum].scaley,objwidth+objheight);

	phyobjp[onbnum].body = dBodyCreate (ode_world);

	if (type==OBJ_SPHERE)
	phyobjp[onbnum].geom =	dCreateSphere(ode_space, (objwidth*phyobjp[onbnum].scalex)/2);
	else if (type==OBJ_CUBE)
	phyobjp[onbnum].geom = dCreateBox (ode_space, objwidth*phyobjp[onbnum].scalex, objheight*phyobjp[onbnum].scaley, (objwidth+objheight)*phyobjp[onbnum].scalex);

	dBodySetPosition (phyobjp[onbnum].body, x, y, 0);
	dBodySetMass(phyobjp[onbnum].body,&m);
	
	dGeomSetBody(phyobjp[onbnum].geom , phyobjp[onbnum].body );

	phyobjp[onbnum].joint = dJointCreatePlane2D (ode_world, 0);
	dJointAttach (phyobjp[onbnum].joint, phyobjp[onbnum].body, 0);

	dGeomSetData(phyobjp[onbnum].geom,&phyobjp[onbnum]);
	*/

}

void CLandscape::BulletHitLandscape(LPParticle particle)
{
	LPParticle dparticle;
	float angle =  atan2(particle->xvel,particle->yvel);
	float randomnumber = (((rand()%1000))/1000.0f);
	float randomangle;
	float randomstrength = 0.5f+((rand()%800)/1000.0f);

	if (particle->type!=PARTICLE_FIRE)
	{
		//particle->x = particle->x-(sin(angle)*2);
		//particle->y = particle->y-(sin(angle)*2);


		

		// DIRTYSPRAY
		if (particle->type!=PARTICLE_SICKBULLET)
		{
			int ri=3+rand()%5;
			for (int i=0;i<ri;i++)
			{
				dparticle = AddParticle(PARTICLE_DIRTSPRAY, particle->x, particle->y);
				if (dparticle!=NULL)
				{
					randomnumber = (((rand()%10000))/10000.0f)-0.5f;
					randomangle = angle + (randomnumber*0.5f);
					randomnumber = 0.3f+(((rand()%10000))/10000.0f);
					dparticle->xvel = sin(randomangle)*((0.6f*randomnumber));
					if (Collision(dparticle->x+dparticle->xvel,dparticle->y)) dparticle->xvel = -dparticle->xvel;
					//randomnumber = (((rand()%10000))/10000.0f);
					dparticle->yvel = cos(randomangle)*((0.6f*randomnumber));
					if (Collision(dparticle->x,dparticle->y+dparticle->yvel)) dparticle->yvel = -dparticle->yvel;
				}
			}
		}

		float xd=0, yd=0;

		if (this->Collision(particle->x-5,particle->y))
		{
			xd = 1;
		}
		else if (this->Collision(particle->x+5,particle->y))
		{
			xd = -1;
		}

		if (this->Collision(particle->x,particle->y-5))
		{
			yd = 1;
		}
		else if (this->Collision(particle->x,particle->y+5))
		{
			yd = -1;
		}


		// smoke
		if ((particle->weapon!=WEP_CHAINSAW) &&
			(particle->type!=PARTICLE_SICKBULLET))
		{
			for (int i=0;i<8;i++)
			{
				dparticle = pcore->landscape.AddParticle(PARTICLE_BULLETHITSMOKE, particle->x, particle->y);
				if (dparticle!=NULL)
				{
					dparticle->xvel = xd*i*0.3;
					dparticle->yvel = yd*i*0.3;
					dparticle->xscale = dparticle->xscale*i*0.4;
					dparticle->alpha = (255/7.0f)*i;
					if (dparticle->alpha>255) dparticle->alpha=255;

				}
			}
		}

		// SPARK
		if (particle->type!=PARTICLE_SICKBULLET)
		{
			if (rand()%3==1 || (particle->weapon==WEP_CHAINSAW && rand()%2==1) || (particle->type==PARTICLE_EYELASERS))
			{
				AddParticle(PARTICLE_SPARKBALL, particle->x, particle->y);
				int ir = rand()%6;

				if (ir==0)
					pcore->sound.PlaySnd(s_ricochet1,particle->x,particle->y,0.3f);
				else if (ir==2)
					pcore->sound.PlaySnd(s_ricochet2,particle->x,particle->y,0.3f);
				else if (ir==3)
					pcore->sound.PlaySnd(s_ricochet3,particle->x,particle->y,0.3f);
				else if (ir==4)
					pcore->sound.PlaySnd(s_ricochet4,particle->x,particle->y,0.3f);


				for (int i=0;i<((particle->type==PARTICLE_EYELASERS)?40:rand()%100);i++)
					{
						dparticle = AddParticle(PARTICLE_RICHOCHETSPARK, particle->x, particle->y);
						if (dparticle!=NULL)
						{
							randomnumber = (((rand()%10000))/10000.0f)-0.5f;
							randomangle = angle + (randomnumber*0.9f);
							randomnumber = (0.1f+(((rand()%10000))/10000.0f))*randomstrength;
							dparticle->xvel = sin(randomangle)*randomnumber;
							//if (Collision(dparticle->x+dparticle->xvel,dparticle->y)) dparticle->xvel = -dparticle->xvel;
							dparticle->yvel = cos(randomangle)*randomnumber;
							//if (Collision(dparticle->x,dparticle->y+dparticle->yvel)) dparticle->yvel = -dparticle->yvel;
						}
					}
			}
		}

		// explosion stuff
		if (particle->type==PARTICLE_ARROW)
		{
			if (rand()%20==1)
			MakeExplosion(particle->x, particle->y, 3, 30);
		}
		else
		{
			MakeExplosion(particle->x, particle->y, 23, 15);
		}


	} // endif (particle=fire)

	
	if (particle->type==PARTICLE_BULLETBONE)
	{
		pcore->sound.PlaySnd(s_bonehit,particle->x,particle->y,0.2000000f);
		//todo: bone thunk sound
		dparticle = this->AddParticle(PARTICLE_BULLETBONEBOUNCE, particle->x, particle->y);
		if (dparticle!=NULL)
		{
			dparticle->rot = particle->rot;
			dparticle->rotvel = particle->rotvel*(rand()%100/100.0f);
			dparticle->xvel = particle->xvel*0.7f;
			dparticle->yvel = particle->yvel*0.7f;
		}
	}
	else if (particle->type==PARTICLE_EYELASERS)
	{
		//pcore->sound.PlaySnd(s_arrowhit1,particle->x,particle->y,0.5000000f);
		dparticle = this->AddParticle(PARTICLE_EYELASERHIT, particle->x, particle->y );

		dparticle = this->AddParticle(PARTICLE_BUMP, particle->x, particle->y,0,0.5f,0.5f,NULL,0);
		
	}
	else if (particle->type==PARTICLE_ARROW)
	{
		pcore->sound.PlaySnd(s_arrowhit1,particle->x,particle->y,0.5000000f);
		float randdepth= (rand()%10000/5000.f);
		//randdepth = 1;//randdepth-1.0f;
		dparticle = this->AddParticle(PARTICLE_ARROWDEAD,particle->x - (sin(atan2(particle->xvel,particle->yvel)) *randdepth),
													     particle->y - (cos(atan2(particle->xvel,particle->yvel)) *randdepth));
		if (dparticle!=NULL)
		{
			dparticle->rot = particle->rot + (((rand()%20000/10000.f)-1)*0.1f);
			dparticle->IsMoving = false;
		}
	}
	else if (particle->type==PARTICLE_FIRE && rand()%4==0)
	{
		float randdepth= (rand()%10000/5000.f);
		randdepth = randdepth-3.0f;
		dparticle = this->AddParticle(PARTICLE_FIREBIT,particle->x - (sin(atan2(particle->xvel,particle->yvel)) *randdepth),
													     particle->y - (cos(atan2(particle->xvel,particle->yvel)) *randdepth));
		if (dparticle!=NULL)
		{
			dparticle->pplayer = particle->pplayer;
			dparticle->rot = particle->rot + (((rand()%20000/10000.f)-1)*0.1f);
			dparticle->IsMoving = false;
			dparticle->SpawnParticle = PARTICLE_FIREBITRISER;
			dparticle->SpawnTimer = 1;
			dparticle->SpawnTimerReset = 5;
			dparticle->damage = FIRE_DAMAGE;
		}
	}


	//this->AddParticle(0,particle->x+(sin(angle)*3), particle->y+(cos(angle)*3));
	pcore->props.BulletHitProp(particle->x+(sin(angle)*2), 
							   particle->y+(cos(angle)*2), 
							   particle);


}

int CLandscape::RegisterPlayer(CPlayers* player)
{
	for (int i=0;i<MAX_PLAYERS;i++)
	{
		if (playerlist[i].inuse==false)
		{
			playerlist[i].inuse=true;
			playerlist[i].player=player;
			break;
		}
	}
	return i;
}
bool CLandscape::Collision(float x, float y)
{

	if (y>this->height*32) return false;
	if (y<=0) return false;
	if (x>this->width*32) return true;
	if (x<=0) return true;


	if (pcore->gfx.InputKeyDown(DIK_C))
		pcore->gfx.SpriteDrawEx(ParticleTypes[PARTICLE_SPARKBALL].particletexture,	(x*m_zoom)-(this->offset_x*m_zoom),	(y*m_zoom)-(this->offset_y*m_zoom)-1,	16,16,1,1);

	if (y<0 || y/m_tilesize>height || x<0 || x/m_tilesize>width) return true;

	if (ptiles[Map2D((int)(x>0?x/m_tilesize:0),(int)(y/m_tilesize))]!=0)
		return true;

	if (pcore->props.Collision(x,y))
		return true;
	
	//if (pcore->enemies.BulletHitEnemy(0,x,y,NULL))
	//	return true;

	return false;
}


//0 = no hit
//1 = landscape
//2 = person
int CLandscape::CollisionType(float x, float y, LPParticle bullet, bool PossibleHitEnemy)
{
	if (pcore->bPaused) return 0;

	if (bullet!=NULL)
	if (bullet->pplayer==NULL) PossibleHitEnemy=false;

	if (y/m_tilesize<0 || y/m_tilesize>height || x/m_tilesize<0 || x/m_tilesize>width) return 0;

	if (Collision(x,y))
	{
		return 1;
	}

	if (bullet!=NULL)
	if (PossibleHitEnemy)
	if (pcore->enemies.BulletHitEnemy(true,x,y,bullet))
	{
		if (bullet->pplayer==&pcore->players[pcore->MyPlayerID].player)
		{
			pcore->LevelStats.ShotsLanded++;
		}
		return 3;
	}

	bool PossHitPlayer = true;
	
	if (bullet!=NULL)
	{
		if (&pcore->players[pcore->MyPlayerID].player == bullet->pplayer) PossHitPlayer = false;
		if (bullet->type==PARTICLE_FIRE ||
			bullet->type==PARTICLE_FIREBITRISER) PossHitPlayer = true;
	}

	if (PossHitPlayer)
	if (pcore->players[pcore->MyPlayerID].inuse)
		{
			if (pcore->players[pcore->MyPlayerID].player.Collision(x,y))
			{
				if (bullet!=NULL)
				{
					if (bullet->type!=PARTICLE_FIRE && bullet->type!=PARTICLE_FIREBITRISER)
						pcore->players[pcore->MyPlayerID].player.TakeDamage(bullet->damage,bullet->pplayer);
					else
						pcore->players[pcore->MyPlayerID].player.TakeFireDamage(bullet);
				}

				return 2;
			}
		}

	return 0;
}

void CLandscape::BulletHitPlayer(LPParticle particle)
{
	if (pcore->bPaused) return;

	LPParticle dparticle;

	float angle =  atan2(particle->xvel,particle->yvel);
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
		for (int j=0;j<5;j++)
		{
			dparticle = AddParticle(PARTICLE_BLOODBLOBS, particle->x, particle->y,0,0,0,t_BloodBlobs[rand()%NUM_BLOODBLOBS],0);
			if (dparticle!=NULL)
			{
				dparticle->xvel = (sin(randomangle)*(0.08+(randomnumber*0.6))) +  (sin(randomangle)* (j*0.016));
				dparticle->yvel = (cos(randomangle)*(0.08+(randomnumber*0.6))) + (cos(randomangle)* (j*0.016));
			}
		}
	}

	// big up random splash crew
	for (int j=0;j<15;j++)
	{
		dparticle = AddParticle(PARTICLE_BLOODBLOBS, particle->x+rand()%20-10, particle->y+rand()%20-10,0,0,0,t_BloodBlobs[rand()%NUM_BLOODBLOBS],0);
		if (dparticle!=NULL)
		{
			dparticle->xvel = (rand()%1000/1000.0f-0.5f)/3;
			dparticle->yvel = (rand()%1000/1000.0f-0.9f)/3;
		}
	}

	dparticle = AddParticle(PARTICLE_BLOODMIST, particle->x+((rand()%10)-5), particle->y+((rand()%10)-5));
	if (dparticle!=NULL)
		{
			randomangle = angle;
			randomnumber = (((rand()%10000))/10000.0f);
			dparticle->xvel = sin(randomangle)*((0.01*randomnumber));
			if (Collision(dparticle->x+dparticle->xvel,dparticle->y)) dparticle->xvel = -dparticle->xvel;
			dparticle->yvel = cos(randomangle)*((0.01*randomnumber));
			if (Collision(dparticle->x,dparticle->y+dparticle->yvel)) dparticle->yvel = -dparticle->yvel;
			
		}

	if (particle->type==PARTICLE_BULLETBONE)
	{
	//	pcore->sound.PlaySnd(s_arrowhit1,particle->x,particle->y,0.5000000f);
		//todo: bone thunk sound
		dparticle = this->AddParticle(PARTICLE_BULLETBONEBOUNCE, particle->x, particle->y);
		if (dparticle!=NULL)
		{
			dparticle->pplayer = particle->pplayer;
			dparticle->rot = particle->rot;
			dparticle->rotvel = particle->rotvel*(rand()%100/100.0f);
			dparticle->xvel = -particle->xvel*0.7f;
			dparticle->yvel = -particle->yvel*0.7f;
		}
	}
	else if (particle->type==PARTICLE_ARROW)
	{
		pcore->sound.PlaySnd(s_arrowhit1,particle->x,particle->y,0.5000000f);
		float randdepth= (rand()%10000/5000.f);
		randdepth = randdepth-3.0f;
		dparticle = this->AddParticle(PARTICLE_ARROWDEAD,particle->x - (sin(atan2(particle->xvel,particle->yvel)) *randdepth),
													     particle->y - (cos(atan2(particle->xvel,particle->yvel)) *randdepth));
		if (dparticle!=NULL)
		{
			dparticle->pplayer = particle->pplayer;
			dparticle->rot = particle->rot + (((rand()%20000/10000.f)-1)*0.1f);
		}
	}
	else if (particle->type==PARTICLE_FIRE && rand()%2==0)
	{
		float randdepth= (rand()%10000/5000.f);
		randdepth = randdepth-3.0f;
		dparticle = this->AddParticle(PARTICLE_FIREBIT,particle->x - (sin(atan2(particle->xvel,particle->yvel)) *randdepth),
													     particle->y - (cos(atan2(particle->xvel,particle->yvel)) *randdepth));
		if (dparticle!=NULL)
		{
			dparticle->pplayer = particle->pplayer;
			dparticle->rot = particle->rot + (((rand()%20000/10000.f)-1)*0.1f);
			dparticle->IsMoving = false;
			dparticle->SpawnParticle = PARTICLE_FIREBITRISER;
			dparticle->SpawnTimer = 1;
			dparticle->SpawnTimerReset = 5;
			dparticle->damage = FIRE_DAMAGE;
		}
	}
	


}

// used by the AI to find a target
float CLandscape::GetNearestPlayer(float x, float y, CPlayers** player, float* xdist, float* ydist)
{
	float realdistance=0;
	float xdistance=0;
	float ydistance=0;

	*player=NULL;

	float tsqr=0;

	for (int i=0;i<MAX_PLAYERS;i++)
	{
		if (pcore->players[i].inuse)
		{
			if (pcore->players[i].player.bDead!=true)
			{
				tsqr = sqrtf(SQUARED(pcore->players[i].player.m_fX-x)+SQUARED(pcore->players[i].player.m_fY-y));

				if (tsqr<realdistance || realdistance==0)
				{
					realdistance = tsqr;
					xdistance = pcore->players[i].player.m_fX - x;
					ydistance = pcore->players[i].player.m_fY - y;
					*player = &pcore->players[i].player;
				}
			}
		}
	}

	*xdist = xdistance;
	*ydist = ydistance;
	return realdistance;
}



bool CLandscape::IsLadder(float x, float y)
{
	return pcore->ents.EntityHere(7, x, y);
}

void CLandscape::UpdateCamera(void)
{
	if (pcore->bShowBuyMenu) return;
	// camera
	if (offset_x!=targetoffset_x) 
	{
		pcore->screenmovex = offset_x - ((targetoffset_x+(offset_x*5))/6.0f);
		offset_x=(int)((targetoffset_x+(offset_x*5))/6.0f);
	}
	
	if (offset_y!=targetoffset_y) 
	{
		pcore->screenmovey = offset_y - ((targetoffset_y+(offset_y*5))/6.0f);
		//pcore->screenmovey = ((pcore->screenmovey*10 + (offset_y-((targetoffset_y+(offset_y*5))/6.0f)))/11.0f);
		offset_y=(int)((targetoffset_y+(offset_y*20))/21.0f);
	}

}

void CLandscape::DoWeather(void)
{
	if (enumWeatherMode==WEATHER_OFF) return;

	fWeatherCounter = fWeatherCounter - 0.1*pcore->gfx.tick;

	if (this->fWeatherCounter>0) return;
	
	LPParticle particle=NULL;

	int particles=0;

	if (enumWeatherMode==WEATHER_WINDYRAIN)
	{
		particles = 60+rand()%60;
	}
	else
	{
		return;
	}

	for (int i=0;i<particles;i++)
	{
		particle = AddParticle(PARTICLE_WEATHER_RAIN,offset_x-200+rand()%((int)this->window_width+3000),offset_y-100-rand()%100);

		if (particle!=NULL)
		{
			particle->IsMoving = true;
			particle->xvel = (-1.8f*particle->xscale)+(rand()%1000/2000.0f);
			particle->yvel = (0.7f*particle->xscale)+(rand()%1000/2000.0f);
		}
	}

	if (enumWeatherMode==WEATHER_WINDYRAIN)
	{
		particles = rand()%2;
	}
	else
	{
		particles = 0;
	}


	//PARTICLE_WEATHER_MIST
	for (int i=0;i<particles;i++)
	{
		particle = AddParticle(PARTICLE_WEATHER_MIST,offset_x+1300+(rand()%300),offset_y-200+(rand()%800));

		if (particle!=NULL)
		{
			particle->IsMoving = true;
		}
	}

	

	if (enumWeatherMode==WEATHER_WINDYRAIN)
	{
		fWeatherCounter = 20;
		if (rand()%5==0 && pcore->Settings.Bumping)
			AddParticle(PARTICLE_RAINSCREENDROP,rand()%1024,rand()%768);
	}



	



}

// this just generates a shockwave - moving every particle in its radius
void CLandscape::MakeExplosion(float x, float y, int size, float power)
{
	LPParticle pointer=m_pFirstParticle;
	LPParticle nextpointer=NULL;
	float distance, randinator, powermp;

	powermp= power/size;


	y=y+2;

	while (pointer!=NULL)
	{
		if (ParticleTypes[pointer->type].AffectedByExplosions)
		if (pointer->x>x-size)
		if (pointer->x<x+size)
		if (pointer->y>y-size)
		if (pointer->y<y+size)
		{
			if (pointer->type==PARTICLE_HANDGRENADE || pointer->type==PARTICLE_IMPACTGRENADE) pointer->time = ParticleTypes[pointer->type].life-0.5;
			else
			{
				randinator = 0.9f + rand()%10000/50000.0f;
				distance = sqrtf(SQUARED(pointer->x-x) + (SQUARED(pointer->y-y)));
				pointer->yvel = pointer->yvel + cos(atan2(pointer->x-x, pointer->y-y))*(size*powermp)*0.01*randinator;
				pointer->xvel = pointer->xvel + sin(atan2(pointer->x-x, pointer->y-y))*(size*powermp)*0.01*randinator;
				pointer->IsMoving = true;
				randinator = rand()%10000/10000.0f;
				pointer->rotvel = (size*powermp)*0.001*randinator;
			}
		}


		nextpointer = pointer->Next;

		pointer=nextpointer;
	}

}

void CLandscape::DrawExplosion(float x, float y, int size)
{
	LPParticle prtcle;

//	AddParticle(PARTICLE_EXPLOSIONWAVE,x,y);

	for (int i=0;i<3;i++)
	{
		AddParticle(PARTICLE_EXPLOSIONSMOKE,x,y);
	}

	for (i=0;i<(size/5);i++)
	{
		AddParticle(PARTICLE_EXPLOSIONORANGE,x,y);
	}

	for (i=0;i<(size/20);i++)
	{
		prtcle = AddParticle(PARTICLE_EXPLOSIONBIT,x,y);
		if (prtcle!=NULL)
		{
			prtcle->SpawnParticle = PARTICLE_EXPLOSIONBITSMOKE;
			prtcle->SpawnDistanceReset = 20.0f;
			prtcle->SpawnDistance = -150;
		}
	}

	//AddParticle(PARTICLE_EXPLOSIONRING,x,y);
	AddParticle(PARTICLE_EXPLOSIONWAVE,x,y);
	AddParticle(PARTICLE_EXPLOSIONWAVE,x,y);
	AddParticle(PARTICLE_EXPLOSIONWAVE,x,y);


	if (pcore->Settings.camerascorch)
	{
		float ex, ey;
		ex = x - pcore->landscape.offset_x;
		ey = y - pcore->landscape.offset_y;
		if (ex>0 && ey>0 && ex<pcore->gfx.m_d3dpp.BackBufferWidth && ey<pcore->gfx.m_d3dpp.BackBufferHeight)
			AddParticle(PARTICLE_GUI_EXPLOSION,ex,ey);
	}

	pcore->sound.PlaySnd(s_explosion,x,y,0.2f);
	pcore->sound.PlaySnd(s_explosion,x,y,0.1f,-5);
	pcore->sound.PlaySnd(s_explosion,x,y,0.1f,-6);


	LPParticle dparticle;
	float randomnumber, randomangle;
	for (int i=0;i<200;i++)
		{
			dparticle = AddParticle(PARTICLE_RICHOCHETSPARK,  x, y);
			if (dparticle!=NULL)
			{
				randomnumber = (((rand()%10000)-5000)/10000.0f);
				dparticle->xvel = 1.8*randomnumber;
				randomnumber = (((rand()%10000)-5000)/10000.0f);
				dparticle->yvel = 1.8*randomnumber;
			}
		}

	ScreenShake(100,100);
	
}

bool CLandscape::DoSplashDamage(float x, float y, float size, float damage, CPlayers* attacker)
{
	bool returnbool=false;
	// out of map
	if (y<0 || y/m_tilesize>height || x<0 || x/m_tilesize>width) return false;

	if (pcore->enemies.SplashDamageEnemy(x, y, size, damage, attacker))
	{
		returnbool= true;
	}

	if (pcore->props.SplashtHitProp(x,y,size, damage))
	{
		returnbool = true;
	}

	for (int i=0;i<MAX_CLIENTS;i++)
	{
		if (pcore->players[i].inuse)
		{
			if (pcore->players[i].player.SplashDamage(x, y, size, damage, attacker))
			{
				returnbool= true;
			}
		}
	}

	return returnbool;
}

void CLandscape::DoBounceSound(LPParticle pointer)
{
	float volume;

	volume = pointer->yvel;
	

	if (volume>1) volume=1;
	if (volume<0) return;

	if (pointer->type==PARTICLE_SHELL) 
	{
		pcore->sound.PlaySnd(s_9mmbounce1,pointer->x, pointer->y, 0.4f);
	}
	else if (pointer->type==PARTICLE_SHOTGUNSHELL) 
	{
		pcore->sound.PlaySnd(s_buckshotbounce1,pointer->x, pointer->y, 0.3f);
	}
	else if (pointer->type==PARTICLE_RIFLESHELL) 
	{
		pcore->sound.PlaySnd(s_riflebounce1,pointer->x, pointer->y, 0.4f);
	}
	else if (pointer->type==PARTICLE_CHAINSHELL)
	{
		pcore->sound.PlaySnd(s_chaingunbounce1,pointer->x, pointer->y, 0.4f);
	}
	else if (pointer->type==PARTICLE_HANDGRENADE)
	{
		pcore->sound.PlaySnd(s_grenadebounce1,pointer->x, pointer->y, 0.3f);	
	}
	else if (pointer->type==PARTICLE_CLIP)
	{
		if (rand()%2==0)
			pcore->sound.PlaySnd(s_clipbounce1,pointer->x, pointer->y, 0.2f);	
		else
			pcore->sound.PlaySnd(s_clipbounce2,pointer->x, pointer->y, 0.2f);	
	}
}

void CLandscape::LoadMapTextures(bool CrashOnError)
{
	char tempstring[500];
	HRESULT hr;

	for (int i=0;i<NUM_LANDCAPETEXTURES;i++)
	{
		if (strcmp(textures[i].path,"")!=0)
		{
			sprintf(tempstring,"textures/world/%s",textures[i].path);
			textures[i].texture = pcore->textures.LoadTexture(tempstring);
		}

		if (strcmp(texturesover[i].path,"")!=0)
		{
			sprintf(tempstring,"textures/world/%s",texturesover[i].path);
			texturesover[i].texture = pcore->textures.LoadTexture(tempstring);
		}

		if (strcmp(texturesunder[i].path,"")!=0)
		{
			sprintf(tempstring,"textures/world/%s",texturesunder[i].path);
			texturesunder[i].texture = pcore->textures.LoadTexture(tempstring);
		}
	}

	pcore->gfx.WriteToLog("Finished Loading Level Textures<br>");

}

void CLandscape::LoadSky(char* skyname)
{
	HRESULT hr;	
	char filename[500];

	for (int i=1;i<=12;i++)
	{
		sprintf(filename, "textures/skys/%s/%i.jpg",skyname,i);
		skymap[i-1] = pcore->textures.LoadTexture(filename);

		//hr = D3DXCreateTextureFromFileEx( pcore->gfx.m_pd3dDevice,filename,256,256,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED ,D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, NULL,  NULL, &skymap[i-1]); //D3DX_FILTER_NONE 

		//if (hr!=D3D_OK) { MessageBox(NULL,"Failed to load a sky texture (needed 1-12)","error",MB_ICONEXCLAMATION); exit(1);}
	}

}

void CLandscape::FocusCameraOnPlayer(void)
{
	pcore->UpdateCursorPosition();
	SetXYoffsetsForce(pcore->players[pcore->MyPlayerID].player.m_fX-((pcore->gfx.m_d3dpp.BackBufferWidth/2)/m_zoom) + ((( pcore->x-(pcore->gfx.m_d3dpp.BackBufferWidth/2) )*0.8)/m_zoom),  
							pcore->players[pcore->MyPlayerID].player.m_fY-((pcore->gfx.m_d3dpp.BackBufferHeight/2)/m_zoom)+ ((( pcore->y-(pcore->gfx.m_d3dpp.BackBufferHeight/2) )*0.8)/m_zoom));

}

void CLandscape::DrawRect(RECT *rct)
{
	pcore->gfx.SpriteDrawEx(this->ParticleTypes[PARTICLE_SPARKBALL].particletexture,
					(rct->left-offset_x)*m_zoom,
					(rct->top-offset_y)*m_zoom,
					0,0,0,
					abs(rct->left-rct->right)*m_zoom,
					abs(rct->top-rct->bottom)*m_zoom,
					16,16,1,1,
					200,255,255,255);
}

void CLandscape::DrawRect(GRECT *rct)
{
	pcore->gfx.SpriteDrawEx(this->ParticleTypes[PARTICLE_SPARKBALL].particletexture,
					(rct->left-offset_x)*m_zoom,
					(rct->top-offset_y)*m_zoom,
					0,0,0,
					abs(rct->left-rct->right)*m_zoom,
					abs(rct->top-rct->bottom)*m_zoom,
					16,16,1,1,
					100,255,255,255);
}
void CLandscape::GenericBloodSpurt(float x, float y, float angle, float speed, CPlayers* player)
{
	LPParticle dparticle;

	float randomnumber = (((rand()%1000)-500)/1000.0f);
	float randomnumber2 = (((rand()%1000)-500)/1000.0f);
	float ultrandomangle = angle;//+(0.2*randomnumber);
	float randomangle;

	float xvadd=0,yvadd=0;

	if (player!=NULL)
	{
		xvadd = player->m_fXVel;
		yvadd = player->m_fYVel;
	}

	if (xvadd>0.1f) xvadd=0.1f;
	if (yvadd>0.1f) yvadd=0.1f;

	speed= speed*0.8f;

	for (int i=0;i<2;i++)
	{
		randomnumber = (((rand()%1000)-500)/1000.0f);
		ultrandomangle = angle+(0.8*randomnumber);
		randomnumber = (((rand()%10000))/10000.0f);
		randomnumber2 = (((rand()%10000))/10000.0f);
		randomnumber = (((rand()%10000))/10000.0f);
		randomangle = ultrandomangle;//+(0.05*randomnumber);
		for (int j=0;j<6;j++)
		{
			dparticle = AddParticle(PARTICLE_BLOODBLOBS, x, y,0,0,0,t_BloodBlobs[rand()%NUM_BLOODBLOBS],0);
			if (dparticle!=NULL)
			{
				dparticle->xvel = (((sin(randomangle)*(0.08+(randomnumber*0.6))) +  (sin(randomangle)* (j*0.016)))*speed)/(j*0.2f);
				dparticle->yvel = (((cos(randomangle)*(0.08+(randomnumber*0.6))) + (cos(randomangle)* (j*0.016)))*speed)/(j*0.2f);
				dparticle->yvel = dparticle->yvel - 0.1f;
				dparticle->time = (j)*-0.1f;
				dparticle->xscale = dparticle->xscale*1.5f;
				dparticle->yscale = dparticle->yscale*1.5f;
				if (player!=NULL)
				{
					dparticle->xvel =dparticle->xvel + xvadd;
					dparticle->yvel =dparticle->yvel + yvadd - 0.1f;
				}
			}
		}
	}

	dparticle = AddParticle(PARTICLE_BLOODMIST, x+((rand()%10)-5), y+((rand()%10)-5));
	if (dparticle!=NULL)
		{
			randomangle = angle;
			randomnumber = (((rand()%10000))/10000.0f);
			dparticle->xvel = sin(randomangle)*((0.01*randomnumber));
			if (pcore->landscape.Collision(dparticle->x+dparticle->xvel,dparticle->y)) dparticle->xvel = -dparticle->xvel;
			dparticle->yvel = cos(randomangle)*((0.01*randomnumber));
			if (pcore->landscape.Collision(dparticle->x,dparticle->y+dparticle->yvel)) dparticle->yvel = -dparticle->yvel;
			
		}

	for (int j=0;j<3;j++)
	{
		dparticle = AddParticle(PARTICLE_BLOODBLOBS, x+rand()%20-10, y+rand()%20-10,0,0,0,t_BloodBlobs[rand()%NUM_BLOODBLOBS],0);
		if (dparticle!=NULL)
		{
			dparticle->xvel = ((rand()%1000/1000.0f-0.5f)/3)*speed;
			dparticle->yvel = ((rand()%1000/1000.0f-0.9f)/3)*speed;
			if (player!=NULL)
			{
					dparticle->xvel =dparticle->xvel + xvadd;
					dparticle->yvel =dparticle->yvel + yvadd;
			}
		}
	}

}
bool CLandscape::Collision(GRECT* rct)
{
	if (pcore->gfx.InputKeyDown(DIK_C))
	pcore->gfx.SpriteDrawEx(ParticleTypes[PARTICLE_SPARKBALL].particletexture,	
	(rct->left*m_zoom)-(this->offset_x*m_zoom),	
	(rct->top*m_zoom)-(this->offset_y*m_zoom)-1,
	0,0,0,
	rct->right-rct->left,
	rct->bottom-rct->top, 
	
	16,16,1,1,200,255,255,255);



	if (rct->top > this->height*32) return false;
	if (rct->bottom <= 0) return false;
	if (rct->right > this->width*32) return false;
	if (rct->left <= 0) return false;

	for (int x=floor(rct->left/m_tilesize) ; x<=floor(rct->right/m_tilesize); x++)
	{
		for (int y=floor(rct->top/m_tilesize) ; y<=floor(rct->bottom/m_tilesize);y++)
		{
			if (TileMapCollision(x,y))
				return true;
		}		
	}

	if (pcore->props.Collision(rct)) return true;
	
	return false;
}

bool CLandscape::TileMapCollision(int x, int y)
{
	if (x<0 || y<0) return false;
	if (y>=this->height) return false;
	if (x>=this->width) return false;

	if (ptiles[Map2D(x,y)]!=0) 
	{
		int a=0;
		return true;
	}

	return false;
}



void CLandscape::ScreenShake(float x, float y)
{
	x = x * (((rand()%2000)-1000)/1000.0f);
	y = y * (((rand()%2000)-1000)/1000.0f);

	offset_x = offset_x + x;
	offset_y = offset_y + y;
	if (offset_x<0) offset_x=0;
	if (offset_y<0) offset_y=0;
}

void CLandscape::ChangeMap(char* name)
{
	pcore->Loading(25);
	pcore->LevelStats.Enemies = 0;
	pcore->LevelStats.Plutonium = 0;
	pcore->LevelStats.TimeTaken = 0;

	this->LoadMap(name);
}

void CLandscape::QueueUpIntroVoice(void)
{
	unsigned int rnum =  rand()%150;

	switch (rnum)
	{
		case 1:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go1.ogg");
			break;
		case 2:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go2.ogg");
			break;
		case 3:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go3.ogg");
			break;
		case 4:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go4.ogg");
			break;
		case 5:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go5.ogg");
			break;
		case 6:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go6.ogg");
			break;
		case 7:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go7.ogg");
			break;
		case 8:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go8.ogg");
			break;
		case 9:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go9.ogg");
			break;
		case 10:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go10.ogg");
			break;
		case 11:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go11.ogg");
			break;
		case 12:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go12.ogg");
			break;
		case 13:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go13.ogg");
			break;
		case 14:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go14.ogg");
			break;
		case 15:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go15.ogg");
			break;
		case 16:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go16.ogg");
			break;
		case 17:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go17.ogg");
			break;
		case 18:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go18.ogg");
			break;
		case 19:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go19.ogg");
			break;
		case 20:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go20.ogg");
			break;
		case 21:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go21.ogg");
			break;
		case 22:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go22.ogg");
			break;
		case 23:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go23.ogg");
			break;
		case 24:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go24.ogg");
			break;
		case 25:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go25.ogg");
			break;
		case 26:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go26.ogg");
			break;
		default:
			pcore->s_go = pcore->sound.LoadSound(7,"sound/announcer/go.ogg");
			break;

	}
}

LPParticle CLandscape::DoChainsawStyleDamage(unsigned int guntype, float angle, float x, float y, float length, float damage)
{
	LPParticle particle = this->AddParticle(PARTICLE_STATICBULLET,x-(sin(angle)*length),y-(cos(angle)*length));

	if (particle!=NULL)
	{
		particle->xvel = sin(angle)*length;
		particle->yvel = cos(angle)*length;
		particle->damage = pcore->weapons.m_GunAttributes[guntype].damage;
		particle->weapon = guntype;
		particle->time = fabs(pcore->weapons.m_GunAttributes[guntype].spawndelay)*-1.0f;

	}

	return particle;
}

void CLandscape::DrawScreenFade(void)
{
	if (!ScreenFade.active) return;

	if (ScreenFade.color==0)
	{
		pcore->SetAlphaMode(true);
	}
	else
	{
		pcore->SetAlphaMode(false);
	}

    pcore->gfx.SpriteDrawEx(pcore->t_color,
					0,0,
					0,0,0,
					pcore->gfx.m_d3dpp.BackBufferWidth,
					pcore->gfx.m_d3dpp.BackBufferHeight,
					ScreenFade.color*3,0,1,1,
					(int)ScreenFade.alpha, 255, 255, 255);

	ScreenFade.alpha = ScreenFade.alpha + ScreenFade.speed*pcore->gfx.RealTick;

	if (ScreenFade.speed<0 && ScreenFade.alpha<0)
	{
		ScreenFade.speed = 0;
		ScreenFade.active = false;
	}
	else if (ScreenFade.speed>0 && ScreenFade.alpha>255)
	{
		ScreenFade.speed = 0;
		ScreenFade.alpha = 255;
	}
}

void CLandscape::LoadLevelShader(void)
{
	char filename[300];

	sprintf(filename,"%s/maps/%s.fx",pcore->modfolder,this->m_currentlevel);
	if (!FileExists(filename))
	{
		sprintf(filename, "%s/maps/%s.fx", DEFAULT_MOD_FOLDER,m_currentlevel);
	}

	if (FileExists(filename))
	{
		HRESULT hr;

		hr = D3DXCreateEffectFromFile( pcore->gfx.m_pd3dDevice, 
		                           filename,
		                           NULL, 
		                           NULL, 
		                           0, 
		                           NULL, 
		                           &pcore->Effect, 
		                           NULL );
		if( FAILED(hr) )
		{
			pcore->gfx.WriteToLog("We found an fx file but it didn't compile right<br>", filename);
			pcore->Effect= NULL;
		}
		else
		{
			pcore->gfx.WriteToLog("fx file loaded!<br>", filename);
		}
	}

	/// load the global shader shamone
	sprintf(filename,"%s/data/globalshader.fx",pcore->modfolder);
	if (!FileExists(filename))
	{
		sprintf(filename, "%s/data/globalshader.fx", DEFAULT_MOD_FOLDER);
	}

	if (FileExists(filename))
	{
		HRESULT hr;

		hr = D3DXCreateEffectFromFile( pcore->gfx.m_pd3dDevice, 
		                           filename,
		                           NULL, 
		                           NULL, 
		                           0, 
		                           NULL, 
		                           &pcore->GlobalEffect, 
		                           NULL );
		if( FAILED(hr) )
		{
			MessageBox(NULL,"Error compiling the global fx file.","Shader Error",0);
			pcore->gfx.WriteToLog("We found the global fx file but it didn't compile right<br>", filename);
			pcore->GlobalEffect= NULL;
		}
		else
		{
			pcore->gfx.WriteToLog("Global fx file loaded!<br>", filename);
		}
	}
	
}

void CLandscape::WaterBullet(int x)
{
	LPParticle dparticle;

	for (int j=0;j<12;j++)
	{
		dparticle = AddParticle(PARTICLE_WATERSPLASH, x, this->WaterLine);
	}

	for (int j=0;j<3;j++)
	{
		dparticle = AddParticle(PARTICLE_WATERSPLASHMIST, x, this->WaterLine);
	}

	AddParticle(PARTICLE_WATERRIPPLE, x, this->WaterLine-5);
}

void CLandscape::WaterLargeObjectPlunk(int x)
{
	LPParticle dparticle;

	for (int j=0;j<24;j++)
	{
		dparticle = AddParticle(PARTICLE_WATERSPLASH, x+(rand()%16-8), this->WaterLine);
	}

	for (int j=0;j<6;j++)
	{
		dparticle = AddParticle(PARTICLE_WATERSPLASHMIST, x+(rand()%16-8), this->WaterLine);
	}

	AddParticle(PARTICLE_WATERRIPPLE, x, this->WaterLine-5);
	
}

void CLandscape::WaterTinySplash(int x)
{
	LPParticle dparticle;

	for (int j=0;j<6;j++)
	{
		dparticle = AddParticle(PARTICLE_WATERSPLASH, x, this->WaterLine);
		if (dparticle!=NULL)
		{
			dparticle->yvel = dparticle->yvel*0.7;
			dparticle->xscale = dparticle->xscale*0.6;
			dparticle->yscale = dparticle->yscale*0.6;
		}
	}

	for (int j=0;j<1;j++)
	{
		dparticle = AddParticle(PARTICLE_WATERSPLASHMIST, x, this->WaterLine);
		if (dparticle!=NULL)
		{
			dparticle->xscale = dparticle->xscale*0.6;
			dparticle->yscale = dparticle->yscale*0.6;
		}
	}

	AddParticle(PARTICLE_WATERRIPPLE, x, this->WaterLine-5);
}



LPParticle CLandscape::AddSpark(int x, int y, float angle, float speedmultiplier)
{
	LPParticle lp=NULL;
	lp = AddParticle(PARTICLE_RICHOCHETSPARK,x,y);
	if (lp!=NULL)
	{
		lp->xvel = sin(angle)*speedmultiplier;
		lp->yvel = cos(angle)*speedmultiplier;
	}

	return lp;
}

LPParticle CLandscape::AddStoneDust(int x, int y, float scalemultiply)
{
	LPParticle lp=NULL;
	lp = AddParticle(PARTICLE_BONEDUST,x,y);
	if (lp!=NULL)
	{
		lp->xscale = lp->xscale*scalemultiply;
		lp->yscale = lp->yscale*scalemultiply;
	}

	return lp;
}

void CLandscape::DrawWaterLayer(LPDIRECT3DTEXTURE9 texture, float offsetx, float scalex, float scaley, int alpha)
{
	if (alpha>255) alpha = 255;
	float x, y;
	for (int i=-1;i<100;i++)
	{
		x = (i*(512.0f*scalex*m_zoom))+(fmod(offsetx-offset_x*m_zoom,512.0f*scalex*m_zoom));

		if (x>pcore->gfx.m_d3dpp.BackBufferWidth) break;

		y = (WaterLine-offset_y-1.0f)*m_zoom;

		pcore->gfx.SpriteDrawFull(texture,
						x,y,
						0,0,0,
						scalex*m_zoom,
						scaley*m_zoom,
						0,0,
						512,256,
						alpha,255,255,255);
	}
}

void CLandscape::DrawNonShaderWater(void)
{
	if (WaterLine>0)
	{	
		pcore->SetAlphaMode(false);
		DrawWaterLayer(t_water,0,1.0f,1.5f+(sin((pcore->TimerA)*0.001*WaterEffect_Speed)*0.1),255);
		DrawWaterLayer(t_water,(pcore->TimerA*0.055*WaterEffect_Speed),1.0f,1.5f+(sin((pcore->TimerA)*0.001*WaterEffect_Speed)*0.1),300*WaterEffect_Size);
		DrawWaterLayer(t_water,(pcore->TimerA*-0.055*WaterEffect_Speed),0.9f,1.5f+(cos((pcore->TimerA)*0.0043*WaterEffect_Speed)*0.1),180*WaterEffect_Size);
		pcore->SetAlphaMode(true);
		DrawWaterLayer(t_water,(pcore->TimerA*-0.1*WaterEffect_Speed),0.9f,1.5f+(sin((pcore->TimerA+(D3DX_PI/2.0f))*0.002*WaterEffect_Speed)*0.09),250*WaterEffect_Size);
		DrawWaterLayer(t_water,(pcore->TimerA*0.1*WaterEffect_Speed),1.1f,1.5f+(cos((pcore->TimerA+(D3DX_PI/2.0f))*0.002*WaterEffect_Speed)*0.09),250*WaterEffect_Size);
		pcore->SetAlphaMode(false);
	}
}

void CLandscape::Lightning(float x, float y, float destx, float desty, int splits, float randomness, float width, bool glow, int chanceofbreakoff)
{
	POINT points[100];
	if (splits>99) splits = 99;

	points[0].x = x;
	points[0].y = y;
	points[1+splits].x = destx;
	points[1+splits].y = desty;

	float angle = atan2(points[1+splits].x-points[0].x,points[1+splits].y-points[0].y);
	float distance = sqrtf(SQUARED(points[1+splits].x-points[0].x)+SQUARED(points[1+splits].y-points[0].y));
	float crazyness;

	for (int i=1;i<splits+1;i++)
	{
		crazyness = ((rand()%2000/1000.0f)-1.0f)*randomness;
		points[i].x = points[i-1].x + sin(angle+crazyness)*((1/(float)splits))*distance;
		points[i].y = points[i-1].y + cos(angle+crazyness)*((1/(float)splits))*distance;
	}

	// todo - random points in between

	for (int i=0;i<splits+1;i++)
	{
		if (chanceofbreakoff>0)
		if (rand()%chanceofbreakoff==0)
		{
			this->Lightning(points[i].x, 
							points[i].y,
							points[i].x+(((rand()%2000)-1000)/20.0f), 
							points[i].y+(((rand()%2000)-100)/20.0f),
							4,0.7,width*0.6f,0,100);
		}

		this->AddParticle(PARTICLE_LTNG_BEAM,
			points[i].x, 
			points[i].y,
			atan2(points[i+1].x-points[i].x,points[i+1].y-points[i].y),
			width,
			sqrtf(SQUARED(points[i+1].x-points[i].x)+SQUARED(points[i+1].y-points[i].y))/8.0f, // distance between teh 2 points
			NULL,0);

		if (glow)
		{
			this->AddParticle(PARTICLE_LTNG_GLOW,
			points[i].x, 
			points[i].y,
			atan2(points[i+1].x-points[i].x,points[i+1].y-points[i].y),
			width*10,
			width*20,
			//sqrtf(SQUARED(points[i+1].x-points[i].x)+SQUARED(points[i+1].y-points[i].y))/32.0f, // distance between teh 2 points
			NULL,0);
		}
	}

}
