#ifndef __INCLANDSCAPE
#define __INCLANDSCAPE

#include "Globals.h"
//#define _WINDOWS
#define ZLIB_DLL
#include "zlib/zlib.h"
#include "garryswrappers/GarrysGraphicsWrapper.h"
#include <stdio.h>


#define NUM_LANDCAPETEXTURES 10
#define NUM_PARTICLETYPES 20
class CPlayers;
class LevelEdit;

struct ParticleList
{
  int type;
  float x,y,time;
  float alpha, alphavel;
  float xscale,yscale;
  float xscalevel,yscalevel;
  bool keepaspect;
  float xvel,yvel;
  float rot,rotvel;

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
   
   keepaspect=false;
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
	LPDIRECT3DTEXTURE9 particletexture;

	sParticleType()
	{
		gravity = 0;  
		friction=1;
		airresistance=1;
		rotinit=0;
		rotvel=0;
		rotinitRndm=0;
		rotvelRndm=0; 
		rotbouncedamper=1; // scale
		particletexture=NULL;
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
	MapProperties()
	{
		skyspeed=1;
		gravity=1;
		sprintf(skyname,"grey");
	}
};

typedef ParticleList* LPParticle;

class CLandscape
{
public:
	unsigned int tiles[MAP_WIDTH][MAP_HEIGHT]; 
	unsigned int tilesover[MAP_WIDTH][MAP_HEIGHT]; 
	unsigned int tilesunder[MAP_WIDTH][MAP_HEIGHT]; 

	TileSet textures[NUM_LANDCAPETEXTURES]; // array for future
	TileSet texturesover[NUM_LANDCAPETEXTURES];
	TileSet texturesunder[NUM_LANDCAPETEXTURES];
	LPDIRECT3DTEXTURE9 skymap[16]; // array for future again
	sParticleType	ParticleTypes[NUM_PARTICLETYPES];
	CGarrysGraphics *p_mGfx;

	float offset_x, offset_y;
	float m_MaxXTile, m_MaxYTile;
	
	void Initialize(CGarrysGraphics *p_Gfx);

	void Clear(unsigned int i);
	void LoadMap(const char *file);
	void SetXYoffsets(float x, float y);
	void Draw(int layer);

  LevelEdit* leved;

	void DrawSky();

	void SetZoom(float zoom);

	void SetWindowSize();

	bool BulletShoot(CPlayers* player, float* shootlength, POINT* retpoint);

	bool Collision(float x, float y);

	int window_width;
	int window_height;
	int window_widthtiles;
	int window_heighttiles;

	LPParticle AddParticle(int type, float x, float y);
	void DrawParticles();
	void RemoveParticle(LPParticle pointer);
	LPParticle PreviousParticle(LPParticle index);
	int m_ParticleCount;
	LPParticle m_pFirstParticle;
	LPParticle m_pLastParticle;

	void FreeTextures();

	float m_skyx;

	float m_tilesize;
	float m_zoom;


	void ShutDown(); // to free memory
	void LoadMapTextures(bool CrashOnError);

	MapProperties m_MapProperties;
	void GetRightPath(char* variabletoset);
};

#endif