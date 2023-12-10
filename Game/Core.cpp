#include "core.h"
#include "fmod.h"

//inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
bool CCore::draggingsize=false;
CGarrysGraphics* CCore::gfxptr=NULL;
CLandscape* CCore::landscapeptr=NULL;


#define GAME_INTRO 0
#define GAME_MENU 1
#define GAME_GAME 2
#define GAME_OVER 3
#define GAME_LEVELEND 4

#define SMALLBB_MULTIPLIER  0.50f
#define SMALLBB_SCALE		2

CCore::CCore(void)
{
	inGame = false;
	bPaused = false;
	iGameState = GAME_INTRO;
	m_Netcode_UpdateTimer = 50; // in milliseconds
	s_MouthOffDelay = 0;

 	Screen_colour_return_speed = 0.1f;
	Screen_alpha_return_speed = 0.01f;

	Screen_r = 255;
	Screen_g = 255;
	Screen_b = 255;
	Screen_a = 255;
	m_FirstLoop = true;
	gravity = 1;
	timemultiplier = 1;
	MyPlayerID = 0;
	s_MouthOffDelay = 1000;
	sprintf(player_Model,"player");
	sprintf(player_Name,"Cock n Balls");
	for (int i=0;i<MAX_CLIENTS;i++)
	{
		players[i].inuse = false;
		players[i].player.pcore = this;
	}

	ViewLayer = 0;
}

CCore::~CCore(void)
{
}

bool CCore::Init(HINSTANCE hInstance, LPSTR lpCmdLine)
{
	TimerA = 0;
	bShowBuyMenu=false;
	sprintf(modfolder,DEFAULT_MOD_FOLDER);
	sprintf(firstmap,"c1");
	landscape.pcore = this;
	weapons.pcore = this;
//	netcode.pcore = this;
	gui.SetCore(this);
	props.pcore = this;
	textures.pcore = this;
	gameintro.pcore = this;
	enemies.pcore = this;
	ents.pcore = this;
	sound.pcore = this;
	gfxptr = &gfx;
	landscapeptr = &landscape;
	menu.pcore = this;
	gameover.pcore = this;
	input.pcore=this;
	EndLevel.pcore=this;
	menu.options.pcore=this;
	gfx.pcore=this;
	cdemo.pcore=this;

	t_Loading = NULL;
	RenderTarget = NULL;
	RenderTargetNormal = NULL;
	RenderTargetTwo = NULL;

	DoCommandLine(lpCmdLine);

	draggingsize = false;

	Width=1024.0f;
	Height=768.0f;
	bool Windowed=false;
	
	LoadSettings(&Width, &Height, &Windowed);
	
	gfx.EnableLogging();
	if (!gfx.CheckDirectXVersion()) exit(1);
	
	char windowname[300];
	char winname[100];

	sprintf(windowname, "Facewound - Alpha version [%.02f]", FW_VERSION);
	sprintf(winname, "Facewound");

	// create window ------------------------
	WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, winname, NULL};
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	
	RegisterClassEx(&wc);
	hWnd = CreateWindow(winname, windowname,Windowed?(WS_VISIBLE|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX):(WS_VISIBLE|WS_POPUP), 0, 0, Width,Height, NULL, NULL, wc.hInstance, NULL);
	// --------------------------------------
	
//	netcode.b_EnableLogging = false;

	int fontsize=14;

	if (Width<1024) fontsize = 11;
	if (Width<800) fontsize = 8;
	if (Width>1024) fontsize = 17;
	if (Width>1280) fontsize = 25;

	
	gfx.CreateDisplay(Width,Height,Windowed,&hWnd);

	CheckCapabilities();

	gfx.CreateDXFont(fontsize,700,"Arial");
	gfx.CreateDirectInput(&hInstance,&hWnd);
	gfx.WindowResize(&hWnd);
	gfx.WriteToLog("<br><br>Core Init<br>");


	

	sound.hWnd = hWnd;
	sound.EnableLogging();
	sound.Init();
	props.Init();
	ents.Init();
	menu.options.Init();
	cdemo.Init();

	
	landscape.ents = &ents;
	landscape.Initialize(&gfx);
	weapons.Init(&gfx);
	landscape.SetWindowSize();
	landscape.m_globalaplha = 255;

	
	enemies.Init();

//	netcode.m_bIsMultiplayer = false;
	
	

	timemultiplier=1;

	gui.WriteToConsole(10,255,255,255,"Finished Initialisation", gfx.tick);
	gui.Exec(CONFIG_FILENAME);

	if (this->bSkipMenus) 
	{
		this->StartGame();
	}

//	netcode.InitNetCode_SinglePlayer();
	MyPlayerID = this->AddPlayer(NULL);
	players[(MyPlayerID)].player.bIsPlayer = true;
	players[(MyPlayerID)].player.SetPlayerName(player_Name);

	ScreenZoom = gfx.m_d3dpp.BackBufferWidth/1024.0f;
	landscape.m_zoom = landscape.m_zoom*ScreenZoom;

	t_color = textures.LoadTexture("textures/colours.bmp");


	InitPostProcessing();
	CreateRenderTargerts();

	return true;
}

