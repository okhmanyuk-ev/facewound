#include "landscape.h"
#include "weapons.h"
#include "Core.h"

#define BLEND_NORMAL 0
#define BLEND_ALPHA 1

void CLandscape::DrawParticles(int layer) // 0 = normal 1 = overall
{
	if (m_ParticleCount<=0) return;

	LPParticle pointer=m_pFirstParticle;
	LPParticle lastpointer=NULL;
	LPParticle nextpointer=NULL;

	float i=0;
	float tst=0;
	bool remove=false;
	bool skipgenericparticlecrap=false;
	bool draw=true;

	int index=0;
	m_ParticleDrawnCount=0;
	while (pointer!=NULL)
	{
		nextpointer = pointer->Next;

		if ((this->ParticleTypes[pointer->type].layer==layer))// &&
			//pointer->type!=PARTICLE_BLOODDECAL))
		{
			draw=true;

			if (!this->ParticleTypes[pointer->type].gui)
			{
				if (pointer->x+256<offset_x) draw=false;
				if (pointer->x-256>offset_x+(pcore->gfx.m_d3dpp.BackBufferWidth/pcore->landscape.m_zoom)) draw=false;
				if (pointer->y+256<offset_y) draw=false;
				if (pointer->y-256>offset_y+(pcore->gfx.m_d3dpp.BackBufferHeight/pcore->landscape.m_zoom)) draw=false;
			}
			else
			{
				//if (layer==2 || layer==10) draw=true;
				//else		  draw=false;
			}
			
			remove=false;
			skipgenericparticlecrap=false;

			if (pointer->type==PARTICLE_ZOMBIEATTACK) draw=false;

			if (pointer->time<0) draw=false;

			if ((pointer->time + (0.01*pcore->gfx.tick)) < 0)
			{
				pointer->time = pointer->time + (0.01*pcore->gfx.tick);
			}
			else if (this->ParticleTypes[pointer->type].gui)
			{
				DoGuiParticle(pointer, &remove, draw);
			}
			else if (pointer->type== PARTICLE_BLOODBLOBS)
			{
				DoBloodBlobsParticle(pointer, &remove, draw);
			}
			else if (pointer->type==PARTICLE_MOLOTOV ||
					 pointer->type==PARTICLE_BULLETBONE || 
					 pointer->type==PARTICLE_FIRE || 
					 pointer->type==PARTICLE_BULLET || 
					 pointer->type==PARTICLE_BUCKSHOT || 
					 pointer->type==PARTICLE_ARROW || 
					 pointer->type==PARTICLE_IMPACTGRENADE || 
					 pointer->type==PARTICLE_RPG ||
					 pointer->type==PARTICLE_STATICBULLET ||
					 pointer->type==PARTICLE_EYELASERS ||
					 pointer->type==PARTICLE_SICKBULLET)
			{
				DoBulletParticle(pointer, &remove, draw);
			}
			else if (pointer->type==PARTICLE_ZOMBIEATTACK || pointer->type==PARTICLE_FIREBITRISER)
			{
				DoDamageParticle(pointer, &remove);
				if (pointer->type!=PARTICLE_FIREBITRISER) skipgenericparticlecrap = true;
			}
			else if (pointer->AttachedTo!=ATTACHED_TO_NOTHING)
			{
				DoAttachedParticle(pointer, &remove, draw);
				skipgenericparticlecrap = true;
			}
			else
			{
				DoGenericParticle(pointer, &remove, draw);
			}

			if (draw)
			{
				m_ParticleDrawnCount++;
				
				if (pointer->type==PARTICLE_BULLET ||
					pointer->type==PARTICLE_BUCKSHOT||
					pointer->type==PARTICLE_ARROW ||
					pointer->type==PARTICLE_WEATHER_RAIN ||
					pointer->type==PARTICLE_RAINSPLASH ||
					pointer->type==PARTICLE_RICHOCHETSPARK ||
					pointer->type==PARTICLE_RPG ||
					pointer->type==PARTICLE_GUI_SPARKS ||
					pointer->type==PARTICLE_EYELASERS ||
					pointer->type==PARTICLE_DRIP)
				{
					pointer->rot = atan2(pointer->xvel, pointer->yvel)+(D3DX_PI/2);
				}

			
			
			if (!remove && pointer->alpha>0 && !pcore->bProcessingLevelStart)
			{
				if (pointer->type==PARTICLE_EXPLOSIONORANGE || 
					pointer->type==PARTICLE_GUNSPARK ||
					pointer->type==PARTICLE_MUZZLEFLASH ||
					pointer->type==PARTICLE_RICHOCHETSPARK ||
					pointer->type==PARTICLE_EXPLOSIONRING ||
					pointer->type==PARTICLE_BULLET ||
					pointer->type==PARTICLE_BUCKSHOT||
					pointer->type==PARTICLE_LANDMINEGREEN||
					pointer->type==PARTICLE_LANDMINERED ||
					pointer->type==PARTICLE_FIRE ||
					pointer->type==PARTICLE_FIREBIT ||
					pointer->type==PARTICLE_FIREBITRISER ||
					pointer->type==PARTICLE_FIREBITATTACHED ||
					pointer->type==PARTICLE_FIREBITRISERATTACHED ||
					pointer->type==PARTICLE_GUI_BOUGHT ||
					pointer->type==PARTICLE_PLUTPICKUP ||
					pointer->type==PARTICLE_GUI_SPARKS ||
					pointer->type==PARTICLE_GUI_PLUTCOUNT ||
					pointer->type==PARTICLE_SPAWNLIGHT ||
					pointer->type==PARTICLE_MOLOTOV ||
					pointer->type==PARTICLE_MOLOTOVTAIL ||
					pointer->type==PARTICLE_PLAYERLINE ||
					pointer->type==PARTICLE_FIRE2 ||
					pointer->type==PARTICLE_EYELASERLIGHTS || 
					pointer->type==PARTICLE_LTNG_BEAM || 
					pointer->type==PARTICLE_EYELASERS ||
					pointer->type==PARTICLE_EYELASERHIT ||
					pointer->type==PARTICLE_SICKBULLET ||
					pointer->type==PARTICLE_LTNG_GLOW)
				{
					pcore->SetAlphaMode(true);
				}
				else
				{
					pcore->SetAlphaMode(false);
				}

				if (this->ParticleTypes[pointer->type].gui)
					pcore->gfx.SpriteDrawFull(pointer->textureoverride!=NULL?pointer->textureoverride:ParticleTypes[pointer->type].particletexture,
					(pointer->x)-(ParticleTypes[pointer->type].offsetx*pointer->xscale),
					(pointer->y)-(ParticleTypes[pointer->type].offsety*pointer->yscale),
					pointer->rot,ParticleTypes[pointer->type].offsetx*pointer->xscale,ParticleTypes[pointer->type].offsety*pointer->yscale,
					pointer->xscale,pointer->yscale,
					0,0,0,0,
					pointer->alpha,255,255,255);
				else
					pcore->gfx.SpriteDrawFull(pointer->textureoverride!=NULL?pointer->textureoverride:ParticleTypes[pointer->type].particletexture,
					(pointer->x-this->offset_x)*m_zoom-(ParticleTypes[pointer->type].offsetx*pointer->xscale*m_zoom),
					(pointer->y-this->offset_y)*m_zoom-(ParticleTypes[pointer->type].offsety*pointer->yscale*m_zoom),
					pointer->rot,ParticleTypes[pointer->type].offsetx*pointer->xscale*m_zoom,ParticleTypes[pointer->type].offsety*pointer->yscale*m_zoom,
					pointer->xscale*m_zoom,pointer->yscale*m_zoom,
					0,0,0,0,
					(int)pointer->alpha,(int)255,(int)255,(int)255);

				if (pointer->type==PARTICLE_RICHOCHETSPARK ||
					pointer->type==PARTICLE_EYELASERS ||
					pointer->type==PARTICLE_LTNG_BEAM )
				{
					pcore->gfx.SpriteDrawFull(pointer->textureoverride!=NULL?pointer->textureoverride:ParticleTypes[pointer->type].particletexture,
					(pointer->x-this->offset_x)*m_zoom-(ParticleTypes[pointer->type].offsetx*pointer->xscale*m_zoom),
					(pointer->y-this->offset_y)*m_zoom-(ParticleTypes[pointer->type].offsety*pointer->yscale*m_zoom),
					pointer->rot,ParticleTypes[pointer->type].offsetx*pointer->xscale*m_zoom,ParticleTypes[pointer->type].offsety*pointer->yscale*m_zoom,
					pointer->xscale*m_zoom,pointer->yscale*m_zoom,
					0,0,0,0,
					(int)pointer->alpha,(int)255,(int)255,(int)255);
				}

			}
		
			}
		

			if (!skipgenericparticlecrap && !remove && pointer->time>=0)
			{
				if (ParticleTypes[pointer->type].gui)
				{
					pointer->x = pointer->x + (pointer->xvel*pcore->gfx.RealTick);
					pointer->y = pointer->y + (pointer->yvel*pcore->gfx.RealTick);
					pointer->rot = pointer->rot + (pointer->rotvel*pcore->gfx.RealTick);
				}
				else //if (pointer->type!=PARTICLE_STATICBULLET)
				{
					pointer->x = pointer->x + (pointer->xvel*pcore->gfx.tick);
					pointer->y = pointer->y + (pointer->yvel*pcore->gfx.tick);
					pointer->rot = pointer->rot + (pointer->rotvel*pcore->gfx.tick);
				}


				if (pointer->alpha<0) pointer->alpha=0;
		
			}

		

			// removes dead paricles
			if ((pointer->time>=ParticleTypes[pointer->type].life) ||
 				(pointer->x<0) ||
				(pointer->y<0) ||
  				(pointer->x>this->width*32) ||
				(pointer->y>=(this->height*32)-1) ||
				pointer->alpha==0 ||
				pointer->y>(height*32) ||
				remove) 
			{

				// things that explode when they blow up
				ParticleRemoveExplosive(pointer);

				// FIRE BOMB EXPLOSION
				if (pointer->type==PARTICLE_MOLOTOV && !pcore->bProcessingLevelStart)
				{
					if (pointer->y < pcore->landscape.WaterLine || pcore->landscape.WaterLine==0)
					{
						pcore->sound.PlaySnd(s_molotov, pointer->x, pointer->y, 0.1f);
						LPParticle tpart=NULL;
						for (int i=0;i<20;i++)
						{
							tpart = AddParticle(PARTICLE_FIREBIT,pointer->x,pointer->y);
							if (tpart!=NULL)
							{
								tpart->yvel = ((rand()%2000)-1000)/1000.0f;
								tpart->xvel = ((rand()%2000)-1000)/2000.0f;
								tpart->SpawnParticle = PARTICLE_FIREBITRISER;
								tpart->SpawnTimer = 1;
								tpart->SpawnTimerReset = 5;
								tpart->damage = FIRE_DAMAGE;
								tpart->pplayer = pointer->pplayer;
							}
						}
						MakeExplosion(pointer->x, pointer->y+10, 300,50);
					}
				}

				RemoveParticle(pointer);
				pointer=NULL;
			}// if remove
	

		} // this layer

		pointer=nextpointer;
		lastpointer = pointer;
		index++;
	}
	pcore->SetAlphaMode(false);

}
void CLandscape::DoBloodBlobsParticle(LPParticle pointer, bool* remove,bool draw)
{
	pointer->time = pointer->time + (0.01*pcore->gfx.tick);
	if (pointer->time<0) return;

	pointer->alpha = pointer->alpha + (pointer->alphavel*pcore->gfx.tick);

	LPParticle lp=NULL;


	//gravity
	if (pointer->IsMoving && ParticleTypes[pointer->type].gravity!=0) pointer->yvel = pointer->yvel + (ParticleTypes[pointer->type].gravity*pcore->gfx.tick*pcore->gravity);

	if (pointer->yvel==0 && pointer->xvel==0) pointer->IsMoving=false;
	//collision
	if (pointer->IsMoving)
	{
		// HIT X
		if (Collision(pointer->x + (pointer->xvel*pcore->gfx.tick), pointer->y))
		{
			*remove = true;
			float bloodangle=0;

			if (Collision((pointer->x + (pointer->xvel*pcore->gfx.tick)) - 5.0f, pointer->y))
			{
				bloodangle = D3DX_PI;
			}
			if (Collision((pointer->x + (pointer->xvel*pcore->gfx.tick)) + 5.0f, pointer->y))
			{
				if (bloodangle == D3DX_PI) bloodangle = -1.0f;
				else bloodangle = D3DX_PI*2;
			}

			if (bloodangle!=-1.0f && (rand()%4==1))
			{

				if (bloodangle==D3DX_PI*2)
				{
					for (int i=0;i<50;i++)
					{
						if (Collision(pointer->x, pointer->y)) break;
						pointer->x = pointer->x+2.0f;
					}
				}
				else
				{
					for (int i=0;i<50;i++)
					{
						if (Collision(pointer->x, pointer->y)) break;
						pointer->x = pointer->x-2.0f;
					}
				}

				lp = this->AddParticle(PARTICLE_BLOODDECAL,pointer->x, 
														   pointer->y, 
														   bloodangle,
														   0,0,t_BloodDrips[rand()%NUM_BLOODDRIPS],0);
				if (lp!=NULL)
				{
					lp->IsMoving=false;
					lp->xscale = 1.0f;
					lp->yscale = 1.0f;
				}
			}
			

			/*
			pointer->xvel=pointer->xvel*0.1;

			// if we're next to wall, stick to it
			if (Collision(pointer->x+(pointer->xvel<0?-2:2),pointer->y))
			{
				pointer->xvel=0;
				pointer->yvel=pointer->yvel/5;
				pointer->rot=0;
				pointer->rotvel=0;
			//	pointer->xscale=0.5;
			//	pointer->yscale=1.5;
			}
			*/
		}

		// hit floor/ceiling
		if (Collision(pointer->x, pointer-> y+ (pointer->yvel*pcore->gfx.tick)))
		{
			// hit floor
			if (pointer->yvel > 0.0f)
			{
				
				//atan2(pointer->xvel, pointer->yvel)-D3DX_PI*0.5
				if (rand()%4==1)
				{
					lp = this->AddParticle(PARTICLE_BLOODDECAL,pointer->x, pointer->y, (-D3DX_PI*0.5),0,0,t_BloodDrips[rand()%NUM_BLOODDRIPS],0);
					if (lp!=NULL)
					{
						lp->IsMoving=true;
					}
				}
				
				*remove = true;
			}

			pointer->yvel = pointer->yvel*0.01;
			pointer->rot=0;
			pointer->rotvel=0;
		//	pointer->xscale=1.5;
		//	pointer->yscale=0.5;
			pointer->xvel = pointer->xvel*(0.04); // friction
		}
		// air resistance
		else if (ParticleTypes[pointer->type].airresistance!=0)
		{
			if (pointer->xvel>0) 
			{
				pointer->xvel = pointer->xvel-(ParticleTypes[pointer->type].airresistance);
				if (pointer->xvel<0) pointer->xvel=0;
			}
			else 
			{
				pointer->xvel = pointer->xvel+(ParticleTypes[pointer->type].airresistance);
				if (pointer->xvel>0) pointer->xvel=0;
			}
		}
		// blood clinging on the wall
		if (pointer->xvel==0 && (Collision(pointer->x-2,pointer->y) || Collision(pointer->x+2,pointer->y)))
		{
			if (pointer->yvel>0.002) pointer->yvel=0.002;
		}
	}
}


