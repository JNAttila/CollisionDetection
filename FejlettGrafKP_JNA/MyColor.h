#pragma once
class MyColor
{
public:
	MyColor(float r, float g, float b) :
		r(r), g(g), b(b) {}

	~MyColor(void);

public:
	float r, g, b;
};

