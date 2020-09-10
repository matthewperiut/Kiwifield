#pragma once

class Camera
{
public:
	int x = 0;
	int y = 0;
	Camera();
	Camera(int x, int y); 

	void u(int& x, int& y);
};

#include "olcPixelGameEngine.h"
using namespace olc;

vi2d camToVec(Camera& c)
{
	return vi2d(c.x, c.y);
}