void CLandscape::DoGenericParticle(LPParticle pointer, bool* remove, bool draw)
{

	pointer->time = pointer->time + (0.01*pcore->gfx.tick);
	if (pointer->time<0) return;

	if (pointer->type==PARTICLE_WATERRIPPLE)
	{
		pointer->y = this->WaterLine - 2;
		pointer->yvel = this->WaterLineVelocity;
	}

		// alpha
		pointer->alpha = pointer->alpha + (pointer->alphavel*pcore->gfx.tick);
		// scale
		if (pointer->type==PARTICLE_RICHOCHETSPARK)
		{
			pointer->xscale = pointer->yscale+((fabs(pointer->xvel)+fabs(pointer->yvel))*(fabs(pointer->xvel)+fabs(pointer->yvel))*1.9f);
			//pointer->yscale = 0.30;//+(fabs(pointer->xvel)+fabs(pointer->yvel))*0.15f;
			//if (pointer->xscale<0.25f)pointer->xscale=0.25f;
			//if (pointer->xscale<pointer->yscale) pointer->yscale = pointer->xscale;
		}
		else
		{
			if (pointer->type==PARTICLE_BLOODDECAL)
			{
				if (pointer->time<40 && pointer->rot==(-D3DX_PI*0.5))
				pointer->xscale = pointer->xscale + (pointer->xscalevel*pcore->gfx.tick*((40-pointer->time)*0.01f));
			}
			else
			{
				pointer->xscale = pointer->xscale + (pointer->xscalevel*pcore->gfx.tick);
			}

			if (pointer->keepaspect) pointer->yscale = pointer->xscale;
			else pointer->yscale = pointer->yscale + (pointer->yscalevel*pcore->gfx.tick);
		}

		if (pointer->yscale<0 && pointer->xscale<0) *remove=true;
		// gravity
		if (pointer->IsMoving && ParticleTypes[pointer->type].gravity!=0) 
			pointer->yvel = pointer->yvel + (ParticleTypes[pointer->type].gravity*pcore->gfx.tick*pcore->gravity);

		if (pointer->IsMoving && ParticleTypes[pointer->type].collide)// && pointer->type!=PARTICLE_BULLET && pointer->type!=PARTICLE_BUCKSHOT)
		{
			// if hit wall on x
			if (Collision(pointer->x + (pointer->xvel*pcore->gfx.tick), pointer->y))
			{
				pointer->xvel=pointer->xvel*-0.7;
				if (ParticleTypes[pointer->type].RemoveOnCollide) *remove=true;
			}

			//if hit floor/ceiling
			if (Collision(pointer->x, pointer-> y+ (pointer->yvel*pcore->gfx.tick)))
			{
				// shell case bounce
				if (fabs(pointer->yvel)>0.2) 
					{
						DoBounceSound(pointer);
					}
				if (pointer->type==PARTICLE_RICHOCHETSPARK)
				{
				//	pointer->alpha = pointer->alpha * 0.45;
				}

				// bounce
				if (pointer->type==PARTICLE_HANDGRENADE) // grenade bounces more.
				{
					pointer->yvel = -pointer->yvel*((0.4*(((rand()%2000))/2000.0f))+0.2);
					if (pointer->yvel<0.06 && pointer->yvel>-0.06){ pointer->yvel=0; } 
				}
				else
				{
					pointer->yvel = -pointer->yvel*((0.2*(((rand()%2000))/2000.0f))+0.2);
					if (fabs(pointer->yvel)<(0.003*pcore->gfx.tick)){ pointer->yvel=0; } 
				}

				// slow rotation
				pointer->rotvel = pointer->rotvel*ParticleTypes[pointer->type].rotbouncedamper;
				// friction
				pointer->xvel = pointer->xvel*(ParticleTypes[pointer->type].friction);

			if (ParticleTypes[pointer->type].RemoveOnCollide) *remove=true;


			/*
			// do air resistance - if not hitting anything
			else if (ParticleTypes[pointer->type].airresistance!=0)
			{

				if (pointer->xvel>0) 
				{
					pointer->xvel = pointer->xvel-(ParticleTypes[pointer->type].airresistance);
					if (pointer->xvel<0) pointer->xvel=0;
				}
				else 
				{
					pointer->xvel = pointer->xvel+(ParticleTypes[pointer->type].airresistance);
					if (pointer->xvel>0) pointer->xvel=0;
				}


			
			}*/
		}
		}

	if (ParticleTypes[pointer->type].airresistance!=0)
	{

		if (pointer->xvel>0) 
		{
			pointer->xvel = pointer->xvel-(ParticleTypes[pointer->type].airresistance*pcore->gfx.tick);
			if (pointer->xvel<0) pointer->xvel=0;
		}
		else if (pointer->xvel<0) 
		{
			pointer->xvel = pointer->xvel+(ParticleTypes[pointer->type].airresistance*pcore->gfx.tick);
			if (pointer->xvel>0) pointer->xvel=0;
		}

		if (pointer->yvel>0) 
		{
			pointer->yvel = pointer->yvel-(ParticleTypes[pointer->type].airresistance*pcore->gfx.tick);
			if (pointer->yvel<0) pointer->yvel=0;
		}
		else if (pointer->yvel<0) 
		{
			pointer->yvel = pointer->yvel+(ParticleTypes[pointer->type].airresistance*pcore->gfx.tick);
			if (pointer->yvel>0) pointer->yvel=0;
		}
	}


	if (pointer->xvel==0 && pointer->yvel==0) pointer->IsMoving=false;

	if (pcore->landscape.WaterLine>0)
	if (pointer->y > pcore->landscape.WaterLine && pointer->type!=PARTICLE_WATERSPLASHMIST)
	{
		if (pointer->xvel!=0 && pointer->yvel!=0)
		{
			if (this->ParticleTypes[pointer->type].SplashType == 1)
			{
				if (pointer->type!=PARTICLE_DIRTSPRAY || rand()%5==0) // don't play splash sound for dirtspray so much
				pcore->sound.PlaySnd((rand()%2==0?s_watershell1:s_watershell2),pointer->x, pointer->y,0.3f);
				this->WaterTinySplash(pointer->x);
			}
			else if (this->ParticleTypes[pointer->type].SplashType == 2)
			{
				pcore->sound.PlaySnd((rand()%2==0?s_waterbullet1:s_waterbullet2),pointer->x, pointer->y,0.3f);
				this->WaterBullet(pointer->x);
			}
			else if (this->ParticleTypes[pointer->type].SplashType == 3)
			{
				pcore->sound.PlaySnd((rand()%2==0?s_waterplonk1:s_waterplonk2),pointer->x, pointer->y,0.3f);
				this->WaterLargeObjectPlunk(pointer->x);
			}
		}

		*remove=true;
	}
	



	DoParticleSpawner(pointer);


}

