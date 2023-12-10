
#include "Landscape.h"
//#include "Players.h"

#include "garryswrappers/GarrysHelperFunctions.h"
#include "LevelEdit.h"

#include "entity-defs.h"

#define PARTICLE_SPARKBALL 0
#define PARTICLE_DIRTSPRAY 1
#define PARTICLE_RICHOCHETSPARK 2
#define PARTICLE_SHELL 3
#define PARTICLE_GUNSMOKE 4

void CLandscape::Initialize(CGarrysGraphics *p_Gfx)
{
	this->p_mGfx = p_Gfx;
	char filename[100];

	int i;

	for (i=0;i<NUM_LANDCAPETEXTURES;i++)
	{
		if (this->textures[i].texture!=NULL)
		{
			this->textures[i].texture->Release();
			this->textures[i].texture=NULL;
		}
	}

	offset_x = 0;
	offset_y = 0;

	HRESULT hr;
	sprintf(textures[0].path,"t1.bmp");
	sprintf(texturesunder[0].path,"u1.bmp");
	sprintf(texturesover[0].path,"o1.bmp");
	LoadMapTextures(false);

	Clear(0);

	m_tilesize = 32;
	m_zoom=1;

	m_ParticleCount=0;
	m_pFirstParticle=NULL;
	m_pLastParticle=NULL;
}

void CLandscape::SetZoom(float scale)
{
	if (scale<0.62) scale=0.62;
	//else if (scale
	m_zoom = scale;
	this->SetWindowSize();
}

void CLandscape::LoadMap(const char *filename)
{
	Clear(0);

	p_mGfx->WriteToLog("<br><b>Map loading::</b><br>");

	unsigned int width=1, height=1;
	gzFile  file;


	file = gzopen(filename,"r+b");

	if (file==NULL)
	{
		//::MessageBox(NULL,"Map Not FOund",NULL,NULL);
	}
	else
	{

	Clear(0);
	gzread(file,&width,sizeof(unsigned int));
	gzread(file,&height,sizeof(unsigned int));

	p_mGfx->WriteToLog("Width: %i Height %i<br>", width, height);

	int y, x;

	//main blocks
	int poffsetx=0, poffsety=0;

	for (y=poffsety;y<=poffsety+height;y++)
	{
		for (x=poffsetx;x<=poffsetx+width;x++)
		{
			gzread(file, &tiles[x][y],sizeof(unsigned int));
		}
	}

	// foreground
	poffsetx=0; poffsety=0;

	for (y=poffsety;y<=poffsety+height;y++)
	{
		for (x=poffsetx;x<=poffsetx+width;x++)
		{
			gzread(file, &tilesover[x][y],sizeof(unsigned int));
		}
	}

	// background
	poffsetx=0; poffsety=0;

	for (y=poffsety;y<=poffsety+height;y++)
	{
		for (x=poffsetx;x<=poffsetx+width;x++)
		{
			gzread(file, &tilesunder[x][y],sizeof(unsigned int));
		}
	}

	unsigned int numents;
	
	gzread(file, &numents,sizeof(unsigned int));
	p_mGfx->WriteToLog("Number of entities: %i<br>", numents);



	for (int i=0;i<MAX_ENTITYS;i++)
	{
		leved->entitys[i].inuse = false;
	}

	if (numents>0 && numents<MAX_ENTITYS)
	{

		for (i = 0;i<numents;i++)
		{
			gzread(file, &leved->entitys[i],sizeof(sEntitys));

			if (leved->entitys[i].type==ET_PHYSICSOBJECT ||
				leved->entitys[i].type==ET_PROP ||
				leved->entitys[i].type==ET_PICKUP ||
				leved->entitys[i].type==ET_SLIDER ||
				leved->entitys[i].type==ENT_LENSFLARE ||
				leved->entitys[i].type==ENT_ROTATINGPROP ||
				leved->entitys[i].type==ENT_PARALLAXPROP)
			{
				leved->UpdateEntTexture(i);
			}
		}

	}

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


	gzread(file, &m_MapProperties, sizeof(MapProperties));

	gzclose(file);

	}
	
	m_MaxXTile = width+1;
	m_MaxYTile = height+1;

	this->LoadMapTextures(false);

	//this->offset_x = this->leved->entitys[0].x;
	//this->offset_y = this->leved->entitys[0].y;
	leved->ox =  this->leved->entitys[0].x-500;
	leved->oy = this->leved->entitys[0].y-400;
}

