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
	int levelid = 0;

	// Image system
	const static int layerNum = 3;
	olc::Sprite* spr[layerNum] = {};
	olc::Decal* dec[layerNum] = {};
	int layers[layerNum] = {};

private:
	// Level size
	vi2d origin = { 0,0 };
	vi2d size = { 256, 144 };

	// Collision
	bool collisionArray[wWidth][wHeight] = {};
	
public:
	void Create(PixelGameEngine& g);	
	Level(PixelGameEngine& g);
	Level(PixelGameEngine& g, std::string imgfilepath, bool pge);
	void CollisionEditor(PixelGameEngine& g);
	void loadCollisions(string filepath);
	void update(PixelGameEngine& g, Player& p, float fElapsed);
};