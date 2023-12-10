#include "options.h"
#include "core.h"

#define GFX this->pcore->gfx
#define SCALE pcore->menu.GuiScale
#define SCRW (GFX.m_d3dpp.BackBufferWidth)
#define SCRH (GFX.m_d3dpp.BackBufferHeight)

#define KEYX (pcore->menu.GuiScale*100)
#define KEYY (pcore->menu.GuiScale*300)

#define SPACING 20

#define MO_FINISHED 1

COptions::COptions(void)
: MouseDown(false)
{
}

COptions::~COptions(void)
{
}

void COptions::Draw(void)
{
	pcore->SetAlphaMode(true);
	GFX.SpriteDrawEx(t_opbits,20*SCALE, 220*SCALE, 0,0,0,1,1,0,0,255,64,255,255,255,255);


	// finsihed
	for (int i=0;i<((mouseover==MO_FINISHED)+1);i++)
	GFX.SpriteDrawEx(t_opbits,SCRW-(200*SCALE), SCRH-(100*SCALE), 0,0,0,1,1,0,64,255,32,255,255,255,255);


	this->DrawSlider(&soundvolume);
	this->DrawSlider(&musicvolume);

	for (int i=0;i<NUM_KEYS;i++)
	{
		DrawKeyBind(&this->keybinds[i]);
	}

}

void COptions::Init(void)
{
	DoingKey=false;
	t_opbits = pcore->textures.LoadTexture("textures/menu/options.tga");
	mouseover=0;

	buttons[MO_FINISHED].top = SCRH-(100*SCALE);
	buttons[MO_FINISHED].left = SCRW-(200*SCALE);
	buttons[MO_FINISHED].right = buttons[MO_FINISHED].left + 160*SCALE;
	buttons[MO_FINISHED].bottom = buttons[MO_FINISHED].top + 36*SCALE;

	sprintf(soundvolume.name,"Sound Volume");
	soundvolume.y = (300*SCALE);
	soundvolume.x = SCRW-(370*SCALE);
	soundvolume.value = pcore->sound.m_volume*100.0f;
	musicvolume.selected = false;

	sprintf(musicvolume.name,"Music Volume");
	musicvolume.y = (370*SCALE);
	musicvolume.x = SCRW-(370*SCALE);
	musicvolume.value = pcore->Settings.musicvolume*100.0f;
	musicvolume.selected = false;


	for (int i=0;i<NUM_KEYS;i++)
	{
		sprintf(keybinds[i].name,"");
		sprintf(keybinds[i].command,"");
		sprintf(keybinds[i].keyname,"");
		keybinds[i].x = 0.0f;
		keybinds[i].y = 0.0f;
	}

	AddKeyBind("Walk Left","+left");
	AddKeyBind("Walk Right","+right");
	AddKeyBind("Climb Up","+up");
	AddKeyBind("Climb Down","+down");
	AddKeyBind("Jump","+jump");
	AddKeyBind("Duck","+duck");
	AddKeyBind("Shoot","+attack");
	AddKeyBind("Reload","reload");
	AddKeyBind("Last Weapon","lastweapon");
	AddKeyBind("Next Weapon","nextweapon");
	AddKeyBind("Bullet Time","bullettimetoggle");
	AddKeyBind("Buy Menu","buymenu");

	AddKeyBind("Weapons 1","slot1");
	AddKeyBind("Weapons 2","slot2");
	AddKeyBind("Weapons 3","slot3");
	AddKeyBind("Weapons 4","slot4");
	AddKeyBind("Weapons 5","slot5");



	LoadKeys();
}

void COptions::Do(void)
{
	OverButton();
	DoMouseClick();

	pcore->sound.m_volume = ((soundvolume.value/100.0f));

	pcore->Settings.musicvolume = ((musicvolume.value/100.0f));
	pcore->RefreshMusicVolume();

	DoKeyBinds();

}

void COptions::OverButton(void)
{
	for (int i=0;i<10;i++)
	{
		if (PtInRect(&buttons[i],pcore->menu.point))
		{
			this->mouseover = i;
			return;
		}
	}

	mouseover = 0;
}

void COptions::DoMouseClick(void)
{
	if (!GFX.m_bMouse1Click) return;

	char buffer[100];
	sprintf(buffer,"");

	if (mouseover == MO_FINISHED)
	{
		BindKeys();
		pcore->menu.bShowOptions = false;
		SaveOptions();
		pcore->menu.ResetInVariables();
	}

}

void COptions::DrawSlider(g_slider* s)
{
	pcore->SetAlphaMode(false);
	GFX.DrawText(s->x,s->y,s->name,255,255,255);
	GFX.SpriteDrawEx(t_opbits,s->x, s->y, 0,0,0,SCALE,SCALE,0,96,255,32,255,255,255,255);

	// handle
	GFX.SpriteDrawEx(t_opbits,s->x+((s->value*2.6f*SCALE))-(16*SCALE), s->y+(5.0f*SCALE), 0,0,0,
	SCALE,SCALE,
	0,128,32,32,	
	255,255,255,255);


	RECT handle;
	handle.top = s->y;
	handle.left = s->x;
	handle.right = s->x+(256*SCALE);
	handle.bottom = s->y+(32*SCALE);

	if (PtInRect(&handle,pcore->menu.point) && GFX.m_bMouse1Click)
	{
		s->selected = true;
		
	}

	if (s->selected)
	{
		s->value = ((pcore->menu.point.x - s->x)/2.6f)/SCALE;
		if (!GFX.m_bMouse1)
		{
			s->selected = false;
		}
	}


	if (s->value>100.0f) s->value = 100.0f;
	if (s->value<0) s->value = 0;
}