void CLandscape::DoAttachedParticle(LPParticle pointer, bool* remove,bool draw)
{
		// time
		pointer->time = pointer->time + (0.01*pcore->gfx.tick);
		// alpha
		pointer->alpha = pointer->alpha + (pointer->alphavel*pcore->gfx.tick);
		// scale
		pointer->xscale = pointer->xscale + (pointer->xscalevel*pcore->gfx.tick);
		if (pointer->keepaspect) pointer->yscale = pointer->xscale;
		else pointer->yscale = pointer->yscale + (pointer->yscalevel*pcore->gfx.tick);

		if (pointer->xscale<0 && pointer->yscale<0) *remove=true;

		pointer->x = pointer->pmonster->GetX() + (pointer->attached_x*pointer->pmonster->GetFacing());
		pointer->y = pointer->pmonster->GetY() + pointer->attached_y;

		pointer->rot = atan2(sin(pointer->attached_rot),cos(pointer->attached_rot)*pointer->pmonster->GetFacing());

		if (!*remove)
		DoParticleSpawner(pointer);
		
}

void CLandscape::DoBulletParticle(LPParticle pointer , bool*  remove,bool draw)
{
	float tick = pcore->gfx.tick;

	if (pointer->type==PARTICLE_STATICBULLET) tick=1.0f;

	if (pointer->type==PARTICLE_RPG)
	{
		pointer->xvel = pointer->xvel + (pointer->xvel*0.01f*tick);
		pointer->yvel = pointer->yvel + (pointer->yvel*0.01f*tick);
	}

	float moveangle = atan2(pointer->xvel,pointer->yvel);
	float movesin = sin(moveangle);
	float movecos = cos(moveangle);

	if (pointer->damage>0) pointer->damage = pointer->damage - (pcore->weapons.m_GunAttributes[pointer->weapon].damagefade * tick);
	if (pointer->damage<0) pointer->damage=0;

//	if (pointer->type==PARTICLE_STATICBULLET && pointer->time>0) *remove=true;
	// time
	pointer->time = pointer->time + (0.01*tick);
	// alpha
	pointer->alpha = pointer->alpha + (pointer->alphavel*tick);
	// scale
	pointer->xscale = pointer->xscale + (pointer->xscalevel*tick);
	if (pointer->keepaspect) pointer->yscale = pointer->xscale;
	else pointer->yscale = pointer->yscale + (pointer->yscalevel*tick);
	// gravity
	if (pointer->IsMoving && ParticleTypes[pointer->type].gravity!=0) 
		pointer->yvel = pointer->yvel + (ParticleTypes[pointer->type].gravity*pcore->gfx.tick*pcore->gravity);

	if (pointer->xvel>0) pointer->xvel = pointer->xvel-(ParticleTypes[pointer->type].airresistance*tick);
	else pointer->xvel = pointer->xvel+(ParticleTypes[pointer->type].airresistance*tick);


	GRECT rectangle;
	rectangle.left = pointer->x;
	rectangle.right = pointer->x+(pointer->xvel*tick);
	rectangle.top = pointer->y;
	rectangle.bottom = pointer->y+(pointer->yvel*tick);

	float buf;
	if (rectangle.top>rectangle.bottom) {buf=rectangle.top;rectangle.top=rectangle.bottom;rectangle.bottom=buf;}
	if (rectangle.left>rectangle.right) {buf=rectangle.left;rectangle.left=rectangle.right;rectangle.right=buf;}

	bool PossibleHitEnemy = pcore->enemies.BulletHitEnemyRect(&rectangle, pointer);
	bool PossibleHitLandscape = pcore->landscape.Collision(&rectangle);
	bool PossibleHitPlayer = false;
		
	PossibleHitPlayer = pcore->players[pcore->MyPlayerID].player.Collision(&rectangle);

	if (pcore->landscape.WaterLine>0 &&
	 (pointer->y+pointer->yvel) > pcore->landscape.WaterLine)
	{
		PossibleHitLandscape = true;
	}
	
	int collisiontype;

	float xvel, yvel;

	xvel = pointer->xvel;
	yvel = pointer->yvel;
	// need to re-do all this shit
	if (PossibleHitEnemy || PossibleHitLandscape || PossibleHitPlayer)
	for (float dist=0;
		dist==0 || 
		(abs(movesin*dist)<abs(xvel*tick)) ||
		(abs(movecos*dist)<abs(yvel*tick))	;
		dist=dist+1)
	{
		if (pointer->weapon==WEP_CHAINSAW && pointer->pplayer!=NULL)
		{
			float randomangle = (rand()%10000/50000.0f);
			randomangle = randomangle/2+0.9f;
			pointer->xvel = (pointer->pplayer->m_LastAimAngle<0?-1:1)*(sin(pointer->pplayer->m_LastAimAngle+(D3DX_PI/2)+(randomangle*(pointer->pplayer->m_LastAimAngle<0?-1:1)))*(((rand()%10000/10000.0f)/2)+0.3));//(rand()%1000/2000.0f);
			pointer->yvel = cos(fabs(pointer->pplayer->m_LastAimAngle)+(D3DX_PI/2)+randomangle)*(((rand()%10000/10000.0f)/2)+0.3);//-(rand()%1000/2000.0f);
		}

		collisiontype = CollisionType(pointer->x+(movesin*dist),pointer->y+(movecos*dist), pointer, PossibleHitEnemy);

		/*
		if (pcore->landscape.WaterLine>0)
		if ((pointer->y+(movecos*dist))>pcore->landscape.WaterLine)
		{
			pointer->x = pointer->x + (movesin*dist);
			pointer->y = pointer->y + (movecos*dist);
			break;
		}
		*/
		if (collisiontype>0)
		{
			pointer->x = pointer->x + (movesin*(dist-1));
			pointer->y = pointer->y + (movecos*(dist-1));
			*remove=true;
		}

		// hitlandscape
		if (collisiontype==1)
		{
			if (pcore->landscape.WaterLine==0 || (pcore->landscape.WaterLine>0 && pointer->y <= pcore->landscape.WaterLine))
			this->BulletHitLandscape(pointer);
			break;
		}

		// hit a player
		else if (collisiontype==2)
		{
			if (pointer->type!=PARTICLE_FIRE && pointer->type!=PARTICLE_FIREBITRISER)
				this->BulletHitPlayer(pointer);
			break;
		}
		else if (collisiontype==3)
		{
			break;
		}

	}

	if (pointer->y<0 || (pointer->type==PARTICLE_STATICBULLET))
		*remove=true;

	if (pcore->landscape.WaterLine>0)
	if (pointer->y >= pcore->landscape.WaterLine)
	{
		pcore->sound.PlaySnd((rand()%2==0?s_waterbullet1:s_waterbullet2),pointer->x, pointer->y,0.3f);
		this->WaterBullet(pointer->x);
		*remove=true;
	}

	if (!*remove)
	DoParticleSpawner(pointer);
		
}