void CLandscape::Clear(unsigned int i)
{
	for (int x=0;x<MAP_WIDTH;x++)
	{
		for (int y=0;y<MAP_HEIGHT;y++)
		{
			tiles[x][y] = i;
			tilesover[x][y] = i;
			tilesunder[x][y] = i;
		}
	}

	for (int x=0;x<MAP_WIDTH;x++)
	{
		for (int y=0;y<MAP_HEIGHT;y++)
		{
			if (tiles[x][y] != i) MessageBox(NULL,"wha","FUCK",0);
		}
	}
}

// traces a bullets path from a player to whereever and returns info about the strike
// coords are returned as world coords



void CLandscape::Draw(int layer)
{	
	int tile=0;
	int posx,posy;
	float fmodx, fmody;


	if (m_ParticleCount>0)	DrawParticles();

	bool drawunder = true;
	bool drawmiddl = true;
	bool drawoverr = true;


	if (this->p_mGfx->InputKeyDown(DIK_1))
	{
		drawunder = false;
		drawmiddl = true;
		drawoverr = false;
	}

	if (this->p_mGfx->InputKeyDown(DIK_2))
	{
		drawunder = true;
		drawmiddl = false;
		drawoverr = false;
	}

	if (this->p_mGfx->InputKeyDown(DIK_3))
	{
		drawunder = false;
		drawmiddl = false;
		drawoverr = true;
	}
	
	for (int x=0 ; x<window_widthtiles ; x++)
	{
		for (int y=0 ; y<window_heighttiles ; y++)
		{

			// the position on the tile map
			posx = x+((offset_x/m_tilesize));
			posy = y+((offset_y/m_tilesize));

			if (posx>=0 && posy>=0)
			{
				
				if (tilesunder[posx][posy]>0 && drawunder && layer==1)
				{
					tile = tilesunder[posx][posy];
					fmodx = fmod(offset_x,m_tilesize)*m_zoom;
					fmody = fmod(offset_y,m_tilesize)*m_zoom;
					
					p_mGfx->SpriteDrawEx(texturesunder[(tile-1)/49].texture,
						
						(x*m_tilesize*m_zoom)-fmodx,
						(y*m_tilesize*m_zoom)-fmody,
						
						0,0,0,

						m_zoom,m_zoom,

						((tile-1)%7)*34+1,
						(((int)(tile-1)/7)%7)*34+1,
						32,
						32,

						255,255,255,255
						);
						
				}

				if (tiles[posx][posy]>0 && drawmiddl && layer==2)
				{
					tile = tiles[posx][posy];
					fmodx = fmod(offset_x,m_tilesize)*m_zoom;
					fmody = fmod(offset_y,m_tilesize)*m_zoom;

					p_mGfx->SpriteDrawEx(textures[(tile-1)/49].texture,
						
						(x*m_tilesize*m_zoom)-fmodx,
						(y*m_tilesize*m_zoom)-fmody,
						
						0,0,0,

						m_zoom,m_zoom,

						((tile-1)%7)*34+1,
						(((tile-1)/7)%7)*34+1,
						32,
						32,

						255,255,255,255
						);
						
				}
			
				if (tilesover[posx][posy]>0 && drawoverr)
				{
					tile = tilesover[posx][posy];
					fmodx = fmod(offset_x,m_tilesize)*m_zoom;
					fmody = fmod(offset_y,m_tilesize)*m_zoom;

					p_mGfx->SpriteDrawEx(texturesover[tile/49].texture,
						
						(x*m_tilesize*m_zoom)-fmodx,
						(y*m_tilesize*m_zoom)-fmody,
						
						0,0,0,

						m_zoom,m_zoom,

						((tile-1)%7)*34+1,
						(((tile-1)/7)%7)*34+1,
						32,
						32,

						255,255,255,255
						);
						
				}
			
			}


		}
	}


	p_mGfx->DrawTextEx(30,10,255,255,255,"w: %i h: %i  tw: %i th: %i zoom:%f",window_width,window_height,window_widthtiles,window_heighttiles,m_zoom);

	p_mGfx->DrawTextEx(30,30,255,255,255,"particle count: %i",m_ParticleCount);



}

