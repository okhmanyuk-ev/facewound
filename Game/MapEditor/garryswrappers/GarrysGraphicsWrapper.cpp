#include "GarrysGraphicsWrapper.h"

// most the functions/classes here just simplify the usage of directx

bool CGarrysGraphics::CreateDisplay(UINT width, UINT height, BOOL windows, HWND* p_hWnd)
{
	WriteToLog("Starting Display Setup<blockquote>");

	m_OffScreenTexture=NULL;
	m_pLine = NULL;
	m_pSprite = NULL;
	m_pFont = NULL;
	Windowed = windows;

	WriteToLog("Creating Direct3D Object (sdk version %i)<br>", D3D_SDK_VERSION );
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if (m_pD3D==NULL) { MessageBox(NULL,"Failed to create Direct3D Object","error",MB_ICONEXCLAMATION); exit(1);}


	
	ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );

	D3DDISPLAYMODE mode;

	// run fullscreen
	if (windows==FALSE)
	{
		WriteToLog("<br><b>Setting Present Parameters:</b><br> FullScreen<br>Screen Width: %i<br>Screen Height: %i<br><br>", width, height );
		
		m_d3dpp.BackBufferHeight = height;
		m_d3dpp.BackBufferWidth = width;

		m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT , &mode);

		mode.RefreshRate = 0; 
		mode.Width = width;
		mode.Height = height;
		m_d3dpp.BackBufferFormat = mode.Format;
	}
	else
	{
		WriteToLog("<b>Setting Present Parameters:</b><br>Windowed<br><br>");
		m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

		m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT , &mode);

		
		m_d3dpp.BackBufferWidth = height;
		m_d3dpp.BackBufferHeight = width;

		mode.RefreshRate = 0; 
		mode.Width = 100;
		mode.Height = 100;
		m_d3dpp.BackBufferFormat = mode.Format;
	}

	m_d3dpp.Windowed = windows;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	

	HRESULT hr;

	WriteToLog("<b>Creating D3D Device...</b> ");
	hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, *p_hWnd,
                                  D3DCREATE_SOFTWARE_VERTEXPROCESSING ,
                                  &m_d3dpp, &m_pd3dDevice );

	if (hr!=D3D_OK) { WriteToLog("<font color=red>Failed! (error: hr %x)</font><br>",hr); MessageBox(NULL,"Failed to create Direct3D Device","error",MB_ICONEXCLAMATION); exit(1);}
	WriteToLog("Created OK!<br>");

	

	SetUpSprites();

	WriteToLog("</blockquote>Display Setup Succeeded<br><br>");
	return true;
}

// don't ever need to call this manually
bool CGarrysGraphics::SetUpSprites()
{
	WriteToLog("<b>Creating Sprite Object...</b> ");
	HRESULT hr = D3DXCreateSprite(m_pd3dDevice, &m_pSprite);
	//if (hr!=D3D_OK) { WriteToLog("<font color=red>Failed! (error: hr %x)</font><br>",hr);MessageBox(NULL,"Failed to create Sprite Object","error",MB_ICONEXCLAMATION); exit(1);}
	WriteToLog("Created OK!<br>");

	return true;
}

bool CGarrysGraphics::ReleaseSprites()
{
	if (m_pSprite!=NULL)
	{
		m_pSprite->Release();
		m_pSprite=NULL;
	}
	return true;
}

bool CGarrysGraphics::ReleaseDXFont()
{
	if (m_pFont!=NULL)
	{
		m_pFont->Release();
		m_pFont=NULL;
	}
	return true;
}

void CGarrysGraphics::WindowResize(HWND *hWnd)
{
	RECT winrect;
	GetClientRect(*hWnd,&winrect);
	ResetDevice((int)winrect.right-winrect.left,(int)winrect.bottom-winrect.top);
}

void CGarrysGraphics::WindowResize(unsigned int width, unsigned int height)
{
	ResetDevice(width,height);
}

