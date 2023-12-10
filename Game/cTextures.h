#pragma once

#include "garryswrappers/GarrysGraphicsWrapper.h"



class CCore;

struct sTexture
{
	char fn[100];
	LPDIRECT3DTEXTURE9 t;

	sTexture* Next;
	sTexture()
	{
		t=NULL;
		Next=NULL;
	}
};

class cTextures
{
public:
	cTextures(void);
	~cTextures(void);
	
	unsigned int NumTextures;
	LPDIRECT3DTEXTURE9 LoadTextureEx(char *lpszText, ...);
	LPDIRECT3DTEXTURE9 LoadTexture(char* filename);
	sTexture* firsttexture;
	void FreeAll();

	CCore* pcore;
};
