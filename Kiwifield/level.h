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
	int layer;

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