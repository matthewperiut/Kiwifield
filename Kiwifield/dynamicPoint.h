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
	Stage* stage;
public:
	DynamicPoint(Stage& s);
	void Update(float time);
};