void CCore::GameLoop()
{
	if (Settings.sleeptime>0)
	{
		Sleep(Settings.sleeptime);
	}
	bool doSFX=false;

	if (m_FirstLoop) gfx.WriteToLog("<br><br><b>Started Game Loop</b>, Logging first loop.<br><br>");
	if (m_FirstLoop) gfx.WriteToLog("Input->Refresh<br>");
	gfx.InputRefreshKeyboard();
	gfx.InputRefreshMouse();
	gfx.UpdateTimer();

	gfx.tick = gfx.tick * timemultiplier;
	cdemo.Loop();

	if (gui.ShowConsole) bPaused=true;
	if (gui.bBuyMenuOpen) bPaused=true;
	if (doingLevelEnd) bPaused=true;

	if (bPaused) gfx.tick = 0;

	if (m_FirstLoop) gfx.WriteToLog("Sound Update<br>");
	FSOUND_Update();

	if (oldtimemultiplier!=1) sound.UpdateFrequency();
	oldtimemultiplier=timemultiplier;

	if (m_FirstLoop) gfx.WriteToLog("Do Weather<br>");
	landscape.DoWeather();

	if (gfx.InputKeyDown(DIK_GRAVE)) gui.ConsoleKey();
	else gui.KeyDown_Console = false;

	// UPDATE CAMERA POSITION (TO PLAYER AIM)
	if (m_FirstLoop) gfx.WriteToLog("Set Camera<br>");
		
	landscape.SetXYoffsets(players[MyPlayerID].player.m_fX-((gfx.m_d3dpp.BackBufferWidth/2)/landscape.m_zoom) + ((( x-(gfx.m_d3dpp.BackBufferWidth/2) )*0.8)/landscape.m_zoom),  
						   players[MyPlayerID].player.m_fY-((gfx.m_d3dpp.BackBufferHeight/2)/landscape.m_zoom)+ ((( y-(gfx.m_d3dpp.BackBufferHeight/2) )*0.8)/landscape.m_zoom));

	landscape.UpdateCamera();

	// START RENDERING
	if (m_FirstLoop) gfx.WriteToLog("GFX begin<br>");

	
	gfx.Begin();

	// DO PLAYERS
	if (m_FirstLoop) gfx.WriteToLog("Do Players (%i)<br>", MAX_CLIENTS);
	for (int i=0;i<MAX_CLIENTS;i++)
	{
		if (players[i].inuse)
		{
			players[i].player.Do();
		}
	}

	bAttacked=false;
	// DO KEYBOARD MOUSE INPUTS
	if (!gui.ShowConsole)
	{
		GameInput();
	}
	if (!bAttacked) DontShootUntilFireHasBeenReleased=false;

	// DO ENTS
	if (m_FirstLoop) gfx.WriteToLog("Do Ents");
	ents.DoEntitys();


	// DRAW WORLD
	if (m_FirstLoop) gfx.WriteToLog("Draw World<br>");
	DrawGameScreen();

	DrawNormalBuffer();

	if (m_FirstLoop) gfx.WriteToLog("Doing Post Processing<br>");

	IDirect3DSurface9 *OldRT;
	IDirect3DSurface9 *NewRT;
	if (Settings.shaders)
	{
		gfx.m_pd3dDevice->GetRenderTarget( 0, &OldRT );
		this->RenderTargetTwo->GetSurfaceLevel(0,&NewRT);
		gfx.m_pd3dDevice->SetRenderTarget( 0, NewRT );
	}

	PostProcessFullScreen(); 	

	if (Settings.shaders)
	{
		gfx.m_pd3dDevice->SetRenderTarget( 0, OldRT );
		NewRT->Release();
		OldRT->Release();
	}

	PostProcessFullScreenFinal();

	// WORK OUT FPS
	fpstimer=fpstimer+ gfx.RealTick;
	if (fpstimer>1000)
	{
		fpsdisplaymodel=fps;
		fpstimer=0;
		fps=0;
	}
	fps++;

	if (m_FirstLoop) gfx.WriteToLog("Do Gui<br>");
	gui.Do();

	if (this->Settings.showfps)
	gfx.DrawTextEx(0,0,255,255,255,"FPS: %i", fpsdisplaymodel);

	if (!doingLevelEnd) gui.Draw();

	

	if (m_FirstLoop) gfx.WriteToLog("Do Enemies<br>");
	enemies.DoEnemies();

	if (m_FirstLoop) gfx.WriteToLog("Round Start Stuff<br>");
	StartLevelLoop();



	if (m_FirstLoop) gfx.WriteToLog("Draw ScreenFade Layer<br>");
	landscape.DrawScreenFade();



	if (m_FirstLoop) gfx.WriteToLog("Gfx End<br>");
	gfx.End();





	if (m_FirstLoop) gfx.WriteToLog("Flip<br>");
	gfx.Flip();
	
	if (m_FirstLoop) 
	{
		gfx.WriteToLog("<br><b>Finished first loop!</b><br><br>");
		m_FirstLoop=false;
	}

	TimerAOld = TimerA;
	TimerA = TimerA + gfx.tick;
}


void CCore::LoadSettings(float* Width, float* Height, bool* Windowed)
{
	FILE* fp;
	int i=0;

	// does the settings folder exist? If not then create it.
	if (!DirectoryExists("settings"))
	{
		CreateDirectory("settings", NULL);
	}

	char string[500];
	fp = fopen("settings/system.cfg","r");

	if (fp!=NULL)
	{
		char item[300];
		char value[300];

		while (fgets (string , 300 , fp)!=NULL)
		{
			sscanf(string,"%s %[^\n]",&item, &value);

			if (strcmp(item, "width")==0)
			{
				*Width = atoi(value);
			}
			else if (strcmp(item, "height")==0)
			{
				*Height = atoi(value);
			}
			else if (strcmp(item, "windowed")==0)
			{
				*Windowed = atoi(value);
			}	
			else if (strcmp(item, "model")==0)
			{
				sprintf(player_Model, "%s", value);
			}	
			else if (strcmp(item, "name")==0)
			{
				sprintf(player_Name, "%s", value);
			}	
			else
			{
				this->gui.RunConsoleCommand(string);
			}	
		}
		

	}
	else
	{/*
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );

		CreateProcess(NULL,"setupguiMFC.exe",NULL,NULL,false,0,NULL,NULL,&si,&pi);*/

		MessageBox(NULL, "You have just launched a bomb! You have killed millions of people!! WHAT ARE YOU DOING!!! Oh wait wrong message. I meant to say 'Run setup.exe first to set your display options'. Don't tell my boss - he'll kick my head in.", "DANGER DANGER DANGER DANGER DANGER",NULL);

		exit(0);
	}

	if (*Width<=0) *Width=800;
	if (*Height<=0) *Height=600;

}

