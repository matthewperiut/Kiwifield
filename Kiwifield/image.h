#pragma once
#pragma once
#include "olcPixelGameEngine.h"
#include <string>
#include <vector>

using namespace olc;
using namespace std;

class Image
{
public:
	string filepath;
	vector<Sprite*> sprite;
	vector<Decal*> decal;
	vi2d position;
	float secAnimate = 0;
	int frame = 0;
	int bglayer = 0;

	Image();
	Image(string file);
	Image(string file, vi2d pos);
	Image(vi2d size, vi2d pos);
	Image(vi2d size);
	void animate(float elapsed);
	Decal* getDecal();
	Sprite* getSprite();
	void setSprite(int location, string file);
	void update();
};