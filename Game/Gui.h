#pragma once

#include "garryswrappers/GarrysGraphicsWrapper.h"

#define CONSOLE_LINES 20
#define NUM_MENUITEMS 100

class CCore;
struct sSound; 

struct sConsoleLine
{
	char line[255];
	int r, g, b;
};

struct sMenuItem
{
	sMenuItem* subitems[20];
	bool display;
	bool keepparentopen;
	bool enabled;
	bool onceonly;
	char name[50];
	char command[300];
	sMenuItem* parent;
	float offsetx, offsety;
	int unlock;
	int cost;
	int width, height;
	LPDIRECT3DTEXTURE9 backgroundtexture;
	LPDIRECT3DTEXTURE9 texture;

	sMenuItem()
	{
		for (int i=0;i<20;i++) subitems[i] = NULL;
		sprintf(name,"empty");
		display=false;
		enabled=false;
		keepparentopen=false;
		parent=NULL;
		offsetx = 0;
		offsety = 0;
		cost=0;
		unlock=0;
		width=100;
		height=20;
	}
};

class CGui
{
private:
	CCore* pcore;
	LPDIRECT3DTEXTURE9 t_WeaponSlotNumbers;
	LPDIRECT3DTEXTURE9 t_Colours;
	LPDIRECT3DTEXTURE9 t_Numbers;
	LPDIRECT3DTEXTURE9 t_Base;
	LPDIRECT3DTEXTURE9 t_Damage;
	LPDIRECT3DTEXTURE9 t_BaseLeft;
	LPDIRECT3DTEXTURE9 t_Locked;
	LPDIRECT3DTEXTURE9 t_WepBack;

	float WeaponMenuFade;
	sConsoleLine ConsoleText[CONSOLE_LINES];
	void DrawConsole(void);
	void DrawWeaponMenu(void);

	
public:
	CGui(void);	~CGui(void);
	void Do(void);
	void Startup(void);
	void SetCore(CCore* rpcore);



	int menu_SelectedItem;
	int menu_SelectedSlot;
	int SelectedWeapon;
	bool menuKeyDown;
	
	
	void HandleWeaponSelect(int numberpressed);


	void DoSelectedWeapon(void);
	void WriteToConsole(int importance,int r, int g, int b, char *lpszText, ...);
	

	void Draw(void);
	void ConsoleKey(void);
	bool KeyDown_Console;
	bool ShowConsole;
	void DrawPlayerStuff(void);
	float GuiScale;
	char InputString[500];
	void ConsoleInput(char chr);
	void DoConsoleCommand(void);
	void DrawNumber(int number, int x, int y, float scale);
	float HealthSize;
	void DrawPlutonium(int i);
	void ShowBuyMenu(void);
	bool bBuyMenuOpen;
	void DrawBuyMenu(void);
	void CloseBuyMenu(void);

	sMenuItem* menuitems[NUM_MENUITEMS];
	sMenuItem* SelectedMenuItem;
	sMenuItem* SelectedMenuItemNext;
	void DrawPopupMenuItem(sMenuItem* mi);
	void DrawMenuSubitems(sMenuItem* mi);

	sSound* s_menu_open;
	sSound* s_menu_move;
	sSound* s_menu_close;
	sSound* s_menu_buy;
	sSound* s_menu_unlock;
	sSound* s_menu_fail;

	sSound* s_weapon_switch;
	sSound* s_weapon_close;
	void TookDamage(void);
	void TookPlutonium(void);

	float TookDamageTimer;
	float TookPlutoniumTimer;

	void WeaponSlot(unsigned int slot);
	void Exec(char* filename);
	void RunConsoleCommand(char* command);
	void NextWeapon(int step);
	int NumWeaponsInSlot(unsigned int slot);
	void DamageSign(void);
};
