#include "leveledit.h"
#define TEXWIDTH 238

void LevelEdit::DoTileDrawMode(void)
{
	TileSet* ts;
	int x=0,y=0;
	bool alt=false;

	if (this->EditMode==1) ts = landscape.textures;
	if (this->EditMode==2) ts = landscape.texturesunder;
	if (this->EditMode==3) ts = landscape.texturesover;





	gfx.SpriteDrawEx(ptex,gfx.m_d3dpp.BackBufferWidth-TEXWIDTH-2,0,0,0,0,2,gfx.m_d3dpp.BackBufferHeight,0,0,1,1,255,255,255,255);


	// draw all the textures
	for (int i =0;i<10;i++)
	{
		if (alt) alt=false; else alt=true;
		gfx.SpriteDrawEx(ptex,gfx.m_d3dpp.BackBufferWidth-TEXWIDTH,TextureBarScroll+(i*TEXWIDTH),0,0,0,TEXWIDTH,TEXWIDTH,0,0,1,1,alt?155:200,255,255,255);
		gfx.SpriteDraw(ts[i].texture,
					gfx.m_d3dpp.BackBufferWidth-TEXWIDTH,
					TextureBarScroll+(i*TEXWIDTH));
		gfx.DrawTextEx(gfx.m_d3dpp.BackBufferWidth-TEXWIDTH+10, TextureBarScroll+(i*TEXWIDTH)+10,255,255,255,ts[i].path);
	}

	if (!AllowEdit) return;

	// draw mouse cursor if over toolbar
	if (spoint.x>gfx.m_d3dpp.BackBufferWidth-TEXWIDTH && spoint.x<gfx.m_d3dpp.BackBufferWidth)
	{
		x = (spoint.x-(gfx.m_d3dpp.BackBufferWidth-TEXWIDTH))/34;
		y = (spoint.y+(TextureBarScroll*-1))/34;//;
		gfx.DrawTextEx(90,90,255,255,255,"x:%i y:%i",x,y);

		gfx.SpriteDrawEx(ptex,gfx.m_d3dpp.BackBufferWidth-TEXWIDTH+(x*32)+x*2,y*34+TextureBarScroll,0,0,0,32,32,0,5,1,1,100,255,255,255);

	}

	
	if (inrect)
	{	
		if (gfx.m_bMouse1)
		{

		}
		else // apply changes
		{

			float tm;
			if (rectpoint.x>point.x)
			{
				tm = rectpoint.x;
				rectpoint.x = point.x;
				point.x = tm;
			}
			if (rectpoint.y>point.y)
			{
				tm = rectpoint.y;
				rectpoint.y = point.y;
				point.y = tm;
			}
			for (int px=rectpoint.x;px<=point.x;px++)
			{
				for (int py=rectpoint.y;py<=point.y;py++)
				{
					if (this->EditMode==1) landscape.tiles[(int)px][(int)py] = DrawTile;
					if (this->EditMode==2) landscape.tilesunder[(int)px][(int)py] = DrawTile;
					if (this->EditMode==3) landscape.tilesover[(int)px][(int)py] = DrawTile;
				}
			}
			inrect = false;
		}
		
	}
	else if (gfx.m_bMouse2) // mouse down
	{
		if (spoint.x>gfx.m_d3dpp.BackBufferWidth-TEXWIDTH && spoint.x<gfx.m_d3dpp.BackBufferWidth)
		{
			TextureBarScroll = TextureBarScroll + gfx.m_nMouseY*2;
			if (TextureBarScroll<((256*10.0f)-gfx.m_d3dpp.BackBufferHeight)*-1.0f) TextureBarScroll=((256.0f*10)-gfx.m_d3dpp.BackBufferHeight)*-1.0f;
			if (TextureBarScroll>0) TextureBarScroll=0;
		}
		else
		{
			if (gfx.InputKeyDown(DIK_LCONTROL))
			{
			 landscape.tiles[(int)point.x][(int)point.y] = 0;
			 landscape.tilesunder[(int)point.x][(int)point.y] = 0;
			 landscape.tilesover[(int)point.x][(int)point.y] = 0;
			}

			if (this->EditMode==1) landscape.tiles[(int)point.x][(int)point.y] = 0;
			if (this->EditMode==2) landscape.tilesunder[(int)point.x][(int)point.y] = 0;
			if (this->EditMode==3) landscape.tilesover[(int)point.x][(int)point.y] = 0;
		}
	}
	else
	if (gfx.m_bMouse1) // mouse down
	{
		if (gfx.InputKeyDown(DIK_LCONTROL))
		{
			rectpoint.x = point.x;
			rectpoint.y = point.y;
			inrect=true;
		}


		if (spoint.x>gfx.m_d3dpp.BackBufferWidth-TEXWIDTH && spoint.x<gfx.m_d3dpp.BackBufferWidth)
		{
			
			DrawTile = x+1 + (y*6) + y;
		}
		else if (point.x>=0 && point.y>=0)
		{
			if (this->EditMode==1) landscape.tiles[(int)point.x][(int)point.y] = DrawTile;
			if (this->EditMode==2) landscape.tilesunder[(int)point.x][(int)point.y] = DrawTile;
			if (this->EditMode==3) landscape.tilesover[(int)point.x][(int)point.y] = DrawTile;
		}
	}

	if (spoint.x<gfx.m_d3dpp.BackBufferWidth-TEXWIDTH)
	{
		gfx.SpriteDrawEx(ts[0].texture,(point.x*32)-landscape.offset_x,	(point.y*32)-landscape.offset_y,0,0,0,1,1,
			1 + (((DrawTile-1)%7)*2) +  ((int)( ((int)(DrawTile-1)%7) *32)%256),
			1 + (((int)(DrawTile-1)/7)*2)    +		((int)((DrawTile-1)/7)*32  ),32,32,200,255,255,255);
	}

	if (gfx.InputKeyDown(DIK_Y)) 
		{
				for (int qx=0;qx<MAP_WIDTH;qx++)
				{
					for (int qy=0;qy<MAP_HEIGHT;qy++)
					{
						if (qx>(int)point.x || qy>(int)point.y)
						{
							landscape.tiles[qx][qy] = 0;
							landscape.tilesunder[qx][qy] = 0;
							landscape.tilesover[qx][qy] = 0;
						}
					}
				}
		}

	// mark over currently selected tile peice
	gfx.SpriteDrawEx(ptex,gfx.m_d3dpp.BackBufferWidth-TEXWIDTH + (((DrawTile-1)%7)*34)+1,	
							  (((DrawTile-1)/7)*34)+TextureBarScroll+1,
							  
							  0,0,0,32,32,0,0,1,1,sin(timeGetTime())*100,255,255,255);

	gfx.DrawTextEx(10,70,0,255,0,"Tile: %i",DrawTile);

}
