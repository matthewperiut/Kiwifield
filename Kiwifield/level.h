#pragma once
#include "olcPixelGameEngine.h"
#include "const.h"

using namespace olc;

class Level
{
private:
	vi2d origin = { 0,0 };
	vi2d size = { 256, 144 };

	const static int layerNum = 3;

	olc::Sprite* spr[layerNum] = {};
	olc::Decal* dec[layerNum] = {};
	int layers[layerNum] = {};

public:
	Level(PixelGameEngine& g);
	void update(PixelGameEngine& g);
};