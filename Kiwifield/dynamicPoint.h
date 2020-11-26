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
protected:
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
	virtual ~DynamicPoint();
	virtual void MoveUp(Stage& stage);
	virtual void MoveDown(Stage& stage);
	virtual void MoveLeft(Stage& stage);
	virtual void MoveRight(Stage& stage);
	void Move(float time, Stage& stage);
};