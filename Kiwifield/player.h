#pragma once
#include "olcPixelGameEngine.h"

using namespace olc;

class Player
{
public:
	vf2d pos;
	vi2d size = vi2d(16, 16);

	vi2d collisionPixel;
public:
	Player(vf2d p);

	void update(float time, PixelGameEngine& g);
};