#pragma once
#include "olcPixelGameEngine.h"
#include "stage.h"
#include <vector>

using namespace olc;
using namespace std;

class DynamicPoint
{
public:
	vf2d pos = { 1, 1 };
	vf2d vel = { 0, 0 };
private:
	enum { up, down, left, right };
	bool directions[4]{ false };

	vf2d timedVelocity;
	vf2d newPos;
public:
	bool Up();
	bool Down();
	bool Left();
	bool Right();
	
	DynamicPoint();
	void MoveUp(Stage& stage);
	void MoveDown(Stage& stage);
	void MoveLeft(Stage& stage);
	void MoveRight(Stage& stage);
	void Move(float time, Stage& stage);
};