LRESULT WINAPI CCore::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps; 
    HDC hdc; 
	switch( msg )
    {
		case WM_CHAR:
				landscapeptr->pcore->gui.ConsoleInput(wParam);
				return 0;

        case WM_DESTROY:
//			landscapeptr->pcore->netcode.SendQuit();
			landscapeptr->ShutDown();
            PostQuitMessage( 0 );
            return 0;

        case WM_PAINT:
            
			if (gfxptr->bSuspended)
			{	
				HBRUSH hbrush, hbrushOld;

				InvalidateRect( hWnd, NULL, true );
				hdc = BeginPaint(hWnd, &ps); 
				hbrush = CreateSolidBrush(RGB(0, 0, 0));
				SelectObject(hdc, hbrush);
				Rectangle(hdc, 0, 0, gfxptr->m_d3dpp.BackBufferWidth, gfxptr->m_d3dpp.BackBufferHeight); 
				SetBkMode(hdc, TRANSPARENT); 
				SetTextColor(hdc, RGB(180,180,180));
				TextOut(hdc, 30, 30, "Paused.", 7); 
				
				EndPaint(hWnd, &ps); 
			}
			else
			{
				ValidateRect( hWnd, NULL );
			}
            return 0;

		case WM_EXITSIZEMOVE:
				gfxptr->WindowResize(&hWnd);
				landscapeptr->SetWindowSize();
			return 0;

		case WM_ACTIVATE:
			gfxptr->WriteToLog("WM_ACTIVATE -- ");
			if (wParam==WA_INACTIVE)
			{
				gfxptr->WriteToLog("WA_INACTIVE<br>");
				landscapeptr->pcore->LostDevice();
				gfxptr->bSuspended = true;
				InvalidateRect( hWnd, NULL, true );
			}
			else
			if ((wParam==WA_ACTIVE || wParam==WA_CLICKACTIVE) && !IsIconic(hWnd))
			{
				gfxptr->WriteToLog("WA_ACTIVE<br>");
				gfxptr->bSuspended = false;	
				gfxptr->ReAquireDevices();
				gfxptr->WindowResize(&hWnd);
				landscapeptr->SetWindowSize();
							
			}
			else //if ((wParam==WA_ACTIVE || wParam==WA_CLICKACTIVE) && IsIconic(hWnd))
			{
				gfxptr->WriteToLog("UNKNOWN WA_ (%i)<br>",(int)wParam);
				landscapeptr->pcore->LostDevice();
				gfxptr->bSuspended = true;
				InvalidateRect( hWnd, NULL, true );
			}
			return 0;

		case WM_ENTERSIZEMOVE:
			{
				gfxptr->WriteToLog("WM_ENTERSIZEMOVE<br>");
				landscapeptr->pcore->LostDevice();
				draggingsize=true;
				//Sleep(500);
			}
			return 0;
		


		case WM_SIZE:
			gfxptr->WriteToLog("WM_SIZE<br>");
			if (wParam==SIZE_MAXHIDE)
			{
				gfxptr->WriteToLog("SIZE_MAXHIDE<br>");
			}
			else if (wParam==SIZE_RESTORED)
			{
				gfxptr->WriteToLog("SIZE_RESTORED<br>");
				Sleep(500);
				gfxptr->WindowResize(&hWnd);
				landscapeptr->SetWindowSize();
			}
			else 
				/*
			if (!draggingsize)
			{

			}*/
			return 0;
    }
		


    return DefWindowProc( hWnd, msg, wParam, lParam );
}

void CCore::Run(void)
{
    MSG msg;
	HRESULT hr;

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (gfx.bSuspended)
		{
		//	landscape.FreeTextures();

			

			hr = gfx.m_pd3dDevice->TestCooperativeLevel();

			if (hr==D3DERR_DEVICENOTRESET)
			{
				gfx.WriteToLog("DEVICE READY!!");
				
				RECT winrect;
				GetClientRect(hWnd,&winrect);

				gfx.bSuspended=false;
				gfx.ResetDevice((int)winrect.right-winrect.left,(int)winrect.bottom-winrect.top);

			}
						
			Sleep(2);
		}
		else
		{

			MainGameHub();
		}
		
	}
}

int CCore::AddPlayer(void)
{
	for (int i=0;i<MAX_CLIENTS;i++)
	{
		if (players[i].inuse==false)
		{
			AddPlayer(i);
			return i;
		}
	}

	exit(9009);
}

int CCore::AddPlayer(unsigned int i)
{
	if (players[i].inuse==false)
	{
//		netcode.WriteToLog("Adding new player on slot [%i]<br>",i);
		players[i].inuse=true;
		players[i].player.CreatePlayer();
//		players[i].addr = NULL;
//		players[i].iaddrsize = sizeof(remoteaddr);

		players[i].player.ChangeModel(player_Model);

		return i;
	}
	
	return -1;
}

float CCore::XPosOnScreen(float xpos)
{
	
	return xpos-(landscape.offset_x+(gfx.m_d3dpp.BackBufferWidth/2));
}

float CCore::YPosOnScreen(float ypos)
{
	return ypos-(landscape.offset_y+(gfx.m_d3dpp.BackBufferHeight/2));
}

void CCore::DrawGameScreen(void)
{
	IDirect3DSurface9 *OldRT;
	IDirect3DSurface9 *NewRT;

	if (Settings.shaders)
	{
		gfx.m_pd3dDevice->GetRenderTarget( 0, &OldRT );

		this->RenderTarget->GetSurfaceLevel(0,&NewRT);
		gfx.m_pd3dDevice->SetRenderTarget( 0, NewRT );
	}
	landscape.DrawSky();
	ents.DrawEnts(0);
	landscape.DrawUnder();
	ents.DrawEnts(1);
	enemies.DrawEnemies(0);
	
	landscape.DrawParticles(0);

	for (int i=0;i<MAX_CLIENTS;i++)
	{
		if (players[i].inuse) players[i].player.Draw();
	}

	props.DrawProps(0);
	landscape.Draw();
	landscape.DrawParticles(1);
	enemies.DrawEnemies(1);
	ents.DrawEnts(2);
	landscape.DrawParticles(2);

	landscape.DrawNonShaderWater();
	

	gfx.m_pSprite->Flush();
/*
	this->RenderTargetSmall->GetSurfaceLevel(0,&NewRTSmall);
	gfx.m_pd3dDevice->SetRenderTarget( 0, NewRTSmall );

	PostProcessSmallTarget();
*/
	if (Settings.shaders)
	{
		gfx.m_pd3dDevice->SetRenderTarget( 0, OldRT );
		NewRT->Release();
		OldRT->Release();
	}
//	NewRTSmall->Release();

	
}

void CCore::UpdateCursorPosition(void)
{	GetCursorPos( &point);
	if (gfx.Windowed) ScreenToClient(hWnd, &point);

	//screenmovex = x - point.x;
	//screenmovey = y - point.y;

	x=(float)point.x;		
	y=(float)point.y;	


}

// move player if standing on 
void CCore::MoverMovePlayer(sProps* p, float x, float y)
{
	//players[MyPlayerID].player.UpdateMyRect();

	if (IntersectGRect(&players[MyPlayerID].player.my_rect, &p->mover_rect))
	players[MyPlayerID].player.Push(x,y);
	if (IntersectGRect(&players[MyPlayerID].player.my_rect, &p->mover_rect))
	players[MyPlayerID].player.Push(x,y);
}

bool CCore::Trigger(char* name)
{
	if (strcmp(name,"")==0) return false;
	bool rval=false;

	if (this->props.Trigger(name)) rval = true;
	if (this->ents.Trigger(name)) rval = true;

	return rval;
}

void CCore::OutputTimer(char* testname)
{
	clock_t ended = clock();
	double time = ((ended-timer_started)/(CLOCKS_PER_SEC / (double) 1000.0))/1000.0f;
	//gfx.WriteToLog("[<font color=blue>TIMER</font>] %s <%f><br>", testname, time);
}

