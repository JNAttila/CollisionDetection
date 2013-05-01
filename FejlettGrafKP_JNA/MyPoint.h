#pragma once

class CMyPoint
{
public:
	float x, y;

public:
	CMyPoint(float X, float Y) :
		x(X), y(Y) {}

	// k�t szakasz egym�shoz k�pesti ir�nyults�ga
	static int RotDirct(CMyPoint *p0, CMyPoint *p1, CMyPoint *p2);

	// p1 �s p2 �ltal kijel�lt n�gyzeten bel�l van-e pt
	static bool IsPointBetween(CMyPoint *p1, CMyPoint *p2, CMyPoint *pt);
};