void CLandscape::SetXYoffsets(float x, float y)
{
	offset_x = x;
	offset_y = y;

	if (offset_x<0) offset_x=0;
	if (offset_y<0) offset_y=0;

//	if (offset_x>(m_MaxXTile*m_tilesize)-(window_width/m_zoom)) offset_x=m_MaxXTile*m_tilesize-(window_width/m_zoom);
//	if (offset_y>(m_MaxYTile*m_tilesize)-(window_height/m_zoom)) offset_y=m_MaxYTile*m_tilesize-(window_height/m_zoom);


	if (offset_x>((MAP_WIDTH*m_tilesize)-((window_widthtiles*m_tilesize)/m_zoom))) offset_x=(MAP_WIDTH*m_tilesize)-((window_widthtiles*m_tilesize)/m_zoom);
	if (offset_y>((MAP_HEIGHT*m_tilesize)-((window_heighttiles*m_tilesize)/m_zoom))) offset_y=(MAP_HEIGHT*m_tilesize)-((window_heighttiles*m_tilesize)/m_zoom);

//	offset_x = m_tilesize;
//	offset_y = m_tilesize;


}

void CLandscape::SetWindowSize()
{
	if (p_mGfx)
	{
		window_width = this->p_mGfx->m_d3dpp.BackBufferWidth;
		window_height = this->p_mGfx->m_d3dpp.BackBufferHeight;
		window_widthtiles = (int)((window_width/m_tilesize)/m_zoom) +(2);
		window_heighttiles = (int)((window_height/m_tilesize)/m_zoom) +(2);
	}
}

void CLandscape::DrawSky()
{

	float sc = ((window_width/4.0f)/256);
	float czoom = m_zoom;

	if (czoom<1) czoom=1;

	if (window_height/window_width<0.8f) sc = ((window_height/3.0f)/256);

	if (sc<1) sc=1;
	int x,y;

	//players[0].m_fX-((gfx.m_d3dpp.BackBufferWidth/2)/landscape.m_zoom

	float offset = fmod((((this->offset_x*0.7)+m_skyx)),256*sc)*czoom;
	int iadd = (int)((((((this->offset_x*0.7)+m_skyx))*czoom)/(256*sc*czoom)))%4;
	int i=0;

	m_skyx	=	m_skyx	+	(0.02*(this->p_mGfx->tick)*czoom);

	if (m_skyx>window_width) m_skyx=fmod(m_skyx, window_width);

	for (y=-2;y<1;y++)
	{

		for (x=-2;x<3;x++)
		{

			p_mGfx->SpriteDrawEx(skymap[((x+2+iadd)%4 +(4*(y+2)))%16],
						
						(((window_width/2)*czoom)  + (x*sc*256*czoom))-offset,
						((window_height/2) + (y*sc*256*czoom))+128*sc,
						
						

						(sc*czoom),(sc*czoom),
						0,0,256,256
						
						
						
						);



	if (x!=2) i++;

		}
	}

	i=0;

	p_mGfx->DrawTextEx(60,60,255,255,255,"sc:%f offset:%i",sc,iadd);


}

bool CLandscape::Collision(float x, float y)
{
	if (tiles[(int)(x/m_tilesize)][(int)(y/m_tilesize)]!=0)
		return true;

	return false;
}