void CCore::StartTimer(void)
{
	timer_started = clock();
}

void CCore::MainGameHub(void)
{
	sound.RefreshSound();

	if (iGameState==GAME_INTRO)
	{
		gameintro.doLoop();
	}
	else if (iGameState==GAME_MENU)
	{
		menu.Loop();
	}
	else if (iGameState==GAME_GAME)
	{
		this->GameLoop();
	}
	else if (iGameState==GAME_OVER)
	{
		gameover.Loop();
	}
	else if (iGameState==GAME_LEVELEND)
	{
		this->EndLevel.Loop();
	}
}

void CCore::StartGame(void)
{



	Loading(5);

	this->EndLevel.Init();
	s_Credit = sound.LoadSound(5,"sound/credit.ogg");

	Loading(15);

	s_3 = sound.LoadSound(5,"sound/announcer/3.ogg");
	s_2 = sound.LoadSound(5,"sound/announcer/2.ogg");
	s_1 = sound.LoadSound(5,"sound/announcer/1.ogg");

	Loading(18);

	inGame = true;
	iGameState=GAME_GAME;
	RecoilMultiplier = 1.0f;

	Loading(20);
	// load buymenu again
	gui.Startup();

	Loading(22);

	// start a new game
	this->ResetGameStats();
	Loading(24);
	landscape.LoadMap(firstmap);
	Loading(70);
	this->players[this->MyPlayerID].player.Spawn(true);
	Loading(85);
	StartLevel();
	Loading(100);
	bShowBuyMenu=false;

}

void CCore::toMenuScreen(void)
{
	sound.PauseChannel(landscape.InGameMusicChannel);
	iGameState=GAME_MENU;
	menu.Init();
}

void CCore::ResumeGame(void)
{
	sound.ResumeChannel(landscape.InGameMusicChannel);
	iGameState=GAME_GAME;
}

void CCore::DoCommandLine(LPSTR lpCmdLine)
{
	gfx.WriteToLog("Starting to parse command line<br>");
	char laststring[100];
	int port;
	char * pch;

	if (strlen(lpCmdLine)>0)
	{
		pch = strtok ((char*)lpCmdLine," ");
		 while (pch != NULL)
		 {
			 if (strcmp(laststring,"-mod")==0)
			 {
				gfx.WriteToLog("mod: '%s'<br>", pch);
				sprintf(this->modfolder,pch);
			 }
			 if (strcmp(laststring,"-map")==0)
			 {
				gfx.WriteToLog("map: '%s'<br>", pch);
				sprintf(this->firstmap,pch);
				this->bSkipMenus = true;
			 }

			sprintf(laststring,pch);
			printf ("%s\n",pch);
			pch = strtok (NULL, " ,.");
		 }
	}
	else
	{
		gfx.WriteToLog("No command line<br>");
	}

	
}

void CCore::GameInput(void)
{
	if (gfx.InputKeyDown(DIK_ESCAPE) && !EscapeDown)
	{
		bShowBuyMenu = false;
		gui.RunConsoleCommand("exit");
	}
	if (!gfx.InputKeyDown(DIK_ESCAPE)) EscapeDown=false;

	if (bShowBuyMenu && !doingLevelEnd)	{gui.ShowBuyMenu();}
	else				gui.CloseBuyMenu();
	
	this->UpdateCursorPosition();

	

	input.Do();

	if (bPaused) return;

	if (!cdemo.bPlaying)
	players[MyPlayerID].player.m_AimAngle = atan2((x-((players[MyPlayerID].player.m_fX-landscape.offset_x)*landscape.m_zoom)),(y-((players[MyPlayerID].player.m_fY-landscape.offset_y))*landscape.m_zoom));
	
}

void CCore::StartLevelEnd(char* nextlevel)
{
	landscape.ScreenFade.active = true;
	landscape.ScreenFade.alpha = 255;
	landscape.ScreenFade.color = 0;
	landscape.ScreenFade.speed = -0.2f;

	sprintf(landscape.m_NextLevel, nextlevel);
	doingLevelEnd = true;
	doingLevelEndTimer = 0;

	
	LastPlut=0; 
	LastEnemies=0;

	sprintf(LevelEndNextLevel,nextlevel);
	sound.CloseStream(landscape.music);

	landscape.music= NULL;
	sound.PlayStream(landscape.music_levelcomplete);

	EndLevel.Init();
	iGameState = GAME_LEVELEND;
	
}

void CCore::SetAlphaMode(bool on)
{
	if (bProcessingLevelStart) return;
	if (m_CurrentBlendMode==on) return;
	if (!gfx.inscene) return;

	gfx.m_pSprite->End();
	gfx.m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_CurrentBlendMode = on;
	if (on)
	{
		//gfx.m_pd3dDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_MAX);
		gfx.m_pd3dDevice->SetRenderState (D3DRS_SRCBLEND, this->Settings.sb );
		gfx.m_pd3dDevice->SetRenderState (D3DRS_DESTBLEND, this->Settings.db  );
	}

}

void CCore::GameOver(void)
{
	// release stuff maybe?
	inGame = false;
	gameover.Init();
	iGameState = GAME_OVER;
}

void CCore::ResetRoundStats(void)
{
	this->LevelStats.Enemies = 0;
	this->LevelStats.Plutonium = 0;
	this->LevelStats.TimeTaken = 0;
	this->LevelStats.HiddenAreas = 0 ;
	this->LevelStats.ShotsFired = 0;
	this->LevelStats.ShotsLanded = 0;
}

void CCore::ResetGameStats(void)
{
	ResetRoundStats();
	this->GlobalStats.Enemies = 0;
	this->GlobalStats.Plutonium = 0;
	this->GlobalStats.TimeTaken = 0;
	this->GlobalStats.Lives = 3;
}

void CCore::StartLevel(void)
{
	bShowBuyMenu=false;
	landscape.ScreenFade.active = true;
	landscape.ScreenFade.alpha = 255;
	landscape.ScreenFade.color = 1;
	landscape.ScreenFade.speed = -0.4f;

	TimerA=0;
	doStartLevel = true;
	m_Timer = 0;	
	
	float oldtick = gfx.tick;
	gfx.tick = 30;
	this->timemultiplier = 1;
	bPaused = false;
	bProcessingLevelStart = true;

	for (int i=0;i<500;i++)
	{
		this->ents.DoEntitys();
		this->landscape.DrawParticles(0);
		this->landscape.DrawParticles(1);
		this->landscape.DoWeather();
	}

	bProcessingLevelStart = false;

	gfx.tick = oldtick;
//	bPaused = true;

	for (int i=0;i<1;i++)
	{
		this->landscape.AddParticle(
			PARTICLE_PLAYERLINE,
			players[MyPlayerID].player.m_fX,
			players[MyPlayerID].player.m_fY);
	}
	landscape.QueueUpIntroVoice();
	bPaused = true;
	//timemultiplier = 0.02f;
	landscape.WaterLine = landscape.WaterLineOriginal;
}

