#include "Entitys.h"
#include "garryswrappers/GarrysGraphicsWrapper.h"
#include "Landscape.h"
#include "Core.h"

#define DRAW_ENTITY_TEXT false
#define MZOOM (pcore->landscape.m_zoom)
#define SCROFFX (pcore->landscape.offset_x)
#define SCROFFY (pcore->landscape.offset_y)

void Entitys::Init()
{
	FirstEnt=NULL;
	NumEnts=0;
	bDraw=DRAW_ENTITY_TEXT;

	checkpoint = pcore->textures.LoadTexture("textures/props/checkpoint.bmp");

	s_hiddenarea = pcore->sound.LoadSound(10,"sound/secret.ogg");
	s_checkpoint = pcore->sound.LoadSound(10,"sound/checkpoint.ogg");
}


// add an entity - edit the values by catching the returned pointer
LPEntity Entitys::AddEntity()
{
	LPEntity pent = new sEntitys();

	pent->Next = NULL; // we're on the end

	if (FirstEnt==NULL) 
	{
		FirstEnt = pent;
	}
	else 
	{
		// set last ent's next to me
		LPEntity Pe=FirstEnt;

		while (Pe->Next!=NULL)
		{
			Pe=Pe->Next;
		} // loop until we land on the last one
		Pe->Next = pent;	
	}

	NumEnts++;

	return pent;
}


// set gfx pointer
void Entitys::SetGfxPointer(CGarrysGraphics *gfxpointer)
{

}
#define ET_SPAWN 1
#define ET_PHYSICSOBJECT 2
#define ET_ZOMBIE 3
#define ET_PROP 4
#define ET_CONSOLECOMMAND 5
#define ET_SOLDIERTURRET 6
#define ET_LADDER 7
#define ET_TRIGGER 9
#define ET_SLIDER 10
#define ET_PICKUP 8