void COptions::SaveOptions(void)
{
	// write keys
	pcore->input.SaveUserBinds();
	
	// append to the file
	FILE* fp=NULL;
	char buffer [20];

	fp = fopen(CONFIG_FILENAME, "at");

	fputs("volume ",fp);
	fputs( gcvt(pcore->sound.m_volume,2,buffer),fp);
	fputs("\n",fp);

	fputs("musicvolume ",fp);
	fputs( gcvt(pcore->Settings.musicvolume,2,buffer),fp);
	fputs("\n",fp);

	fclose(fp);

}

void COptions::DoKeyBinds(void)
{
	if (!GFX.m_bMouse1) MouseDown = false;
	if (MouseDown) return;

	char cbuf[100];

	// handle key's that are selected
	for (int i=0;i<NUM_KEYS;i++)
	{
		if (keybinds[i].selected)
		{
			sprintf(keybinds[i].keyname,"PRESS KEY");

			// stop processing
			if (GFX.InputKeyDown(DIK_ESCAPE))
			{
				sprintf(keybinds[i].keyname, "");
				keybinds[i].selected = false;
				keybinds[i].keynum = 0;
				DoingKey=false;
			}
			else
			{
				for (int j=0;j<NUM_GKEYS;j++)
				{
					if (pcore->input.InputDown(pcore->input.m_keys[j].keycode))
					{
						DoingKey=false;
						sprintf(pcore->input.m_keys[j].action,"");

						sprintf(keybinds[i].keyname, pcore->input.m_keys[j].keyname);
						keybinds[i].keynum = pcore->input.m_keys[j].keycode;
						keybinds[i].selected = false;
						
						for (int k=0;k<NUM_KEYS;k++)
						{
							if (keybinds[i].keynum==keybinds[k].keynum && i!=k)
							{
								sprintf(keybinds[k].keyname, "");
								keybinds[k].keynum = 0;
							}
						}

						BindKeys();
						LoadKeys();
						break;
					}
				}

				if (GFX.m_bMouse1)
				{
					MouseDown = true;
					return;
				}
			}
		}
	}
	RECT handle;


	for (int i=0;i<NUM_KEYS;i++)
	{
		handle.top = keybinds[i].y;
		handle.left = keybinds[i].x;
		handle.right = keybinds[i].x+(200*SCALE);
		handle.bottom = keybinds[i].y+(16*SCALE);

		if (PtInRect(&handle,pcore->menu.point))
		{
			if (GFX.m_bMouse1Click)
			{
				if (keybinds[i].keynum>0)
				{
					pcore->input.BindCommandToKey(keybinds[i].keynum,"null");
				}
				
				keybinds[i].selected =true;
				MouseDown=true;
				DoingKey = true;
			}
		}
	}


}

void COptions::DrawKeyBind(g_keybind* k)
{
	if (k->x==0) return; 

	pcore->SetAlphaMode(false);
	GFX.SpriteDrawEx(t_opbits,k->x, k->y, 0,0,0,SCALE,SCALE,0,176,255,16,200,255,255,255);

	GFX.DrawTextEx(k->x+4, k->y+2, 255,255,255, k->name);
	GFX.DrawTextEx(k->x+(100*SCALE), k->y+2, 255,255,255, k->keyname);


	if (!this->DoingKey)
	{
		RECT handle;
		handle.top = k->y;
		handle.left = k->x;
		handle.right = k->x+(200*SCALE);
		handle.bottom = k->y+(16*SCALE);

		if (PtInRect(&handle,pcore->menu.point))
		{
			pcore->SetAlphaMode(true);

			for (int i=0;i<10;i++)
			{
				GFX.SpriteDrawEx(t_opbits,k->x+(rand()%10)-5, k->y+(rand()%10)-5, 0,0,0,SCALE,SCALE,0,160,255,16,5,255,255,255);
			}
		}
	}


	
}

void COptions::LoadKeys(void)
{
	for (int i=0;i<NUM_KEYS;i++)
	{
		sprintf(keybinds[i].keyname,"");
		keybinds[i].keynum = 0;
	}

	char buffer[100];

	for (int i=0;i<NUM_KEYS;i++)
	{
		if (keybinds[i].x>0)
		{
			for (int j=0;j<NUM_GKEYS;j++)
			{
				sprintf(buffer, "+%s",pcore->input.m_keys[j].action);
				if (strcmp(pcore->input.m_keys[j].action, keybinds[i].command)==0 || strcmp(buffer, keybinds[i].command)==0)
				{
					sprintf(keybinds[i].keyname, pcore->input.m_keys[j].keyname);
					keybinds[i].keynum = pcore->input.m_keys[j].keycode;
				}
			}
		}
	}
}

void COptions::AddKeyBind(char* name, char* command)
{
	for (int i=0;i<NUM_KEYS;i++)
	{
		if (keybinds[i].x==0)
		{
			sprintf(keybinds[i].name,name);
			sprintf(keybinds[i].command,command);
			keybinds[i].x = KEYX+(1*i*SCALE);
			keybinds[i].y = KEYY+(SPACING*i*pcore->menu.GuiScale);
			return;
		}
	}


}

// bind keys in options to keys in real life
void COptions::BindKeys(void)
{
	char buffer[100];

		for (int i=0;i<NUM_KEYS;i++)
		{
			if (keybinds[i].keynum>0 && keybinds[i].x>0)
			{
				sprintf(buffer,"bind %s %s",keybinds[i].keyname, keybinds[i].command);
				pcore->gui.WriteToConsole(5,255,255,100,buffer);
				pcore->gui.RunConsoleCommand(buffer);
				sprintf(buffer,"");
			}
		}
}