void CCore::StartLevelLoop(void)
{
	if (!doStartLevel) return;
	bShowBuyMenu=false;

	LPParticle lp=NULL;
	float OldTimer = m_Timer;

	m_Timer = m_Timer + gfx.RealTick;
//	bPaused = true;

	if (m_Timer==0) m_Timer=1.0f;

	landscape.SetZoom(1.0f);

	if (OldTimer<400 && m_Timer>=400)
	{
		sound.PlaySnd(s_3,0,0,0,-5.0f,1.0f);
		for (int i=0;i<1;i++)
		{
			lp = this->landscape.AddParticle(PARTICLE_GUI_COUNTDOWN,gfx.m_d3dpp.BackBufferWidth/2,gfx.m_d3dpp.BackBufferHeight/2);
			if (lp!=NULL)
			{
				lp->textureoverride = textures.LoadTexture("textures/gui/three.tga");
				lp->xscale = lp->xscale - (i*0.07f);
			}
		}
	}

	if (OldTimer<800 && m_Timer>=800)
	{
		sound.PlaySnd(s_2,0,0,0,-5.0f,1.0f);
		for (int i=0;i<1;i++)
		{
			lp = this->landscape.AddParticle(PARTICLE_GUI_COUNTDOWN,gfx.m_d3dpp.BackBufferWidth/2,gfx.m_d3dpp.BackBufferHeight/2);
			if (lp!=NULL)
			{
				lp->textureoverride = textures.LoadTexture("textures/gui/two.tga");
				lp->xscale = lp->xscale - (i*0.07f);
			}
		}
	}

	if (OldTimer<1200 && m_Timer>=1200)
	{
		sound.PlaySnd(s_1,0,0,0,-5.0f,1.0f);
		for (int i=0;i<1;i++)
		{
			lp = this->landscape.AddParticle(PARTICLE_GUI_COUNTDOWN,gfx.m_d3dpp.BackBufferWidth/2,gfx.m_d3dpp.BackBufferHeight/2);
			if (lp!=NULL)
			{
				lp->textureoverride = textures.LoadTexture("textures/gui/one.tga");
				lp->xscale = lp->xscale - (i*0.07f);
			}
		}
	}

	if (m_Timer>1600)
	{
		sound.PlaySnd(s_go,0,0,0,-5.0f,1.0f);
		bPaused = false;
		doStartLevel=false;
		landscape.SetZoom(1.0f);
		this->StartLevelMusic();
		timemultiplier = 1.0f;
	}
}

void CCore::StartNextLevel(char* levelname)
{
	iGameState = GAME_GAME;

	this->Loading(10);
	landscape.ChangeMap(levelname);

	this->Loading(50);
	doingLevelEnd=false;
	bPaused=false;
	
	GlobalStats.Enemies += LevelStats.Enemies;
	GlobalStats.Plutonium += LevelStats.Plutonium;
	GlobalStats.TimeTaken += LevelStats.TimeTaken;
	GlobalStats.HiddenAreas += LevelStats.HiddenAreas;

	this->Loading(70);
	ResetRoundStats();
	this->Loading(90);
	StartLevel();
	players[MyPlayerID].player.Spawn(false);
	this->Loading(95);
}


void CCore::StartLevelMusic(void)
{
	if (strcmp(landscape.m_MapProperties.music, "")!=0)
	{
		char musicfile[200];
		sprintf(musicfile, "sound/music/%s", landscape.m_MapProperties.music);
		if (Settings.enablemusic)
		{
			landscape.music = sound.LoadStream(musicfile,true);
			landscape.InGameMusicChannel = sound.PlayStream(landscape.music);
		}
	}

	RefreshMusicVolume();
}
void CCore::FreeEverything(void)
{
	textures.FreeAll();
	sound.FreeAll();
}

void CCore::ExitGame(void)
{
	menu.options.SaveOptions();
	FreeEverything();
	exit(0);
}

// this is called whenever we die
void CCore::DoDieRoutine(void)
{
	
	sound.CloseStream(landscape.music);
	landscape.music=NULL;
	ResetRoundStats();
	GlobalStats.Lives--;
	timemultiplier = 0.0;
	
	landscape.ScreenFade.active = true;
	landscape.ScreenFade.alpha = 1;
	landscape.ScreenFade.color = 1;
	landscape.ScreenFade.speed = 0.05f;

	// todo: play death music

}

void CCore::RestartAfterDead()
{
	Loading(20);
	doingLevelEnd=false;
	bPaused=false;
	Loading(30);
	landscape.RemoveAllParticle();
	Loading(40);
	players[MyPlayerID].player.Spawn(false);
	Loading(70);
	StartLevel();
	Loading(90);
}

void CCore::DoAttack(void)
{
	bAttacked=true;

	if (!DontShootUntilFireHasBeenReleased)
		{
			if (bShowBuyMenu)
			{
				gui.CloseBuyMenu();
				DontShootUntilFireHasBeenReleased=true;
			}
			else if (bWeaponSelect==true)
			{
				gui.DoSelectedWeapon();
				players[MyPlayerID].player.ChangeWeapon(gui.SelectedWeapon);
				bWeaponSelect=false;
				DontShootUntilFireHasBeenReleased=true;
			}
			else
			{
				AimLength = sqrt(((players[MyPlayerID].player.m_fX-landscape.offset_x)*landscape.m_zoom-x)*((players[MyPlayerID].player.m_fX-landscape.offset_x)*landscape.m_zoom-x)
									+
								   ((players[MyPlayerID].player.m_fY-landscape.offset_y)*landscape.m_zoom-y)*((players[MyPlayerID].player.m_fY-landscape.offset_y)*landscape.m_zoom-y));
				AimLength = AimLength/((gfx.m_d3dpp.BackBufferHeight+gfx.m_d3dpp.BackBufferWidth)/2);
				players[MyPlayerID].player.AimLength = AimLength;
				players[MyPlayerID].player.Shoot();
			}
		}
}

void CCore::RefreshMusicVolume(void)
{
	if (landscape.InGameMusicChannel>=0)
	{
		sound.SetChannelVolume(landscape.InGameMusicChannel, Settings.musicvolume);
	}
/*
	if (menu.musicchannel>=0)
	{
		sound.SetChannelVolume(menu.musicchannel, Settings.musicvolume);
	}
*/
}

