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
};

