// LevelEdit.cpp: implementation of the LevelEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapEditor.h"
#include "LevelEdit.h"
//#include "Landscape.h"
//#include "Landscape.h"
#include "Globals.h"

LevelEdit::LevelEdit()
: TextureBarScroll(0)
{
	inrect=false;
	//gfx.bSuspended=true;
}

LevelEdit::~LevelEdit()
{

}





void LevelEdit::Draw()
{
//	WaitForSingleObject( hMutex, INFINITE);



	PlutCount=0;

	if (!gfx.bSuspended)
	{

		gfx.UpdateTimer();
		TimerA = TimerA + gfx.tick;

	gfx.InputRefreshKeyboard();
	gfx.InputRefreshMouse();
	GetCursorPos( &spoint);

	ScreenToClient(hWnd, &spoint);
	spoint.y=spoint.y-26;
	point.x = spoint.x;
	point.y = spoint.y;
	

	rpoint.x = point.x + landscape.offset_x;
	rpoint.y = point.y + landscape.offset_y;


	gfx.Begin();
	gfx.Clear(70,100,150);

	RECT winrect;
	GetClientRect(hWnd,&winrect);


	point.x = point.x + ((int)landscape.offset_x%32);
	point.x = (int)(point.x/32);
	point.x = point.x + ((int)landscape.offset_x/32);

	point.y = point.y + ((int)landscape.offset_y%32);
	point.y = (int)(point.y/32);
	point.y = point.y + ((int)landscape.offset_y/32);



	if (!gfx.InputKeyDown(DIK_1) && !gfx.InputKeyDown(DIK_2) && !gfx.InputKeyDown(DIK_3))
	for (unsigned int i=0;i<MAX_ENTITYS;i++)
	{
		if (entitys[i].inuse==true)
		{

				if (entitys[i].type==ENT_PARALLAXPROP)
				{
					float alpha=255;

					if (entitys[i].g==1) ChangeAlphaMode(true);
					else ChangeAlphaMode(false);

					if (entitys[i].h==1) alpha = rand()%255;
						if (entitys[i].h==2) alpha = 150+rand()%105;
						if (entitys[i].h==3) alpha = 200+rand()%55;
						if (entitys[i].h==4) alpha = ((1+sin(this->TimerA/1000.0f))/2.0f)*255;
						if (entitys[i].h==5) alpha = 150+(((1+sin(this->TimerA/1000.0f))/2.0f)*105);
						if (entitys[i].h==6) alpha = 200+(((1+sin(this->TimerA/1000.0f))/2.0f)*55);
						if (entitys[i].h==7) alpha = ((1+sin(this->TimerA/500.0f))/2.0f)*255;
						if (entitys[i].h==8) alpha = 150+(((1+sin(this->TimerA/500.0f))/2.0f)*105);
						if (entitys[i].h==9) alpha = 200+(((1+sin(this->TimerA/500.0f))/2.0f)*55);
						if (entitys[i].h==10) alpha = ((1+sin(this->TimerA/100.0f))/2.0f)*255;
						if (entitys[i].h==11) alpha = 150+(((1+sin(this->TimerA/100.0f))/2.0f)*105);
						if (entitys[i].h==12) alpha = 200+(((1+sin(this->TimerA/100.0f))/2.0f)*55);
						if (entitys[i].h==13) alpha = ((1+sin(this->TimerA/10.0f))/2.0f)*255;
						if (entitys[i].h==14) alpha = 150+(((1+sin(this->TimerA/10.0f))/2.0f)*105);
						if (entitys[i].h==15) alpha = 200+(((1+sin(this->TimerA/10.0f))/2.0f)*55);
					
						//gfx.WriteToLog("DRAWING ENTITY TYPE %i!!!",entitys[i].type); 
						gfx.SpriteDrawEx(
							
							entitytextures[i],
							
							(this->gfx.m_d3dpp.BackBufferWidth/2.0f)-(((this->gfx.m_d3dpp.BackBufferWidth/2.0f)-((entitys[i].x-landscape.offset_x)))*entitys[i].c), // get distance from the middle
							(this->gfx.m_d3dpp.BackBufferHeight/2.0f)-(((this->gfx.m_d3dpp.BackBufferHeight/2.0f)-((entitys[i].y-landscape.offset_y)))*entitys[i].d),
							0,0,0,
							(entitys[i].f==0?1:entitys[i].f),
							(entitys[i].f==0?1:entitys[i].f),
							
							0,0,0,0,

							
							alpha, 255, 255, 255);
						
						
						if (this->SelectedEntity==i)
						gfx.SpriteDrawEx(this->ptex,entitys[i].x-landscape.offset_x,entitys[i].y-landscape.offset_y,0,0,0,entitys[i].a,entitys[i].b,0,0,1,1,100,255,255,255);
						else
						gfx.SpriteDrawEx(this->ptex,entitys[i].x-landscape.offset_x+1,entitys[i].y-landscape.offset_y+1,0,0,0,entitys[i].a-2,entitys[i].b-2,0,0,1,1,50,255,255,255);

						ChangeAlphaMode(false);
				}
		}
	}


	landscape.Draw(1);
	landscape.Draw(2);
	

	if (AllowEdit)
	{

		if (gfx.InputKeyDown(DIK_1)) EditMode=1;
		if (gfx.InputKeyDown(DIK_2)) EditMode=2;
		if (gfx.InputKeyDown(DIK_3)) EditMode=3;
		if (gfx.InputKeyDown(DIK_4)) EditMode=4;
		if (gfx.InputKeyDown(DIK_5)) EditMode=5;

		// delete everything to the right


		if (gfx.InputKeyDown(DIK_SPACE))
		{
			ox=ox-(gfx.m_nMouseX*5);		
			oy=oy-(gfx.m_nMouseY*5);	
		}

		if (ox<0) ox=0;
		if (oy<0) oy=0;
		landscape.SetXYoffsets(ox,oy);
		
	}




	int color=0;
	// draw entitys
	if (!gfx.InputKeyDown(DIK_1) && !gfx.InputKeyDown(DIK_2) && !gfx.InputKeyDown(DIK_3))
	for (unsigned int i=0;i<MAX_ENTITYS;i++)
	{
		if (entitys[i].inuse==true)
		{
				
				if (entitys[i].type==ENT_E_HARDROCK)
				{
						gfx.SpriteDraw(enttextures[ENT_E_HARDROCK],entitys[i].x-landscape.offset_x-48,entitys[i].y-landscape.offset_y-48);
				} else
				if (entitys[i].type==ET_SPAWN)
				{
						gfx.SpriteDraw(enttextures[ET_SPAWN],entitys[i].x-landscape.offset_x-15,entitys[i].y-landscape.offset_y-32);
				} else
				if (entitys[i].type==ET_PHYSICSOBJECT)
				{
						gfx.SpriteDraw(entitytextures[i],entitys[i].x-landscape.offset_x-(entitys[i].a/2),entitys[i].y-landscape.offset_y-(entitys[i].b/2),entitys[i].e, entitys[i].f);
						
				} else
				if (entitys[i].type==ET_ZOMBIE)
				{
						gfx.SpriteDraw(enttextures[ET_ZOMBIE],entitys[i].x-landscape.offset_x-18,entitys[i].y-landscape.offset_y-10);
				} else 
				if (entitys[i].type==ENT_E_BONESY)
				{
						gfx.SpriteDraw(enttextures[ENT_E_BONESY],entitys[i].x-landscape.offset_x-32,entitys[i].y-landscape.offset_y);
				} else 
				if (entitys[i].type==ENT_E_RAT)
				{
						gfx.SpriteDraw(enttextures[ENT_E_RAT],entitys[i].x-landscape.offset_x-16,entitys[i].y-landscape.offset_y);
				} 
				else 
				if (entitys[i].type==ENT_E_GHOST)
				{
						gfx.SpriteDraw(enttextures[ENT_E_GHOST],entitys[i].x-landscape.offset_x-16,entitys[i].y-landscape.offset_y);
				} 
				else 
				if (entitys[i].type==ENT_E_BAT)
				{
						gfx.SpriteDraw(enttextures[ENT_E_BAT],entitys[i].x-landscape.offset_x-16,entitys[i].y-landscape.offset_y);
				} 
				else 
				if (entitys[i].type==ENT_E_STATUE)
				{
						gfx.SpriteDraw(enttextures[ENT_E_STATUE],entitys[i].x-landscape.offset_x-16,entitys[i].y-landscape.offset_y);
				} 
				else 
				if (entitys[i].type==ENT_E_GOTH)
				{
						gfx.SpriteDraw(enttextures[ENT_E_GOTH],entitys[i].x-landscape.offset_x-16,entitys[i].y-landscape.offset_y);
				} 
				else if (entitys[i].type==ENT_E_LANDMINE)
				{
						gfx.SpriteDraw(enttextures[ENT_E_LANDMINE],entitys[i].x-landscape.offset_x-16,entitys[i].y-landscape.offset_y);
				}
				
				else if (entitys[i].type==ENT_PROP || entitys[i].type==ET_PICKUP || entitys[i].type==ET_SLIDER)
				{
					float alpha=255;

					if (entitys[i].g==1) ChangeAlphaMode(true);
					else ChangeAlphaMode(false);

					if (entitys[i].type==ENT_PROP)
					{
						if (entitys[i].h==1) alpha = rand()%255;
						if (entitys[i].h==2) alpha = 150+rand()%105;
						if (entitys[i].h==3) alpha = 200+rand()%55;
						if (entitys[i].h==4) alpha = ((1+sin(this->TimerA/1000.0f))/2.0f)*255;
						if (entitys[i].h==5) alpha = 150+(((1+sin(this->TimerA/1000.0f))/2.0f)*105);
						if (entitys[i].h==6) alpha = 200+(((1+sin(this->TimerA/1000.0f))/2.0f)*55);
						if (entitys[i].h==7) alpha = ((1+sin(this->TimerA/500.0f))/2.0f)*255;
						if (entitys[i].h==8) alpha = 150+(((1+sin(this->TimerA/500.0f))/2.0f)*105);
						if (entitys[i].h==9) alpha = 200+(((1+sin(this->TimerA/500.0f))/2.0f)*55);
						if (entitys[i].h==10) alpha = ((1+sin(this->TimerA/100.0f))/2.0f)*255;
						if (entitys[i].h==11) alpha = 150+(((1+sin(this->TimerA/100.0f))/2.0f)*105);
						if (entitys[i].h==12) alpha = 200+(((1+sin(this->TimerA/100.0f))/2.0f)*55);
						if (entitys[i].h==13) alpha = ((1+sin(this->TimerA/10.0f))/2.0f)*255;
						if (entitys[i].h==14) alpha = 150+(((1+sin(this->TimerA/10.0f))/2.0f)*105);
						if (entitys[i].h==15) alpha = 200+(((1+sin(this->TimerA/10.0f))/2.0f)*55);
					}
						//gfx.WriteToLog("DRAWING ENTITY TYPE %i!!!",entitys[i].type); 
						gfx.SpriteDraw(entitytextures[i],entitys[i].x-landscape.offset_x,entitys[i].y-landscape.offset_y,alpha, 255, 255, 255);
						if (this->SelectedEntity==i)
						gfx.SpriteDrawEx(this->ptex,entitys[i].x-landscape.offset_x,entitys[i].y-landscape.offset_y,0,0,0,entitys[i].a,entitys[i].b,0,0,1,1,100,255,255,255);
						else
						gfx.SpriteDrawEx(this->ptex,entitys[i].x-landscape.offset_x+1,entitys[i].y-landscape.offset_y+1,0,0,0,entitys[i].a-2,entitys[i].b-2,0,0,1,1,50,255,255,255);

						ChangeAlphaMode(false);
				}
				else if (entitys[i].type==ENT_PARALLAXPROP)
				{
					float alpha=255;

					if (entitys[i].g==1) ChangeAlphaMode(true);
					else ChangeAlphaMode(false);
			
						if (this->SelectedEntity==i)
						gfx.SpriteDrawEx(this->ptex,entitys[i].x-landscape.offset_x,entitys[i].y-landscape.offset_y,0,0,0,entitys[i].a,entitys[i].b,0,0,1,1,100,255,255,255);
						else
						gfx.SpriteDrawEx(this->ptex,entitys[i].x-landscape.offset_x+1,entitys[i].y-landscape.offset_y+1,0,0,0,entitys[i].a-2,entitys[i].b-2,0,0,1,1,50,255,255,255);

						ChangeAlphaMode(false);
				}
				else if (entitys[i].type==ENT_LENSFLARE)
				{ 
					if (entitys[i].g==0) ChangeAlphaMode(true);
					else ChangeAlphaMode(false);
					float alpha=255;
					if (entitys[i].type==ENT_LENSFLARE)
					{
						if (entitys[i].h==1) alpha = rand()%255;
						if (entitys[i].h==2) alpha = 150+rand()%105;
						if (entitys[i].h==3) alpha = 200+rand()%55;
						if (entitys[i].h==4) alpha = ((1+sin(this->TimerA/1000.0f))/2.0f)*255;
						if (entitys[i].h==5) alpha = 150+(((1+sin(this->TimerA/1000.0f))/2.0f)*105);
						if (entitys[i].h==6) alpha = 200+(((1+sin(this->TimerA/1000.0f))/2.0f)*55);
						if (entitys[i].h==7) alpha = ((1+sin(this->TimerA/500.0f))/2.0f)*255;
						if (entitys[i].h==8) alpha = 150+(((1+sin(this->TimerA/500.0f))/2.0f)*105);
						if (entitys[i].h==9) alpha = 200+(((1+sin(this->TimerA/500.0f))/2.0f)*55);
						if (entitys[i].h==10) alpha = ((1+sin(this->TimerA/100.0f))/2.0f)*255;
						if (entitys[i].h==11) alpha = 150+(((1+sin(this->TimerA/100.0f))/2.0f)*105);
						if (entitys[i].h==12) alpha = 200+(((1+sin(this->TimerA/100.0f))/2.0f)*55);
						if (entitys[i].h==13) alpha = ((1+sin(this->TimerA/10.0f))/2.0f)*255;
						if (entitys[i].h==14) alpha = 150+(((1+sin(this->TimerA/10.0f))/2.0f)*105);
						if (entitys[i].h==15) alpha = 200+(((1+sin(this->TimerA/10.0f))/2.0f)*55);
					}

					
					if (entitys[i].d==0) entitys[i].d=1.0f;
					gfx.SpriteDrawEx(entitytextures[i],
						entitys[i].x-landscape.offset_x-((entitys[i].a/2)*entitys[i].d),
						entitys[i].y-landscape.offset_y-((entitys[i].b/2)*entitys[i].d),0,0,0,entitys[i].d,entitys[i].d,0,0,entitys[i].a,entitys[i].b,alpha, 255, 255, 255);
					ChangeAlphaMode(false);
					if (this->SelectedEntity==i)
					gfx.SpriteDrawEx(this->ptex,entitys[i].x-landscape.offset_x,entitys[i].y-landscape.offset_y,0,0,0,entitys[i].a,entitys[i].b,0,0,1,1,30,255,255,255);
				}
				else if (entitys[i].type==ENT_ROTATINGPROP)
				{ 
					if (entitys[i].g==0) ChangeAlphaMode(true);
					else ChangeAlphaMode(false);
					float alpha=255;

					if (entitys[i].h==1) alpha = rand()%255;
					if (entitys[i].h==2) alpha = 150+rand()%105;
					if (entitys[i].h==3) alpha = 200+rand()%55;
					if (entitys[i].h==4) alpha = ((1+sin(this->TimerA/1000.0f))/2.0f)*255;
					if (entitys[i].h==5) alpha = 150+(((1+sin(this->TimerA/1000.0f))/2.0f)*105);
					if (entitys[i].h==6) alpha = 200+(((1+sin(this->TimerA/1000.0f))/2.0f)*55);
					if (entitys[i].h==7) alpha = ((1+sin(this->TimerA/500.0f))/2.0f)*255;
					if (entitys[i].h==8) alpha = 150+(((1+sin(this->TimerA/500.0f))/2.0f)*105);
					if (entitys[i].h==9) alpha = 200+(((1+sin(this->TimerA/500.0f))/2.0f)*55);
					if (entitys[i].h==10) alpha = ((1+sin(this->TimerA/100.0f))/2.0f)*255;
					if (entitys[i].h==11) alpha = 150+(((1+sin(this->TimerA/100.0f))/2.0f)*105);
					if (entitys[i].h==12) alpha = 200+(((1+sin(this->TimerA/100.0f))/2.0f)*55);
					if (entitys[i].h==13) alpha = ((1+sin(this->TimerA/10.0f))/2.0f)*255;
					if (entitys[i].h==14) alpha = 150+(((1+sin(this->TimerA/10.0f))/2.0f)*105);
					if (entitys[i].h==15) alpha = 200+(((1+sin(this->TimerA/10.0f))/2.0f)*55);
					

					
					if (entitys[i].d==0) entitys[i].d=1.0f;
					gfx.SpriteDrawEx(entitytextures[i],
						entitys[i].x-landscape.offset_x-(entitys[i].a/2),
						entitys[i].y-landscape.offset_y-(entitys[i].b/2),
						(this->TimerA*entitys[i].e)*0.001f,

						((entitys[i].a)/2.0f),
						((entitys[i].b)/2.0f),

						entitys[i].d,
						entitys[i].d,
						0,0,
						entitys[i].a,entitys[i].b,
						alpha, 
						255, 255, 255);
					ChangeAlphaMode(false);
					if (this->SelectedEntity==i)
					gfx.SpriteDrawEx(this->ptex,entitys[i].x-landscape.offset_x,entitys[i].y-landscape.offset_y,0,0,0,entitys[i].a,entitys[i].b,0,0,1,1,30,255,255,255);
				}
				
				else if (entitys[i].type==ET_CONSOLECOMMAND)
				{
					gfx.SpriteDrawEx(this->ptex,entitys[i].x-landscape.offset_x,entitys[i].y-landscape.offset_y,0,0,0,entitys[i].a,entitys[i].b,0,0,1,1,150,255,255,255);
				}
				else if (entitys[i].type==ENT_SOUND)
				{
					gfx.SpriteDraw(enttextures[ENT_SOUND],entitys[i].x-landscape.offset_x-16,entitys[i].y-landscape.offset_y-16);
				}
				else if (entitys[i].type==ENT_PLUTONIUM)
				{
					gfx.SpriteDraw(enttextures[ENT_PLUTONIUM],entitys[i].x-landscape.offset_x-16,entitys[i].y-landscape.offset_y-16);
					PlutCount++;
				}
				else if (entitys[i].type==ENT_LEVELEND)
				{
					gfx.SpriteDraw(enttextures[ENT_LEVELEND],entitys[i].x-landscape.offset_x-16,entitys[i].y-landscape.offset_y-16);
				}
				else if (entitys[i].type==ENT_REFLECTION)
				{
					gfx.SpriteDrawEx(this->ptex,entitys[i].x-landscape.offset_x,entitys[i].y-landscape.offset_y,0,0,0,entitys[i].a,entitys[i].b,0,0,1,1,150,255,255,255);
				}
				else if (entitys[i].type==ENT_WATERLINE)
				{
					gfx.SpriteDrawEx(this->ptex,1,entitys[i].y-landscape.offset_y,0,0,0,gfx.m_d3dpp.BackBufferWidth,2,0,0,1,1,150,255,255,255);
				}
				else
				{
					gfx.SpriteDrawEx(this->ptex,entitys[i].x-landscape.offset_x,entitys[i].y-landscape.offset_y,0,0,0,entitys[i].a,entitys[i].b,0,0,1,1,150,255,255,255);
				}

				

				if (EditMode==4)
				if (SelectedEntity==i) color = 0; else color = 255;
			//	{
				gfx.DrawTextEx(entitys[i].x-landscape.offset_x+1-4   ,entitys[i].y-landscape.offset_y+1-5,    0,0,0,"X");
				gfx.DrawTextEx(entitys[i].x-landscape.offset_x-4   ,entitys[i].y-landscape.offset_y-5,    255,color,color,"X");
				gfx.DrawTextEx(entitys[i].x-landscape.offset_x-4   ,entitys[i].y-landscape.offset_y-5,    255,color,color,"X");

				//gfx.DrawTextEx(entitys[i].x-landscape.offset_x+41,entitys[i].y-landscape.offset_y+1,0,0,0,"%i %i\n x:%i y:%i",i,(int)entitys[i].type, (int)entitys[i].x,(int)entitys[i].y);
				//gfx.DrawTextEx(entitys[i].x-landscape.offset_x+40,entitys[i].y-landscape.offset_y,255,color,color,"%i %i\n x:%i y:%i",i,(int)entitys[i].type,(int)entitys[i].x,(int)entitys[i].y);
			//	}
		}
	}


	if (EditMode==4)
	{
		DoEntityMode();
	}
	else if (EditMode==5)
	{
		DoPrefabMode();
	}
	else
	{
		DoTileDrawMode();
	}

	if (EditMode==1)
		gfx.DrawText(10,50,"Edit Mode: solid foreground", 0,0,0);
	else if (EditMode==2)
		gfx.DrawText(10,50,"Edit Mode: background", 0,0,0);
	else if (EditMode==3)
		gfx.DrawText(10,50,"Edit Mode: foreground", 0,0,0);
	else if (EditMode==4)
		gfx.DrawText(10,50,"Edit Mode: Entities", 0,0,0);
	else if (EditMode==5)
		gfx.DrawText(10,50,"Edit Mode: Prefabs", 0,0,0);

	gfx.DrawTextEx(20   ,200, 255,255,0,"Plutonium: %i",PlutCount);

	gfx.End();


	gfx.Flip(1,26,(int)winrect.right-winrect.left-2 , (int)(winrect.bottom-winrect.top)-20);

	}




	//ReleaseMutex(hMutex);
}





