void CCore::Loading(int percent)
{
	if (t_Loading==NULL) t_Loading = textures.LoadTexture("textures/menu/loading.tga");
	gfx.Begin();
	gfx.Clear(0,0,0);

	gfx.SpriteDrawEx(t_Loading, gfx.m_d3dpp.BackBufferWidth-(300*ScreenZoom), gfx.m_d3dpp.BackBufferHeight-(200*ScreenZoom),0,0,0,ScreenZoom,ScreenZoom,0,0,256,63,255,255,255,255);

	gfx.SpriteDrawEx(t_Loading, gfx.m_d3dpp.BackBufferWidth-(300*ScreenZoom), gfx.m_d3dpp.BackBufferHeight-(140*ScreenZoom),0,0,0,ScreenZoom*256,1,0,65,1,1,255,255,255,255);
	gfx.SpriteDrawEx(t_Loading, gfx.m_d3dpp.BackBufferWidth-(300*ScreenZoom), gfx.m_d3dpp.BackBufferHeight-(130*ScreenZoom),0,0,0,ScreenZoom*256,1,0,65,1,1,255,255,255,255);

	gfx.SpriteDrawEx(t_Loading, gfx.m_d3dpp.BackBufferWidth-(300*ScreenZoom), gfx.m_d3dpp.BackBufferHeight-(140*ScreenZoom),0,0,0,1,10*ScreenZoom,0,65,1,1,255,255,255,255);
	gfx.SpriteDrawEx(t_Loading, gfx.m_d3dpp.BackBufferWidth-( 44*ScreenZoom), gfx.m_d3dpp.BackBufferHeight-(140*ScreenZoom),0,0,0,1,10*ScreenZoom,0,65,1,1,255,255,255,255);

	gfx.SpriteDrawEx(t_Loading, gfx.m_d3dpp.BackBufferWidth-(299*ScreenZoom), gfx.m_d3dpp.BackBufferHeight-(139*ScreenZoom),0,0,0,ScreenZoom*percent*2.5f,ScreenZoom*10,0,65,1,1,200,255,255,255);



	gfx.End();
	gfx.Flip();
}

void CCore::PostProcessSmallTarget(void)
{
	return;
	/*
	gfx.m_pSprite->Flush();

	

	UINT uPasses=0;

	Effect[EF_CONTRAST]->Begin( &uPasses, 0 );
	for( UINT uPass = 0; uPass < uPasses; ++uPass )
    {
		Effect[EF_CONTRAST]->BeginPass( uPass );
		gfx.SpriteDraw(this->RenderTarget,0,0,1,1);
		gfx.m_pSprite->Flush();
		Effect[EF_CONTRAST]->EndPass();
	}
	Effect[EF_CONTRAST]->End();



	// blur the mofo

	
	Effect[EF_BLUR]->SetFloat("xblur",(0.04f+screenmovex)*-0.001f);
	Effect[EF_BLUR]->SetFloat("yblur",(0.04f+screenmovey)*-0.001f);

	//Effect[EF_BLUR]->SetFloat("xblur",0);//sin((landscape.offset_x+landscape.offset_y)*0.003)*0.006*this->Settings.BlurScale);
	//Effect[EF_BLUR]->SetFloat("yblur",0.05*this->Settings.BlurScale);

	Effect[EF_BLUR]->Begin( &uPasses, 0 );
	for( UINT uPass = 0; uPass < uPasses; ++uPass )
    {
		Effect[EF_BLUR]->BeginPass( uPass );
		gfx.SpriteDraw(this->RenderTargetSmall,0,0,SMALLBB_SCALE,SMALLBB_SCALE); 
		gfx.m_pSprite->Flush();
		Effect[EF_BLUR]->EndPass();
	}
	Effect[EF_BLUR]->End();




	Effect[EF_BLUR]->SetFloat("xblur",(0.04f+screenmovex)*0.001f);
	Effect[EF_BLUR]->SetFloat("yblur",(0.04f+screenmovey)*0.001f);

//	Effect[EF_BLUR]->SetFloat("yblur",0);//sin((landscape.offset_x+landscape.offset_y)*0.003)*0.006*this->Settings.BlurScale);
//	Effect[EF_BLUR]->SetFloat("xblur",0.005*this->Settings.BlurScale);

	Effect[EF_BLUR]->Begin( &uPasses, 0 );
	for( UINT uPass = 0; uPass < uPasses; ++uPass )
    {
		Effect[EF_BLUR]->BeginPass( uPass );
		gfx.SpriteDraw(this->RenderTargetSmall,0,0,SMALLBB_SCALE,SMALLBB_SCALE); 
		gfx.m_pSprite->Flush();
		Effect[EF_BLUR]->EndPass();
	}
	Effect[EF_BLUR]->End();

*/
}

void CCore::InitPostProcessing(void)
{
	if (!Settings.shaders) return;
}

void CCore::CreateRenderTargerts(void)
{
//	if (!Settings.shaders) return;

	if (gfx.m_pd3dDevice==NULL) return;
	if (RenderTarget!=NULL) return;

	gfx.m_pd3dDevice->CreateTexture( gfx.m_d3dpp.BackBufferWidth,
                                             gfx.m_d3dpp.BackBufferHeight,
                                             1,
                                             D3DUSAGE_RENDERTARGET,
                                             gfx.m_d3dpp.BackBufferFormat,
                                             D3DPOOL_DEFAULT,
                                             &RenderTarget,
                                             NULL );
	gfx.WriteToLog("Created RT2: %ix%i<br>", (int)gfx.m_d3dpp.BackBufferWidth, (int)gfx.m_d3dpp.BackBufferHeight);

	gfx.m_pd3dDevice->CreateTexture( gfx.m_d3dpp.BackBufferWidth,
                                             gfx.m_d3dpp.BackBufferHeight,
                                             1,
                                             D3DUSAGE_RENDERTARGET,
                                             gfx.m_d3dpp.BackBufferFormat,
                                             D3DPOOL_DEFAULT,
                                             &RenderTargetTwo,
                                             NULL );

	gfx.WriteToLog("Created RT1: %ix%i<br>", (int)gfx.m_d3dpp.BackBufferWidth, (int)gfx.m_d3dpp.BackBufferHeight);
	

	gfx.m_pd3dDevice->CreateTexture( gfx.m_d3dpp.BackBufferWidth*0.5f,
                                             gfx.m_d3dpp.BackBufferHeight*0.5f,
                                             1,
                                             D3DUSAGE_RENDERTARGET,
                                             gfx.m_d3dpp.BackBufferFormat,
                                             D3DPOOL_DEFAULT,
                                             &RenderTargetNormal,
                                             NULL );

	gfx.WriteToLog("Created RT3 (Norm): %ix%i<br>", (int)(gfx.m_d3dpp.BackBufferWidth*0.5), (int)(gfx.m_d3dpp.BackBufferHeight*0.5));
	gfx.Begin();
	IDirect3DSurface9 *OldRT;
	IDirect3DSurface9 *NewRT;

	gfx.m_pd3dDevice->GetRenderTarget( 0, &OldRT );
	this->RenderTargetTwo->GetSurfaceLevel(0,&NewRT);
	gfx.m_pd3dDevice->SetRenderTarget( 0, NewRT );
	gfx.Clear(0,255,0);
	gfx.m_pd3dDevice->SetRenderTarget( 0, OldRT );
	NewRT->Release();
	OldRT->Release();

	gfx.m_pd3dDevice->GetRenderTarget( 0, &OldRT );
	this->RenderTargetNormal->GetSurfaceLevel(0,&NewRT);
	gfx.m_pd3dDevice->SetRenderTarget( 0, NewRT );
	gfx.Clear(0,0,255);
	gfx.m_pd3dDevice->SetRenderTarget( 0, OldRT );
	NewRT->Release();
	OldRT->Release();


	gfx.End();

	
}

