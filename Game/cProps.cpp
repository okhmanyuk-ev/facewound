#include "cProps.h"
#include "core.h"


#define PCORE pcore

cProps::cProps(void)
{
	firstprop = NULL;
}

cProps::~cProps(void)
{
}


void cProps::Init()
{
	s_pickup_plutonium = pcore->sound.LoadSound(5,"sound/pickup.ogg");
}


// 0 = normal
// 1 = behind 
// 2 = infront of landscape, behind player
// 3 = infront of everything
// 4 = bump (infront of everything)
void cProps::DrawProps(unsigned int layer) 
{
	sProps* p=this->firstprop;
	bool draw;
	unsigned int count=0, drawcount=0;
	char label[100];

	pcore->SetAlphaMode(false);

	while (p!=NULL)
	{
		count++;
		draw = true;

		// don't draw if there's no way they could be on screen
		if (p->x+512<pcore->landscape.offset_x) draw=false;
		if (p->x-512>pcore->landscape.offset_x+(pcore->gfx.m_d3dpp.BackBufferWidth/pcore->landscape.m_zoom)) draw=false;;
		if (p->y+512<pcore->landscape.offset_y) draw=false;;
		if (p->y-512>pcore->landscape.offset_y+(pcore->gfx.m_d3dpp.BackBufferHeight/pcore->landscape.m_zoom)) draw=false;;

		if (draw)
		{
			drawcount++;
			DoMover(p);
			
			if (p->blendmode==1) pcore->SetAlphaMode(true);
			else pcore->SetAlphaMode(false);

			pcore->gfx.SpriteDraw(p->texture,
								(p->x-pcore->landscape.offset_x)*pcore->landscape.m_zoom,
								(p->y-pcore->landscape.offset_y)*pcore->landscape.m_zoom,
								0,0,0,
								pcore->landscape.m_zoom,
								pcore->landscape.m_zoom);
			

			if (pcore->gfx.InputKeyDown(DIK_C))
			{
				if (p->type==PTYPE_PROP) sprintf(label,"[prop]");
				else if (p->type==PTYPE_DESTROYABLE) sprintf(label,"[destroyable]");
				else if (p->type==PTYPE_MOVER) sprintf(label,"[mover]");
				else if (p->type==PTYPE_PICKUP) sprintf(label,"[pickup]");
				pcore->gfx.DrawTextEx((p->x-pcore->landscape.offset_x+5)*pcore->landscape.m_zoom,(p->y-pcore->landscape.offset_y)*pcore->landscape.m_zoom,255,255,255,label);
			}
		}
		p = p->Next;
	}

	pcore->SetAlphaMode(false);
}

sProps* cProps::AddProp()
{
	sProps* np = new sProps();

	np->Next = NULL; // we're on the end

	if (firstprop==NULL) 
	{
		firstprop = np;
	}
	else 
	{
		// set last ent's next to me
		sProps* p=firstprop;

		while (p->Next!=NULL)
		{
			p=p->Next;
		} // loop until we land on the last one
		p->Next = np;
	}

	return np;
}

bool cProps::Collision(float x, float y)
{
	sProps* p=firstprop;

	while (p!=NULL)
	{
		if (!p->ispickup)
		if (p->width>2 || p->height>2)
		if (x>=p->x)
		if (x<=p->x+p->width)
		if (y>=p->y)
		if (y<=p->y+p->height)
			return true;

		p=p->Next;
	} 

	return false;
}

bool cProps::Collision(RECT* rct)
{
	exit(9);
	return false;
}