// Call this to change resolutions - or if the window changes shape
bool CGarrysGraphics::ResetDevice(UINT width, UINT height)
{
	if (!m_pd3dDevice) return false;

	HRESULT hr;
	WriteToLog("Resetting Device (%ix%i)<br> ",width, height);

	hr = m_pd3dDevice->TestCooperativeLevel();
	WriteToLog("<font color=red> Test Coop Returned: (hr %x (%s))</font><br>",hr, DXGetErrorString9(hr));



	

	if (Windowed)
	{
		m_d3dpp.BackBufferWidth = width;
		m_d3dpp.BackBufferHeight = height;
		WriteToLog("Windowed (%ix%i)<br> ",m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight);
	}

	
	WriteToLog("Releasing Sprites<br>");
	ReleaseSprites();
	WriteToLog("Releasing Font<br>");
	ReleaseDXFont();

	if (m_OffScreenTexture!=NULL)
	{
		IDirect3DSurface9* surfacepointer;
		m_OffScreenTexture->GetSurfaceLevel(0,&surfacepointer);
		surfacepointer->Release();
		m_OffScreenTexture->Release();
		m_OffScreenTexture=NULL;
	}

	if (m_pLine!=NULL) {WriteToLog("Releasing Line<br>");m_pLine->Release();}

	

	if (width==0 && height==0) 
	{
		bSuspended = true; 
		WriteToLog("Suspending D3D, the window is minimized<br>");
	}




	if (bSuspended == true)
	{
		WriteToLog("Device is still suspended.. Not doing anything<br>");
	}
	else
	{
		WriteToLog("<b>Resetting D3D...</b><br> ");
		hr = this->m_pd3dDevice->Reset(&m_d3dpp);
		WriteToLog("Windowed (%ix%i)<br> ",m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight);

		if (hr!=D3D_OK) 
		{ 
			WriteToLog("<br><font color=red> DEVICE RESET Failed! (error: hr %x (%s))</font><br>",hr, DXGetErrorString9(hr));
			if (hr==D3DERR_DEVICELOST)
			{
				bSuspended = true; 
				WriteToLog("Suspending D3D, we lost the device");
			}
			else
			{
				exit(1);
			}
		}
		else WriteToLog("Device reset fine<br>");


		SetUpSprites();
		ReCreateDXFont();

		if (m_OffScreenTexture==NULL)
		{
			WriteToLog("Creating SpecialFXBackbuffer (%i %i %d)<br>", m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, m_d3dpp.BackBufferFormat);
			
			hr = m_pd3dDevice->CreateTexture( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 1, D3DUSAGE_RENDERTARGET,
			m_d3dpp.BackBufferFormat, D3DPOOL_DEFAULT, &m_OffScreenTexture, NULL );
			WriteToLog("SpecialFXBackbuffer returned: (hr %x (%s))<br>",hr, DXGetErrorString9(hr));

		}

		bSuspended = false;
	}

	return true;
}

// warning: D3DXLines crashes some computers on windows XP for some reason
bool CGarrysGraphics::SetupLines()
{
	WriteToLog("<b>Creating Line Object...</b> ");
	HRESULT hr = D3DXCreateLine(m_pd3dDevice,&m_pLine);
	if (hr!=D3D_OK) {WriteToLog("<font color=red>Failed! (error: hr %x)</font><br>",hr);MessageBox(NULL,"Failed to create Line Object","error",MB_ICONEXCLAMATION); exit(1);}
	WriteToLog("Created OK!<br>");
	return true;
}




// normal sprite blit
void CGarrysGraphics::SpriteDraw(LPDIRECT3DTEXTURE9 pSrcTexture, float x, float y)
{
	SpriteDrawFull(pSrcTexture, x, y, 0, 0, 0, 1, 1, 0, 0, 0, 0,  255, 255, 255, 255);
}

// alpha red green blue
void CGarrysGraphics::SpriteDraw(LPDIRECT3DTEXTURE9 pSrcTexture, float x, float y, int a, int r, int g, int b)
{
	SpriteDrawFull(pSrcTexture, x, y, 0, 0, 0, 1, 1, 0, 0, 0, 0,  a, r, g, b);
}