// damages dont move, they are just damages in the air.
void CLandscape::DoDamageParticle(LPParticle pointer , bool*  remove, bool draw)
{
	if (pcore->bPaused) return;

	if (pointer->type==PARTICLE_FIREBITRISER && pointer->damage==0) pointer->damage = SKILL_GENERICFIREDAMAGE;

	DoGenericParticle(pointer, remove, draw);
	int collisiontype = CollisionType(pointer->x, pointer->y, pointer, pointer->type==PARTICLE_FIREBITRISER);

	// hit a player
	if (collisiontype==2)
	{
		// don't spray if it's fire damage
		if (pointer->type!=PARTICLE_FIRE && pointer->type!=PARTICLE_FIREBITRISER)
			this->BulletHitPlayer(pointer);
		*remove=true;
	}
	
		
}

void CLandscape::UnAttachAttachmentsMonster(enemy_base* monster)
{
	LPParticle pointer=m_pFirstParticle;
	LPParticle lastpointer=NULL;
	LPParticle nextpointer=NULL;

	bool remove=false;

	int index=0;
	m_ParticleDrawnCount=0;
	while (pointer!=NULL)
	{
		remove=false;

		if (pointer->AttachedTo==ATTACHED_TO_MONSTER)
		{
			if (pointer->pmonster==monster)
			{
			//	remove=true;
				pointer->IsMoving = true;
				pointer->pmonster=NULL;
				if (pointer->type==PARTICLE_ARROWDEAD)
				{
					pointer->rotvel = rand()%1000/20000.0f;
					pointer->xvel = (rand()%10000/10000.0f-0.5)/2;
					pointer->yvel = (rand()%10000/10000.0f-0.7f)/2;
				}
				if (pointer->type==PARTICLE_FIREBITATTACHED)
				{
					pointer->rotvel = rand()%1000/20000.0f;
					pointer->xvel = (rand()%10000/10000.0f-0.5)/2;
					pointer->yvel = (rand()%10000/10000.0f-0.7f)/5;
				}
				pointer->AttachedTo=ATTACHED_TO_NOTHING;
			}
		}
	
		nextpointer = pointer->Next;


		if (remove) 
		{
			 RemoveParticle(pointer);
			 pointer=NULL;
		}
	

		pointer=nextpointer;
		lastpointer = pointer;
		index++;
	}
}

