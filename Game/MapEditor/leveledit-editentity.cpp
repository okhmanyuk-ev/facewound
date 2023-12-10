#include "leveledit.h"
#include "entity-defs.h"

void LevelEdit::DoEntityMode(void)
{
	if (!AllowEdit) return;

	if (gfx.m_bMouse2)
	{
		entitys[SelectedEntity].x = rpoint.x;
		entitys[SelectedEntity].y = rpoint.y;

		// prop, snap to grid
		if (gfx.InputKeyDown(DIK_LSHIFT))
		{
			entitys[SelectedEntity].x = ((int)((int)rpoint.x/16)*16);
			entitys[SelectedEntity].y  = ((int)((int)rpoint.y/16)*16);
		}

	}

	if (gfx.m_bMouse1)
	{
		if (gfx.InputKeyDown(DIK_LCONTROL))
		{
			entitys[SelectedEntity].a = rpoint.x - entitys[SelectedEntity].x;
			entitys[SelectedEntity].b = rpoint.y - entitys[SelectedEntity].y;

			if (gfx.InputKeyDown(DIK_LSHIFT))
			{
				entitys[SelectedEntity].a = (int)((int)entitys[SelectedEntity].a/16)*16;
				entitys[SelectedEntity].b = (int)((int)entitys[SelectedEntity].b/16)*16;
			}
		}
		else
		{
			for (int i=0;i<MAX_ENTITYS;i++)
				if (entitys[i].inuse==true)
					if (entitys[i].x-16<rpoint.x && entitys[i].x+16>rpoint.x)
						if (entitys[i].y-16<rpoint.y && entitys[i].y+16>rpoint.y)
						{
							SelectedEntity=i;
							break;
						}
		}
	}
}

void LevelEdit::DoPrefabMode(void)
{
	if (!AllowEdit) return;

	if (gfx.m_bMouse2)
	{
		for (int i=0;i<MAX_ENTITYS;i++)
		{
			if (entitys[i].x==(16.0f+(int)point.x*32) && 	
				entitys[i].y==(16.0f+(int)point.y*32))
			{
				SelectedEntity = i;
				this->DelEntity();
			}
		}
	}

	if (gfx.m_bMouse1)
	{
		bool alreadyexists=false;
		for (int i=0;i<MAX_ENTITYS;i++)
		{
			if (entitys[i].inuse==true &&
				entitys[i].type==ENT_PLUTONIUM &&
				entitys[i].x==(16.0f+(int)point.x*32) &&
				entitys[i].y==(16.0f+(int)point.y*32))
				alreadyexists = true;
		}

		if (!alreadyexists)
		for (int i=0;i<MAX_ENTITYS;i++)
		{
			if (entitys[i].inuse==false)
			{
				memcpy(&entitys[i],&entitys[SelectedEntity],sizeof(sEntitys));
				entitys[i].type=ENT_PLUTONIUM;
				entitys[i].x = (16.0f+(int)point.x*32);
				entitys[i].y = (16.0f+(int)point.y*32);
				entitys[i].c = 0;
				entitys[i].a = 0;
				entitys[i].b = 0;
				sprintf(entitys[i].stringa,"");
				entitys[i].inuse = true;
				SelectedEntity = i;
				UpdateEntTexture(i);
				break;
			}
		}
	}
}