void CLandscape::DrawParticles()
{
	LPParticle pointer=m_pFirstParticle;
	LPParticle lastpointer=NULL;
	LPParticle nextpointer=NULL;

	float i=0;
	float tst=0;

	int index=0;
	while (pointer!=NULL)
	{
		if (pointer->type!=2)
			p_mGfx->SpriteDrawEx(ParticleTypes[pointer->type].particletexture,
			(pointer->x-this->offset_x)*m_zoom-(ParticleTypes[pointer->type].offsetx*pointer->xscale*m_zoom),
			(pointer->y-this->offset_y)*m_zoom-(ParticleTypes[pointer->type].offsety*pointer->yscale*m_zoom),
			pointer->rot,ParticleTypes[pointer->type].offsetx*pointer->xscale*m_zoom,ParticleTypes[pointer->type].offsety*pointer->yscale*m_zoom,
			pointer->xscale*m_zoom,pointer->yscale*m_zoom,
			0,0,32,32,
			pointer->alpha,255,255,255);


		


		if (pointer->type==2)
				p_mGfx->SpriteDrawEx(ParticleTypes[pointer->type].particletexture,
			(pointer->x-this->offset_x)*m_zoom-(ParticleTypes[pointer->type].offsetx*pointer->xscale*m_zoom),
			(pointer->y-this->offset_y)*m_zoom-(ParticleTypes[pointer->type].offsety*pointer->yscale*m_zoom),
				atan2(pointer->xvel, pointer->yvel),ParticleTypes[pointer->type].offsetx*pointer->xscale*m_zoom,ParticleTypes[pointer->type].offsety*pointer->yscale*m_zoom,
				pointer->xscale*m_zoom,pointer->yscale*m_zoom,
				0,0,1,32,
				pointer->alpha,255,255,255);
				pointer->time = pointer->time + (0.01*p_mGfx->tick);
			


		pointer->time = pointer->time + (0.01*p_mGfx->tick);

		pointer->alpha = pointer->alpha + (pointer->alphavel*p_mGfx->tick);
		pointer->xscale = pointer->xscale + (pointer->xscalevel*p_mGfx->tick);
		if (pointer->keepaspect)
			pointer->yscale = pointer->xscale;
		else
			pointer->yscale = pointer->yscale + (pointer->yscalevel*p_mGfx->tick);

		// if gravity, apply gravity
		if (ParticleTypes[pointer->type].gravity!=0) 
			pointer->yvel = pointer->yvel + (ParticleTypes[pointer->type].gravity*p_mGfx->tick);

		// if hit wall on x
		if (Collision(pointer->x + (pointer->xvel*p_mGfx->tick), pointer->y))
		{
			pointer->xvel = -pointer->xvel*(0.7*(((rand()%2000))/2000.0f));
		}
		//if hit floor/ceiling
		if (Collision(pointer->x, pointer-> y+ (pointer->yvel*p_mGfx->tick)))
		{
			pointer->yvel = -pointer->yvel*(0.7*(((rand()%2000))/2000.0f));
			if (pointer->yvel<0.05 && pointer->yvel>-0.05) pointer->yvel=0;

			pointer->rotvel = pointer->rotvel*ParticleTypes[pointer->type].rotbouncedamper; // stop rotating

			//if (pointer->rot>-1 && pointer->rot<1) 
			
			if (pointer->rotvel<0.09)
			{pointer->rot=0;pointer->rotvel=0;}

			pointer->xvel = pointer->xvel*(ParticleTypes[pointer->type].friction); // friction
		}
		else
		{
			pointer->xvel = pointer->xvel*(ParticleTypes[pointer->type].airresistance);
		}

		pointer->x = pointer->x + (pointer->xvel*p_mGfx->tick);
		pointer->y = pointer->y + (pointer->yvel*p_mGfx->tick);
		pointer->rot = pointer->rot + (pointer->rotvel*p_mGfx->tick);

		
		
		 // air friction probably

		if (pointer->alpha<0) pointer->alpha=0;
		
		// get next particle here - because if we kill one we wont be able to get the next one
		nextpointer = pointer->Next;
		
		

		// removes dead paricles
		if ( (pointer->time>ParticleTypes[pointer->type].life) ||
			 (pointer->xscalevel<=0 && pointer->xscale<0) ||
			 (pointer->yscalevel<=0 && pointer->yscale<0) ||
			 pointer->alpha==0) 
		{
			 RemoveParticle(pointer);
			 pointer=NULL;
		}
	

		pointer=nextpointer;
		lastpointer = pointer;
		index++;
	}

}

