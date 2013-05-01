#include "MyPoint.h"

//#include <math.h>
//#include <stdio.h>
//#include <cstdlib>
#include <set>
//#include <vector>
using namespace std;

// k�t szakasz egym�shoz k�pest milyen ir�nyban �ll
int CMyPoint::RotDirct(CMyPoint *p0, CMyPoint *p1, CMyPoint *p2){
	// Kimenet: +1 ha p0->p1->p2 balra fordul,
	//			0 ha p0,p1 �s p2 kolline�risak,
	//			-1 ha p0->p1->p2 jobbra fordul.

	double p1xp2 = (p1->x - p0->x) * (p2->y - p0->y) - (p2->x - p0->x) * (p1->y - p0->y);
	int signum = (p1xp2 < 0) ? (-1) : ((p1xp2 > 0) ? (1) : (0));
	return signum;
}


// P1 �s P2 �ltal kijel�l t�glalapon bel�l van-e PT
bool CMyPoint::IsPointBetween(CMyPoint *p1, CMyPoint *p2, CMyPoint *pt)
{
	float xMin = min<int>(p1->x, p2->x);
	float xMax = max<int>(p1->x, p2->x);
	float yMin = min<int>(p1->y, p2->y);
	float yMax = max<int>(p1->y, p2->y);

	if ((xMin <= pt->x) && (pt->x <= xMax) && (yMin <= pt->y) && (pt->y <= yMax))
		return true;

	return false;
}
