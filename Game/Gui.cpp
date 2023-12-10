#include "gui.h"
#include "core.h"

#define WEPSEL_YOFFSET 20

#define CURSORX pcore->point.x
#define CURSORY pcore->point.y

#define GUI_HEALTH_OFFX 20
#define GUI_HEALTH_OFFY pcore->gfx.m_d3dpp.BackBufferHeight-(14*GuiScale)

#define GUI_WEPAMMOCLIP_OFFX pcore->gfx.m_d3dpp.BackBufferWidth-(200*GuiScale)
#define GUI_WEPAMMOCLIP_OFFY pcore->gfx.m_d3dpp.BackBufferHeight-(100*GuiScale)

#define GUI_WEPAMMOSPARE_OFFX pcore->gfx.m_d3dpp.BackBufferWidth-(120*GuiScale)
#define GUI_WEPAMMOSPARE_OFFY pcore->gfx.m_d3dpp.BackBufferHeight-(35*GuiScale)


#define BWIDTH (256*GuiScale)
#define BHEIGHT (64*GuiScale)
#define BASEX ((pcore->gfx.m_d3dpp.BackBufferWidth/2) - (BWIDTH/2))
#define BASEY ((pcore->gfx.m_d3dpp.BackBufferHeight/2) - (BHEIGHT/2))


CGui::CGui(void)
: KeyDown_Console(false)
, ShowConsole(false)
, GuiScale(0)
, HealthSize(0)
, bBuyMenuOpen(false)
, SelectedMenuItem(NULL)
{
	t_WeaponSlotNumbers=NULL;
	t_Colours=NULL;
	t_Numbers=NULL;

	for (int i=0;i<CONSOLE_LINES;i++)
	{
		sprintf(ConsoleText[i].line,"-");
		ConsoleText[i].r = 0;
		ConsoleText[i].g = 0;
		ConsoleText[i].b = 0;
	}

	sprintf(InputString,"");
}

CGui::~CGui(void)
{
}

void CGui::SetCore(CCore* rpcore)
{
	this->pcore = rpcore;
	
}

void CGui::Startup(void)
{
	if (t_WeaponSlotNumbers==NULL)
	{
		pcore->gfx.WriteToLog("GUI: Loading wep_numbers.bmp..<br>");
		t_WeaponSlotNumbers = pcore->textures.LoadTexture("textures/gui/wep_numbers.tga");
	}

	if (t_Colours==NULL)
	{
		pcore->gfx.WriteToLog("GUI: Loading colours.bmp..<br>");
		t_Colours = pcore->textures.LoadTexture("textures/gui/colours.tga");
	}

	if (t_Numbers==NULL)
	{
		pcore->gfx.WriteToLog("GUI: Loading gui_numbers.bmp..<br>");
		t_Numbers = pcore->textures.LoadTexture("textures/gui/gui_numbers.tga");
	}

	t_Damage = pcore->textures.LoadTexture("textures/gui/damage.tga");

	t_Base = pcore->textures.LoadTexture("textures/gui/gui_base.tga");
	t_BaseLeft = pcore->textures.LoadTexture("textures/gui/gui_left.tga");
	t_WepBack = pcore->textures.LoadTexture("textures/gui/dropdown/wep_back.tga");
	


	FILE* fp;
	FILE* temp_file;
	int i=0;
	int editing_id=0;
	int temp=0;
	int tempfloat=0.0f;
	char string[500];

	sprintf(string,"%s/data/menus.cfg",pcore->modfolder);
	if (!FileExists(string)) sprintf(string,"%s/data/menus.cfg",DEFAULT_MOD_FOLDER);

	fp = fopen(string,"r");

	if (fp!=NULL)
	{
		char item[300];
		char value[300];

		i=0;
		while (fgets (string , 300 , fp)!=NULL)
		{
			if (string[0]!='#' && string[0]!='\0' && string[0]!='\n' && string[0]!=' ')
			{
				
				sscanf(string,"%s %[^\n]",&item, &value);
				this->WriteToConsole(0,0,0,0,"item: %s", item);
				this->WriteToConsole(0,0,0,0,"valu: %s", value);
				this->WriteToConsole(0,0,0,0," ");

				if (strcmp(item,"id")==0) 
				{
					editing_id = atoi(value);
					menuitems[editing_id] = new sMenuItem();
					menuitems[editing_id]->enabled = true;
					menuitems[editing_id]->onceonly = false;
				}

				if (strcmp(item,"parent")==0)
				{
					temp = atoi(value); // get parent id

					menuitems[editing_id]->parent = menuitems[temp];
					for (int j=0;j<10;j++)
					{
						if (menuitems[temp]->subitems[j]==NULL)
						{
							menuitems[temp]->subitems[j] = menuitems[editing_id];
							break;
						}
					}
				}

				if (strcmp(item,"onceonly")==0)
				{
					menuitems[editing_id]->onceonly = true;
				}
				

				if (strcmp(item,"name")==0) sprintf(menuitems[editing_id]->name,value);

				if (strcmp(item,"x")==0) menuitems[editing_id]->offsetx = menuitems[editing_id]->parent->offsetx + atof(value);
				if (strcmp(item,"y")==0) menuitems[editing_id]->offsety = menuitems[editing_id]->parent->offsety + atof(value);

				if (strcmp(item,"command")==0) sprintf(menuitems[editing_id]->command,value);

				if (strcmp(item,"unlock")==0) menuitems[editing_id]->unlock = atoi(value);
				if (strcmp(item,"cost")==0) menuitems[editing_id]->cost = atoi(value);

				if (strcmp(item,"width")==0) menuitems[editing_id]->width = atoi(value);
				if (strcmp(item,"height")==0) menuitems[editing_id]->height = atoi(value);
				
				if (strcmp(item,"parentopen")==0) menuitems[editing_id]->keepparentopen = true;

				// menu textures
				if (strcmp(item,"gui")==0)
				{
					sprintf(item,"textures/gui/menu/%s",value);
					menuitems[editing_id]->texture = pcore->textures.LoadTexture(item);
				}

				// background textures
				sprintf(value,"textures/gui/menu/menu_%ix%i.bmp",menuitems[editing_id]->width, menuitems[editing_id]->height);
				temp_file = fopen(value, "r");
				if (temp_file!=NULL)
				{
					menuitems[editing_id]->backgroundtexture = pcore->textures.LoadTexture(value);
					fclose(temp_file);
				}
				else menuitems[editing_id]->backgroundtexture = NULL;




			}
		}
	}

	t_Locked = pcore->textures.LoadTexture("textures/gui/menu/menu_locked.tga");



	s_menu_open = pcore->sound.LoadSound(7,"sound/gui/menu_open.ogg");
	s_menu_move = pcore->sound.LoadSound(7,"sound/gui/menu_move.ogg");
	s_menu_close = pcore->sound.LoadSound(7,"sound/gui/menu_close.ogg");
	s_menu_buy = pcore->sound.LoadSound(7,"sound/gui/menu_buy.ogg");
	s_menu_unlock = pcore->sound.LoadSound(7,"sound/gui/menu_unlock.ogg");
	s_menu_fail = pcore->sound.LoadSound(7,"sound/gui/menu_fail.ogg");

	s_weapon_switch = pcore->sound.LoadSound(7,"sound/gui/weps_switch.ogg");
	s_weapon_close = pcore->sound.LoadSound(7,"sound/gui/weps_close.ogg");

	TookDamageTimer = timeGetTime();
	TookPlutoniumTimer = timeGetTime();
}


