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
float Common::_PI = 3.14159265358979323846;

int Common::maxPoligonNum = 7;

bool Common::_RENDER_PAUSE = false;
bool Common::_RENDER_STEP = false;
bool Common::_DETAIL = false;

set<GraphObject*> *Common::graphObjSet = new set<GraphObject*>();

bool Common::renderForward = true;

// t�vols�g ellen�rz�se
bool Common::DistanceCheck(GraphObject *c)
{
	// minden grafikus objektumra
	for (set<GraphObject*>::iterator i = Common::graphObjSet->begin(); i != Common::graphObjSet->end(); ++i)
	{
		// saj�tmag�val nem hasonl�tunk �ssze && ha a befoglal� n�gyzetek �ssze�rnek
		if (((*c) != (*i)) && c->IsNear(*i))
		{
			if (_DETAIL)
				c->clr = CLR_NEAR;

			if (c->IsContactConcave(*i))
			{
				// sebess�g cser�hez
				float tmpVx = 0.0;
				float tmpVy = 0.0;

				if (!c->crushed)
				{
					tmpVx = c->vX;
					tmpVy = c->vY;

					c->vX = (*i)->vX;
					c->vY = (*i)->vY;

					c->clr = Common::CLR_CHG;
				}

				if (!(*i)->crushed)
				{
					(*i)->vX = tmpVx;
					(*i)->vY = tmpVy;
				}
			}
		}
	}

	return false;
}

void Common::UpDateGraphObjectPosition(GraphObject *c)
{
	if (!c) return;

	if (!renderForward)
	{
		c->vX *= -1.0;
		c->vY *= -1.0;
	}

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
		shiftX -= 2.0 * (c->XMax() + c->vX - screenXMax);
		c->vX *= -1.0;
	}

	dist = c->vX + c->XMin();
	if (dist < screenXMin)
	{
		shiftX -= 2.0 * (c->XMin() + c->vX - screenXMin);
		c->vX *= -1.0;
	}

	dist = c->vY + c->YMax();
	if (dist > screenYMax)
	{
		shiftY -= 2.0 * (c->YMax() + c->vY - screenYMax);
		c->vY *= -1.0;
	}

	dist = c->vY + c->YMin();
	if (dist < screenYMin)
	{
		shiftY -= 2.0 * (c->YMin() + c->vY - screenYMin);
		c->vY *= -1.0;
	}

	///////////////////////////////////////////////////////////////////////////
	// az elmozdul�s alkalmaz�sa
	for(unsigned int i = 0; i < c->points->size(); ++i)
	{
		(*(c->points))[i]->x += shiftX;
		(*(c->points))[i]->y += shiftY;
	}

	if (!renderForward)
	{
		c->vX *= -1.0;
		c->vY *= -1.0;
	}
}

void Common::GetRandSpeed(float &vx, float &vy)
{
	float absSpeed = ((rand() % 6) + 3) / 40.0;
	float direction = (rand() % 100 / 100.0) * _PI;

	vx = absSpeed * cos(direction);
	vy = absSpeed * sin(direction);
}

void Common::GetRandPosition(float &pX, float &pY)
{
	if (!_RENDER_PAUSE)
	{
		// fut�sk�zben mindig a k�perny� k�zep�n j�nnek l�tre az �j objektumok
		pX = 0.0;
		pY = 0.0;
		return;
	}

	pX = rand() % 30 - 15.0;
	pY = rand() % 30 - 15.0;
}

bool Common::AddGraphObject()
{
	if (graphObjSet->size() >= 25)
		return false;

	float pX = 0.0;
	float pY = 0.0;
	GetRandPosition(pX, pY);

	float vX = 0.0;
	float vY = 0.0;
	GetRandSpeed(vX, vY);

	float orient = (rand() % 100) / 100.0 * _PI;

	Common::graphObjSet->insert(new GraphObject(
		pX,				// x
		pY,				// y
		(rand() % 20) / 20.0 * 0.2 + 2.0,	// r
		rand() % (Common::maxPoligonNum - 2) + 3,		// point num
		orient,				// orientation
		CLR_NORMAL,			// color
		vX, vY)				// speed
		);

	return true;
}

bool Common::DelGraphObject()
{
	if (graphObjSet->size() <= 5)
		return false;

	int delPos = rand() % graphObjSet->size();

	set<GraphObject*>::iterator it = graphObjSet->begin();
	for (int i = 0; i < delPos; ++i, ++it);

	graphObjSet->erase(it);

	return true;
}