// 0 = behind everything
// 1 = behind player
// 2 = infront of eveything
// 4 = normalmap
void Entitys::DrawEnts(int layer)
{
	char string[255];
	LPEntity Pe=FirstEnt;

	while (Pe!=NULL)
	{
		// always 'draw' sound
		if (Pe->type==ENT_SOUND && layer==0) DrawSound(Pe);

		if (Pe->x+2024 > SCROFFX && 
			Pe->y+2024 > SCROFFY &&
			Pe->y-2024 < SCROFFY+(pcore->gfx.m_d3dpp.BackBufferHeight/MZOOM) && 
			Pe->x-2024 < SCROFFX+(pcore->gfx.m_d3dpp.BackBufferWidth/MZOOM))
		{
			if (Pe->type==ENT_LENSFLARE && layer==2) DrawLensFlare(Pe);
			else if (Pe->type==ENT_PLUTONIUM && layer==1) DrawPlutonium(Pe);
			else if (Pe->type==ENT_PROP && layer==0 && Pe->Width==0) DrawProp(Pe);
			else if (Pe->type==ENT_PROP && layer==1 && Pe->Width==1) DrawProp(Pe);
			else if (Pe->type==ENT_PROP && layer==2 && Pe->Width==2) DrawProp(Pe);
			else if (Pe->type==ENT_PROP && layer==4 && Pe->Width==4) DrawProp(Pe);
			
			else if (Pe->type==ENT_PARALLAXPROP && layer==0 && Pe->e==0) DrawParallaxProp(Pe);
			else if (Pe->type==ENT_PARALLAXPROP && layer==1 && Pe->e==1) DrawParallaxProp(Pe);
			else if (Pe->type==ENT_PARALLAXPROP && layer==2 && Pe->e==2) DrawParallaxProp(Pe);
			else if (Pe->type==ENT_PARALLAXPROP && layer==4 && Pe->e==4) DrawParallaxProp(Pe);


			else if (Pe->type==ENT_REFLECTION && layer==1 && Pe->c==0) DrawReflection(Pe);
			else if (Pe->type==ENT_REFLECTION && layer==2 && Pe->c==1) DrawReflection(Pe);
			else if (Pe->type==ENT_CHECKPOINT && layer==1) DrawCheckPoint(Pe);

			else if (Pe->type==ENT_ROTATINGPROP && Pe->f==0 && layer==0) DrawRotatingProp(Pe);
			else if (Pe->type==ENT_ROTATINGPROP && Pe->f==1 && layer==1) DrawRotatingProp(Pe);
			else if (Pe->type==ENT_ROTATINGPROP && Pe->f==2 && layer==2) DrawRotatingProp(Pe);
			else if (Pe->type==ENT_ROTATINGPROP && Pe->f==4 && layer==4) DrawRotatingProp(Pe);
			
			

			if (pcore->gfx.InputKeyDown(DIK_C))
			{
				sprintf(string,"(unknown entity)");
				if (Pe->type==1) sprintf(string,"(spawn)");
				else if (Pe->type==2) sprintf(string,"(physics)");
				else if (Pe->type==3) sprintf(string,"(zomb)");
				else if (Pe->type==4) sprintf(string,"(prop)"); // props draw themselves
				else if (Pe->type==5) sprintf(string,"(consolecom)");
				else if (Pe->type==6) sprintf(string,"(sldrtrrt)");
				else if (Pe->type==7) sprintf(string,"(ladder)");
				else if (Pe->type==8) sprintf(string,"(pickup)");
				else if (Pe->type==9) sprintf(string,"(trigger)");
				else if (Pe->type==10) sprintf(string,"(mover)");
				else if (Pe->type==ENT_SOUND) sprintf(string,"(sound)");
				else if (Pe->type==ENT_PUSH) sprintf(string,"(push)");
				else if (Pe->type==ENT_E_RAT) sprintf(string,"(rat)");
				else if (Pe->type==ENT_LEVELEND) sprintf(string,"(levelend)");
				else if (Pe->type==ENT_E_BONESY) sprintf(string,"(bonesy)");
				
				pcore->gfx.DrawTextEx((Pe->x-pcore->landscape.offset_x+5)*pcore->landscape.m_zoom,(Pe->y-pcore->landscape.offset_y)*pcore->landscape.m_zoom,200,200,255,"%s",string);
				pcore->gfx.DrawTextEx((Pe->x-pcore->landscape.offset_x-3)*pcore->landscape.m_zoom,(Pe->y-pcore->landscape.offset_y-3)*pcore->landscape.m_zoom,255,255,0,"X");
				pcore->landscape.DrawRect(&Pe->rect);
			}
		}
		Pe = Pe->Next;
	}

	
}

void Entitys::RemoveAll()
{
	LPEntity Pe=FirstEnt, del;

	while (Pe!=NULL)
	{
		del = Pe;
		Pe = Pe->Next;
		delete del;
	}

	FirstEnt=NULL;
}

void Entitys::StartLevelSpawners(void)
{
	pcore->gfx.tick = 0.0f;
	LPEntity Pe=FirstEnt;

	while (Pe!=NULL)
	{
		pcore->enemies.AddEnemy(Pe->type,Pe->x,Pe->y, Pe->Width, Pe->Height, Pe->String1, "");
		

		if (Pe->type==ENT_WEATHER)
		{
			if (Pe->Width==0) pcore->landscape.enumWeatherMode = WEATHER_WINDYRAIN;
		}
		Pe = Pe->Next;
	}
}

