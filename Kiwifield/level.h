#pragma once
#include "olcPixelGameEngine.h"
#include "player.h"
#include "const.h"

using namespace olc;

class Level
{
public:
	int levelid = 0;

private:
	vi2d origin = { 0,0 };
	vi2d size = { 256, 144 };

	const static int layerNum = 3;

	olc::Sprite* spr[layerNum] = {};
	olc::Decal* dec[layerNum] = {};
	int layers[layerNum] = {};

	bool collisionArray[wWidth][wHeight] = {};

public:
	Level(PixelGameEngine& g);
	void CollisionEditor(PixelGameEngine& g);
	void update(PixelGameEngine& g, Player& p, float fElapsed);
};