void CCore::PostProcessFullScreen(void)
{
	float WaterLine;
	this->SetAlphaMode(false);

	landscape.WaterLine = landscape.WaterLine + landscape.WaterLineVelocity*gfx.tick;

	WaterLine = ((landscape.WaterLine-landscape.offset_y)/gfx.m_d3dpp.BackBufferHeight)*landscape.m_zoom;
	if (WaterLine>1) WaterLine=1.0f;

	if (Settings.shaders && Effect==NULL)
	{
		SetAlphaMode(false);
		gfx.SpriteDrawFull(this->RenderTarget,0,0,0,0,0,1,1,0,0,gfx.m_d3dpp.BackBufferWidth,gfx.m_d3dpp.BackBufferHeight,255,255,255,255);
	}

	
	if (!Settings.shaders || Effect==NULL) 
	{


		if (!Settings.shaders)	return;
	}

	SetAlphaMode(false);

	//t_water

	// no shader - just draw the bb to the screen
	if (Effect==NULL)
	{
		gfx.m_pSprite->Flush();
		return;
	}
	gfx.m_pSprite->Flush();
	UINT uPasses=0;

	Effect->SetFloat("sinslow", sin(TimerA*0.0001f));
	Effect->SetFloat("timeraslow", TimerA*0.01f);

	if (landscape.WaterLine==0)
	{
		Effect->SetFloat("waterline", 2);
	}
	else
	{
		Effect->SetFloat("waterline", WaterLine);
	}

	


	// pass mousex and y to the shader
	if (gfx.m_d3dpp.BackBufferWidth>0)Effect->SetFloat("mousex", this->x/gfx.m_d3dpp.BackBufferWidth);
	if (gfx.m_d3dpp.BackBufferHeight>0)Effect->SetFloat("mousey", this->y/gfx.m_d3dpp.BackBufferHeight);
	Effect->SetFloat("offsetx", this->landscape.offset_x/1024.0f);
	Effect->CommitChanges();

	Effect->Begin( &uPasses, 0 );
	for( UINT uPass = 0; uPass < uPasses; ++uPass )
    {
		Effect->BeginPass( uPass );
		gfx.SpriteDrawFull(RenderTarget,0,0,0,0,0,1,1,0,0,gfx.m_d3dpp.BackBufferWidth,gfx.m_d3dpp.BackBufferHeight,255,255,255,255);
		gfx.m_pSprite->Flush();
		Effect->EndPass();
	}
	Effect->End();
	gfx.m_pSprite->Flush();
}


void CCore::LostDevice(void)
{
	// pause music
	if (RenderTarget!=NULL) RenderTarget->Release();
	RenderTarget = NULL;

	if (RenderTargetNormal!=NULL) RenderTargetNormal->Release();
	RenderTargetNormal = NULL;

	if (RenderTargetTwo!=NULL) RenderTargetTwo->Release();
	RenderTargetTwo = NULL;
	

	if (Effect!=NULL) Effect->Release();
	Effect = NULL;

	if (GlobalEffect!=NULL) GlobalEffect->Release();
	GlobalEffect = NULL;

	sound.PauseChannel(landscape.InGameMusicChannel);

}


void CCore::ResetDevice(void)
{
	gfx.WriteToLog("<b>Creating Render Target..</b><br>");
	this->CreateRenderTargerts();
	gfx.WriteToLog("<b>Loading Level Shader..</b><br>");
	landscape.LoadLevelShader();


	sound.ResumeChannel(landscape.InGameMusicChannel);
}

void CCore::CheckCapabilities(void)
{
	bool Accept=true;
	gfx.WriteToLog("<b>Checking Capabilities</b>...<blockquote>");


	if (D3DSHADER_VERSION_MAJOR(gfx.D3DCaps.PixelShaderVersion)<2) 
	{	
		gfx.WriteToLog("Turning off shader effects - Your card doesn't support pixel shader 2.0");
		Accept=false;
	}

	if (gfx.D3DCaps.MaxTextureWidth<gfx.m_d3dpp.BackBufferWidth) 
	{	
		gfx.WriteToLog("Turning off shader effects - Resolution is higher than the max texture size");
		Accept=false;
	}

	if (gfx.D3DCaps.MaxTextureHeight<gfx.m_d3dpp.BackBufferHeight) 
	{	
		gfx.WriteToLog("Turning off shader effects - Resolution is higher than the max texture size");
		Accept=false;
	}

	if (!Accept)
	{
		Settings.shaders = false;
		gfx.WriteToLog("<br><b>Shaders Off</b><br>");
	}
	else
	{
		gfx.WriteToLog("<font color=green>PASSED!</font>");
	}
	gfx.WriteToLog("</blockquote>");
}