// draw vgui
void CGui::Do(void)
{
	TookDamageTimer = TookDamageTimer + pcore->gfx.RealTick;
	TookPlutoniumTimer = TookPlutoniumTimer + pcore->gfx.RealTick;
	GuiScale = (pcore->gfx.m_d3dpp.BackBufferWidth+pcore->gfx.m_d3dpp.BackBufferHeight)/((float)1024+768);
}


void CGui::DrawWeaponMenu(void)
{
	if (pcore->bWeaponSelect) WeaponMenuFade=255;

	if (WeaponMenuFade<0) return;

	if (!pcore->bWeaponSelect) { WeaponMenuFade = WeaponMenuFade - 0.3 * pcore->gfx.RealTick;}

	int count=0;
	int color=100;
	float scale;
	float xoffst;


	SelectedWeapon = pcore->players[pcore->MyPlayerID].player.m_iWeapon;
	pcore->SetAlphaMode(true);

	for (int slot=1;slot<6;slot++)
	{
		count=0;
		if (menu_SelectedSlot==slot)
		for (int i=0;i<MAX_WEAPONS_HOLDING;i++)
		{
			
			if (pcore->players[pcore->MyPlayerID].player.WeaponCarrying[i]>=0)
			if (pcore->weapons.m_GunAttributes[pcore->players[pcore->MyPlayerID].player.WeaponCarrying[i]].slot==slot)
			{
				
				if (menu_SelectedItem==count && menu_SelectedSlot==slot)
				{
					color=0;
					SelectedWeapon = pcore->players[pcore->MyPlayerID].player.WeaponCarrying[i];
					scale=1.0f;
					xoffst=0;
				}
				else
				{
					color=60;
					scale=0.8f;
					xoffst = 11.0f*GuiScale;
				}


				pcore->SetAlphaMode(false);
				pcore->gfx.SpriteDrawEx(this->t_WepBack,
									(slot*32*GuiScale)+xoffst,
									(64*count*GuiScale)+WEPSEL_YOFFSET+xoffst,
									0,0,0,
									GuiScale*1.0f*scale,GuiScale*1.0f*scale,
									(int)(WeaponMenuFade-color<0?0:WeaponMenuFade-color),
									(int)(255-color),
									(int)(255-color),
									(int)(255-color));
				//pcore->SetAlphaMode(true);

				pcore->gfx.SpriteDrawEx(pcore->weapons.m_GuiTexture[pcore->players[pcore->MyPlayerID].player.WeaponCarrying[i]],
									(slot*32*GuiScale)+(6*GuiScale*scale)+xoffst,
									(64*count*GuiScale)+(3*GuiScale*scale)+WEPSEL_YOFFSET+xoffst,
									0,0,0,
									GuiScale*0.9f*scale,GuiScale*0.9f*scale,
									(int)(WeaponMenuFade-color<0?0:WeaponMenuFade-color),
									(int)(255-color),
									(int)(255-color),
									(int)(255-color));

				
				count++;
			}
		}
			
		if (menu_SelectedSlot!=slot)
			{
				
				pcore->SetAlphaMode(true);
				pcore->gfx.SpriteDrawEx(this->t_WeaponSlotNumbers,
								((slot)*32*this->GuiScale)+(menu_SelectedSlot<slot?(128-32)*GuiScale:0),
									0+WEPSEL_YOFFSET,
									0,0,0,
									1*GuiScale,1*GuiScale,
									(slot-1)*32,0,32,32,
									(WeaponMenuFade<0?0:WeaponMenuFade),255,255,255);
				pcore->gfx.SpriteDrawEx(this->t_WeaponSlotNumbers,
								((slot)*32*this->GuiScale)+(menu_SelectedSlot<slot?(128-32)*GuiScale:0)+1,
									0+WEPSEL_YOFFSET+2,
									0,0,0,
									1*GuiScale,1*GuiScale,
									(slot-1)*32,0,32,32,
									(WeaponMenuFade<0?0:WeaponMenuFade),255,255,255);
			}
	}

	pcore->SetAlphaMode(false);
}