float SQUAREDD(float val)
{
	return val*val;
}

void CLandscape::DoParticleSpawner(LPParticle particle, bool skipconditions)
{
	if (pcore->bPaused) return;

	if (particle->SpawnParticle==0) return;

	if (particle->x+512<offset_x) return;
	if (particle->x-512>offset_x+(pcore->gfx.m_d3dpp.BackBufferWidth/pcore->landscape.m_zoom)) return;
	if (particle->y+512<offset_y) return;
	if (particle->y-512>offset_y+(pcore->gfx.m_d3dpp.BackBufferHeight/pcore->landscape.m_zoom)) return;
		

	if (particle->SpawnTimerReset!=0)
	{
		particle->SpawnTimer = particle->SpawnTimer - 0.1*pcore->gfx.tick;
		if (particle->SpawnTimer>0) return;
		particle->SpawnTimer = particle->SpawnTimerReset;

		LPParticle prtcle = AddParticle(particle->SpawnParticle, 
											particle->x, 
											particle->y);
		if (prtcle!=NULL)
		{
			if (prtcle->type==PARTICLE_FIREBITRISER)
			{
				prtcle->damage = FIRE_DAMAGE;
				prtcle->pplayer = particle->pplayer;

				if (rand()%3==0)
				{
					AddParticle(PARTICLE_HEATSHIMMER, particle->x, particle->y-30);
					
				}
			}
		}
	}
	else
	if (particle->SpawnDistanceReset!=0)
	{
		particle->SpawnDistance = particle->SpawnDistance + sqrt(SQUAREDD(particle->xvel*pcore->gfx.tick)+SQUAREDD(particle->yvel*pcore->gfx.tick));
		
		if (particle->SpawnDistance<particle->SpawnDistanceReset && !skipconditions) return;

		float angle = atan2(particle->xvel, particle->yvel);

		for (float sd=-particle->SpawnDistance;
			sd<=0;
			sd=sd+particle->SpawnDistanceReset)
		{
			LPParticle prtcle = AddParticle(particle->SpawnParticle, 
											particle->x-(sin(angle)*sd), 
											particle->y-(cos(angle)*sd));
			if (prtcle!=NULL)
			{
				if (particle->type==PARTICLE_EXPLOSIONBIT)
				{
					prtcle->alpha = particle->alpha;
				}
				else if (prtcle->type==PARTICLE_BONEDUST)
				{
					prtcle->xscale = prtcle->xscale* (particle->alpha/205.0f);
					prtcle->alpha = particle->alpha;
				}
			}
		}

		particle->SpawnDistance = 0;
		return;
	}
	else return;


	
	
}