void LevelEdit::Setup()
{
	hWnd = AfxGetMainWnd()->m_hWnd;  

	HINSTANCE hInstance = AfxGetInstanceHandle();

	RECT winrect;
	GetClientRect(hWnd,&winrect);


	gfx.EnableLogging();
	gfx.CreateDisplay((int)(winrect.right-winrect.left)-4 , (int)(winrect.bottom-winrect.top)-46,true,&hWnd);
	gfx.CreateDXFont(12,0,"Arial");
	gfx.CreateDirectInput(&hInstance,&hWnd);
	gfx.WindowResize((int)(winrect.right-winrect.left)-4 , (int)(winrect.bottom-winrect.top)-46);
	// = true;

	char basefolder[1000], filename[1000];
	GetRightPath(basefolder);

	sprintf(filename, "%smapedit/spawn.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ET_SPAWN]);

	sprintf(filename, "%smapedit/enemy_zombie.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ET_ZOMBIE]);

	sprintf(filename, "%smapedit/enemy_bonesy.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ENT_E_BONESY]);

	sprintf(filename, "%smapedit/sound.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ENT_SOUND]);

	sprintf(filename, "%smapedit/plut.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ENT_PLUTONIUM]);

	
	sprintf(filename, "%smapedit/p.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &ptex);
	
	sprintf(filename, "%smapedit/rat.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ENT_E_RAT]);

	sprintf(filename, "%smapedit/mine.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ENT_E_LANDMINE]);
	
	sprintf(filename, "%smapedit/levelend.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ENT_LEVELEND]);
	
	sprintf(filename, "%smapedit/eghost.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ENT_E_GHOST]);
	
	sprintf(filename, "%smapedit/ebat.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ENT_E_BAT]);
	
	sprintf(filename, "%smapedit/estatue.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ENT_E_STATUE]);
	
	sprintf(filename, "%smapedit/egoth.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ENT_E_GOTH]);
	
	sprintf(filename, "%sfwound/textures/enemies/hardrock/body.bmp",basefolder);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice,filename, &enttextures[ENT_E_HARDROCK]);
	

	landscape.Initialize(&gfx);

	landscape.SetWindowSize();
	
//	landscape.LoadMap("maps/one");

	landscape.SetXYoffsets(0,0);
	DrawTile=1;
/*
	hMutex = CreateMutex( 
    NULL,                       // no security attributes
    FALSE,                      // initially not owned
    "MutexToProtectDatabase");  // name of mutex
*/
	EditMode=1;


	entitys[0].inuse = true;
	entitys[0].x = 200;
	entitys[0].y = 40;
	entitys[0].type = ET_SPAWN;

	SelectedEntity = 0;

	AllowEdit=true;

	landscape.leved = this;


	SetupEntitys();

	//gfx.bSuspended=false;
}













void LevelEdit::FileNew()
{
	landscape.Clear(0);
}

void LevelEdit::SaveMap(const char *filename)
{
//	WaitForSingleObject( hMutex, INFINITE);


	int firstx=9999, firsty=9999, lastx=0, lasty=0;

	int qx,qy;
	unsigned int numents;

	for (qx=0;qx<MAP_WIDTH;qx++)
	{
		for (qy=0;qy<MAP_HEIGHT;qy++)
		{
			if (landscape.tiles[qx][qy] > 0 || landscape.tilesover[qx][qy] > 0)
			{

				if (qx>lastx)
				{
					lastx=qx;
				}
				if (qy>lasty)
				{
					lasty=qy;
				}
			}
		}
	}
				firstx=0;
				firsty=0;

	unsigned int width, height;

	width  = lastx-firstx;
	height = lasty-firsty;

	gzFile  file;



	file = gzopen(filename,"w+b");
	gzwrite(file, &width,sizeof(width));
	gzwrite(file, &height,sizeof(height));

	// the blocks
	for (int y=firsty;y<=lasty;y++)
	{
		for (int x=firstx;x<=lastx;x++)
		{
			gzwrite(file, &landscape.tiles[x][y],sizeof(unsigned int));
		}
	}

	// the foreground
	for (y=firsty;y<=lasty;y++)
	{
		for (int x=firstx;x<=lastx;x++)
		{
			gzwrite(file, &landscape.tilesover[x][y],sizeof(unsigned int));
		}
	}

	// the back background
	for (y=firsty;y<=lasty;y++)
	{
		for (int x=firstx;x<=lastx;x++)
		{
			gzwrite(file, &landscape.tilesunder[x][y],sizeof(unsigned int));
		}
	}

	// entitys
	numents=0;
	for (int i=0;i<MAX_ENTITYS;i++)
	{
		if (this->entitys[i].inuse)
		{
			numents++;
		}
	}

	gzwrite(file, &numents, sizeof(unsigned int));
	
	if (numents>0)
	{
		for (int i=0;i<MAX_ENTITYS;i++)
		{
			if (this->entitys[i].inuse)
			{
				gzwrite(file, &entitys[i],sizeof(sEntitys));
			}
		}

	}

	for (int i=0;i<NUM_LANDCAPETEXTURES;i++)
	{
		gzwrite(file, landscape.textures[i].path,sizeof(char)*255);
	}
	for (int i=0;i<NUM_LANDCAPETEXTURES;i++)
	{
		gzwrite(file, landscape.texturesover[i].path,sizeof(char)*255);
	}
	for (int i=0;i<NUM_LANDCAPETEXTURES;i++)
	{
		gzwrite(file, landscape.texturesunder[i].path,sizeof(char)*255);
	}

	gzwrite(file, &landscape.m_MapProperties, sizeof(MapProperties));

	

	gzclose(file);

	// WIDTH
	// HEIGHT
	// COLLISIONTILES
	// FOREGROUNDTILES
	// BACKGROUNDTILES
	// NUMBER OF ENTITYS
	// ENTITYS
	// TEXTURE FILENAME x10 (255char)
	// TEXTUREOVER FILENAME x10 (255char)
	// TEXTUREUNDER FILENAME x10 (255char)
	// MAPINFO (MapProperties)

	/*
	// map is compressed
	FILE*  file;

	file = fopen(filename,"w");
	fwrite(landscape.tiles,sizeof(landscape.tiles),1,file);
	fclose(file);
*/
	//ReleaseMutex(hMutex);

}

void LevelEdit::AddEntity()
{
	for (int i=0;i<MAX_ENTITYS;i++)
	{
		if (entitys[i].inuse==false)
		{
			memcpy(&entitys[i],&entitys[SelectedEntity],sizeof(sEntitys));
		//	entitys[i].type=entitys[SelectedEntity].type;
			entitys[i].x=entitys[SelectedEntity].x+40;
			entitys[i].y=entitys[SelectedEntity].y+40;
			entitys[i].inuse = true;
			SelectedEntity = i;
			UpdateEntTexture(i);
			break;
		}
	}
}

void LevelEdit::DelEntity()
{
	if (SelectedEntity==0) return;
	if (!AllowEdit) return;
	
	entitys[SelectedEntity].inuse=false;
	SelectedEntity=0;
}

void LevelEdit::UpdateEntTexture(int num)
{
	char filename[300];

	char basefolder[1000];
	GetRightPath(basefolder);

	sprintf(filename, "%sfwound/textures/props/%s",basefolder, this->entitys[num].stringa);
	D3DXCreateTextureFromFile( gfx.m_pd3dDevice, filename, &this->entitytextures[num]);
//	gfx.WriteToLog("<b>Loaded entity texture</b>: %i %s<br>", num, filename);
}

void LevelEdit::GetRightPath(char* variabletoset)
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
void LevelEdit::ChangeAlphaMode(bool additive)
{
	gfx.m_pSprite->End();
	gfx.m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	if (additive)
	{
		gfx.m_pd3dDevice->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		gfx.m_pd3dDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
	}
}

void LevelEdit::SetupEntitys(void)
{
	sprintf(entitytypes[ENT_SPAWN].name,"spawn");
	sprintf(entitytypes[ENT_PHYSICS].name,"physics object");
	sprintf(entitytypes[ENT_E_ZOMBIE].name,"enemy_zombie");
	sprintf(entitytypes[ENT_PROP].name,"prop");
	sprintf(entitytypes[ENT_CONSOLE].name,"consolecommand");
	sprintf(entitytypes[ENT_E_SOLTUR].name,"enemy_soldierturret");
	sprintf(entitytypes[ENT_LADDER].name,"ent_ladder");
	sprintf(entitytypes[ENT_PICKUP].name,"ent_pickup");
	sprintf(entitytypes[ENT_TRIGGER].name,"ent_trigger");
	sprintf(entitytypes[ENT_ELEVATOR].name,"ent_elevator");
	sprintf(entitytypes[ENT_SOUND].name,"ent_sound");
	sprintf(entitytypes[ENT_PUSH].name,"ent_push");
	sprintf(entitytypes[ENT_E_BONESY].name,"enemy_bonesy");
	sprintf(entitytypes[ENT_E_RAT].name,"enemy_rat");
	sprintf(entitytypes[ENT_E_LANDMINE].name,"enemy_landmine");
	sprintf(entitytypes[ENT_E_MINE].name,"enemy_landmine");
	sprintf(entitytypes[ENT_LEVELEND].name,"ent_levelend");
	sprintf(entitytypes[ENT_PARTICLESPAWNER].name,"ent_particlespawn");
	sprintf(entitytypes[ENT_LENSFLARE].name,"ent_lensflare");
	sprintf(entitytypes[ENT_PLUTONIUM].name,"plutonium");
	sprintf(entitytypes[ENT_REFLECTION].name,"ent_reflection");
	sprintf(entitytypes[ENT_E_GHOST].name,"enemy_ghost");
	sprintf(entitytypes[ENT_E_BAT].name,"enemy_bat");
	sprintf(entitytypes[ENT_E_STATUE].name,"enemy_statue");
	sprintf(entitytypes[ENT_E_GOTH].name,"enemy_goth");
	sprintf(entitytypes[ENT_E_SICKZOMBIE].name,"enemy_sickzombie");
	sprintf(entitytypes[ENT_E_PISTOLSOLDIER].name,"enemy_pistolsoldier");
	sprintf(entitytypes[ENT_E_INSANESOLDIER].name,"enemy_insanesoldier");
	sprintf(entitytypes[ENT_WEATHER].name,"ent_weather");
	sprintf(entitytypes[ENT_E_M16SOLDIER].name,"enemy_m16soldier");
	sprintf(entitytypes[ENT_E_SMAWSOLDIER].name,"enemy_smawsoldier");
	sprintf(entitytypes[ENT_WATERLINE].name,"waterline");
	sprintf(entitytypes[ENT_CHECKPOINT].name,"checkpoint");
	sprintf(entitytypes[ENT_ROTATINGPROP].name,"ent_rotatingprop");
	sprintf(entitytypes[ENT_SECRETAREA].name,"secretarea");
	sprintf(entitytypes[ENT_PARALLAXPROP].name,"ent_propparallax");
	sprintf(entitytypes[ENT_E_JAWFISH].name,"enemy_jawfish");
	sprintf(entitytypes[ENT_E_HARDROCK].name,"enemy_hardrock");
	sprintf(entitytypes[ENT_E_MURDERORB].name,"enemy_murderorb");
	sprintf(entitytypes[ENT_TRIGGER_ONNOENEMIES].name,"trigger_onnoenemies");
	sprintf(entitytypes[ENT_E_ELECTRONONCE].name,"enemy_electrononce");

	
}
