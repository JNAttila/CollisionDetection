#define _USE_MATH_DEFINES

#include "Common.h"

#include <math.h>


GraphObject::GraphObject(float x, float y, float r, int nP, float rotA, MyColor *c, float vx, float vy) :
	cX(x), cY(y), R(r), numPoints(nP), rotAngle(rotA), clr(c), vX(vx), vY(vy)
{
	points = new vector<CMyPoint*>();

	crushed = false;

	float xMin = 1000;
	float xMax = -1000;
	float yMin = 1000;
	float yMax = -1000;

	float newX, newY;
	
	numPoints = (numPoints < 3) ? (3) : ((numPoints > 15) ? (15) : (numPoints));	

	float angle = rotAngle;
	for(int i = 0;  i < numPoints; i++)
	{
		angle += 2.0 * M_PI / numPoints;

		newX = cX + R * cos(angle);
		newY = cY + R * sin(angle);

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

	return (
		((xMin <= objxMin && objxMin <= xMax) || (xMin <= objxMax && objxMax <= xMax)) &&
		((yMin <= objyMin && objyMin <= yMax) || (yMin <= objyMax && objyMax <= yMax))
		);
}

// a k�t objektum �riintkezik-e egym�ssal?
bool GraphObject::IsContact2(GraphObject *obj)
{
	// az alap objektum pontjainak sz�ma
	long s = points->size();

	// amit hasonl�tunk hozz� objektum pontjainak sz�ma
	long so = obj->points->size();

	// a k�t objektum pontjaib�l sz�molt forg�sir�ny
	int sgn;

	// saj�tmagam minden pontj�ra
	for(int i = 0; i < s; ++i)
	{
		// a saj�t k�rvonalam ment�n, az �n k�vetkez� pontom ebben az ir�nyban van
		int mySgn = CMyPoint::RotDirct((*points)[i % s], (*points)[(i + 1) % s], (*points)[(i + 2) % s]);

		// a vizsg�lt pont a m�sik oldalon van, �rdemes folytatni

		// az �sszehasonl�tott objektum minden pontj�val
		for(int j = 0; j < so; ++j)
		{
			sgn = CMyPoint::RotDirct((*points)[i % s], (*points)[(i + 1) % s],
				(*(obj->points))[j % so]);

			// azonos oldalon vannak �s az els� kett� �ltal t�glalapon bel�l a harmadik
			if (mySgn == sgn && CMyPoint::IsPointBetween((*points)[i % s],
				(*points)[(i + 1) % s], (*(obj->points))[j % so]))
			{
				// �ssze�rt
				return true;
			}
		}

	}

	return false;
}


// a k�t objektum �riintkezik-e egym�ssal?
bool GraphObject::IsContact(GraphObject *obj)
{
	// az alap objektum pontjainak sz�ma
	long s = points->size();

	// amit hasonl�tunk hozz� objektum pontjainak sz�ma
	long so = obj->points->size();

	// objektumokat �sszehasonl�t� szakaszokb�l sz�molt forg�sir�ny
	int sgn;

	// saj�t objektumon bel�l a forg�sir�ny
	int mySgn = CMyPoint::RotDirct((*points)[0], (*points)[1], (*points)[2]);

	// poligon v�g�st kell csin�lni, hogy a vizsg�lt objektum b�rmelyik pontja
	// bel�l van-e az n k�rvonalamon
	bool ret = false;
	
	// az �sszehasonl�tott objektum minden pontj�ra
	for(int i = 0; i < so && !ret; ++i)
	{
		int sgnCnt = 0;

		// minden saj�tpontommal
		for(int j = 0; j < s; ++j)
		{
			sgn = CMyPoint::RotDirct((*points)[j % s], (*points)[(j + 1) % s],
				(*(obj->points))[i % so]);

			sgnCnt += sgn;
		}

		if (ret)
			printf("RET!!\n");

		if (sgnCnt == s-1)
			printf("s-1 :  %d\n", sgnCnt); 
		
		if (sgnCnt == s)
			printf("s :  %d\n", sgnCnt);

		if (abs(sgnCnt) >= s-1)
			ret = true;
	}

	return ret;
}


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
