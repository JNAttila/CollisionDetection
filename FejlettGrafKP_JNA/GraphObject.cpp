#define _USE_MATH_DEFINES

#include "Common.h"

#include <math.h>


bool GraphObject::randomRadius = true;

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

	float rFactor = 1.0;

	numPoints = (numPoints < 3) ? (3) : ((numPoints > 15) ? (15) : (numPoints));	

	// orient�ci� sz�ge
	float angle = rotAngle;
	
	// a poligon cs�cspontjai
	for(int i = 0;  i < numPoints; i++)
	{
		// glob�lis v�ltoz� alapj�n szab�lyos alakzatok, vagy sem
		if (GraphObject::randomRadius)
			rFactor = 1.0 + (rand() % 20 - 10) / 30.0;

		newX = cX + R * rFactor * cos(angle);
		newY = cY + R * rFactor * sin(angle);
		
		// konk�v alakzatok 5-sz�g eset�n
		if (numPoints == 5 && i == 2)
		{
			newX = cX;
			newY = cY;
		}

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

		// �j pont t�rol�sa
		points->push_back(new CMyPoint( newX, newY));

		// k�z�ppont k�r�li elforgat�s
		angle += 2.0 * M_PI / numPoints;
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
bool GraphObject::IsContactConcave(GraphObject *obj)
{
	// objektumokat �sszehasonl�t� szakaszokb�l sz�molt forg�sir�ny
	int sgn;

	// saj�t objektumon bel�l a forg�sir�ny
	int mySgn = CMyPoint::RotDirct((*points)[0], (*points)[1], (*points)[2]);

	// poligon v�g�st kell csin�lni, hogy a vizsg�lt objektum b�rmelyik pontja
	// bel�l van-e az n k�rvonalamon
	bool ret = false;

	// az �sszehasonl�tott objektum minden pontj�ra
	for(int i = 0; i < obj->points->size() && !ret; ++i)
	{
		sgn = 0;

		// minden saj�tpontommal
		for(int j = 1; j < points->size() - 1; ++j)
		{
			sgn += CMyPoint::RotDirct((*points)[0], (*points)[j],
				(*(obj->points))[i]);

			sgn += CMyPoint::RotDirct((*points)[j], (*points)[j + 1],
				(*(obj->points))[i]);

			sgn += CMyPoint::RotDirct((*points)[j + 1], (*points)[0],
				(*(obj->points))[i]);
		}

		if (sgn == mySgn * 3)
		{
			printf("Collision!!\n");
			ret = true;
		}
	}

	return ret;
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
	points->clear();
	delete points;
	points = NULL;

	if (clr)
		delete clr;
	clr = NULL;
}

// objektumok egyez�s�ge
bool GraphObject::operator==(const GraphObject *c)
{
	return (c->cX == cX) && (c->cY == cY) && (c->R == R) && (c->rotAngle == rotAngle);
}

bool GraphObject::operator!=(const GraphObject *c)
{
	return !(this->operator==(c));
}
