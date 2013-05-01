#pragma once

#include <set>
#include <vector>

#include "MyColor.h"
#include "MyPoint.h"


using namespace std;

class GraphObject
{
public:
	// k�z�ppont
	float cX, cY;
	// sug�r
	float R;
	// ker�leti pontok
	int numPoints;
	// orient�ci�
	float rotAngle;

	// a pontok list�ja
	vector<CMyPoint*> *points;

	// a hat�rol� t�glalap koordin�t�inak indexe a list�ban
	int iXMin, iXMax;
	int iYMin, iYMax;

	// sz�n komponensek
	MyColor *clr;

	// sebess�g komponensek
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

	bool operator==(const GraphObject *c);
	bool operator!=(const GraphObject *c);
};