bool Entitys::PlayerTrigger(CPlayers* player, GRECT* playerrect)
{
	LPEntity Pe=FirstEnt, Next=NULL;
	bool rt=false;

	while (Pe!=NULL)
	{
		Next = Pe->Next;
		if (Pe->type==ENTITY_TRIGGER)
		{
			Pe->timer = Pe->timer - (pcore->gfx.tick);
			if (Pe->timer<0)
			if (IntersectGRect(playerrect,&Pe->rect))
			{
				pcore->gui.WriteToConsole(1,0,0,0,"[%s] Triggered [%s]",Pe->name, Pe->trigger);
				pcore->Trigger(Pe->trigger);
                Pe->timer = (Pe->c*1000.0f);
				if (Pe->c==0) Pe->timer = 9999999999;
				rt= true;
				return true;
			}
		}
		else
		if (Pe->type==ENT_PUSH)
		{
			Pe->timer = Pe->timer - (pcore->gfx.tick);
		//	pcore->gui.WriteToConsole(10,0,0,0,"timer: %f",Pe->timer);

			if (Pe->timer<0)
			if (IntersectGRect(playerrect,&Pe->rect))
			{
				player->m_fXVel = player->m_fXVel + Pe->c;
				player->m_fYVel = player->m_fYVel + Pe->d;
				if (Pe->d!=0) player->m_iState=PLAYERSTATE_FALLING;
				Pe->timer = (50.0f);
				pcore->Trigger(Pe->trigger);
				return true;
			}
		}
		else
		if (Pe->type==ENT_CHECKPOINT)
		{
			if (IntersectGRect(playerrect,&Pe->rect))
			{
				DoCheckPoint(Pe);
				this->RemoveEnt(Pe);
				return true;
			}
		}
		else
		if (Pe->type==ENT_SECRETAREA)
		{
			if (IntersectGRect(playerrect,&Pe->rect))
			{
				pcore->sound.PlaySnd(s_hiddenarea,0,0,0.0f,-5.0);
				pcore->LevelStats.HiddenAreas++;
				this->RemoveEnt(Pe);
				return true;
			}
		}

			Pe = Next;
	}

	return rt;
}

bool Entitys::EntityHere(unsigned int entity, float x, float y)
{
	LPEntity Pe=FirstEnt;

	while (Pe!=NULL)
	{
		if (Pe->type==entity)
		{
			if (x>=Pe->x)
			if (y>=Pe->y)
			if (x<=Pe->x+Pe->Width)
			if (y<=Pe->y+Pe->Height)
			{
				return true;
			}
		}
			Pe = Pe->Next;
	}

	return false;
}

bool Entitys::Trigger(char* name)
{
	if (strcmp(name,"")==0) return false;
	
	LPEntity Pe=FirstEnt;
	bool returnvalue = false;

	while (Pe!=NULL)
	{
		if (strcmp(Pe->name, name)==0)
		{
			if (Pe->type==ENT_PARTICLESPAWNER)
			{
				if (Pe->inuse) Pe->inuse=false; else Pe->inuse=true;
			}
			if (Pe->type==ENTITY_COMMANDCONSOLE)
			{
				pcore->gui.RunConsoleCommand(Pe->String1);
				returnvalue = true;
				return true;
			}
			if (Pe->type==ENT_SOUND)
			{
				this->doEnt_Sound(Pe);
				returnvalue = true;
				return true;
			}
			if (Pe->type==ENT_LEVELEND)
			{
				pcore->StartLevelEnd(Pe->String1);
				returnvalue = true;
				return true;
			}
		}

		Pe = Pe->Next;
	}
	return returnvalue;
}
void Entitys::doEnt_Sound(LPEntity p)
{
	if (p==NULL) return;
	pcore->gui.WriteToConsole(1,0,0,255,"Playing Sound [%s]",p->String1);

	char soundfilename[200];

	sprintf(soundfilename,"sound/%s",p->String1);

	sSound* s;

	s = pcore->sound.LoadSound(6,soundfilename);
	pcore->sound.PlaySnd(s,p->x,p->y,0,p->d);


}

void Entitys::DoEntityParticleSpawner(LPEntity Pe)
{
	if (Pe->type!=ENT_PARTICLESPAWNER) return;


	Pe->f = Pe->f - pcore->gfx.tick;
	if (Pe->f>0) return;
	
	// add delay
	Pe->f = Pe->d;
	if ((int)Pe->e!=0)
		Pe->f = Pe->f + (rand()%(int)Pe->e);

	for (int i=0;i<Pe->g;i++)
	pcore->landscape.AddParticle((int)Pe->c, Pe->x+(rand()%(int)Pe->Width), Pe->y+(rand()%(int)Pe->Height));
}

