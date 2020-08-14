#pragma once
#include "olcPixelGameEngine.h"
#include "const.h"

using namespace olc;

class Player
{
public:
	vf2d pos;
	vi2d size = vi2d(16, 16);

	vi2d collisionPixel;
public:
	Player(vf2d p);

	void update(float time, bool(&collisionArray)[wWidth][wHeight], PixelGameEngine& g);
};