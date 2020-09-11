#pragma once
#include "olcPixelGameEngine.h"
#include "stage.h"
#include "const.h"
#include <vector>

using namespace olc;
using namespace std;

class Player
{
public:
	vf2d pos;
	vi2d size = vi2d(16, 16);

	vi2d collisionPixel;
	
	Sprite* sprite;
	Decal* decal;

private:
	int csize = 8;
public:
	Player(vf2d p);

	void update(float time, Stage& stage, PixelGameEngine& g);
};