void Entitys::DoTriggerOnNoEnemies(LPEntity Pe)
{
	if (Pe->type!=ENT_TRIGGER_ONNOENEMIES) return;

	if (pcore->enemies.NumEnemies==0)
	{
		Trigger(Pe->trigger);
		pcore->props.Trigger(Pe->trigger);
		Pe->inuse=false;
	}

	pcore->gui.WriteToConsole(10,255,0,0,"enemies left: %i", pcore->enemies.NumEnemies);
}

void Entitys::DoEntitys(void)
{
	LPEntity Pe=FirstEnt;

	while (Pe!=NULL)
	{
		if (Pe->inuse)
		{
			if (Pe->type==ENT_PARTICLESPAWNER)
			{
				DoEntityParticleSpawner(Pe);
			}
			else if (Pe->type==ENT_TRIGGER_ONNOENEMIES)
			{
				DoTriggerOnNoEnemies(Pe);
			}
		}

		Pe = Pe->Next;
	}
}


#define FLARESCALE pcore->Settings.fFlareScale

void Entitys::DrawLensFlare(LPEntity lp)
{
	if (FLARESCALE<=0) return;

	if (lp->x-SCROFFX<0 ||
		lp->y-SCROFFY<0 ||
		lp->x-SCROFFX>(pcore->gfx.m_d3dpp.BackBufferWidth/MZOOM) ||
		lp->y-SCROFFY>(pcore->gfx.m_d3dpp.BackBufferHeight/MZOOM))
	{
		if (lp->e>0) lp->e = lp->e - 0.5f*pcore->gfx.tick;
	}
	else 
	{
		if (lp->e<255) lp->e = lp->e + 1.0f*pcore->gfx.tick;
	}
	
	if (lp->c==0)
	{
		if (pcore->landscape.CollisionType((lp->x), (lp->y), NULL,true)>0)
		if (lp->e>0) lp->e = lp->e - 10.0f*pcore->gfx.tick;
	}

	if (lp->e>255) lp->e = 255;
	if (lp->e<0) lp->e = 0;
	if (lp->e==0) return;


	float alpha=255;

	if (lp->h==1) alpha = rand()%255;
	if (lp->h==2) alpha = 150+rand()%105;
	if (lp->h==3) alpha = 200+rand()%55;
	if (lp->h==4) alpha = ((1+sin(pcore->TimerA/1000.0f))/2.0f)*255;
	if (lp->h==5) alpha = 150+(((1+sin(pcore->TimerA/1000.0f))/2.0f)*105);
	if (lp->h==6) alpha = 200+(((1+sin(pcore->TimerA/1000.0f))/2.0f)*55);
	if (lp->h==7) alpha = ((1+sin(pcore->TimerA/500.0f))/2.0f)*255;
	if (lp->h==8) alpha = 150+(((1+sin(pcore->TimerA/500.0f))/2.0f)*105);
	if (lp->h==9) alpha = 200+(((1+sin(pcore->TimerA/500.0f))/2.0f)*55);
	if (lp->h==10) alpha = ((1+sin(pcore->TimerA/100.0f))/2.0f)*255;
	if (lp->h==11) alpha = 150+(((1+sin(pcore->TimerA/100.0f))/2.0f)*105);
	if (lp->h==12) alpha = 200+(((1+sin(pcore->TimerA/100.0f))/2.0f)*55);
	if (lp->h==13) alpha = ((1+sin(pcore->TimerA/10.0f))/2.0f)*255;
	if (lp->h==14) alpha = 150+(((1+sin(pcore->TimerA/10.0f))/2.0f)*105);
	if (lp->h==15) alpha = 200+(((1+sin(pcore->TimerA/10.0f))/2.0f)*55);

	alpha = alpha/255.0f;

	pcore->SetAlphaMode(true);
	pcore->gfx.SpriteDrawFull(lp->texture,
							(lp->x-SCROFFX-((lp->Width/2)*lp->d*FLARESCALE))*MZOOM,
							(lp->y-SCROFFY-((lp->Height/2)*lp->d*FLARESCALE))*MZOOM,
							((lp->x-SCROFFX)+(lp->y-SCROFFY))*0.001f,
							(lp->Width/2)*lp->d*FLARESCALE*MZOOM,
							(lp->Height/2)*lp->d*FLARESCALE*MZOOM,
							lp->d*MZOOM*FLARESCALE,
							lp->d*MZOOM*FLARESCALE,0,0,0,0,lp->e*alpha,255,255,255);

}

