#ifndef __INCLANDSCAPE
#define __INCLANDSCAPE

#include "Globals.h"
#include "zlib/zlib.h"
#include "garryswrappers/GarrysGraphicsWrapper.h"
#include "GarrysSoundWrapper.h"
#include "fmod.h"
//#include "ode/ode.h"

#define NUM_LANDCAPETEXTURES 10
#define NUM_PARTICLETYPES 100
#define NUM_PHYSOBJ	1
#define NUM_PHYSOBJPLAYERS	30
#define NUM_BLOODDRIPS 6
#define NUM_BLOODBLOBS 3

#define PARTICLE_LANDMINE 10001 // fake - it isn't a particle at all - it's an enemy

#define PARTICLE_SPARKBALL 0
#define PARTICLE_DIRTSPRAY 1
#define PARTICLE_RICHOCHETSPARK 2
#define PARTICLE_SHELL 3
#define PARTICLE_GUNSMOKE 4
#define PARTICLE_MUZZLEFLASH 5
#define PARTICLE_GUNSPARK 7
#define PARTICLE_BULLET 8
#define PARTICLE_CLIP 9
#define PARTICLE_BUCKSHOT 10
#define PARTICLE_BLOODBLOBS 11
#define PARTICLE_BLOODMIST 12
#define PARTICLE_BULLETHITSMOKE 13

#define PARTICLE_ZOMBIE_HEAD 14
#define PARTICLE_ZOMBIE_ARM 15

#define PARTICLE_SPAWNLIGHT 16
#define PARTICLE_ZOMBIEATTACK 17
#define PARTICLE_SHOTGUNSHELL 18
#define PARTICLE_RIFLESHELL 19
#define PARTICLE_ARROW 20
#define PARTICLE_ARROWDEAD 21
#define PARTICLE_CHAINSHELL 22
#define PARTICLE_WEATHER_RAIN 23
#define PARTICLE_RAINSPLASH 24
#define PARTICLE_IMPACTGRENADE 25

#define PARTICLE_EXPLOSIONORANGE 26
#define PARTICLE_EXPLOSIONSMOKE 27
#define PARTICLE_EXPLOSIONBIT 28
#define PARTICLE_EXPLOSIONBITSMOKE 29

#define PARTICLE_TRACER 30

#define PARTICLE_EXPLOSIONRING 31

#define PARTICLE_HANDGRENADE 32

#define PARTICLE_EXPLOSIONWAVE 33

#define PARTICLE_GIB_CRATE 34
#define PARTICLE_GIB_CRATE2 35

#define PARTICLE_GIB_METAL 36
#define PARTICLE_GIB_METAL2 37

#define PARTICLE_GENERIC_32X16 38
#define PARTICLE_GENERIC_32X32 39
#define PARTICLE_GENERIC_16X64 40
#define PARTICLE_GENERIC_16X32 42

#define PARTICLE_GENERIC_64X64 41


#define PARTICLE_LANDMINEGREEN	43
#define PARTICLE_LANDMINERED	44


#define PARTICLE_GUI_EXPLOSION 46

#define PARTICLE_RPG 47
#define PARTICLE_RPG_TRAIL 48

#define PARTICLE_FIRE 49
#define PARTICLE_FIREBIT 50
#define PARTICLE_FIREBITRISER 51

#define PARTICLE_FIREBITATTACHED 52
#define PARTICLE_FIREBITRISERATTACHED 53

#define PARTICLE_GUI_BOUGHT 54

#define PARTICLE_PLUTPICKUP 56

#define PARTICLE_GUI_PLUTCOUNT 57
#define PARTICLE_GUI_SPARKS 58

#define PARTICLE_LANDMINEDUST 59

#define PARTICLE_BULLETBONE 60
#define PARTICLE_BULLETBONEBOUNCE 61
#define PARTICLE_BONECHUNK 62
#define PARTICLE_BONEDUST 63

#define PARTICLE_MOLOTOV 64
#define PARTICLE_MOLOTOVTAIL 65

#define PARTICLE_GUI_COUNTDOWN 66

#define PARTICLE_PLAYERLINE 67
#define PARTICLE_FIRE2 68

#define PARTICLE_STATICBULLET 69

#define PARTICLE_CHIMNEYSMOKE 55

#define PARTICLE_EYELASERLIGHTS 70
#define PARTICLE_EYELASERS 71
#define PARTICLE_EYELASERHIT 72

#define PARTICLE_SICK 73
#define PARTICLE_SICKBULLET 74

#define PARTICLE_WEATHER_MIST 75