// rotations
void CGarrysGraphics::SpriteDraw(LPDIRECT3DTEXTURE9 pSrcTexture, float x, float y, float rot, float rotx, float roty)
{
	SpriteDrawFull(pSrcTexture, x, y, rot, rotx, roty, 1, 1, 0, 0, 0, 0,  255, 255, 255, 255);
}

void CGarrysGraphics::SpriteDraw(LPDIRECT3DTEXTURE9 pSrcTexture, float x, float y, float scalex, float scaley)
{
	SpriteDrawFull(pSrcTexture, x, y, 0, 0, 0, scalex, scaley, 0, 0, 0, 0,  255, 255, 255, 255);
}


void CGarrysGraphics::SpriteDraw(LPDIRECT3DTEXTURE9 pSrcTexture, float x, float y, float rot, float rotx, float roty, float scalex, float scaley)
{
	SpriteDrawFull(pSrcTexture, x, y, rot, rotx, roty, scalex, scaley, 0, 0, 0, 0,  255, 255, 255, 255);
}



void CGarrysGraphics::SpriteDrawEx(LPDIRECT3DTEXTURE9 pSrcTexture, float x, float y, float src_x, float src_y, float sizex, float sizey)
{
	SpriteDrawFull(pSrcTexture, x, y, 0, 0, 0, 1, 1, src_x, src_y, sizex, sizey,  255, 255, 255, 255);
}

void CGarrysGraphics::SpriteDrawEx(LPDIRECT3DTEXTURE9 pSrcTexture, float x, float y, float scalex, float scaley, float src_x, float src_y, float sizex, float sizey)
{
	SpriteDrawFull(pSrcTexture, x, y, 0, 0, 0, scalex, scaley, src_x, src_y, sizex, sizey,  255, 255, 255, 255);
}

void CGarrysGraphics::SpriteDrawEx(LPDIRECT3DTEXTURE9 pSrcTexture, float x, float y, float rot, float rotx, float roty, float scalex, float scaley, float src_x, float src_y, float sizex, float sizey)
{
	SpriteDrawFull(pSrcTexture, x, y, rot, rotx, roty, scalex, scaley, src_x, src_y, sizex, sizey,  255, 255,255, 255);
}

void CGarrysGraphics::SpriteDrawEx(LPDIRECT3DTEXTURE9 pSrcTexture, float x, float y, float rot, float rotx, float roty, float scalex, float scaley, float src_x, float src_y, float sizex, float sizey,  int a, int r, int g, int b)
{
	SpriteDrawFull(pSrcTexture, x, y, rot, rotx, roty, scalex, scaley, src_x, src_y, sizex, sizey,  a, r, g, b);
}


void CGarrysGraphics::SpriteDrawFull(LPDIRECT3DTEXTURE9 pSrcTexture, float x, float y, float rot, float rotx, float roty, float scalex, float scaley, float src_x, float src_y, float sizex, float sizey,  int a, int r, int g, int b)
{
	D3DXVECTOR3	pos(1,1,1);
	D3DXVECTOR2	pos2d(x,y);

	RECT* source=NULL;

	if (src_x!=0 || src_y!=0 || sizex!=0 || sizey!=0)
	{
		source = new RECT;
		source->left=(long)src_x;
		source->top=(long)src_y;
		source->right=(long)src_x+sizex;
		source->bottom=(long)src_y+sizey;
	}

	D3DXVECTOR2	scale(scalex,scaley);
	D3DXVECTOR3	rotxyz(rotx,roty,1);
	D3DXVECTOR2	rotxy(rotx,roty);

	D3DXMATRIX tmatrix;

	D3DXMatrixTransformation2D
    ( &tmatrix, 
	  &rotxy,
      0, 
	  &scale,
      &rotxy, 
	  -rot,
      &pos2d);



	m_pSprite->SetTransform(&tmatrix);

	m_pSprite->Draw(pSrcTexture,source,NULL, NULL, D3DCOLOR_ARGB(a,r,g,b));
	
	if (src_x!=0 || src_y!=0 || sizex!=0 || sizey!=0)
	{
		delete source;
	}
}