void CGui::HandleWeaponSelect(int numberpressed)
{
	if (menu_SelectedSlot == numberpressed)
	{
//		pcore->sound.PlaySnd(S_GUI_ROTATEWEAPON,0.0f,0.0f,0.0f,0.0f, 1.0f);
		//pcore->sound.PlaySnd(S_GUI_ROTATEWEAPON,0,0);
	}
	else
	{
//		pcore->sound.PlaySnd(S_GUI_OPENWEAPON,0.0f,0.0f,0.0f,0.0f, 1.0f+(numberpressed/3.0f));
	}

	menu_SelectedItem++;
	menu_SelectedSlot = numberpressed;

	int count=0;

	for (int i=0;i<MAX_WEAPONS_HOLDING;i++)
	{
		if (pcore->players[pcore->MyPlayerID].player.WeaponCarrying[i]>=0)
		if (pcore->weapons.m_GunAttributes[pcore->players[pcore->MyPlayerID].player.WeaponCarrying[i]].slot==menu_SelectedSlot)
		{
			count++;
		}
	}

	if (menu_SelectedItem>=count) menu_SelectedItem=0;
}

void CGui::DoSelectedWeapon(void)
{
	pcore->sound.PlaySnd(this->s_weapon_close,0,0,0,-5,1.0f);
}

void CGui::WriteToConsole(int importance,int r, int g, int b, char *lpszText, ...)
{
	va_list argList;
	char temp[500];

	for (int i=0;i<CONSOLE_LINES-1;i++)
	{
		sprintf(ConsoleText[i].line,ConsoleText[i+1].line);
		ConsoleText[i].r = ConsoleText[i+1].r;
		ConsoleText[i].g = ConsoleText[i+1].g;
		ConsoleText[i].b = ConsoleText[i+1].b;
	}


	va_start(argList, lpszText);
	vsprintf(temp, lpszText, argList);
	sprintf(ConsoleText[CONSOLE_LINES-1].line,temp);
	ConsoleText[CONSOLE_LINES-1].r = r;
	ConsoleText[CONSOLE_LINES-1].g = g;
	ConsoleText[CONSOLE_LINES-1].b = b;
}

void CGui::DrawConsole(void)
{
	if (!ShowConsole) return;

	pcore->gfx.SpriteDrawEx(t_Colours, 0,0,0,0,0,pcore->gfx.m_d3dpp.BackBufferWidth,350,0,0,1,1,240,255,255,255);
	pcore->gfx.SpriteDrawEx(t_Colours, 0,350,0,0,0,pcore->gfx.m_d3dpp.BackBufferWidth,2,4,0,1,1,255,255,255,255);


	for (int i=0;i<CONSOLE_LINES;i++)
	{
		pcore->gfx.DrawText(19,19+(i*15),ConsoleText[i].line, 200, 200, 200);
		pcore->gfx.DrawText(22,22+(i*15),ConsoleText[i].line, 200, 200, 200);
		pcore->gfx.DrawText(21,21+(i*15),ConsoleText[i].line, 230, 230, 230);
		pcore->gfx.DrawText(20,20+(i*15),ConsoleText[i].line, ConsoleText[i].r, ConsoleText[i].g, ConsoleText[i].b);
	}

		
	pcore->gfx.DrawText(21,30+(CONSOLE_LINES*15),InputString, 255, 255, 255);
	pcore->gfx.DrawText(19,30+(CONSOLE_LINES*15),InputString, 255, 255, 255);
	pcore->gfx.DrawText(20,30+(CONSOLE_LINES*15),InputString, 0, 0, 0);

}

void CGui::Draw(void)
{
	DamageSign();
	DrawPlayerStuff();
	DrawBuyMenu();
	pcore->SetAlphaMode(false);
	DrawWeaponMenu();
	DrawConsole();
}

void CGui::ConsoleKey(void)
{
	if (KeyDown_Console) return;
	
	KeyDown_Console = true;

	if (ShowConsole)
	{
		ShowConsole=false;
		pcore->bPaused=false;
	}
	else 
	{
		ShowConsole=true;
	}

}

#define BLX ((16*GuiScale))
#define BLY	(pcore->gfx.m_d3dpp.BackBufferHeight-(126*GuiScale))

