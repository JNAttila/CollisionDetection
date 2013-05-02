#pragma once

#include "GraphObject.h"

using namespace std; 

class Common
{
public:
	static bool PointLineDist(CMyPoint *p1, CMyPoint *p2, CMyPoint *pp);
	static bool DistanceCheck(GraphObject *c);
	static void UpDateGraphObjectPosition(GraphObject *c);

	static set<GraphObject*> *graphObjSet;
	static bool renderForward;

	static MyColor *CLR_NEAR;
	static MyColor *CLR_NORMAL;
	static MyColor *CLR_CHG;

	static float screenXMin;
	static float screenYMin;
	static float screenXMax;
	static float screenYMax;

	static float nRange;
};

