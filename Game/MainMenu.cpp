#include "mainmenu.h"
#include "core.h"

#define GFX this->pcore->gfx

#define PLAYRECT 0,0,255,64
#define OPTIONSRECT 0,64,255,64
#define QUITRECT 0,128,255,64
#define RESURECT 0,196,255,64

#define HOVER_NOTHING 0
#define HOVER_PLAY 1
#define HOVER_QUIT 2
#define HOVER_RESU 3
#define HOVER_OPTI 4

#define LINKSTOP 900

#define SCRW (GFX.m_d3dpp.BackBufferWidth)
#define SCRH (GFX.m_d3dpp.BackBufferHeight)

#define PLAYX ((30*GuiScale))
#define PLAYY (GFX.m_d3dpp.BackBufferHeight-(80*GuiScale))
#define PLAYXY PLAYX, PLAYY

#define RESUMEX ((60*GuiScale))
#define RESUMEY ((60*GuiScale))
#define RESUMEXY RESUMEX, RESUMEY

#define OPTIONSX ((300*GuiScale))
#define OPTIONSY (GFX.m_d3dpp.BackBufferHeight-(80*GuiScale))
#define OPTIONSXY OPTIONSX, OPTIONSY

#define QUITX ((930*GuiScale))
#define QUITY (GFX.m_d3dpp.BackBufferHeight-(80*GuiScale))
#define QUITXY QUITX, QUITY

#define SCALE (GuiScale)

CMainMenu::CMainMenu(void)
: musicchannel(0)
, SwipeType(0)
{
}

CMainMenu::~CMainMenu(void)
{
}

void CMainMenu::Do(void)
{
	GFX.InputRefreshKeyboard();
	GFX.InputRefreshMouse();
	GFX.UpdateTimer();
	GetCursorPos(&point);
	if (GFX.Windowed) ScreenToClient(pcore->hWnd, &point);


	if (!GFX.InputKeyDown(DIK_ESCAPE) &&
		!GFX.InputKeyDown(DIK_SPACE) &&
		!GFX.InputKeyDown(DIK_RETURN))
	{
		EscapeKeyDown = false;
	}

	if (!EscapeKeyDown)
	{
		if (GFX.InputKeyDown(DIK_ESCAPE) && this->bShowOptions)
		{
			//exit options screen - HACK
			options.BindKeys();
			bShowOptions = false;
			options.SaveOptions();
			EscapeKeyDown = true;
			this->ResetInVariables();

		}
		else if (GFX.InputKeyDown(DIK_ESCAPE) && pcore->inGame)
		{
			//resume game
			pcore->EscapeDown = true;
			this->UnInit();
			pcore->ResumeGame();
		}
	}

	if (bShowOptions)
	{
		options.Do();
	}

	if (this->SplashTimer>255) f_timer += GFX.tick/1000.0f;
	onMouseOne();
}

void CMainMenu::Loop(void)
{
	Draw();
	Do();
}

void CMainMenu::Draw(void)
{
	GFX.Begin();
/*
	// BACKGROUND GRADIENT
	GFX.SpriteDrawEx(t_menubits,0,0,0,0,0,
		GFX.m_d3dpp.BackBufferWidth,(1024.0f/256.0f)*GuiScale,
		0,0,1,256,
		255,255,255,255);*/
	GFX.Clear(63,62,63);

//	DrawSky();

	

	if (bShowOptions)
	{
		options.Draw();
	}
	else
	{
		DrawSplash();
		DrawPicture();
	}

	DrawMainLogo();

	DrawButtons();

	DrawTextScroller();


	

	GFX.End();
	GFX.Flip();
}