void Entitys::ProcessEntitys(void)
{
	LPEntity Pe=FirstEnt;
	char string[200];

	while (Pe!=NULL)
	{
		if (Pe->type==ENT_LENSFLARE || Pe->type==ENT_ROTATINGPROP)
		{
			sprintf(string,"textures/props/%s",Pe->String1);
			Pe->texture = pcore->textures.LoadTexture(string);
			if (Pe->type==ENT_LENSFLARE) Pe->e = 255;
		}
		else if (Pe->type==ENT_PLUTONIUM)
		{
			sprintf(string,"textures/props/plutonium.bmp");
			Pe->texture = pcore->textures.LoadTexture(string);
		}
		else if (Pe->type==ENT_PROP || Pe->type==ENT_PARALLAXPROP)
		{
			if (strcmp(Pe->String1,"")!=0)
			{
				sprintf(string,"textures/props/%s",Pe->String1);
				Pe->texture = pcore->textures.LoadTexture(string);
			}
			else
			{
				Pe->texture = NULL;
			}
		}
		else if (Pe->type==ENT_REFLECTION)
		{
			sprintf(string,"textures/props/%s",Pe->String1);
			Pe->texture = pcore->textures.LoadTexture(string);
		}

		Pe = Pe->Next;
	}
}

void Entitys::DrawPlutonium(LPEntity lp)
{
	pcore->SetAlphaMode(true);
	pcore->gfx.SpriteDrawEx(lp->texture,
					(lp->x-SCROFFX-(16))*MZOOM,
					(lp->y-SCROFFY-(16))*MZOOM,
					(lp->c+lp->x+lp->y)/100.0f,
					(16)*MZOOM,
					(16)*MZOOM,
					MZOOM,MZOOM,
					0,0,0,0,
					100+(rand()%150),255,255,255);
	lp->c += pcore->gfx.tick;
}

void Entitys::DrawProp(LPEntity lp)
{
	float alpha=255;

	if (lp->h==1) alpha = rand()%255;
	if (lp->h==2) alpha = 150+rand()%105;
	if (lp->h==3) alpha = 200+rand()%55;
	if (lp->h==4) alpha = ((1+sin(pcore->TimerA/1000.0f))/2.0f)*255;
	if (lp->h==5) alpha = 150+(((1+sin(pcore->TimerA/1000.0f))/2.0f)*105);
	if (lp->h==6) alpha = 200+(((1+sin(pcore->TimerA/1000.0f))/2.0f)*55);
	if (lp->h==7) alpha = ((1+sin(pcore->TimerA/500.0f))/2.0f)*255;
	if (lp->h==8) alpha = 150+(((1+sin(pcore->TimerA/500.0f))/2.0f)*105);
	if (lp->h==9) alpha = 200+(((1+sin(pcore->TimerA/500.0f))/2.0f)*55);
	if (lp->h==10) alpha = ((1+sin(pcore->TimerA/100.0f))/2.0f)*255;
	if (lp->h==11) alpha = 150+(((1+sin(pcore->TimerA/100.0f))/2.0f)*105);
	if (lp->h==12) alpha = 200+(((1+sin(pcore->TimerA/100.0f))/2.0f)*55);
	if (lp->h==13) alpha = ((1+sin(pcore->TimerA/10.0f))/2.0f)*255;
	if (lp->h==14) alpha = 150+(((1+sin(pcore->TimerA/10.0f))/2.0f)*105);
	if (lp->h==15) alpha = 200+(((1+sin(pcore->TimerA/10.0f))/2.0f)*55);

	if (lp->g==1 && lp->Width!=4) pcore->SetAlphaMode(true);
	else pcore->SetAlphaMode(false);
	pcore->gfx.SpriteDrawEx(lp->texture,
					(lp->x-SCROFFX)*MZOOM,
					(lp->y-SCROFFY)*MZOOM,
					0,0,0,
					MZOOM,MZOOM,
					0,0,0,0,
					alpha,255,255,255);
}