LPParticle CLandscape::AddParticle(int type, float x, float y)
{
//	p_mGfx->WriteToLog("adding particle<br>");

	float randomiser;

	if (x<=0 || y<=0) return NULL;

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
	


//	p_mGfx->WriteToLog("setting pointers<br>");

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
//	p_mGfx->WriteToLog("removing particle<br>");
	LPParticle delme=NULL;

	if (pointer==m_pFirstParticle)
	{
	//	p_mGfx->WriteToLog("1st<br>");
		if (m_pLastParticle==m_pFirstParticle) m_pLastParticle=NULL;
		m_pFirstParticle = pointer->Next;

		delme = pointer;
		delete delme;
		m_ParticleCount--;
	}
	else if  (pointer==m_pLastParticle)
	{
	//	p_mGfx->WriteToLog("last<br>");
		m_pLastParticle = PreviousParticle(pointer);
		PreviousParticle(pointer)->Next = NULL;

		delme = pointer;
		delete delme;
		m_ParticleCount--;
	}
	else
	{
	//	p_mGfx->WriteToLog("middle one<br>");
		PreviousParticle(pointer)->Next = pointer->Next;

		delme = pointer;
		delete delme;
		m_ParticleCount--;
	}

	
	
//	p_mGfx->WriteToLog("removind : numparticles: %i<br><br>",m_ParticleCount);
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

	p_mGfx->WriteToLog("released <b>%i</b> sprites<br>",count);
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
			this->p_mGfx->WriteToLog("<b>Freed Texture:</b> Landscape %i<br>",i);
		}
	}

	for (i=0;i<NUM_PARTICLETYPES;i++)
	{
		if (this->ParticleTypes[i].particletexture!=NULL)
		{
			this->ParticleTypes[i].particletexture->Release();
			this->ParticleTypes[i].particletexture=NULL;
			this->p_mGfx->WriteToLog("<b>Freed Texture:</b> Particle %i<br>",i);
		}
	}

	for (i=0;i<16;i++)
	{
		if (this->skymap[i]!=NULL)
		{
			this->skymap[i]->Release();
			this->skymap[i]=NULL;
			this->p_mGfx->WriteToLog("<b>Freed Texture:</b> skymap %i<br>",i);
		}
	}
	
	

}

void CLandscape::LoadMapTextures(bool CrashOnError)
{
	char tempstring[500];
	HRESULT hr;

	char basefolder[1000];
	GetRightPath(basefolder);

	for (int i=0;i<NUM_LANDCAPETEXTURES;i++)
	{
		
		
		

		if (strcmp(textures[i].path,"")!=0)
		{
		sprintf(tempstring,"fwound/textures/world/%s",textures[i].path);
		if (textures[i].texture!=NULL)
		{
			textures[i].texture->Release();
			textures[i].texture=NULL;
		}
		hr = D3DXCreateTextureFromFile( p_mGfx->m_pd3dDevice,tempstring, &textures[i].texture);
		if (hr!=D3D_OK && CrashOnError) { this->p_mGfx->WriteToLog("<font color=red>MISSING TEXTURE (%s)</font>", tempstring);MessageBox(NULL,"Couldn't load Texture! See logfile for details.","error",MB_ICONEXCLAMATION); exit(1);}
		else { this->p_mGfx->WriteToLog("Loaded '%s'<br>", tempstring); }
		}

		if (strcmp(texturesover[i].path,"")!=0)
		{
		if (texturesover[i].texture!=NULL)
		{
			texturesover[i].texture->Release();
			texturesover[i].texture=NULL;
		}
		sprintf(tempstring,"fwound/textures/world/%s",texturesover[i].path);
		hr = D3DXCreateTextureFromFile( p_mGfx->m_pd3dDevice,tempstring, &texturesover[i].texture);
		if (hr!=D3D_OK && CrashOnError) { this->p_mGfx->WriteToLog("<font color=red>MISSING TEXTURE (%s)</font>", tempstring);MessageBox(NULL,"Couldn't load Texture! See logfile for details.","error",MB_ICONEXCLAMATION); exit(1);}
		else { this->p_mGfx->WriteToLog("Loaded '%s'<br>", tempstring); }
		}

		if (strcmp(texturesunder[i].path,"")!=0)
		{
		if (texturesunder[i].texture!=NULL)
		{
			texturesunder[i].texture->Release();
			texturesunder[i].texture=NULL;
		}
		sprintf(tempstring,"fwound/textures/world/%s",texturesunder[i].path);
		hr = D3DXCreateTextureFromFile( p_mGfx->m_pd3dDevice,tempstring, &texturesunder[i].texture);
		if (hr!=D3D_OK && CrashOnError) { this->p_mGfx->WriteToLog("<font color=red>MISSING TEXTURE (%s)</font>", tempstring);MessageBox(NULL,"Couldn't load Texture! See logfile for details.","error",MB_ICONEXCLAMATION); exit(1);}
		else { this->p_mGfx->WriteToLog("Loaded '%s'<br>", tempstring); }
		}
	}
}

void CLandscape::GetRightPath(char* variabletoset)
{
	char tmppath[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
	char cdir[_MAX_DIR+_MAX_DRIVE];
    GetModuleFileName(NULL, tmppath, sizeof(tmppath));
    _splitpath(tmppath, drive, dir, NULL, NULL);
	sprintf(cdir,"%s%s",drive,dir);
	bool boolret = SetCurrentDirectory(cdir);
	sprintf(variabletoset, cdir);
}
