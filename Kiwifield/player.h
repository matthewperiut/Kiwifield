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
	vf2d pos = { 0, 0 };
	vf2d velocity = { 0, 0 };
	vi2d size = vi2d(16, 16);

	vi2d collisionPixel;
	PixelGameEngine* g;
	
	Sprite* sprite;
	Decal* decal;
    
public:
	Player(vf2d p, PixelGameEngine& g);

	void keyboardInput(float time, Stage& stage);

	void move(float time, Stage& stage);
};