void CMainMenu::Init(void)
{
	FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), TRUE);

	GuiScale = (pcore->gfx.m_d3dpp.BackBufferWidth+pcore->gfx.m_d3dpp.BackBufferHeight)/((float)1024+768);
	f_timer = 0;
	scrolloffset = 0;
	GFX.InputRefreshKeyboard();
	bShowOptions=false;


	if (GFX.InputKeyDown(DIK_ESCAPE) ||
		GFX.InputKeyDown(DIK_SPACE) ||
		GFX.InputKeyDown(DIK_RETURN))
	{
		EscapeKeyDown = true;
	}

	t_menubits	= pcore->textures.LoadTexture("textures/menu/menu_bits.tga");
	t_menua		= pcore->textures.LoadTexture("textures/menu/logoa.tga");
	t_menub		= pcore->textures.LoadTexture("textures/menu/logob.tga");
	t_glint		= pcore->textures.LoadTexture("textures/menu/glint.tga");
	picture		= pcore->textures.LoadTexture("textures/menu/menuscreen.tga");

	xscale = GuiScale;
	yscale = GuiScale;

	music = pcore->sound.LoadStream("sound/music/menuscreen.ogg",true);
	musicchannel = pcore->sound.PlayStream(music);
	pcore->RefreshMusicVolume();

	this->options.pcore = pcore;
	this->options.Init();

	for (int i=0;i<24;i++)
	{
		splash[i] = pcore->textures.LoadTextureEx("textures/menu/mi/main_%02i.jpg",i+1);
	}


	ResetInVariables();
	LoadSky("menu");
}

void CMainMenu::DrawGlint(float x, float y)
{
	float size = rand()%2000/500.0f;
	GFX.SpriteDrawEx(t_glint,x-128*size,y-128*size,
		0,0,0,
		size,size,
		0,0,256,256,
		255,255,255,255);
}

void CMainMenu::DrawGlintFaded(float x, float y)
{
	float size = rand()%10000/6000.0f;
	GFX.SpriteDrawEx(t_glint,x-128*size,y-128*size,
		0,0,0,
		size,size,
		0,0,256,256,
		rand()%50,255,255,255);
}

void CMainMenu::DrawMainLogo(void)
{
	if (bShowOptions) return;
	float scalex, scaley;
	if (f_timer<0.1) return;

	if (logoyvel!=0)
	{
		logoy = logoy + logoyvel;
		logoyvel = logoyvel + (0.05f*pcore->gfx.RealTick);
	}
	if (logoy>10)
	{
		logoyvel = logoyvel*-0.4;
		logoy = 10;
		if (fabs(logoyvel)<3) logoyvel=0;
	}

	scalex = 0.5;
	scaley = 0.5;//(1+((sin(f_timer*0.1f)*0.1f)/2));

    float *spectrum;
	float multiplier =0;


	float alpha = f_timer*50.0f;
	if (alpha>255) alpha = 255;

    spectrum = FSOUND_DSP_GetSpectrum(); 

	if (spectrum)
	{
		for (int i=100;i<500;i++)
		{
			multiplier = multiplier + spectrum[i];
		}
		alpha = (0.2+(multiplier*0.2f));
	}

	alpha = alpha*255.0f;
	if (alpha<0) alpha = 0;
	if (alpha>255) alpha = 255;

	scaley = 0.95f;
	scalex = scaley;

	GFX.SpriteDrawEx(t_menua,
		SCRW-(280*scalex*xscale),
		(logoy)*yscale*scaley,
		0,0,0,
		scalex*xscale,yscale*scaley,
		0,0,256,512,
		255,alpha,alpha,alpha);

	GFX.SpriteDrawEx(t_menub,
		SCRW-(280*scalex*xscale),
		(logoy+512)*yscale*scaley,
		0,0,0,
		xscale*scalex,yscale*scaley,
		0,0,256,256,
		255,alpha,alpha,alpha);

	pcore->SetAlphaMode(true);


	if (alpha>254)
	{
		if (fmod(f_timer,10)>1.5 && fmod(f_timer,10)<1.59)
		DrawGlint((130*xscale)+((-322)*xscale*scalex),(50+10)*yscale*scaley);

		if (fmod(f_timer,10)>1.58 && fmod(f_timer,10)<1.61)
		DrawGlint((130*xscale)+((-7)*xscale*scalex),(50+40)*yscale*scaley);

		if (fmod(f_timer,10)>1.60 && fmod(f_timer,10)<1.63)
		DrawGlint((130*xscale)+((190)*xscale*scalex),(50+60)*yscale*scaley);

		if (fmod(f_timer,10)>1.62 && fmod(f_timer,10)<1.66)
		DrawGlint((130*xscale)+((250)*xscale*scalex),(50+130)*yscale*scaley);
		
		
		for (int i=0;i<5;i++)
		{
			DrawGlintFaded((130*xscale)+((-322)*xscale*scalex),(50+10)*yscale*scaley);
			DrawGlintFaded((130*xscale)+((-7)*xscale*scalex),(50+40)*yscale*scaley);
			DrawGlintFaded((130*xscale)+((190)*xscale*scalex),(50+60)*yscale*scaley);
			DrawGlintFaded((130*xscale)+((510)*xscale*scalex),(50+40)*yscale*scaley);
		}
	}

	pcore->SetAlphaMode(false);
}