LPEntity Entitys::CollidePickup(GRECT r1)
{
	LPEntity p=FirstEnt;
	bool hit;

	while (p!=NULL)
	{
		if (p->type==ENT_PLUTONIUM)
		{
			hit = true;
			if (r1.bottom < p->y-16 || p->y+16 < r1.top) hit = false;    
			if (r1.right < p->x-16 || p->x+16 < r1.left) hit = false;

			if (hit)
			{
				LPParticle prt;
				for (int i=0;i<5;i++)
				{
					prt = pcore->landscape.AddParticle(PARTICLE_PLUTPICKUP,(p->x),(p->y));
					if (prt!=NULL)
					{
						prt->xvel = (prt->xvel + pcore->players[pcore->MyPlayerID].player.m_fXVel)*(0.2+((pcore->players[pcore->MyPlayerID].player.m_plutonium%100)*0.01));
						prt->yvel = (prt->yvel + pcore->players[pcore->MyPlayerID].player.m_fYVel)*(0.2+((pcore->players[pcore->MyPlayerID].player.m_plutonium%100)*0.01));
					}
				}
				return p;
			}
			
		}
	
		p = p->Next;
	} 

	return NULL;
}

void Entitys::RemoveEnt(LPEntity delme)
{
	LPEntity p=FirstEnt;

	if (delme==FirstEnt)
	{
		FirstEnt = delme->Next;
		delete delme;
	}
	else
	{
		Previous(delme)->Next = delme->Next;
		delete delme;
	}
}

LPEntity Entitys::Previous(LPEntity index)
{
	LPEntity temp=FirstEnt;
	if(index==FirstEnt) return FirstEnt;
	while(temp->Next != index)
		temp=temp->Next;
	return temp;
}

void Entitys::DrawReflection(LPEntity lp)
{
	pcore->SetAlphaMode(true);

	if (lp->h==0) lp->h = 255;

	float w, y;

	if (lp->e==0)
	{
		w = 8.0f;
	}
	else
	{
		w = lp->e;
	}

	y = w;

/*
	pcore->gfx.SpriteDrawFull(lp->texture,
							((lp->x-SCROFFX)*MZOOM)-((fmod(lp->x-SCROFFX/2.0f,(256-64)))*((lp->Width/w)*MZOOM)),
							((lp->y-SCROFFY)*MZOOM)-((fmod(lp->y-SCROFFY/2.0f,(256-64)))*((lp->Height/y)*MZOOM)),
							0,0,0,
							(lp->Width/w)*MZOOM,
							(lp->Height/y)*MZOOM,
							0,0,0,0,
							255,255,255,255);
*/

	pcore->gfx.SpriteDrawFull(lp->texture,
							(lp->x-SCROFFX)*MZOOM,
							(lp->y-SCROFFY)*MZOOM,
							0,0,0,
							(lp->Width/w)*MZOOM,
							(lp->Height/y)*MZOOM,
							fmod(((lp->x+lp->y)*16.0f)-SCROFFX/10.0f,(256-64)),
							fmod(((lp->y+lp->y)*16.0f)-SCROFFY/10.0f,(256-64)),
							w,y,
							lp->h,255,255,255);

}
void Entitys::DrawSound(LPEntity p)
{
	bool draw=false;

	if (p->x+1024 > SCROFFX && 
		p->y+1024 > SCROFFY &&
		p->y-1024 < SCROFFY+(pcore->gfx.m_d3dpp.BackBufferHeight/MZOOM) && 
		p->x-1024 < SCROFFX+(pcore->gfx.m_d3dpp.BackBufferWidth/MZOOM))
		draw=true;

	if (p->soundchannel==0)
	{
		if (draw)
		{
			char sname[200];
			sprintf(sname, "sound/%s",p->String1);
			p->soundchannel = pcore->sound.PlayFromFilename(sname,p->x,p->y,p->d);
		}
	}
	else
	{
		if (!pcore->sound.IsPlaying(p->soundchannel))
		{
			if (draw)
			{
				char sname[200];
				sprintf(sname, "sound/%s",p->String1);
				p->soundchannel = pcore->sound.PlayFromFilename(sname,p->x,p->y,p->d);			
			}
			else
			{
				p->soundchannel = 0;
			}
		}
		else
		{
			pcore->sound.UpdateSound(p->soundchannel,p->x,p->y);
		}
	}
}