void CGui::DrawPlayerStuff(void)
{
	float health = pcore->players[pcore->MyPlayerID].player.m_health;
	float bullettime = pcore->players[pcore->MyPlayerID].player.BulletTime;

	DrawPlutonium(1);

	pcore->SetAlphaMode(false);
	// gui
	pcore->gfx.SpriteDraw(t_Base,
	pcore->gfx.m_d3dpp.BackBufferWidth-(512*GuiScale),
	pcore->gfx.m_d3dpp.BackBufferHeight-(512*GuiScale),
	0,0,0,
	GuiScale,GuiScale);

	pcore->gfx.SpriteDraw(t_BaseLeft,
	BLX,
	BLY,
	0,0,0,
	GuiScale,GuiScale);

	pcore->SetAlphaMode(true);

	// bullettime
	if (bullettime>0)
	for (int i=0;i<1;i++)
	pcore->gfx.SpriteDrawEx(t_Colours, 
		BLX+(425*GuiScale),
		BLY+(68*GuiScale)+(30*GuiScale)-(0.69f*GuiScale*bullettime),
		0,0,0,
		56*GuiScale,0.70f*GuiScale*bullettime,
		10,0,1,1,
		230,100,100,100);

	
	DrawPlutonium(2);



	float hd = 70;

	if (TookDamageTimer<2000)
	{
		hd = 255 -(TookDamageTimer/(2000.0f/185.0f));
	}



	// health bar
	if (health>0)
	pcore->gfx.SpriteDrawEx(t_Colours, 
		pcore->gfx.m_d3dpp.BackBufferWidth-(38*GuiScale),
		pcore->gfx.m_d3dpp.BackBufferHeight-((266*GuiScale)-(2.56f*GuiScale*(100-health))),
		0,0,0,
		32*GuiScale,2.56f*GuiScale*health,
		7,0,1,1,
		200,hd,hd,hd);

	


	

	int weaponclip = pcore->players[pcore->MyPlayerID].player.m_BulletsInClip[pcore->players[pcore->MyPlayerID].player.m_iWeapon];
	DrawNumber(weaponclip,
				(int)(pcore->gfx.m_d3dpp.BackBufferWidth-(451*GuiScale)),
			    (int)(pcore->gfx.m_d3dpp.BackBufferHeight-(40*GuiScale)),
				0.8f);

	int spareammo = pcore->players[pcore->MyPlayerID].player.m_SpareAmmo[pcore->weapons.m_GunAttributes[pcore->players[pcore->MyPlayerID].player.m_iWeapon].AmmoType];
	DrawNumber(spareammo,
				(int)(pcore->gfx.m_d3dpp.BackBufferWidth-(420*GuiScale)),
			    (int)(pcore->gfx.m_d3dpp.BackBufferHeight-(17*GuiScale)),
				0.40f);

	// todo: draw weapon name on a texture
//	pcore->gfx.DrawText((int)(pcore->gfx.m_d3dpp.BackBufferWidth-(335*GuiScale)),
//						(int)(pcore->gfx.m_d3dpp.BackBufferHeight-(63*GuiScale)),
//						pcore->weapons.m_GunAttributes[pcore->players[pcore->MyPlayerID].player.m_iWeapon].name,
//						255,255,255);
	
	
	// number of lives

	if (pcore->GlobalStats.Lives>=0)
	for (int i=0;i<2;i++)
		DrawNumber(pcore->GlobalStats.Lives-1,
				BLX+(31*GuiScale),
			    BLY+(87*GuiScale),
				0.60f);

	pcore->SetAlphaMode(false);
}

void CGui::ConsoleInput(char chr)
{
	if (chr=='`') return;
	if (!ShowConsole) return;
	if (chr==13) {this->WriteToConsole(5,255,0,0,InputString);DoConsoleCommand();return;}
	if (chr==27) {ShowConsole=false; return;}
	
	// delete
	if (chr==8) 
	{
		if (strlen(InputString)==0) return;
		InputString[strlen(InputString)-1]='\0';
		return;
	}

	char oldstring[500];
	sprintf(oldstring,InputString);
	sprintf(InputString,"%s%c",oldstring, chr);
}
#define STRINGEQUAL(a,b) strcmp(a,b)==0

void CGui::DoConsoleCommand(void)
{
	this->RunConsoleCommand(InputString);
	sprintf(InputString,"");
}

void CGui::DrawNumber(int number, int x, int y, float scale)
{
	char buffer[20], bufferchar[2];
	bufferchar[1] = '\0';

	sprintf(buffer,"%i\0", number);

	int i, num;
	i=0;
	while (i<strlen(buffer))
	{
		bufferchar[0] = buffer[i];
		num = atoi(bufferchar);
		pcore->gfx.SpriteDrawEx(t_Numbers, x+(i*20*scale*GuiScale),y,0,0,0,GuiScale*scale,GuiScale*scale,num*26,0,26,32,255,255,255,255);
		i++;
	}

}

void CGui::DrawPlutonium(int l)
{
	float hd = 100;

	if (l==1)
	{
		if (TookPlutoniumTimer<500)
		{
			hd = 255 -(TookPlutoniumTimer/(500.0f/155.0f));
		}

		// bar
		pcore->gfx.SpriteDrawEx(t_Colours, 
			pcore->gfx.m_d3dpp.BackBufferWidth-(368*GuiScale),
			pcore->gfx.m_d3dpp.BackBufferHeight-(28*GuiScale),
			0,0,0,
			(pcore->players[pcore->MyPlayerID].player.m_plutonium%100)*GuiScale*2.56f,
			16*GuiScale,
			10,0,1,1,
			235,hd,hd,hd);

		return;
	}

	// number
	DrawNumber(((int)pcore->players[pcore->MyPlayerID].player.m_plutonium/100),
				(int)(pcore->gfx.m_d3dpp.BackBufferWidth-(90*GuiScale)),
			    (int)(pcore->gfx.m_d3dpp.BackBufferHeight-(29*GuiScale)),
				0.5f);
}



void CGui::ShowBuyMenu(void)
{
	if (bBuyMenuOpen) return;
	if (pcore->doStartLevel) return;
	
	// if we're dead, close the menu
	if (pcore->players[pcore->MyPlayerID].player.bDead) 
	{
		bBuyMenuOpen = false;
		pcore->bShowBuyMenu=false;
		return;
	}

	if (pcore->Settings.repositionbuycursor)
	{
		RECT r;
		GetWindowRect(pcore->hWnd,&r);
		SetCursorPos(BASEX+r.left+(BWIDTH/2.0f),BASEY+r.top+(BHEIGHT/2.0f)+5.0f);
		pcore->UpdateCursorPosition();
	}

	pcore->sound.PlaySnd(s_menu_open,0,0,0,-5,1.0f);
	

	SelectedMenuItem=menuitems[0];
	SelectedMenuItemNext=menuitems[0];
		
	bBuyMenuOpen=true;
}