bool cProps::Collision(GRECT* rct)
{
	sProps* p=firstprop;
	//GRECT trect;

	while (p!=NULL)
	{
		if (!p->ispickup)
		if (p->width>2 || p->height>2)
		if (IntersectGRect(rct,&p->colrect))
			return true;

		p=p->Next;
	} 

	return false;
}
bool cProps::BulletHitProp(int x, int y, LPParticle bullet)
{
	sProps* p=firstprop;

	float bangle = atan2(bullet->xvel, bullet->yvel);
	float bspeed = sqrtf((bullet->xvel*bullet->xvel) + (bullet->yvel*bullet->yvel))*0.1;
	float ohealth;

	while (p!=NULL)
	{
		if (p->health!=0)
		if (x>=p->x-3)
		if (x<=p->x+p->width+3)
		if (y>=p->y-3)
		if (y<=p->y+p->height+3)
		{
			// do damage
			ohealth = p->health;

			

			p->health = p->health - bullet->damage;
			if (ohealth>0 && p->health<=0)
			{
				KillProp(p, bangle, bspeed);
			}
			return true;
		}

		p=p->Next;
	} 
	return false;
}

bool cProps::SplashtHitProp(float x, float y, float size, float power)
{
	sProps* p=firstprop;

	float ohealth;

	while (p!=NULL)
	{
		if (p->health!=0)
		if (x-(size/2) < p->x+(p->width/2))
		if (x+(size/2) > p->x+(p->width/2))
		if (y-(size/2) < p->y+(p->height/2))
		if (y+(size/2) > p->y+(p->height/2))
		{
			ohealth = p->health;

			p->health = p->health - power;
			if (ohealth>0 && p->health<=0)
			{
				KillProp(p, 9, 9);
				SplashtHitProp(x,y,size, power);
			//	pcore->landscape.MakeExplosion(x,y,size, power);
				return true;
			}
		}

		p=p->Next;
	} 
	return false;
}

sProps* cProps::Previous(sProps* index)
{
	sProps* temp=firstprop;
	if(index==firstprop) return firstprop;
	while(temp->Next != index)
		temp=temp->Next;

	return temp;
}

void cProps::RemoveProp(sProps* delme)
{
	sProps* p=firstprop;

	if (delme==firstprop)
	{
		firstprop = delme->Next;
		delete delme;
	}
	else
	{
		Previous(delme)->Next = delme->Next;
		delete delme;
	}
}

void cProps::RemoveAll()
{
	sProps* tempp=firstprop;
	sProps* del;
	while (tempp!=NULL)
	{
		del = tempp;
		tempp = tempp->Next;
		delete del;
	} 

	firstprop=NULL;
}

void cProps::KillProp(sProps* p, float attack_angle, float speed)
{
	float rangle, rnum;
	int particle;

	pcore->landscape.MakeExplosion(p->x+(p->width/2),p->y+(p->height/2),(int)((p->width+p->height)/2),p->explosion+1);

	if (p->particle>0 && p->particle_num>0)
	{
		for (int i =0;i<p->particle_num;i++)
		{
			particle = p->particle;
			// wood
			if (particle==1001)
			{
				if (rand()%2==0)	particle = PARTICLE_GIB_CRATE;
				else				particle = PARTICLE_GIB_CRATE2;
			}
			else if (particle==1002)
			{
				if (rand()%2==0)	particle = PARTICLE_GIB_METAL;
				else				particle = PARTICLE_GIB_METAL2;
			}
			else if (particle==1003)
			{
					particle = PARTICLE_GENERIC_32X32;
			}

			LPParticle pa =pcore->landscape.AddParticle(particle,
											p->x+(rand()%(int)p->width),
											p->y+(rand()%(int)p->height));

			if (pa!=NULL)
			{
				if (particle==PARTICLE_GENERIC_32X32)
				{
					pa->xscale = 0.5f + ((rand()%1000)/5000.0f);
					pa->yscale = pa->xscale;

					if (rand()%3==0)		pa->textureoverride = PCORE->textures.LoadTexture("textures/particles/gib_statue.bmp");
					else if (rand()%2==1)	pa->textureoverride = PCORE->textures.LoadTexture("textures/particles/gib_statue2.bmp");
					else 					pa->textureoverride = PCORE->textures.LoadTexture("textures/particles/gib_statue3.bmp");
				}

				rnum = ((rand()%10000)/11000.0f)+0.1;
				rangle = attack_angle + 0.5*rnum;
				rnum = ((rand()%10000)/11000.0f)+0.1;

				pa->xvel = (sin(rangle)*rnum)*(speed);
				pa->yvel = (cos(rangle)*rnum)*(speed);
			}
		}
	}

	if (p->explosion>0)
	{
		p->health = -1;
		pcore->landscape.DoSplashDamage(p->x+(p->width/2),p->y+(p->height/2),p->explosion,p->explosion,NULL);
		pcore->landscape.DrawExplosion(p->x+(p->width/2),p->y+(p->height/2),p->explosion);
		pcore->landscape.MakeExplosion(p->x+(p->width/2),p->y+(p->height/2),p->explosion*2,p->explosion*0.1);
	}

	pcore->Trigger(p->trigger);
	RemoveProp(p);
}