void Entitys::DrawCheckPoint(LPEntity lp)
{
	pcore->gfx.SpriteDraw(checkpoint,
							((lp->x-pcore->landscape.offset_x)*pcore->landscape.m_zoom)-(32.0f*pcore->landscape.m_zoom),
							((lp->y-pcore->landscape.offset_y)*pcore->landscape.m_zoom)-(32.0f*pcore->landscape.m_zoom),
							sin(pcore->TimerA*0.02f)*0.2,
							32.0f*pcore->landscape.m_zoom,32.0f*pcore->landscape.m_zoom,
							pcore->landscape.m_zoom,pcore->landscape.m_zoom);

}

void Entitys::DoCheckPoint(LPEntity lp)
{
	pcore->sound.PlaySnd(s_checkpoint,0,0,0.0f,-5.0);

	pcore->landscape.ScreenFade.color = 2;
	pcore->landscape.ScreenFade.alpha = 200;
	pcore->landscape.ScreenFade.speed = -0.3;
	pcore->landscape.ScreenFade.active = true;

	LPParticle p;
	for (int i=0;i<30;i++)
	{
		p = pcore->landscape.AddParticle(PARTICLE_CHECKPOINTBLUE,lp->x+(rand()%32-16),lp->y+(rand()%32-16));
		if (p!=NULL)
		{
			//p->yvel = fabs(p->yvel)*-1.2f;
		}
	}
	pcore->landscape.spawnx = lp->x;
	pcore->landscape.spawny = lp->y;
}


