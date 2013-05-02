#include "Common.h"
#include <math.h>

using namespace std;

MyColor* Common::CLR_NEAR = new MyColor(0.99, 0.0, 0.0);
MyColor* Common::CLR_NORMAL = new MyColor(0.0, 0.7, 0.0);
MyColor* Common::CLR_CHG = new MyColor(0.8, 0.8, 0);

float Common::screenXMin = 0;
float Common::screenYMin = 0;
float Common::screenXMax = 0;
float Common::screenYMax = 0;

float Common::nRange = 0;

set<GraphObject*> *Common::graphObjSet = new set<GraphObject*>();

bool Common::renderForward = true;

bool Common::PointLineDist(CMyPoint *p1, CMyPoint *p2, CMyPoint *pp)
{
	if (!p1 || !p2 || !pp)
		return false;

	float  xMin = min<float>(p1->x, p2->x);
	float  xMax = max<float>(p1->y, p2->y);
	float  yMin = min<float>(p1->x, p2->x);
	float  yMax = max<float>(p1->y, p2->y);

	if (pp->x < xMin || pp->x > xMax || pp->y < yMin || pp->y > yMax)
		return false;

	if (abs(abs((p1->x - p2->x)/(p1->y - p2->y)) - abs((p1->x - pp->x)/(p1->y - pp->y))) < 0.1)
		return true;

	return false;
}

bool Common::DistanceCheck(GraphObject *c)
{
	for (set<GraphObject*>::iterator i = Common::graphObjSet->begin(); i != Common::graphObjSet->end(); ++i)
	{
		if (((*c) != (*i)))// && c->IsNear(*i))
		{
			c->clr = CLR_NEAR;

			if (c->IsContact(*i))
			{
				if (!c->crushed)
				{
					c->vX *= -1;
					c->vY *= -1;
					c->crushed = true;

					c->clr = Common::CLR_CHG;
				}

				if (!(*i)->crushed)
				{
					(*i)->vX *= -1;
					(*i)->vY *= -1;
					(*i)->crushed = true;
				}
				return true;
			}
		}
	}

	return false;
}


void Common::UpDateGraphObjectPosition(GraphObject *c)
{
	if (!c) return;

	if (renderForward)
	{
		c->cX += c->vX;
		c->cY += c->vY;
	}
	else
	{
		c->cX -= c->vX;
		c->cY -= c->vY;
	}

	///////////////////////////////////////////////////////////////////////////
	// falakról visszaverõdés

	// faltól mért távolság
	float dist;
	// az elmozdulás mértéke
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
	// az egymással érintkezés
	set<GraphObject*>::iterator iter;
	for(iter = graphObjSet->begin(); iter != graphObjSet->end(); ++iter)
	{
		if ((*iter)->clr == CLR_NEAR)
		{
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// az elmozdulás alkalmazása
	for(unsigned int i = 0; i < c->points->size(); ++i)
	{
		(*(c->points))[i]->x += shiftX;
		(*(c->points))[i]->y += shiftY;
	}
}
