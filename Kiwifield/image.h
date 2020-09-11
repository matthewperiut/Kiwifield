#pragma once
#pragma once
#include "olcPixelGameEngine.h"
#include <string>

using namespace olc;
using namespace std;

class Image
{
public:
	Sprite* sprite;
	Decal* decal;
	vi2d position;
	int bglayer = 0;

	Image(string file);
	Image();
	Image(string file, vi2d pos);
	Image(vi2d size, vi2d pos);
	Image(vi2d size);
	void update();
};