void CGui::DrawBuyMenu(void)
{
	if (!bBuyMenuOpen) return;

	pcore->SetAlphaMode(false);

	// if we're dead, close the menu
	if (pcore->players[pcore->MyPlayerID].player.bDead) 
	{
		bBuyMenuOpen = false;
		return;
	}

	SelectedMenuItemNext = SelectedMenuItem;
	
	bool done=false;
	bool selected=false;
	int i=0;
	sMenuItem* mip=menuitems[0];

	if (SelectedMenuItem!=mip && SelectedMenuItem!=NULL) mip=SelectedMenuItem;

	

	if (SelectedMenuItem==menuitems[0])
	{
		DrawMenuSubitems(menuitems[0]);
	}
	else
	{
		while (mip!=menuitems[0])
		{
			DrawPopupMenuItem(mip);
			if (SelectedMenuItem==mip) 
			{
				DrawMenuSubitems(mip);
				if (SelectedMenuItem->keepparentopen) DrawMenuSubitems(mip->parent);
			}
			

			mip = mip->parent;
		}

		DrawPopupMenuItem(menuitems[0]);		
	}

	SelectedMenuItem = SelectedMenuItemNext;

}

void CGui::CloseBuyMenu(void)
{
	if (!bBuyMenuOpen) return;


	bool closemenu=true;

	if (SelectedMenuItem!=NULL)
	if (SelectedMenuItem->enabled)
	{
		int px, py;

		px = BASEX + SelectedMenuItem->offsetx*GuiScale;
		py = BASEY + SelectedMenuItem->offsety*GuiScale;

		if (CURSORX>px && CURSORX<(px+(SelectedMenuItem->width*GuiScale)) && CURSORY>(py) && CURSORY<(py+(SelectedMenuItem->height*GuiScale)) )
		{
			this->WriteToConsole(5,255,0,0,"chose something");

			if (SelectedMenuItem->unlock>0)
			{
				if (pcore->players[pcore->MyPlayerID].player.SpendMoney(SelectedMenuItem->unlock))
				{
					SelectedMenuItem->unlock = 0;
					pcore->sound.PlaySnd(s_menu_unlock,0,0,0,-5,1.0f);
					closemenu=false;
				}
				else
				{
					pcore->sound.PlaySnd(s_menu_fail,0,0,0,-5,1.0f);
					closemenu=false;
				}
			}

			if (SelectedMenuItem->cost>0)
			if (SelectedMenuItem->unlock==0)
			{
				if (pcore->players[pcore->MyPlayerID].player.SpendMoney(SelectedMenuItem->cost))
				{
					// BOUGHT ITEM OK
					RunConsoleCommand(SelectedMenuItem->command);
					if (SelectedMenuItem->onceonly) SelectedMenuItem->enabled=false;
				
					pcore->sound.PlaySnd(s_menu_buy,0,0,0,-5,1.0f);
					LPParticle p = pcore->landscape.AddParticle(PARTICLE_GUI_BOUGHT,px+(pcore->landscape.ParticleTypes[PARTICLE_GUI_BOUGHT].offsetx*GuiScale), py+(pcore->landscape.ParticleTypes[PARTICLE_GUI_BOUGHT].offsety*GuiScale));
					if (p!=NULL)
					{
						p->textureoverride = SelectedMenuItem->texture;
						p->xscale = GuiScale;
						p->yscale = GuiScale;
					}

				}
				else
				{
					this->WriteToConsole(5,255,0,0,"not enough money");
					pcore->sound.PlaySnd(s_menu_fail,0,0,0,-5,1.0f);
					closemenu=false;
				}
			}
		}
		else
		{
			this->WriteToConsole(5,255,0,0,"Nothing chosen from menu");
			pcore->sound.PlaySnd(s_menu_close,0,0,0,-5,1.0f);
		}
	}
	else
	{
		pcore->sound.PlaySnd(s_menu_close,0,0,0,-5,1.0f);
	}

	if (closemenu)
	{
		SelectedMenuItem = NULL;
		bBuyMenuOpen =false;
		pcore->bPaused=false;
		pcore->bShowBuyMenu=false;
	}
}
void CGui::DrawPopupMenuItem(sMenuItem* mi)
{
	if (mi==NULL) return;
	if (!mi->enabled) return;

	char buffer[100];

	int px, py;

	float scm = 1;
	float scmo = 1;
	
	

	px = BASEX + (mi->offsetx*GuiScale);
	py = BASEY + (mi->offsety*GuiScale);

	if (CURSORX>(px) && CURSORX<(px+(mi->width*GuiScale)) && CURSORY>(py) && CURSORY<(py+(mi->height*GuiScale)) )
	{
		if (SelectedMenuItem != mi)
			pcore->sound.PlaySnd(s_menu_move,0,0,0,-5,1.0f);
			SelectedMenuItemNext = mi;
		scm=1.0f;
		scmo=1.0f;
	}

/*
	if (mi->backgroundtexture!=NULL)
		pcore->gfx.SpriteDrawEx(mi->backgroundtexture,
		px-(scmo*mi->width*0.5f)+(mi->width/2.0f),py-(scmo*mi->height*0.5f)+(mi->height/2.0f),
		0,0,0,GuiScale*scmo,GuiScale*scmo,0,0,
		mi->width,mi->height,
		SelectedMenuItem==mi?255:100,255,255,255);*/
/*

*/
	if (pcore->gfx.InputKeyDown(DIK_C))
		pcore->gfx.SpriteDrawEx(t_Colours,px,py,0,0,0,mi->width*GuiScale,mi->height*GuiScale,1,1,1,1,SelectedMenuItem==mi?255:200,255,255,255);

	bool canafford = pcore->players[pcore->MyPlayerID].player.CanSpendMoney(mi->cost);

	if (!pcore->players[pcore->MyPlayerID].player.CanSpendMoney(mi->unlock)) canafford=false;

	

	pcore->gfx.SpriteDrawEx(mi->texture,
		px-(scm*mi->width*0.5f)+(mi->width/2.0f),
		py-(scmo*mi->height*0.5f)+(mi->height/2.0f),
		0,0,0,
		GuiScale*scm,
		GuiScale*scmo,
		0,0,
		mi->width,mi->height,
		SelectedMenuItem==mi?255:200,
		255,
		canafford?255:150,
		canafford?255:150);

	
//	pcore->gfx.DrawText(px+(10*GuiScale), py+(10*GuiScale),	mi->name,255,255,255);

	if (mi->unlock>0) 
	{
		pcore->gfx.SpriteDrawEx(t_Locked,
		px-(20*GuiScale),py-(20*GuiScale),
		0,32,32,
		GuiScale*0.7f,GuiScale*0.7f,
		0,0,64,64,
		255,255,255,255);

		pcore->SetAlphaMode(true);
		this->DrawNumber(mi->unlock,px-(4*GuiScale),py-(0*GuiScale),0.6f);
		pcore->SetAlphaMode(false);
	}
}