int CMainMenu::HoveredOver(void)
{
	//play
	if (point.x>(PLAYX) && 
		point.x<(PLAYX+(256*xscale)) &&
		point.y>(PLAYY) &&
		point.y<(PLAYY+(64*yscale)))
		return HOVER_PLAY;


	//quit
	if (point.x>(QUITX) && 
		point.x<(QUITX+(256*xscale)) &&
		point.y>(QUITY) &&
		point.y<(QUITY+(64*yscale)))
		return HOVER_QUIT;

	if (point.x>(OPTIONSX) && 
		point.x<(OPTIONSX+(256*xscale)) &&
		point.y>(OPTIONSY) &&
		point.y<(OPTIONSY+(64*yscale)))
		return HOVER_OPTI;

	// resume
	if (pcore->inGame)
	if (point.x>(RESUMEX) && 
		point.x<(RESUMEX+(256*xscale)) &&
		point.y>(RESUMEY) &&
		point.y<(RESUMEY+(64*yscale)))
		return HOVER_RESU;

	return 0;
}

void CMainMenu::UnInit(void)
{
	FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), FALSE);
	pcore->sound.CloseStream(music);
	music=NULL;
	musicchannel=0;
}

void CMainMenu::onMouseOne(void)
{
	if (!GFX.m_bMouse1Click) return;
	if (this->bShowOptions) return;

	if (HoveredOver()==HOVER_PLAY) 
	{
		this->UnInit();
		pcore->StartGame();
	}
	else
	if (HoveredOver()==HOVER_RESU)
	{
		this->UnInit();
		pcore->ResumeGame();
		pcore->players[0].player.bFireDown = true;
	}
	else
	if (HoveredOver()==HOVER_QUIT)
	{
		pcore->ExitGame();
	}
	else
	if (HoveredOver()==HOVER_OPTI)
	{
		this->options.Init();
		bShowOptions=true;
	}

}

void CMainMenu::DrawButtons(void)
{
	if (bShowOptions) return;

	pcore->SetAlphaMode(false);

	float alpha;

	for (int i=0;i<2;i++)
	{

		if (pcore->inGame)
		{
			pcore->SetAlphaMode(false);
			if (HoveredOver()==HOVER_RESU){pcore->SetAlphaMode(true);}
			GFX.SpriteDrawEx(t_menubits,
				RESUMEXY,
				0,0,0,
				xscale,yscale,
				RESURECT,
				255,255,255,255);
		}

		pcore->SetAlphaMode(false);
		if (HoveredOver()==HOVER_PLAY){pcore->SetAlphaMode(true);}
		GFX.SpriteDrawEx(t_menubits,
			i+PLAYXY,
			0,0,0,
			xscale,yscale,
			PLAYRECT,
			255,255,255,255);

		pcore->SetAlphaMode(false);
		if (HoveredOver()==HOVER_OPTI){pcore->SetAlphaMode(true);}
		GFX.SpriteDrawEx(t_menubits,
			i+OPTIONSXY,
			0,0,0,
			xscale,yscale,
			OPTIONSRECT,
			255,255,255,255);

		pcore->SetAlphaMode(false);
		if (HoveredOver()==HOVER_QUIT){pcore->SetAlphaMode(true);}
		GFX.SpriteDrawEx(t_menubits,
			i+QUITXY,
			0,0,0,
			xscale,yscale,
			QUITRECT,
			255,255,255,255);

	}


	pcore->SetAlphaMode(false);
}

void CMainMenu::DrawTextScroller(void)
{
}