#define PARTICLE_WATERSPLASH 45
#define PARTICLE_WATERSPLASHMIST 76

#define PARTICLE_CHECKPOINTBLUE 77

#define PARTICLE_DRIP 78

#define PARTICLE_BLOODDECAL 79

#define PARTICLE_WATERRIPPLE 80
#define PARTICLE_HEATSHIMMER 81
#define PARTICLE_BENDBEAM 82
#define PARTICLE_SONIC 83
#define PARTICLE_RAINSCREENDROP 84
#define PARTICLE_BUMP 85
#define PARTICLE_DENT 86

#define PARTICLE_LTNG_BEAM 87
#define PARTICLE_LTNG_GLOW 88

//#define PARTICLE_GUI_DAMAGE 45

#define MAX_PLAYERS 128

class CPlayers;
class Entitys;
class CEnemies;
class enemy_base;

 enum enum_AttachedTo { ATTACHED_TO_NOTHING, ATTACHED_TO_PLAYER, ATTACHED_TO_MONSTER };
 enum enum_WeatherMode { WEATHER_OFF, 
						WEATHER_WINDYRAIN};


struct ParticleList
{
  int type;
  float x, y, attached_x, attached_y, attached_rot, time;
  float alpha, alphavel;
  float xscale,yscale;
  float xscalevel,yscalevel;
  bool keepaspect;
  float xvel,yvel;
  float rot,rotvel;
  

  float SpawnTimer;
  float SpawnTimerReset;
  float SpawnDistance;
  float SpawnDistanceReset;
  int SpawnParticle;
 

  enum_AttachedTo AttachedTo;

  CPlayers* pplayer; // for bullets - who shot the bullet - or for attatched
  enemy_base* pmonster;

  float damage; // the amount of damage to inflict on whatever touches it
				// if damage is a melee attack, it's time based
  unsigned int weapon;
  LPDIRECT3DTEXTURE9 textureoverride;


  bool IsMoving;
 

  ParticleList* Next;
  ParticleList()
  {
   Next=NULL;
   x=0;
   y=0;
   xvel=0;
   yvel=0;
   time=0;
   alpha=255;
   xscale=1;
   yscale=1;
   alphavel=0;
   xscalevel=0;
   yscalevel=0;
   pplayer=NULL;
   IsMoving=true;
   keepaspect=false;
   damage=0;
   weapon=0;
   AttachedTo = ATTACHED_TO_NOTHING;
   textureoverride = NULL;

   SpawnParticle=0;

	SpawnTimer=0;	
	SpawnTimerReset=0;
	SpawnDistance=0;	
	SpawnDistanceReset=0;
  }
};

struct sParticleType
{
	float offsetx, offsety;
	float life;
	float alphainit, alphainitRndm, alphaovertime, alphaovertimeRndm; // if it fades
	float scalexinit, scalexovertime, scalexinitRndm, scalexovertimeRndm;
	float scaleyinit, scaleyovertime, scaleyinitRndm, scaleyovertimeRndm;
	float xvel, xvelRndm;
	float rotinit, rotvel, rotinitRndm, rotvelRndm, rotbouncedamper;
	float yvel, yvelRndm;
	float friction, airresistance;
	bool keepaspect;
	float gravity;
	float width, height;
	LPDIRECT3DTEXTURE9 particletexture;
	bool collide;
	bool RemoveOnCollide;
	bool AffectedByExplosions;
	unsigned int layer;
	bool gui;
	int SplashType; // 0 none;1 bullet; 2 plop;
	float SpawnDelayRndm;
	float SpawnDelay;

	sParticleType()
	{
		SplashType=0;
		gui=false;
		gravity = 0;  
		friction=1;
		scalexinit = 1.0f;
		scaleyinit = 1.0f;
		scalexovertime = 0.0f;
		scaleyovertime = 0.0f;
		scalexinitRndm = 0.0f;
		scaleyinitRndm = 0.0f;
		scalexovertimeRndm = 0.0f;
		scaleyovertimeRndm = 0.0f;
		alphainit=255.0f;
		airresistance=0;
		rotinit=0;
		rotvel=0;
		rotinitRndm=0;
		rotvelRndm=0; 
		rotbouncedamper=1; // scale
		particletexture=NULL;
		width=32;
		height=32;
		collide=true;
		RemoveOnCollide=false;
		AffectedByExplosions=false;		
		SpawnDelayRndm = 0;
		SpawnDelay = 0;
	}
	

};

struct TileSet
{
	char path[255];
	LPDIRECT3DTEXTURE9 texture;
  TileSet()
  {
	  texture=NULL;
  }
};