void CGui::DrawMenuSubitems(sMenuItem* mi)
{
	if (mi==NULL) return;
	if (mi->unlock!=0) return;

	for (int i=0;mi->subitems[i]!=NULL;i++)
	{
		DrawPopupMenuItem(mi->subitems[i]);
	}
}

void CGui::TookDamage(void)
{
	TookDamageTimer = 0; // count the # of milliseconds since last damage
}

void CGui::TookPlutonium(void)
{
	TookPlutoniumTimer = 0; // count the # of milliseconds since last damage
}

void CGui::WeaponSlot(unsigned int slot)
{
	pcore->sound.PlaySnd(this->s_weapon_switch,0,0,0,-5,1.0f);
	if (menu_SelectedSlot!=slot && !menuKeyDown) menu_SelectedItem=-1;		
	HandleWeaponSelect(slot);
	pcore->bWeaponSelect=true;
}

void CGui::Exec(char* filename)
{
	char string[200];
	FILE* fp = fopen(filename,"r");

	if (fp!=NULL)
	{
		while (fgets (string , 100 , fp)!=NULL)
		{
			RunConsoleCommand(string);	
		}
	}
	else
	{
		this->WriteToConsole(8,255,0,0,"File not found");
	}
}

void CGui::RunConsoleCommand(char* inputcommand)
{
	char arg1[100], arg2[100], command[100];
	sprintf(arg1,"");
	sprintf(arg2,"");
	sscanf(inputcommand, "%s %s %s", &command, &arg1, &arg2);

	if (STRINGEQUAL(command,"map"))
	{
		if (STRINGEQUAL(arg1,"de_dust")) this->WriteToConsole(0,0,0,0,"Fuck you idiot.");
		else if (!STRINGEQUAL(arg1,""))
		{
			pcore->landscape.LoadMap(arg1);
			pcore->StartLevel();
		}
	}
	if (STRINGEQUAL(command,"bind"))
	{
		pcore->input.bind(inputcommand);
	}
	if (STRINGEQUAL(command,"switchbuffer"))
	{
		if (pcore->ViewLayer==0)
		{
			pcore->ViewLayer=1;
			this->WriteToConsole(0,0,0,0,"Switched to Normal Map Buffer");
		}
		else
		{
			pcore->ViewLayer=0;
			this->WriteToConsole(0,0,0,0,"Switched to Backbuffer");
		}
	}
	else if (STRINGEQUAL(command,"music"))
	{
		if (STRINGEQUAL(arg1,"1"))
		{
			pcore->Settings.enablemusic = true;
			this->WriteToConsole(0,0,0,0,"Music Enabled");
			pcore->StartLevelMusic();
		}
		else if (STRINGEQUAL(arg1,"0"))
		{
			pcore->Settings.enablemusic = false;
			this->WriteToConsole(0,0,0,0,"Music Disabled");
			pcore->sound.CloseStream(pcore->landscape.music);
			pcore->landscape.music=NULL;
		}
		else
		{
			if (pcore->Settings.enablemusic) this->WriteToConsole(0,0,0,0,"Music is Enabled");
			else  this->WriteToConsole(0,0,0,0,"Music is Disabled");
		}
	}
	else if (STRINGEQUAL(command,"soundstretching"))
	{
		if (STRINGEQUAL(arg1,"1"))
		{
			pcore->Settings.SoundStretching = true;
		}
		else if (STRINGEQUAL(arg1,"0"))
		{
			pcore->Settings.SoundStretching = false;
		}

		if (pcore->Settings.SoundStretching) 
			this->WriteToConsole(0,0,0,0,"Sound Stretching is Enabled");
		else  
			this->WriteToConsole(0,0,0,0,"Sound Stretching is Disabled");
	}
	else if (STRINGEQUAL(command,"fastswitch"))
	{
		if (STRINGEQUAL(arg1,"1")) pcore->Settings.FastSwitch = true;
		else if (STRINGEQUAL(arg1,"0"))	pcore->Settings.FastSwitch = false;

		if (pcore->Settings.FastSwitch) this->WriteToConsole(0,0,0,0,"fastswitch is Enabled");
		else this->WriteToConsole(0,0,0,0,"fastswitch is Disabled");
	}
	
	else if (STRINGEQUAL(command,"dynamicsoundstretching"))
	{
		if (STRINGEQUAL(arg1,"1"))
		{
			pcore->Settings.DynamicSoundStretching = true;
		}
		else if (STRINGEQUAL(arg1,"0"))
		{
			pcore->Settings.DynamicSoundStretching = false;
		}
		
		if (pcore->Settings.DynamicSoundStretching) 
			this->WriteToConsole(0,0,0,0,"Dynamic Sound Stretching is Enabled");
		else  
			this->WriteToConsole(0,0,0,0,"Dynamic Sound Stretching is Disabled");

	}
	else if (STRINGEQUAL(command,"record"))
	{
			pcore->cdemo.Record(arg1);
	}
	else if (STRINGEQUAL(command,"stop"))
	{
			pcore->cdemo.RecordEnd();
	}
	else if (STRINGEQUAL(command,"play"))
	{
			pcore->cdemo.Play(arg1);
	}
	else if (STRINGEQUAL(command,"quit"))
	{
		pcore->ExitGame();
	}
	else if (STRINGEQUAL(command,"giveall"))
	{
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(WEP_DEAGLE);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(WEP_PYTHON);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(1);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(0);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(2);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(3);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(4);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(5);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(6);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(7);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(8);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(9);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(10);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(11);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(WEP_LANDMINE);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(WEP_SMAW);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(WEP_AK47);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(WEP_MAC10);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(WEP_MOLOTOV);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(WEP_FLAMER);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(WEP_CHAINSAW);
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(WEP_BAT);
	}
	
	else if (STRINGEQUAL(command,"sleeptime"))
	{
		if (STRINGEQUAL(arg1,"")) this->WriteToConsole(0,0,0,0,"SleepTime : %i", pcore->Settings.sleeptime);
		else pcore->Settings.sleeptime = atoi(arg1);
	}
	else if (STRINGEQUAL(command,"volume"))
	{
		if (STRINGEQUAL(arg1,"")) this->WriteToConsole(0,0,0,0,"Volume : %.02f", pcore->sound.m_volume);
		else pcore->sound.m_volume = atof(arg1);
	}
	else if (STRINGEQUAL(command,"musicvolume"))
	{
		if (STRINGEQUAL(arg1,"")) this->WriteToConsole(0,0,0,0,"Music Volume : %.02f", pcore->Settings.musicvolume);
		else pcore->Settings.musicvolume = atof(arg1);
		pcore->RefreshMusicVolume();
	}
	else if (STRINGEQUAL(command,"shadersoff"))
	{
		pcore->Settings.shaders = false;
		this->WriteToConsole(10,0,0,0,"Shaders Turned Off");
	}
	else if (STRINGEQUAL(command,"shaderson"))
	{
		pcore->Settings.shaders = true;
		pcore->InitPostProcessing();
		pcore->CreateRenderTargerts();
		this->WriteToConsole(10,0,0,0,"Shaders Turned on");
	}
	else if (STRINGEQUAL(command,"flarescale"))
	{
		if (STRINGEQUAL(arg1,"")) this->WriteToConsole(0,0,0,0,"flarescale = %.02f", pcore->Settings.fFlareScale);
		else pcore->Settings.fFlareScale = atof(arg1);
	}
	else if (STRINGEQUAL(command,"sb"))
	{
		if (STRINGEQUAL(arg1,"")) this->WriteToConsole(0,0,0,0,"sb = %.02f", pcore->Settings.sb);
		else pcore->Settings.sb = atoi(arg1);
	}
	else if (STRINGEQUAL(command,"db"))
	{
		if (STRINGEQUAL(arg1,"")) this->WriteToConsole(0,0,0,0,"db = %.02f", pcore->Settings.db);
		else pcore->Settings.db = atoi(arg1);
	}
	else if (STRINGEQUAL(command,"giveweapon"))
	{
		pcore->players[pcore->MyPlayerID].player.GiveWeapon(pcore->weapons.NameToID(arg1));

		// + 2 clips
		for (int i=0;i<2;i++)
		pcore->players[pcore->MyPlayerID].player.GiveAmmo(pcore->weapons.m_GunAttributes[pcore->weapons.NameToID(arg1)].AmmoType, pcore->weapons.m_GunAttributes[pcore->weapons.NameToID(arg1)].ClipSize);
		
	}
	else if (STRINGEQUAL(command,"give9mmammo"))
	{
		pcore->players[pcore->MyPlayerID].player.GiveAmmo(AMMO_9MM, atoi(arg1));
	}
	else if (STRINGEQUAL(command,"giverifleammo"))
	{
		pcore->players[pcore->MyPlayerID].player.GiveAmmo(AMMO_RIFLE, atoi(arg1));
	}
	else if (STRINGEQUAL(command,"giveshotgunammo"))
	{
		pcore->players[pcore->MyPlayerID].player.GiveAmmo(AMMO_SHOTGUN, atoi(arg1));
	}
	else if (STRINGEQUAL(command,"givechainammo"))
	{
		pcore->players[pcore->MyPlayerID].player.GiveAmmo(AMMO_CHAIN, atoi(arg1));
	}
	else if (STRINGEQUAL(command,"givearrowammo"))
	{
		pcore->players[pcore->MyPlayerID].player.GiveAmmo(AMMO_ARROWS, atoi(arg1));
	}
	else if (STRINGEQUAL(command,"givegrenadeammo"))
	{
		pcore->players[pcore->MyPlayerID].player.GiveAmmo(AMMO_GRENADES, atoi(arg1));
	}
	else if (STRINGEQUAL(command,"givepetrolammo"))
	{
		pcore->players[pcore->MyPlayerID].player.GiveAmmo(AMMO_PETROL, atoi(arg1));
	}
	else if (STRINGEQUAL(command,"playsound"))
	{
		pcore->sound.PlayFromFilename(arg1);
	}
	else if (STRINGEQUAL(command,"camerascorch"))
	{
		if (pcore->Settings.camerascorch)
		{
			WriteToConsole(5,255,0,0,"Camera Scorch turned OFF");
			pcore->Settings.camerascorch=false;
		}
		else
		{
			WriteToConsole(5,255,0,0,"Camera Scorch turned ON");
			pcore->Settings.camerascorch=true;
		}
	}
	else if (STRINGEQUAL(command,"duck"))
	{
		pcore->players[pcore->MyPlayerID].player.Duck();
	}
	else if (STRINGEQUAL(command,"slot1"))
	{
		this->WeaponSlot(1);
	}
	else if (STRINGEQUAL(command,"slot2"))
	{
		this->WeaponSlot(2);
	}
	else if (STRINGEQUAL(command,"slot3"))
	{
		this->WeaponSlot(3);
	}
	else if (STRINGEQUAL(command,"slot4"))
	{
		this->WeaponSlot(4);
	}
	else if (STRINGEQUAL(command,"slot5"))
	{
		this->WeaponSlot(5);
	}
	else if (STRINGEQUAL(command,"reload"))
	{
		pcore->players[pcore->MyPlayerID].player.Reload();
	}
	else if (STRINGEQUAL(command,"setzoom"))
	{
		pcore->landscape.SetZoom(atof(arg1));
	}
	else if (STRINGEQUAL(command,"exit"))
	{
		pcore->toMenuScreen();
	}
	else if (STRINGEQUAL(command,"up"))
	{
		pcore->players[pcore->MyPlayerID].player.ClimbUpLadder();
	}
	else if (STRINGEQUAL(command,"down"))
	{
		pcore->players[pcore->MyPlayerID].player.ClimbDownLadder();
	}
	else if (STRINGEQUAL(command,"left"))
	{
		pcore->players[pcore->MyPlayerID].player.WalkLeft();
	}
	else if (STRINGEQUAL(command,"right"))
	{
		pcore->players[pcore->MyPlayerID].player.WalkRight();
	}
	else if (STRINGEQUAL(command,"jump"))
	{
		pcore->players[pcore->MyPlayerID].player.Jump();
	}
	else if (STRINGEQUAL(command,"mouthoff"))
	{
		pcore->players[pcore->MyPlayerID].player.DoMouthOff();
	}
	else if (STRINGEQUAL(command,"attack"))
	{
		pcore->DoAttack();
	}
	else if (STRINGEQUAL(command,"exec"))
	{
		Exec(arg1);
	}
	else if (STRINGEQUAL(command,"buymenu"))
	{
		pcore->bShowBuyMenu = true;
	}
	else if (STRINGEQUAL(command,"setspeed"))
	{
		pcore->timemultiplier = atof(arg1);
	}
	else if (STRINGEQUAL(command,"gravity"))
	{
		if (STRINGEQUAL(arg1,"")) this->WriteToConsole(0,0,0,0,"Gravity : %.02f", pcore->gravity);
		else pcore->gravity = atof(arg1);
	}
	else if (STRINGEQUAL(command,"recoil"))
	{
		if (STRINGEQUAL(arg1,"")) this->WriteToConsole(0,0,0,0,"Recoil Multiplier : %.02f", pcore->RecoilMultiplier);
		else pcore->RecoilMultiplier = atof(arg1);
	}
	else if (STRINGEQUAL(command, "nextweapon"))
	{
		NextWeapon(1);
	}
	else if (STRINGEQUAL(command, "lastweapon"))
	{
		NextWeapon(-1);
	}
	else if (STRINGEQUAL(command, "bullettimetoggle"))
	{
		pcore->players[pcore->MyPlayerID].player.DoBulletTimeToggle();
	}
}

