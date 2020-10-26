#pragma once
// The way to cross between stages
#include "olcPixelGameEngine.h"

#include <string>
using namespace olc;
class Portal
{
	const vi2d size{ 9,16 };
public:
	vi2d pos{ 0, 0 };
	std::string destination{ "./stages/" };
	vi2d desPos{ 0, 0 };

	Portal(vi2d pos);
	void Destination(std::string des, vi2d pos);
	bool Update(float time, vi2d p, PixelGameEngine& g);
};