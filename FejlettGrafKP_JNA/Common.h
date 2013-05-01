#pragma once

#include "GraphObject.h"

using namespace std; 

class Common
{
public:
	static bool DistanceCheck(GraphObject *c);
	static void UpDateGraphObjectPosition(GraphObject *c);

	static set<GraphObject*> *graphObjSet;
	//static set<GraphObject*>::const_iterator it;

	static MyColor *CLR_NEAR;
	static MyColor *CLR_NORMAL;

	static float screenXMin;
	static float screenYMin;
	static float screenXMax;
	static float screenYMax;
};