void cProps::UpdateProp(sProps* p)
{
	p->colrect.left = p->x;
	p->colrect.right = p->x + p->width;
	p->colrect.top = p->y;
	p->colrect.bottom = p->y + p->height;

	SetGRect(&p->mover_rect,p->x, p->y-1, p->x+p->width, p->y+p->height);
}

void cProps::ActivateMover(sProps* p)
{
	if (p->MoverActivated) return;
	p->ytarget = p->y + p->yoff;
	p->MoverActivated = true;
	pcore->Trigger(p->trigger);
}

void cProps::DoMover(sProps* p)
{
	if (!p->MoverActivated)
	{
		if (p->y==p->ytarget && p->ElevatorOut)
		{
			p->timer1 = p->timer1 + (pcore->gfx.tick/1000.0f);
			if (p->timer1 > p->xoff)
			{
				p->timer1 = 0;
				this->ActivateMover(p);
			}
		}

		return;
	}
	 
	float ym=0.0f;

	if (p->yvel!=0 && p->yoff!=0)
	{
		if (p->y < p->ytarget)
		{
			ym = p->yvel*pcore->gfx.tick;
			p->y = p->y + ym;
			if (p->y >= p->ytarget) p->y = p->ytarget;
		}
		else if (p->y > p->ytarget)
		{
			ym = -(p->yvel*pcore->gfx.tick);
			p->y = p->y + ym;
			if (p->y <= p->ytarget) p->y = p->ytarget;
		}
	}

	GRECT prect;
	SetGRect(&prect,p->x, p->y, p->x+p->width, p->y+p->height);
	pcore->MoverMovePlayer(p, 0,ym);
	pcore->landscape.ShiftParticles(&prect,0,ym);


	if (p->y==p->ytarget)
	{
		p->MoverActivated = false;
		if (p->ElevatorOut) p->ElevatorOut=false; else p->ElevatorOut=true;
		p->yoff = -p->yoff;
	} 

	UpdateProp(p);


}
/*
PTYPE_PROP 
PTYPE_DESTROYABLE 
PTYPE_MOVER 
PTYPE_PICKUP 
*/
bool cProps::Trigger(char* name)
{
	pcore->gui.WriteToConsole(1,255,0,0,"Triggered name: '%s'",name);
	if (strcmp(name,"")==0) return false;

	sProps* p=firstprop;
	bool returnvalue = false;
	while (p!=NULL)
	{
		if (strcmp(p->name, name)==0)
		{
			if (p->type == PTYPE_MOVER)
			{
				this->ActivateMover(p);
				pcore->gui.WriteToConsole(1,0,0,0,"Triggered a Mover (%s)",name);
			}
			else if (p->type == PTYPE_DESTROYABLE) 
			{
				this->KillProp(p,D3DX_PI,0.2f);
				//this->RemoveProp(p);
				pcore->gui.WriteToConsole(1,0,0,0,"Triggered a Destroyable (%s)",name);
				Trigger(name);
				return true;
			}
			
			returnvalue = true;
		}

		if (p!=NULL) p=p->Next;
	} 

	return returnvalue;
}
