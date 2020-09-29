#pragma once
#include "character.h"

using namespace olc;
using namespace std;

class Player : public Character
{
public:
	Player(vf2d p, PixelGameEngine& g);

	void keyboardInput(float time, Stage& stage);
};