void CCore::PostProcessFullScreenFinal(void)
{
	if (!Settings.shaders) return;

	float WaterLine;
	this->SetAlphaMode(false);
	WaterLine = ((landscape.WaterLine-landscape.offset_y)/gfx.m_d3dpp.BackBufferHeight)*landscape.m_zoom;
	if (WaterLine>1) WaterLine=1.0f;

	// no shader - just draw the bb to the screen
	if (GlobalEffect==NULL)
	{
		gfx.SpriteDrawFull(this->RenderTargetTwo,0,0,0,0,0,1,1,0,0,gfx.m_d3dpp.BackBufferWidth,gfx.m_d3dpp.BackBufferHeight,255,255,255,255);
		return;
	}
	gfx.m_pSprite->Flush();
	UINT uPasses=0;

	GlobalEffect->SetFloat("sinslow", sin(TimerA*0.0001f));
	GlobalEffect->SetFloat("timeraslow", TimerA*0.01f);

	if (landscape.WaterLine==0)	GlobalEffect->SetFloat("waterline", 1);
	else GlobalEffect->SetFloat("waterline", WaterLine);

	if (this->bPaused)
		GlobalEffect->SetFloat("BulletTime", 1);
	else
		GlobalEffect->SetFloat("BulletTime", this->timemultiplier);


	GlobalEffect->SetFloat("ScreenMoveX", this->screenmovex);
	GlobalEffect->SetFloat("ScreenMoveY", this->screenmovey);


	// pass mousex and y to the shader
	if (gfx.m_d3dpp.BackBufferWidth>0)GlobalEffect->SetFloat("mousex", this->x/gfx.m_d3dpp.BackBufferWidth);
	if (gfx.m_d3dpp.BackBufferHeight>0)GlobalEffect->SetFloat("mousey", this->y/gfx.m_d3dpp.BackBufferHeight);
	GlobalEffect->SetFloat("offsetx", this->landscape.offset_x/1024.0f);

	PPVERT Quad[4] =
    {
        { -0.5f,                        -0.5f,                         1.0f, 1.0f, 0.0f, 0.0f },
        { gfx.m_d3dpp.BackBufferWidth-0.5f, -0.5,                          1.0f, 1.0f, 1.0f, 0.0f },
        { -0.5,                         gfx.m_d3dpp.BackBufferHeight-0.5f, 1.0f, 1.0f,  0.0f, 1.0f },
        { gfx.m_d3dpp.BackBufferWidth-0.5f, gfx.m_d3dpp.BackBufferHeight-0.5f, 1.0f, 1.0f, 1.0f, 1.0f }
    };
	
    IDirect3DVertexBuffer9 *pVB;
    HRESULT hr = gfx.m_pd3dDevice->CreateVertexBuffer( sizeof(PPVERT) * 4,
                                         D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
                                         0,
                                         D3DPOOL_DEFAULT,
                                         &pVB,
                                         NULL );
    LPVOID pVBData;
    if( SUCCEEDED( pVB->Lock( 0, 0, &pVBData, D3DLOCK_DISCARD ) ) )
    {
        CopyMemory( pVBData, Quad, sizeof(Quad) );
        pVB->Unlock();
    }

	gfx.m_pd3dDevice->SetTexture(0,this->RenderTargetTwo);
	GlobalEffect->SetTexture("g_texturea",this->RenderTargetTwo);
	GlobalEffect->SetTexture("g_textureb",this->RenderTargetNormal);
	GlobalEffect->CommitChanges();
	
	GlobalEffect->Begin( &uPasses, 0 );
	for( UINT uPass = 0; uPass < uPasses; ++uPass )
    {
		GlobalEffect->BeginPass( uPass );
		//gfx.SpriteDrawFull(RenderTarget,0,0,0,0,0,1,1,0,0,gfx.m_d3dpp.BackBufferWidth,gfx.m_d3dpp.BackBufferHeight,255,255,255,255);
		//gfx.m_pSprite->Flush();
		gfx.m_pd3dDevice->SetStreamSource( 0, pVB, 0, sizeof(PPVERT) );
        gfx.m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		GlobalEffect->EndPass();
	}
	GlobalEffect->End();

	if (ViewLayer==1)
	{
		gfx.SpriteDrawFull(this->RenderTargetNormal,0,0,0,0,0,2,2,0,0,gfx.m_d3dpp.BackBufferWidth,gfx.m_d3dpp.BackBufferHeight,255,255,255,255);
	}


	gfx.m_pSprite->Flush();
	pVB->Release();
	pVB=NULL;

}

void CCore::DrawNormalBuffer(void)
{
	if (!this->Settings.shaders) return;
	if (RenderTargetNormal==NULL) return;
	gfx.m_pSprite->Flush();
	this->SetAlphaMode(false);

	IDirect3DSurface9 *OldRT;
	IDirect3DSurface9 *NewRT;

	if (Settings.shaders)
	{
		gfx.m_pd3dDevice->GetRenderTarget( 0, &OldRT );
		this->RenderTargetNormal->GetSurfaceLevel(0,&NewRT);
		gfx.m_pd3dDevice->SetRenderTarget( 0, NewRT );
	}

	if (!gfx.InputKeyDown(DIK_X))
		gfx.Clear(128,128,128);
	RenderWater();

	landscape.DrawParticles(10);
	enemies.DrawEnemies(10);
	ents.DrawEnts(4);
	
	gfx.m_pSprite->Flush();


	

	SetAlphaMode(false);
	if (Settings.shaders)
	{
		gfx.m_pd3dDevice->SetRenderTarget( 0, OldRT );
		NewRT->Release();
		OldRT->Release();
	}
}

void CCore::RenderWater(void)
{
	//WaterEffect_Speed = 1.0f;
	//WaterEffect_Size = 0.2f;

	landscape.DrawWaterLayer(landscape.t_normalwater,(TimerA*0.05*landscape.WaterEffect_Speed),1.0f,1.5f+(sin((TimerA)*0.005*landscape.WaterEffect_Speed)*0.1),200*landscape.WaterEffect_Size);
	landscape.DrawWaterLayer(landscape.t_normalwater,(TimerA*-0.05*landscape.WaterEffect_Speed),0.9f,1.5f+(sin((TimerA+(D3DX_PI/2.0f))*0.0041*landscape.WaterEffect_Speed)*0.095),120*landscape.WaterEffect_Size);
	landscape.DrawWaterLayer(landscape.t_normalwater,(TimerA*0.055*landscape.WaterEffect_Speed),1.1f,1.5f+(cos((TimerA+(D3DX_PI/2.0f))*0.0043*landscape.WaterEffect_Speed)*0.11),120*landscape.WaterEffect_Size);
	landscape.DrawWaterLayer(landscape.t_normalwater,(TimerA*-0.055*landscape.WaterEffect_Speed),0.9f,1.5f+(cos((TimerA)*0.0043*landscape.WaterEffect_Speed)*0.1),120*landscape.WaterEffect_Size);

	//landscape.DrawWaterLayer(landscape.t_normalwater,(TimerA*-0.03*landscape.WaterEffect_Speed),0.8f,1.0+sin(TimerA*0.01*landscape.WaterEffect_Speed)*0.25,40*landscape.WaterEffect_Size);


}
