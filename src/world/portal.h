#pragma once
// The way to cross between stages
#include "../lib/olcPixelGameEngine.h"

#include <string>
using namespace olc;
class Portal
{
	const vi2d size{ 9,16 };
	float elapsed = 0;
public:
	vi2d pos{ 0, 0 };
	std::string destination{ "./assets/stages/" };
	vi2d desPos{ 0, 0 };

	Portal(vi2d pos);
	void Destination(std::string des, vi2d pos);
	
	bool Update(float time, vi2d p, PixelGameEngine& g);
	void DebugDraw(PixelGameEngine& g);
};