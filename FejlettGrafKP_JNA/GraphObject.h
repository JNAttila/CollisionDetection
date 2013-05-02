#pragma once

#include <set>
#include <vector>

#include "MyColor.h"
#include "MyPoint.h"


using namespace std;

class GraphObject
{
public:
	// középpont
	float cX, cY;
	// sugár
	float R;
	// kerületi pontok
	int numPoints;
	// orientáció
	float rotAngle;

	// ütközött
	bool crushed;

	// a pontok listája
	vector<CMyPoint*> *points;

	// a határoló téglalap koordinátáinak indexe a listában
	int iXMin, iXMax;
	int iYMin, iYMax;

	// szín komponensek
	MyColor *clr;

	// sebesség komponensek
	float vX, vY;

public:
	GraphObject(float x, float y, float r, int nP, float rotA, MyColor *c, float vx, float vy);
	~GraphObject(void);

	float XMin();
	float XMax();
	float YMin();
	float YMax();

	bool IsNear(GraphObject *obj);
	bool IsContact(GraphObject *obj);
	bool IsContact2(GraphObject *obj);

	bool operator==(const GraphObject *c);
	bool operator!=(const GraphObject *c);
};

