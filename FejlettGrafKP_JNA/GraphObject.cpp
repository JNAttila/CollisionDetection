#define _USE_MATH_DEFINES

#include "GraphObject.h"
#include "MyPoint.h"

#include <math.h>


GraphObject::GraphObject(float x, float y, float r, int nP, float rotA, MyColor *c, float vx, float vy) :
	cX(x), cY(y), R(r), numPoints(nP), rotAngle(rotA), clr(c), vX(vx), vY(vy)
{
	points = new vector<CMyPoint*>();

	float xMin = 1000;
	float xMax = -1000;
	float yMin = 1000;
	float yMax = -1000;

	float newX, newY;

	float angle = rotA;
	for(int i = 0;  i < nP; i++)
	{
		angle += 2.0 * M_PI / nP;

		newX = x + r * cos(angle);
		newY = y + r * sin(angle);

		// a sz�ls� pontok x �s y koordin�t�inak megjegyz�se
		if (xMax < newX)
		{
			xMax = newX;
			iXMax = i;
		}
		if (yMax < newY)
		{
			yMax = newY;
			iYMax = i;
		}

		if (xMin > newX)
		{
			xMin = newX;
			iXMin = i;
		}
		if (yMin > newY)
		{
			yMin = newY;
			iYMin = i;
		}

		points->push_back(new CMyPoint( newX, newY));
	}

	numPoints = (numPoints < 3) ? (3) : ((numPoints > 15) ? (15) : (numPoints));	  
}

float GraphObject::XMin()
{
	return (*points)[iXMin]->x;
}
float GraphObject::XMax()
{
	return (*points)[iXMax]->x;
}
float GraphObject::YMin()
{
	return (*points)[iYMin]->y;
}
float GraphObject::YMax()
{
	return (*points)[iYMax]->y;
}

bool GraphObject::IsNear(GraphObject *obj)
{
	if (!obj) return false;

	float xMin = XMin();
	float xMax = XMax();
	float yMin = YMin();
	float yMax = YMax();

	float objxMin = obj->XMin();
	float objxMax = obj->XMax();
	float objyMin = obj->YMin();
	float objyMax = obj->YMax();

	if (((xMin <= objxMin && objxMin <= xMax) || (xMin <= objxMax && objxMax <= xMax)) &&
		((yMin <= objyMin && objyMin <= yMax) || (yMin <= objyMax && objyMax <= yMax)) )
		return true;

	return false;
}

// a k�t objektum �riintkezik-e egym�ssal?
bool GraphObject::IsContact(GraphObject *obj)
{
	// saj�tmagam minden pontj�ra
	for(int i = 0; i < points->size(); ++i)
	{
		int sgn;

		// a saj�t k�rvonalam ment�n, az �n k�vetkez� pontom ebben az ir�nyban van
		int signumMy = CMyPoint::RotDirct((*points)[i % points->size()], (*points)[(i + 1) % points->size()],
			(*points)[(i + 2) % points->size()]);

		// a vizsg�lt pont a m�sik oldalon van, �rdemes folytatni

		// az �sszehasonl�tott objektum minden pontj�val
		for(int j = 0; j < obj->points->size(); ++j)
		{
			sgn = CMyPoint::RotDirct((*points)[i % points->size()], (*points)[(i + 1) % points->size()],
				(*(obj->points))[j % obj->points->size()]);

			// azonos oldalon vannak �s az els� kett� �ltal t�glalapon bel�l a harmadik
			if (signumMy == sgn && CMyPoint::IsPointBetween((*points)[i % points->size()], (*points)[(i + 1) % points->size()],
				(*(obj->points))[j % obj->points->size()]))
			{
				// �ssze�rt
				return true;
			}
		}

	}

	return false;
}


/*
// a k�t objektum �riintkezik-e egym�ssal?
bool IsContact(GraphObject *obj)
{
// saj�tmagam minden pontj�ra
for(int i = 0; i < points->size(); ++i)
{
bool metszes = true;
int sgn;

// a saj�t k�rvonalam ment�n, az �n k�vetkez� pontom ebben az ir�nyban van
int signumMy = RotDirct((*points)[i], (*points)[(i + 1) % points->size()],
(*points)[(i + 2) % points->size()]);

int signumComp = RotDirct((*points)[i], (*points)[(i + 1) % points->size()],
(*(obj->points))[0]);

if (signumMy != signumComp)
{
// a vizsg�lt pont a m�sik oldalon van, �rdemes folytatni

// az �sszehasonl�tott objektum minden pontj�val
for(int j = 1; j < obj->points->size() + 1; ++j)
{
sgn = RotDirct((*points)[i % points->size()], (*points)[(i + 1) % points->size()],
(*(obj->points))[j % obj->points->size()]);

// ha b�rmely k�t pont 
if (signumComp != sgn)
{
// �ssze�rhet
continue;
}

// nem �r �ssze
return false;
}
}
}

return true;
}
/* */


GraphObject::~GraphObject()
{
	if (clr)
		delete clr;
	clr = NULL;
}

bool GraphObject::operator==(const GraphObject *c)
{
	return (c->cX == cX) && (c->cY == cY) && (c->R == R) && (c->rotAngle == rotAngle);
}

bool GraphObject::operator!=(const GraphObject *c)
{
	return !(this->operator==(c));
}
