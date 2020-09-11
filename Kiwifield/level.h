#pragma once
#include "olcPixelGameEngine.h"
#include "player.h"
#include "const.h"
#include <string>

using namespace olc;
using namespace std;

class Level
{
public:
	string levelid = "";

	// Image system
	olc::Sprite* spr;
	olc::Decal* dec;
	int drawLayer;

private:
	// Level size
	vi2d origin = { 0,0 };
	vi2d size = { 256, 144 };

	// Collision
	bool collisionArray[wWidth][wHeight] = {};
	
public:
	void Create(PixelGameEngine& g);	
	Level(PixelGameEngine& g);
	Level(PixelGameEngine& g, std::string id, bool pge);
	void CollisionEditor(PixelGameEngine& g);
	void ImageEditor(PixelGameEngine& g);
	void update(PixelGameEngine& g, Player& p, float fElapsed);
};

void FillCircleInSpr(int x, int y, int radius, Pixel p, Sprite& file);
void DrawLineIn2DBoolean(int& x1, int& y1, int& x2, int& y2, bool(&collisionArray)[wWidth][wHeight], uint32_t pattern = 0xFFFFFFFF);