struct sEntitysLoad
{
	bool inuse;
	char name[50];
	unsigned int type;
	float x, y;

	float a, b, c, d, e, f, g, h, i, j, k, l, m, n;

	char stringa[200];
	char stringb[200];
	char stringc[200];
	char stringd[200];
	char stringsma[50];
	char stringsmb[50];
	char stringsmc[50];

	sEntitysLoad()
	{
		inuse=false;
		x=0;
		y=0;
		a=0;
		b=0;
		c=0;
		d=0;
		e=0;
		f=0;
		g=0;
		h=0;
		i=0;
		j=0;
		k=0;
		l=0;
		m=0;
		n=0;
		sprintf(name, "");
		sprintf(stringa,"");
		sprintf(stringb,"");
	}
};

struct sPlayers
{
	bool inuse;
	CPlayers* player;
	sPlayers()
	{
		inuse=false;
		player=NULL;
	}
};

struct MapProperties
{
	char skyname[50];
	char maptitle[100];
	char author[100];
	char music[100];
	char website[100];

	char spare1[1000];
	char spare2[500];
	char spare3[200];
	char spare4[200];
	char spare5[50];
	char spare6[50];

	float skyspeed;
	float gravity;
	float sparef1;
	float sparef2;
	float sparef3;

	int sparei1;
	int sparei2;
	int sparei3;
};

#define COL_BLACK 0
#define COL_WHITE 1

struct sScreenFade
{
	bool active;
	float speed;
	int color;
	float alpha;

	sScreenFade()
	{
		active=false;
		speed=0.1f;
		alpha=0;
		color=0;
	}
};

typedef ParticleList* LPParticle;
class CCore;
class CLandscape
{
public:
	void AddPhysicalObject(float x, float y, float objwidth, float objheight, unsigned int type, LPSTR texturefilename, unsigned int material, float scalex, float scaley);
	void AddPlayerPhysObj(int num);
	void DrawPhysicsObjects();
	void GetSpawnPoint(float* x, float* y);
	void DrawOverlay();

	TileSet textures[NUM_LANDCAPETEXTURES]; // array for future
	TileSet texturesover[NUM_LANDCAPETEXTURES];
	TileSet texturesunder[NUM_LANDCAPETEXTURES];

	LPDIRECT3DTEXTURE9 skymap[16]; // array for future again
	LPDIRECT3DTEXTURE9 t_water; // water for the cunts with shit cards.
	LPDIRECT3DTEXTURE9 t_normalwater;

	LPDIRECT3DTEXTURE9 t_BloodDrips[NUM_BLOODDRIPS];
	LPDIRECT3DTEXTURE9 t_BloodBlobs[NUM_BLOODBLOBS];
	
	
	
	sParticleType	ParticleTypes[NUM_PARTICLETYPES];
	sPlayers playerlist[MAX_PLAYERS];

	int Map2D(int x, int y);

	char m_currentlevel[100];

	unsigned int *ptiles; // pointer to the array of tiles
	unsigned int *ptilesover; // overlay tiles
	unsigned int *ptilesunder;

	unsigned int width, height;

	float offset_x, offset_y;
	float targetoffset_x, targetoffset_y;
	float m_MaxXTile, m_MaxYTile;
	
	void Initialize(CGarrysGraphics *p_Gfx);