void CGui::NextWeapon(int step)
{
	pcore->bWeaponSelect=true;

	pcore->sound.PlaySnd(this->s_weapon_switch,0,0,0,-5,1.0f);

	menu_SelectedItem += step;

	if (menu_SelectedItem>=NumWeaponsInSlot(menu_SelectedSlot) || menu_SelectedItem<0)
	{
		menu_SelectedSlot += step;
		while (NumWeaponsInSlot(menu_SelectedSlot)==0)
		{
			menu_SelectedSlot += step;
			if (menu_SelectedSlot>5) menu_SelectedSlot=1;
			if (menu_SelectedSlot<1) menu_SelectedSlot=5;
		}
		if (step<0) menu_SelectedItem = NumWeaponsInSlot(menu_SelectedSlot)-1;
		else if (step>0) menu_SelectedItem=0;
	}

	if (pcore->Settings.FastSwitch)
	{
		DoSelectedWeapon(); // plays sound
		DrawWeaponMenu();
		pcore->players[pcore->MyPlayerID].player.ChangeWeapon(SelectedWeapon);
		pcore->bWeaponSelect=false;
	}
}

int CGui::NumWeaponsInSlot(unsigned int slot)
{
	int count=0;

	for (int i=0;i<MAX_WEAPONS_HOLDING;i++)
	{
		if (pcore->players[pcore->MyPlayerID].player.WeaponCarrying[i]>=0)
		if (pcore->weapons.m_GunAttributes[pcore->players[pcore->MyPlayerID].player.WeaponCarrying[i]].slot==slot)
		{
			count++;
		}
	}

	return count;
}

void CGui::DamageSign(void)
{
	if (TookDamageTimer>500) return;

	//pcore->SetAlphaMode(true);

	pcore->gfx.SpriteDrawEx(t_Damage,0,0,0,0,0,(pcore->gfx.m_d3dpp.BackBufferWidth/256.0f), (pcore->gfx.m_d3dpp.BackBufferHeight/256.0f),(int)(((500-TookDamageTimer)/500.0f)*255.0f),255,255,255);
}
