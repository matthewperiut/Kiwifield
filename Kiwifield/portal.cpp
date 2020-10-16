// The way to cross between stages
#include "olcPixelGameEngine.h"
#include <string>
using namespace olc;
class Portal
{
	const vi2d size{ 10,10 };
	PixelGameEngine* g;
public:
	vi2d pos{ 0, 0 };
	std::string destination{ "./stages/" };
	vi2d desPos{ 0, 0 };

	Portal(vi2d pos, PixelGameEngine& g) : pos{ pos }, g{ &g }
	{

	}
	void Destination(std::string des, vi2d pos)
	{
		destination = des;
		desPos = pos;
	}
	void Update(float time, vi2d p)
	{
		//animation
		g->DrawRect(vi2d(pos.x - (size.x / 2), pos.y - size.y), size, GREY);
		g->Draw(pos);
		//check to teleport

	}
};