void CMainMenu::DrawSky(void)
{

	pcore->gfx.Clear(60,60,63);

	return;

	float sc = ((pcore->landscape.window_width/4.0f)/256.0f);
	float czoom = 1;

	if (pcore->landscape.window_height>pcore->landscape.window_width) sc = ((pcore->landscape.window_height/3.0f)/256.0f);
	if (pcore->landscape.window_height/3>256*sc) sc = ((pcore->landscape.window_height/3.0f)/256.0f);

	if (sc<1) sc=1;
	int x,y;
	int i=0;

	m_skyx = m_skyx	+ (0.01f*(pcore->gfx.RealTick));

	if (m_skyx>sc*4*256) m_skyx=0;
	int iadd = (int)(((((float)m_skyx))/(256*sc)))%4;

	for (y=-2;y<1;y++)
	{

		for (x=-2;x<3;x++)
		{

			pcore->gfx.SpriteDrawEx(skymap[((x+2+iadd)%4 +(4*(y+2)))%16],
						
						(((pcore->landscape.window_width/2)*czoom)  + (x*sc*256*czoom))-(fmod(m_skyx,256*sc)),
						((pcore->landscape.window_height/2) + (y*sc*256*czoom))+128*sc,
						
						

						0,0,0,
						
						(sc*czoom),(sc*czoom),
						0,0,256,256,
						255,255,255,255
						
						
						
						);



	if (x!=2) i++;

		}
	}

	i=0;

}

void CMainMenu::LoadSky(char* skyname)
{
	return;

	HRESULT hr;	
	char filename[500];
	for (int i=1;i<=12;i++)
	{
		sprintf(filename, "textures/skys/%s/%i.jpg",skyname,i);
		skymap[i-1] = pcore->textures.LoadTexture(filename);
	}
}
void CMainMenu::DrawPicture(void)
{
	if (this->SplashTimer<255) return;

	float timer =f_timer-3;
	if (timer<0) return;

	int alpha = timer*200.0f;
	if (alpha>255) alpha = 255;
	int color = (timer*200.0f)-200;
	if (color<0) color = 0;
	if (color>255) color = 255;

	pcore->SetAlphaMode(false);
	pcore->gfx.SpriteDrawEx(picture,((380*GuiScale)),((540*GuiScale)),0,0,0,SCALE,SCALE,alpha, color, color, color);
}

void CMainMenu::DrawSplash(void)
{
	
	float scale=(pcore->gfx.m_d3dpp.BackBufferWidth/1600.0f)*1.0;
	float ox=25*scale, oy=32*scale;

	if (SplashTimer<256)
	{
		if (SplashTimer==0) SplashTimer = 5.0f;
		SplashTimer = SplashTimer + (pcore->gfx.RealTick*1.1f);//(((pcore->gfx.RealTick*0.1f)*SplashTimer)*0.1f);
	}
	else
	{
		SplashTimer = 256;
	}

    float *spectrum = FSOUND_DSP_GetSpectrum(); 

	float multiplier=0;
	if (spectrum)
	{
		for (int i=100;i<500;i++)
		{
			multiplier = multiplier + spectrum[i];
		}
		multiplier = multiplier/300.0f;
	}


	int width, alpha, height, oscale;

	for (int i=0;i<24;i++)
	{
		//if (i>(SplashTimer*0.01f)) break;

		width = 256;
		alpha = 255;
		height = 256;




		float oscale = scale;

		if (SwipeType==0)
		{
			width = SplashTimer;
		}
		else if (SwipeType==1)
		{
			height = SplashTimer;
			width = SplashTimer;
		}
		else if (SwipeType==2)
		{
			height = SplashTimer;
		}
		else if (SwipeType==3)
		{
			alpha =  SplashTimer;
			if (alpha>255) alpha=255;
		}
		else if (SwipeType==4)
		{
			oscale = scale*(SplashTimer/255.0f);
			height = SplashTimer;
			width = SplashTimer;
		}
		else
		{
			oscale = scale*(SplashTimer/255.0f);
		}

		

	//	oscale = oscale + multiplier;

		//width = SplashTimer;

	//	

	//	height = SplashTimer;
	//	width = SplashTimer;

		pcore->gfx.SpriteDrawEx(splash[i],
							(158*scale)+((i%6)*256*scale)-(oscale*128),
							(140*scale)+((i/6)*256*scale)-(oscale*128),
							0,128,128,
							oscale,oscale,
							0,0,width,height,
							alpha,255,255,255);
	}
}

void CMainMenu::ResetInVariables(void)
{
	SplashTimer = 0;
	logoy = -800.0f;
	logoyvel = 0.00001f;
	f_timer=0;
	SwipeType=rand()%5;
}