void Entitys::DrawRotatingProp(LPEntity lp)
{
	float alpha=255;

	if (lp->h==1) alpha = rand()%255;
	if (lp->h==2) alpha = 150+rand()%105;
	if (lp->h==3) alpha = 200+rand()%55;
	if (lp->h==4) alpha = ((1+sin(pcore->TimerA/1000.0f))/2.0f)*255;
	if (lp->h==5) alpha = 150+(((1+sin(pcore->TimerA/1000.0f))/2.0f)*105);
	if (lp->h==6) alpha = 200+(((1+sin(pcore->TimerA/1000.0f))/2.0f)*55);
	if (lp->h==7) alpha = ((1+sin(pcore->TimerA/500.0f))/2.0f)*255;
	if (lp->h==8) alpha = 150+(((1+sin(pcore->TimerA/500.0f))/2.0f)*105);
	if (lp->h==9) alpha = 200+(((1+sin(pcore->TimerA/500.0f))/2.0f)*55);
	if (lp->h==10) alpha = ((1+sin(pcore->TimerA/100.0f))/2.0f)*255;
	if (lp->h==11) alpha = 150+(((1+sin(pcore->TimerA/100.0f))/2.0f)*105);
	if (lp->h==12) alpha = 200+(((1+sin(pcore->TimerA/100.0f))/2.0f)*55);
	if (lp->h==13) alpha = ((1+sin(pcore->TimerA/10.0f))/2.0f)*255;
	if (lp->h==14) alpha = 150+(((1+sin(pcore->TimerA/10.0f))/2.0f)*105);
	if (lp->h==15) alpha = 200+(((1+sin(pcore->TimerA/10.0f))/2.0f)*55);

	if (lp->g==0 && lp->Width!=4) pcore->SetAlphaMode(true);
	else  pcore->SetAlphaMode(false);
	pcore->gfx.SpriteDrawFull(lp->texture,
							(lp->x-SCROFFX-((lp->Width/2)*lp->d))*MZOOM,
							(lp->y-SCROFFY-((lp->Height/2)*lp->d))*MZOOM,
							(pcore->TimerA*lp->e)*0.001f,
							(lp->Width/2)*lp->d*FLARESCALE*MZOOM,
							(lp->Height/2)*lp->d*FLARESCALE*MZOOM,
							lp->d*MZOOM,
							lp->d*MZOOM,0,0,0,0,alpha,255,255,255);

}

#define BBW this->pcore->gfx.m_d3dpp.BackBufferWidth
#define BBH this->pcore->gfx.m_d3dpp.BackBufferHeight

void Entitys::DrawParallaxProp(LPEntity lp)
{
	float alpha=255;

	if (lp->h==1) alpha = rand()%255;
	if (lp->h==2) alpha = 150+rand()%105;
	if (lp->h==3) alpha = 200+rand()%55;
	if (lp->h==4) alpha = ((1+sin(pcore->TimerA/1000.0f))/2.0f)*255;
	if (lp->h==5) alpha = 150+(((1+sin(pcore->TimerA/1000.0f))/2.0f)*105);
	if (lp->h==6) alpha = 200+(((1+sin(pcore->TimerA/1000.0f))/2.0f)*55);
	if (lp->h==7) alpha = ((1+sin(pcore->TimerA/500.0f))/2.0f)*255;
	if (lp->h==8) alpha = 150+(((1+sin(pcore->TimerA/500.0f))/2.0f)*105);
	if (lp->h==9) alpha = 200+(((1+sin(pcore->TimerA/500.0f))/2.0f)*55);
	if (lp->h==10) alpha = ((1+sin(pcore->TimerA/100.0f))/2.0f)*255;
	if (lp->h==11) alpha = 150+(((1+sin(pcore->TimerA/100.0f))/2.0f)*105);
	if (lp->h==12) alpha = 200+(((1+sin(pcore->TimerA/100.0f))/2.0f)*55);
	if (lp->h==13) alpha = ((1+sin(pcore->TimerA/10.0f))/2.0f)*255;
	if (lp->h==14) alpha = 150+(((1+sin(pcore->TimerA/10.0f))/2.0f)*105);
	if (lp->h==15) alpha = 200+(((1+sin(pcore->TimerA/10.0f))/2.0f)*55);

	if (lp->g==1 && lp->Width!=4) pcore->SetAlphaMode(true);
	else pcore->SetAlphaMode(false);
	pcore->gfx.SpriteDrawEx(lp->texture,
					
		
	((BBW/2.0f)-(((BBW/2.0f)-((lp->x-SCROFFX)*MZOOM))*lp->c)), // get distance from the middle
	((BBH/2.0f)-(((BBH/2.0f)-((lp->y-SCROFFY)*MZOOM))*lp->d)),
							
							
			//		(lp->x-SCROFFX)*MZOOM*lp->c,
			//		(lp->y-SCROFFY)*MZOOM*lp->d,
					0,0,0,
					(lp->f==0?1:lp->f)*MZOOM,
					(lp->f==0?1:lp->f)*MZOOM,

					0,0,0,0,
					alpha,255,255,255);
}

