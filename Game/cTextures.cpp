#include "ctextures.h"
#include "core.h"

cTextures::cTextures(void)
{
}

cTextures::~cTextures(void)
{
}

LPDIRECT3DTEXTURE9 cTextures::LoadTextureEx(char *lpszText, ...)
{
	va_list argList;
	char temp[500];
	va_start(argList, lpszText);
	vsprintf(temp, lpszText, argList);

	return LoadTexture(temp);
}

LPDIRECT3DTEXTURE9 cTextures::LoadTexture(char* inputname)
{
	char filename[500];

	
	//pcore->gfx.WriteToLog("Loading texture '%s'<br>", inputname); 

	// todo, does texture already exist
	sTexture* p=firsttexture;
	while (p!=NULL)
	{
		if (strcmp(inputname, p->fn)==0)
		{
		//	pcore->gfx.WriteToLog("Already Loaded Texture '%s'<br>", inputname); 
			return p->t;
		}
		p=p->Next;
	}

	sprintf(filename, "%s/%s", pcore->modfolder,inputname);
	if (!FileExists(filename))
		sprintf(filename, "%s/%s", DEFAULT_MOD_FOLDER,inputname);


	// if not then load it
	sTexture* np = new sTexture();
	np->Next = NULL; // we're on the end
	if (firsttexture==NULL) 
	{
		firsttexture = np;
	}
	else 
	{
		// set last ent's next to me
		p=firsttexture;
		while (p->Next!=NULL)
		{
			p=p->Next;
		} // loop until we land on the last one
		p->Next = np;
	}

	HRESULT hr = D3DXCreateTextureFromFile( pcore->gfx.m_pd3dDevice,filename, &np->t);
	if (hr!=D3D_OK) 
	{ 
		char error[500];
		//todo - make this more friendly (maybe load an error texture)
		pcore->gfx.WriteToLog("<font color=red size=4><b>MISSING TEXTURE (%s)</b></font><br>", filename);
		sprintf(error, "Missing Texture: '%s'",filename);
		MessageBox(NULL,error,"Error Loading Texture",MB_ICONEXCLAMATION); 
		pcore->ExitGame();
	}
	else 
	{ 
		sprintf(np->fn, inputname);
	}


	

	return np->t;
}


void cTextures::FreeAll()
{
	sTexture* p=firsttexture;
	while (p!=NULL)
	{
		if (p->t!=NULL)	p->t->Release();
		p=p->Next;
	}
}