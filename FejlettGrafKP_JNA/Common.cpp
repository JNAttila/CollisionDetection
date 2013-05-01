#include "Common.h"

using namespace std;

MyColor* Common::CLR_NEAR = new MyColor(0.99, 0.0, 0.0);
MyColor* Common::CLR_NORMAL = new MyColor(0.0, 0.7, 0.0);

float Common::screenXMin = -30.0;
float Common::screenYMin = -20.0;
float Common::screenXMax = 30.0;
float Common::screenYMax = 20.0;

set<GraphObject*> *Common::graphObjSet = new set<GraphObject*>();

bool Common::DistanceCheck(GraphObject *c)
{
	set<GraphObject*>::const_iterator i;
	for (i = Common::graphObjSet->begin(); i != Common::graphObjSet->end(); ++i)
	{
		if (((*c) != (*i)) && c->IsNear(*i))
		{
			c->clr = CLR_NEAR;

			if (c->IsContact(*i))
			{
				c->vX *= -1;
				c->vY *= -1;

				(*i)->vX *= -1;
				(*i)->vY *= -1;
			}

			return true;
		}
	}

	return false;
}


void Common::UpDateGraphObjectPosition(GraphObject *c)
{
	if (!c) return;

	c->cX += c->vX;
	c->cY += c->vY;

	///////////////////////////////////////////////////////////////////////////
	// falakr�l visszaver�d�s

	// falt�l m�rt t�vols�g
	float dist;
	// az elmozdul�s m�rt�ke
	float shiftX = c->vX;
	float shiftY = c->vY;

	dist = c->vX + c->XMax();
	if (dist > screenXMax)
	{
		shiftX -= 2 * (c->XMax() + c->vX - screenXMax);
		c->vX *= -1;
	}

	dist = c->vX + c->XMin();
	if (dist < screenXMin)
	{
		shiftX -= 2 * (c->XMin() + c->vX - screenXMin);
		c->vX *= -1;
	}

	dist = c->vY + c->YMax();
	if (dist > screenYMax)
	{
		shiftY -= 2 * (c->YMax() + c->vY - screenYMax);
		c->vY *= -1;
	}

	dist = c->vY + c->YMin();
	if (dist < screenYMin)
	{
		shiftY -= 2 * (c->YMin() + c->vY - screenYMin);
		c->vY *= -1;
	}

	///////////////////////////////////////////////////////////////////////////
	// az egym�ssal �rintkez�s
	set<GraphObject*>::iterator iter;
	for(iter = graphObjSet->begin(); iter != graphObjSet->end(); ++iter)
	{
		if ((*iter)->clr == CLR_NEAR)
		{
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// az elmozdul�s alkalmaz�sa
	for(unsigned int i = 0; i < c->points->size(); ++i)
	{
		(*(c->points))[i]->x += shiftX;
		(*(c->points))[i]->y += shiftY;
	}
}