bool CGarrysGraphics::CreateDirectInput(HINSTANCE* hInst, HWND* hWnd)
{
	WriteToLog("<b>Direct Input Setup</b><blockquote> ");

	HRESULT hr;

	WriteToLog("<b>Creating Direct Input Device...</b> ");
	hr = DirectInput8Create(*hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	if (hr!=D3D_OK) { WriteToLog("<font color=red>Failed! (error: hr %x)</font><br>",hr);MessageBox(NULL,"Failed to create Direct Input Object","error",MB_ICONEXCLAMATION); exit(1);}
	WriteToLog("Created OK!<br>");

	WriteToLog("<b>Creating Keyboard Device...</b> ");
	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	if (hr!=D3D_OK) { WriteToLog("<font color=red>Failed! (error: hr %x)</font><br>",hr);MessageBox(NULL,"Failed to create Keyboard Device","error",MB_ICONEXCLAMATION); exit(1);}
	WriteToLog("Created OK!<br>");

	WriteToLog("<b>Setting Keyboard Data Format...</b> ");
	hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);


	if (hr!=D3D_OK) 
	{ 
	//	CleanUpDirectInput();
		WriteToLog("<font color=red>Failed! (error: hr %x)</font><br>",hr);
		MessageBox(NULL,"Failed to set Keyboard Data Format","error",MB_ICONEXCLAMATION); 
		exit(1);
	
	}
	WriteToLog("Created OK!<br>");

	WriteToLog("<b>Setting Keyboard Coop Level...</b> ");
	m_pKeyboard->SetCooperativeLevel(*hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	WriteToLog("Created OK!<br>");

	WriteToLog("<b>Grabbing Keyboard..</b> ");
	m_pKeyboard->Acquire(); 
	WriteToLog("Created OK!<br>");

	WriteToLog("<b>Creating Mouse Device...</b> ");
	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if (hr!=D3D_OK) {WriteToLog("<font color=red>Failed! (error: hr %x)</font><br>",hr); MessageBox(NULL,"Failed to Create Mouse Device","error",MB_ICONEXCLAMATION); exit(1);}
	WriteToLog("Created OK!<br>");
	
	WriteToLog("<b>Setting mouse data format...</b> ");
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	WriteToLog(" OK!<br>");
	
	WriteToLog("<b>Setting Coop level...</b> ");
	m_pMouse->SetCooperativeLevel(*hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	WriteToLog(" OK!<br>");

	WriteToLog("<b>Grabbing mouse..</b> ");
	m_pMouse->Acquire();
	WriteToLog("OK!<br>");



	WriteToLog("</blockquote>Input setup successfully<br><br>");
	return true;
}



bool CGarrysGraphics::ReAquireDevices()
{
	WriteToLog("<b>ReAquiring Devices</b><br><br> ");

	if (m_pMouse)
	{
	WriteToLog("<b>Grabbing mouse..</b> ");
	m_pMouse->Acquire();
	WriteToLog("OK!<br>");
	}

	if (m_pKeyboard)
	{
	WriteToLog("<b>Grabbing Keyboard..</b> ");
	m_pKeyboard->Acquire(); 
	WriteToLog("Created OK!<br>");
	}

	
//	if (bSuspended)
//	{
	//	ResetDevice(800,600);
	//	bSuspended=false;
//	}

	return true;
}

bool CGarrysGraphics::InputRefreshKeyboard()
{
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(KeyboardState),(LPVOID)&KeyboardState);

	if (hr!=DI_OK)
	{
		WriteToLog("Keyboard Refresh Failed! (error: hr %x (%s))<br>",hr, DXGetErrorString9(hr));
		ZeroMemory( &KeyboardState, sizeof(KeyboardState) );

		if (!this->bSuspended)
		{
			this->ReAquireDevices();
		}

		return false; 
	}

	return true;
}

bool CGarrysGraphics::InputRefreshMouse()
{
    DIMOUSESTATE MouseState;
	HRESULT hr = m_pMouse->GetDeviceState(sizeof(MouseState),(LPVOID)&MouseState);
	
	if (hr!=DI_OK)
	{
		WriteToLog("Mouse Refresh Failed! (error: hr %x (%s))<br>",hr, DXGetErrorString9(hr));
		ZeroMemory( &MouseState, sizeof(MouseState) );
		return false; 
	}    

	//all off
	m_nMouseX = 0;
	m_nMouseY = 0;
	m_nMouseZ = 0;

    //Is the left mouse button down etc
    if(MOUSEBUTTONDOWN(MouseState.rgbButtons[0])) 
	{	
		if (m_bMouse1==false) m_bMouse1Click = true;
		else m_bMouse1Click = false;
		m_bMouse1= true;
	} else m_bMouse1=false;
    if(MOUSEBUTTONDOWN(MouseState.rgbButtons[1]))
    {	
		if (m_bMouse2==false) m_bMouse2Click = true;
		else m_bMouse2Click = false;
		m_bMouse2= true;
	} else m_bMouse2=false;
    if(MOUSEBUTTONDOWN(MouseState.rgbButtons[2])) 
			{	
		if (m_bMouse3==false) m_bMouse3Click = true;
		else m_bMouse3Click = false;
		m_bMouse3= true;
	} else m_bMouse3=false;
    if(MOUSEBUTTONDOWN(MouseState.rgbButtons[3]))  
	{	
		if (m_bMouse4==false) m_bMouse4Click = true;
		else m_bMouse4Click = false;
		m_bMouse4= true;
	} else m_bMouse4=false;

    m_nMouseX = MouseState.lX;
	m_nMouseY = MouseState.lY;
	m_nMouseZ = MouseState.lZ;

	return true;
}

bool CGarrysGraphics::InputKeyDown(UINT key)
{
	if(KEYDOWN(KeyboardState, key))
		return true;
	else
		return false;
}

bool CGarrysGraphics::ReCreateDXFont()
{

	WriteToLog("<b>Creating Font..</b> ");
//	HFONT hFont;
//	hFont = CreateFont(m_iFontSize, 0, 0, 0, m_iFontWeight, 0, 0, 0, ANSI_CHARSET, 0, 0, DRAFT_QUALITY, 0, m_sFontFace);
	HRESULT hr = D3DXCreateFontIndirect(m_pd3dDevice, &this->FontDescription, &m_pFont);

	if (FAILED(hr))
	{
		WriteToLog("Failed (error:%x)<br><br>",hr);
		return false;
	}
	else
	{
		WriteToLog("OK!<br><br> ");
		return true;
	}

}

bool CGarrysGraphics::CreateDXFont(int size, int weight, LPCTSTR face)
{
	sprintf(FontDescription.FaceName, face);

	FontDescription.Height = size;
	FontDescription.Weight = weight;
	FontDescription.MipLevels = 1;
	FontDescription.OutputPrecision = OUT_DEFAULT_PRECIS;
	FontDescription.PitchAndFamily = DEFAULT_PITCH;
	FontDescription.Quality = DEFAULT_QUALITY;
	FontDescription.Italic = false;
	FontDescription.CharSet = ANSI_CHARSET;
	
	WriteToLog("<b>Creating Font..</b> ");
	HRESULT hr = D3DXCreateFontIndirect(m_pd3dDevice, &FontDescription, &m_pFont);

	if (FAILED(hr))
	{
		WriteToLog("Failed (error:%x)<br><br>",hr);
		return false;
		
	}
	else
	{
		WriteToLog("OK!<br><br> ");
		return true;
	}
}

void CGarrysGraphics::DrawText(float x, float y,LPCTSTR string, int r, int g, int b)
{
	RECT location={(long)x,(long)y,(long)x+10,(long)y+10};

	D3DXMATRIX tmatrix;

	D3DXMatrixTranslation(&tmatrix,0,0,0);



	m_pSprite->SetTransform(&tmatrix);
	m_pFont->DrawText(this->m_pSprite ,string, -1,&location,DT_NOCLIP, D3DCOLOR_XRGB(r,g,b));

}



// a to b line function
// set width, antialias and pattern by calling them straight from the line function
void CGarrysGraphics::DrawLine(float x1, float y1, float x2, float y2, UINT a, UINT r,UINT g, UINT b)
{
	D3DXVECTOR2 vecs[2];

	vecs[0].x = x1;
	vecs[0].y = y1;
	vecs[1].x = x2;
	vecs[1].y = y2;

	m_pLine->Draw (vecs, 2, D3DCOLOR_ARGB(a,r,g,b));

}

void CGarrysGraphics::DrawLineEx1(D3DXVECTOR2* vec, int num, UINT a, UINT r,UINT g, UINT b)
{
	m_pLine->Draw (vec, num, D3DCOLOR_ARGB(a,r,g,b));
}
void CGarrysGraphics::Clear(UINT r,UINT g, UINT b)
{
	this->m_pd3dDevice->Clear(0,NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(r,g,b) , 1,0);
}

void CGarrysGraphics::WriteToLog(char *lpszText, ...)
{
	if(b_EnableLogging)
	{
		va_list argList;
		FILE *pFile;

		//Initialize variable argument list
		va_start(argList, lpszText);

		//Open the log file for appending
		pFile = fopen("Log.html", "a+");

		//Write the text and a newline
		vfprintf(pFile, lpszText, argList);
		putc('\n', pFile);

		//Close the file
		fclose(pFile);
		va_end(argList);
	}
}

void CGarrysGraphics::DrawTextEx(float x, float y, int r, int g, int b, char *lpszText, ...)
{
	char str[1024];

	va_list argList;
	va_start(argList, lpszText);

	vsprintf(str, lpszText, argList);

	this->DrawText(x,y,str,r,g,b);

	va_end(argList);
	
}

// LOGGING
void CGarrysGraphics::EnableLogging()
{
	b_EnableLogging = true;
	
	FILE* pFile;
	pFile = fopen("Log.html", "wb");

	WriteToLog("<font face=arial size=%i><font color='#dd8800'>Logging Enabled</font> (<font color=blue>",2);

	char            szTime[128];
	time_t          tp = time(NULL);

	strftime(szTime, sizeof szTime - 1, "%#c", localtime(&tp));

	WriteToLog(" %s</font>)<br><br>",szTime);



	fclose(pFile);
}


void CGarrysGraphics::Begin()
{
	this->m_pd3dDevice->BeginScene();
	this->m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGarrysGraphics::End()
{
	this->m_pSprite->End();
	this->m_pd3dDevice->EndScene();
}

void CGarrysGraphics::Flip()
{
	HRESULT hr;

	hr = this->m_pd3dDevice->Present(NULL,NULL,NULL,NULL);

	if (hr==D3DERR_DEVICELOST)
	{
		WriteToLog("The Device has been lost - suspended drawing.<br>");
		bSuspended = true;
	}
}

void CGarrysGraphics::Flip(int x, int y, int x2, int y2)
{
	HRESULT hr;

	RECT destrect={(long)x,(long)y,(long)x2,(long)y2};

	hr = this->m_pd3dDevice->Present(NULL,&destrect,NULL,NULL);

	if (hr==D3DERR_DEVICELOST)
	{
		WriteToLog("The Device has been lost - suspended drawing.<br>");
		bSuspended = true;
	}
}



void CGarrysGraphics::UpdateTimer()
{

  tick = (timeGetTime()-lasttick);
  lasttick = timeGetTime();

  if (tick>60) tick=60;

  RealTick = tick; // this is so you can alter tick (for bullettime etc..) and still have a reasonable bearing on time.

  //  this->WriteToLog("%d<br>",tick);

}