#define SHIFTBORDER 2.0f

void CLandscape::ShiftParticles(GRECT* rect, float x, float y)
{
	if (m_ParticleCount<=0) return;

	LPParticle pointer=m_pFirstParticle;

	while (pointer!=NULL)
	{
		if (this->ParticleTypes[pointer->type].layer==0)
		 if (pointer->x > rect->left-SHIFTBORDER)
		  if (pointer->x < rect->right+SHIFTBORDER)
		   if (pointer->y > rect->top-SHIFTBORDER-(y>0?5:0))
		    if (pointer->y < rect->bottom+SHIFTBORDER)
		    {
			    pointer->y = pointer->y + y;
			    pointer->x = pointer->x + x;
		    }

		pointer=pointer->Next;
	}
}

void CLandscape::DoGuiParticle(LPParticle pointer, bool* remove, bool draw)
{
	// time
	pointer->time = pointer->time + (0.01*pcore->gfx.RealTick);
	// alpha
	pointer->alpha = pointer->alpha + (pointer->alphavel*pcore->gfx.RealTick);
	// scale
	pointer->xscale = pointer->xscale + (pointer->xscalevel*pcore->gfx.RealTick);
	if (pointer->keepaspect) pointer->yscale = pointer->xscale;
	else pointer->yscale = pointer->yscale + (pointer->yscalevel*pcore->gfx.RealTick);

	if (pointer->yscale<0 && pointer->xscale<0) *remove=true;
	// gravity
	if (pointer->IsMoving && ParticleTypes[pointer->type].gravity!=0) 
		pointer->yvel = pointer->yvel + (ParticleTypes[pointer->type].gravity*pcore->gfx.RealTick*pcore->gravity);

	if (pointer->xvel==0 && pointer->yvel==0) pointer->IsMoving=false;

	DoParticleSpawner(pointer);
}

// called on remove of all particles
void CLandscape::ParticleRemoveExplosive(LPParticle pointer)
{
	if (pointer->type==PARTICLE_HANDGRENADE || 
		pointer->type==PARTICLE_IMPACTGRENADE || 
		pointer->type==PARTICLE_RPG)
	{
		if (pointer->y < pcore->landscape.WaterLine || pcore->landscape.WaterLine==0)
		{
			// if fired by player - do a shit load of damage
			if (pointer->pplayer!=NULL)
			{
				DoSplashDamage(pointer->x,pointer->y, 300,400, pointer->pplayer);
			}
			else
			{
				DoSplashDamage(pointer->x,pointer->y, 300,40, pointer->pplayer);
			}
			DrawExplosion(pointer->x, pointer->y, 100);
			
			MakeExplosion(pointer->x, pointer->y+10, 300,50);
		}
	}
}