	void Clear(unsigned int i);
	void LoadMap(const char *file);
	void SetXYoffsets(float x, float y);
	void SetXYoffsetsForce(float x, float y);
	void Draw();
	void DrawUnder();
	void DrawSky();
	void SetZoom(float zoom);
	void SetWindowSize();
	bool BulletShoot(CPlayers* player);
	LPParticle BulletShoot(int guntype, float x, float y, float angle, float speedmultiplier, bool enemy=false);
	bool Collision(float x, float y);
	int window_width;
	int window_height;
	float window_widthtiles;
	float window_heighttiles;
	LPParticle AddParticle(int type, float x, float y);
	void DrawParticles(int layer=0);
	void RemoveParticle(LPParticle pointer);
	LPParticle PreviousParticle(LPParticle index);
	int m_ParticleCount;
	int m_ParticleDrawnCount;
	LPParticle m_pFirstParticle;
	LPParticle m_pLastParticle;
	void FreeTextures();
	float m_skyx;
	float m_tilesize;
	float m_zoom; // zoom with the screen size factor applied
	float m_zoom_real; // real zoom
	Entitys* ents;
	sEntitysLoad LoadEnt;
	void ShutDown(); // to free memory
	void BulletHitLandscape(LPParticle particle);
	float m_globalaplha;
	int RegisterPlayer(CPlayers* player);
	int CollisionType(float x, float y, LPParticle bullet, bool PossibleHitEnemy);
	void BulletHitPlayer(LPParticle particle);
	void DoBloodBlobsParticle(LPParticle particle, bool* Remove, bool draw=true);
	void DoGenericParticle(LPParticle pointer, bool* Remove, bool draw=true);
	void DoBulletParticle(LPParticle pointer , bool*  remove,bool draw=true);
	void DoDamageParticle(LPParticle pointer , bool*  remove,bool draw=true);
	void DoGuiParticle(LPParticle pointer , bool*  remove,bool draw=true);
	void DoAttachedParticle(LPParticle pointer, bool* remove,bool draw);
	float GetNearestPlayer(float x, float y, CPlayers** player, float* xdist, float* ydist);
	CCore* pcore;
	void UnAttachAttachmentsMonster(enemy_base* monster);
	bool IsLadder(float x, float y);
	void UpdateCamera(void);
	void DoWeather(void);
	float fWeatherCounter;
	enum_WeatherMode enumWeatherMode;
	void MakeExplosion(float x, float y, int size, float power);
	void DrawExplosion(float x, float y, int size);
	void DoParticleSpawner(LPParticle particle, bool skipconditions=false);
	bool DoSplashDamage(float x, float y, float size, float damage, CPlayers* attacker);
	void DoBounceSound(LPParticle pointer);
	void LoadMapTextures(bool CrashOnError);
	void LoadSky(char* skyname);
	void RemoveAllParticle();

	MapProperties m_MapProperties;
	void FocusCameraOnPlayer(void);
	void DrawRect(RECT * rct);
	void DrawRect(GRECT *rct);
	void GenericBloodSpurt(float x, float y, float angle, float speed, CPlayers* player=NULL);
	LPParticle AddParticle(int type, float x, float y, float angle, float scalex, float scaley, LPDIRECT3DTEXTURE9 texture, float rotvel);


	sSound* s_9mmbounce1;
	sSound* s_arrowhit1;
	sSound* s_buckshotbounce1;
	sSound* s_buckshotbounce2;
	sSound* s_buckshotbounce3;
	sSound* s_chaingunbounce1;
	sSound* s_clipbounce1;
	sSound* s_clipbounce2;
	sSound* s_grenadebounce1;
	sSound* s_ricochet1;
	sSound* s_ricochet2;
	sSound* s_ricochet3;
	sSound* s_ricochet4;
	sSound* s_riflebounce1;


	sSound* s_fire1;
	sSound* s_fire2;
	sSound* s_fire3;
	sSound* s_fire4;

	sSound* s_molotov;

	sSound* s_bonehit;

	sSound* s_waterbullet1;
	sSound* s_waterbullet2;
	sSound* s_waterplonk1;
	sSound* s_waterplonk2;
	sSound* s_watershell1;
	sSound* s_watershell2;

	sSound* s_explosion;
	bool Collision(GRECT* rct);
	bool TileMapCollision(int x, int y);
	void ShiftParticles(GRECT* rect, float x, float y);
	void ScreenShake(float x, float y);
	void ChangeMap(char* name);
	FSOUND_STREAM* music;
	FSOUND_STREAM* music_playerdie;
	FSOUND_STREAM* music_levelcomplete;
	void QueueUpIntroVoice(void);
	char m_NextLevel[100];
	LPParticle DoChainsawStyleDamage(unsigned int guntype, float angle, float x, float y, float length, float damage);

	int InGameMusicChannel;
	void DrawScreenFade(void);
	sScreenFade ScreenFade;

	void LoadLevelShader(void);


	float WaterLineOriginal;
	float WaterLine;
	float WaterLineVelocity;
	float WaterEffect_Speed;
	float WaterEffect_Size;

	void WaterBullet(int x);
	void WaterLargeObjectPlunk(int x);
	void WaterTinySplash(int x);

	void ParticleRemoveExplosive(LPParticle pointer);
	float spawnx, spawny;
	int HiddenAreas;
	LPParticle AddSpark(int x, int y, float angle, float speedmultiplier);
	LPParticle AddStoneDust(int x, int y, float scalemultiply);
	void DrawWaterLayer(LPDIRECT3DTEXTURE9 texture, float offsetx, float scalex, float scaley, int alpha);
	void DrawNonShaderWater(void);
	void Lightning(float x, float y, float destx, float desty, int splits, float randomness, float width, bool glow, int chanceofbreakoff);
};

#endif